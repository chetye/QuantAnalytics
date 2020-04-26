#include "Curve.h"
#include "Pricer.h"
#include "IR_Model.h"

namespace marketdata
{
Curve::Curve(vector<date::Date> dates, vector<double> discount_factors, 
  curve::InterpolationType interp, curve::ExtrapolationType extrap)
  : m_dates(dates), m_discount_factors(discount_factors)
{
  m_interpextrap.reset(new curve::InterpolationExtrapolation2D<date::Date, double>(m_dates, m_discount_factors, interp, extrap));
}

double Curve::get_discount_factor(date::Date date) const
{
  const double discount_factor = m_interpextrap->get_value(date);
  return discount_factor;
}

double Curve::get_forecast_factor(date::Date start_date, date::Date end_date) const
{
  const double discount_factor_start = m_interpextrap->get_value(start_date);
  const double discount_factor_end = m_interpextrap->get_value(end_date);
  const double forecast_factor = discount_factor_start / discount_factor_end;
  return forecast_factor;
}

void Curve::set_discount_factors(vector<double> discount_factors)
{
  copy(discount_factors.begin(), discount_factors.end(), m_discount_factors.begin() + 1);
  m_interpextrap.reset(new curve::InterpolationExtrapolation2D<date::Date, double>(m_dates, m_discount_factors, m_interpextrap->get_interpolation(), m_interpextrap->get_extrapolation()));
}


void Curve::dump(ostream & os) const
{
  os << "\n";
  for (int i = 0; i < m_dates.size(); i++)
  {
    os << m_dates[i] << " " << m_discount_factors[i] << "\n";
  }
}

Interest_Rate_Curve::Interest_Rate_Curve(vector<instrument::Instrument_Ptr> instruments,
  curve::InterpolationType interp, curve::ExtrapolationType extrap)
  : m_instruments(instruments), Curve(vector<date::Date>(), vector<double>(), interp, extrap)
{
  // First Date
  m_dates.push_back(instruments[0]->get_start_date());
  m_discount_factors.push_back(1.0);

  for (size_t i = 0; i < instruments.size(); i++)
  {
    m_dates.push_back(instruments[i]->get_end_date());
    m_discount_factors.push_back(1.0);
  }

  m_interpextrap.reset(new curve::InterpolationExtrapolation2D<date::Date, double>(m_dates, m_discount_factors, interp, extrap));
}

vector<double> Interest_Rate_Curve::get_discount_factors(size_t from_index) const
{
  vector<double> discount_factors;

  copy(m_discount_factors.begin() + from_index, m_discount_factors.end(), back_inserter(discount_factors));

  return discount_factors;
}

Curve_System::Curve_System(date::Date market_date, std::string currency)
  : Marketdata(market_date, CURVE), m_currency(currency)
{
}

struct bootstrap_helper
{
private:
  static boost::shared_ptr<Interest_Rate_Curve_System> m_p_curve;

public:
  bootstrap_helper(Interest_Rate_Curve_System* p_curve) { m_p_curve.reset(p_curve); }

  static double bootstrap(const dlib::matrix<double, 0, 1>& v)
  {
    vector<double> updated_discount_factors;
    for (int i = 0; i < v.size(); i++)
    {
      updated_discount_factors.push_back(v(i));
    }
    m_p_curve->set_discount_factors(updated_discount_factors);

    // Create a pricers
    model::IR_Model_Ptr curve_model(new model::IR_Model(m_p_curve));
    std::vector<pricer::Pricer> curve_pricers;
    std::vector<double> prices;
    std::vector<instrument::Instrument_Ptr> instruments = m_p_curve->get_instruments();
    for (int i = 0; i < instruments.size(); i++)
    {
      curve_pricers.push_back(pricer::Pricer(instruments[i], curve_model));
      prices.push_back(instruments[i]->price(curve_pricers[i]));
    }

    double return_value = 0;
    int i;
    for (i = 0; i < prices.size(); i++)
    {
      return_value += pow(prices[i], 2);
    }

    return_value /= i; // mean 
    return_value = sqrt(return_value);

    return return_value;
  }
};
boost::shared_ptr<Interest_Rate_Curve_System> bootstrap_helper::m_p_curve;

std::vector<instrument::Instrument_Ptr> Interest_Rate_Curve_System::get_instruments() const
{
  std::vector<instrument::Instrument_Ptr> instruments;

  std::map<std::string, Interest_Rate_Curve_Ptr>::const_iterator iter;
  for (iter = m_curves.begin(); iter != m_curves.end(); ++iter)
  {
    std::vector<instrument::Instrument_Ptr> curve_instruments = (iter->second)->get_instruments();
    std::copy(curve_instruments.begin(), curve_instruments.end(), std::back_inserter(instruments));
  }

  return instruments;
}

std::vector<double> Interest_Rate_Curve_System::get_discount_factors() const
{
  std::vector<double> discount_factors;

  std::map<std::string, Interest_Rate_Curve_Ptr>::const_iterator iter;
  for (iter = m_curves.begin(); iter != m_curves.end(); ++iter)
  {
    std::vector<double> curve_discount_factors = (iter->second)->get_discount_factors();
    std::copy(curve_discount_factors.begin(), curve_discount_factors.end(), std::back_inserter(discount_factors));
  }

  return discount_factors;
}

void Interest_Rate_Curve_System::set_discount_factors(vector<double> discount_factors)
{
  std::map<std::string, Interest_Rate_Curve_Ptr>::const_iterator iter;
  size_t start_index = 0;

  for (iter = m_curves.begin(); iter != m_curves.end(); ++iter)
  {
    std::vector<double> curve_discount_factors = (iter->second)->get_discount_factors();
    copy(discount_factors.begin() + start_index, discount_factors.begin() + start_index + curve_discount_factors.size(), curve_discount_factors.begin());
    (iter->second)->set_discount_factors(curve_discount_factors);
    start_index += curve_discount_factors.size(); // update the start index for next curve
  }
}

Interest_Rate_Curve_System::Interest_Rate_Curve_System(date::Date market_date, std::string currency)
  : Curve_System(market_date, currency)
{
}

void Interest_Rate_Curve_System::add_curve(std::string type, std::string index, date::FlowFrequency frequency, Interest_Rate_Curve_Ptr curve)
{
  std::string curve_type = type;

  if (type == "FORECASTING")
  {
    std::ostringstream curve_type_stream;
    curve_type_stream << "FORECASTING_" << index << "_" << frequency;
    curve_type = curve_type_stream.str();
  }

  m_curves.insert(std::pair<std::string, Interest_Rate_Curve_Ptr>(curve_type, curve));
}

void Interest_Rate_Curve_System::add_external_curve_system(boost::shared_ptr<Interest_Rate_Curve_System> external_curve_system)
{
  m_external_curve_system = external_curve_system;
}

void Interest_Rate_Curve_System::add_external_curve_system(Interest_Rate_Curve_System& external_curve_system)
{
  m_external_curve_system.reset(new Interest_Rate_Curve_System(external_curve_system));
}

double Interest_Rate_Curve_System::get_discount_factor(date::Date date) const
{
  double discount_factor = 1.0;
  std::map<std::string, Interest_Rate_Curve_Ptr>::const_iterator iter = m_curves.find("DISCOUNTING");
  if (iter != m_curves.end())
  {
    Interest_Rate_Curve_Ptr discounting_curve = iter->second;
    discount_factor = discounting_curve->get_discount_factor(date);
  }
  else
  {
    std::runtime_error("DISCOUNTING curve not found");
  }


  if(m_external_curve_system)
  {
    double external_discount_factor = m_external_curve_system->get_discount_factor(date);

    discount_factor = discount_factor * external_discount_factor;
  }

  return discount_factor;
}

double Interest_Rate_Curve_System::get_forecast_factor(date::Date start_date, date::Date end_date) const
{
  double forecast_factor = 1.0;
  std::map<std::string, Interest_Rate_Curve_Ptr>::const_iterator iter = m_curves.find("DISCOUNTING");
  if (iter != m_curves.end())
  {
    Interest_Rate_Curve_Ptr discounting_curve = iter->second;
    forecast_factor = discounting_curve->get_forecast_factor(start_date, end_date);
  }
  else
  {
    std::runtime_error("DISCOUNTING curve not found");
  }


  if (m_external_curve_system)
  {
    double external_forecast_factor = m_external_curve_system->get_forecast_factor(start_date, end_date);

    forecast_factor = forecast_factor * external_forecast_factor;
  }

  return forecast_factor;
}

double Interest_Rate_Curve_System::get_forward_rate(string index, date::FlowFrequency frequency, date::Date start_date, date::Date end_date, date::DayCountConvention dc) const
{
  std::ostringstream curve_type;
  curve_type << "FORECASTING_" << index << "_" << frequency;

  double forward = 0;
  std::map<std::string, Interest_Rate_Curve_Ptr>::const_iterator iter = m_curves.find(curve_type.str());
  if (iter != m_curves.end())
  {
    Interest_Rate_Curve_Ptr forecasting_curve = iter->second;

    double forecast_factor = forecasting_curve->get_forecast_factor(start_date, end_date);
    double accrual_factor = start_date.accrual_factor(end_date, dc);

    forward = pow(forecast_factor, accrual_factor) - 1;
  }
  else
  {
    iter = m_curves.find("DISCOUNTING");
    if (iter != m_curves.end())
    {
      Interest_Rate_Curve_Ptr discounting_curve = iter->second;

      double forecast_factor = discounting_curve->get_forecast_factor(start_date, end_date);
      double accrual_factor = start_date.accrual_factor(end_date, dc);

      forward = pow(forecast_factor, accrual_factor) - 1;
    }
    else
    {
      std::runtime_error("DISCOUNTING curve not found");
    }
  }

  return forward;
}

void Interest_Rate_Curve_System::bootstrap()
{
  if (m_external_curve_system)
  {
    m_external_curve_system->bootstrap();
  }

  std::vector<double> discount_factors = get_discount_factors();
  std::vector< instrument::Instrument_Ptr> instruments = get_instruments();

  // minimize the differences between instruments price() and results()
  dlib::matrix<double, 0, 1> starting_point(discount_factors.size());
  for (int i = 0; i < starting_point.size(); i++)
  {
    starting_point(i) = 1.0;
  }

  // column_vector starting_point{-95.5, -4.5};
  bootstrap_helper b(this);

  dlib::find_min_box_constrained(dlib::lbfgs_search_strategy(get_instruments().size()),
    dlib::objective_delta_stop_strategy(1e-10), &bootstrap_helper::bootstrap, dlib::derivative(&bootstrap_helper::bootstrap),
    starting_point, 0, 2.0);

  for (int i = 0; i < discount_factors.size(); i++)
  {
    discount_factors[i] = starting_point(i);
  }

  // set back the discount factors to respective curves
  set_discount_factors(discount_factors);
}

void Interest_Rate_Curve_System::dump(ostream & os) const
{
  Marketdata::dump(os);
  os << "\n";

  std::map<std::string, Interest_Rate_Curve_Ptr>::const_iterator iter;
  for (iter = m_curves.begin(); iter != m_curves.end(); ++iter)
  {
    os << iter->first << "\n";
    (iter->second)->dump(os);
  }
}
  
}
