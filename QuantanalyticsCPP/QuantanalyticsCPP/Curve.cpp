#include "Curve.h"
#include "Pricer.h"
#include "IR_Model.h"

namespace marketdata
{
  struct bootstrap_helper
  {
  private:
    static boost::shared_ptr<Curve> m_p_curve;

  public:
    bootstrap_helper(Curve* p_curve) { m_p_curve.reset(new Curve(*p_curve)); }

    static double bootstrapper(const dlib::matrix<double, 0, 1>& v)
    {
      vector<double> updated_discount_factors;
      for (int i = 0; i < v.size(); i++)
      {
        updated_discount_factors.push_back(v(i));
      }
      m_p_curve->update_discount_factors(updated_discount_factors);

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

  boost::shared_ptr<Curve> bootstrap_helper::m_p_curve;

  Curve::Curve(date::Date market_date, string currency, string index, vector<instrument::Instrument_Ptr> instruments,
    curve::InterpolationType interp, curve::ExtrapolationType extrap)
    : Marketdata(market_date), m_instruments(instruments)
  { 
    for (size_t i = 0; i < instruments.size(); i++)
    {
      m_dates.push_back(instruments[i]->get_end_date());
      m_discount_factors.push_back(1.0);
    }

    m_interpextrap.reset(new curve::InterpolationExtrapolation2D<date::Date, double>(m_dates, m_discount_factors, interp, extrap));
  }

  Curve::Curve(date::Date market_date, string currency, string index, curve::InterpolationType interp, curve::ExtrapolationType extrap)
    : Marketdata(market_date)
  {
    m_interpextrap.reset(new curve::InterpolationExtrapolation2D<date::Date, double>(vector<date::Date>(), vector<double>(), interp, extrap));
  }

  bool Curve::add_curve(curve::CurveType curve_type, string index, vector<instrument::Instrument_Ptr> instruments)
  {
    if (curve_type == curve::DISCOUNTING)
    {
      
    }
    else
    {
      Curve c(get_market_date(), m_currency, index, instruments, m_interpextrap->get_interpolation(), m_interpextrap->get_extrapolation());
      ostringstream key;
      key << m_currency << "_" << curve_type;
      m_sub_curve[key.str()] = c;
    }

    return true;
  }

  bool Curve::add_curve(curve::CurveType curve_type, Curve curve)
  {
    ostringstream key;
    key << curve_type;
    m_sub_curve[key.str()] = curve;

    return true;
  }

  bool Curve::bootstrap()
  {
    // minimize the differences between instruments price() and results()
    dlib::matrix<double, 0, 1> starting_point(m_discount_factors.size());
    for (int i = 0; i < starting_point.size(); i++)
    {
      starting_point(i) = 1.0;
    }

    // column_vector starting_point{-95.5, -4.5};
    bootstrap_helper b(this);

    dlib::find_min_box_constrained(dlib::lbfgs_search_strategy(m_discount_factors.size()),
      dlib::objective_delta_stop_strategy(1e-10), &bootstrap_helper::bootstrapper, dlib::derivative(&bootstrap_helper::bootstrapper),
      starting_point, 0, 2.0);

    for (int i = 0; i < m_discount_factors.size(); i++)
    {
      m_discount_factors[i] = starting_point(i);
    }

    m_bootstrapped = true;
    return m_bootstrapped;
  }

  void Curve::update_discount_factors(vector<double> discount_factors)
  {
    m_discount_factors = discount_factors;
    m_interpextrap->reset_y_values(discount_factors);
  }

  double Curve::get_discount_factor(date::Date date) const
  {
    const double discount_factor = m_interpextrap->get_value(date);
    return discount_factor;
  }

  double Curve::get_forecast_factor(string index, date::Date start_date, date::Date end_date) const
  {
    const double discount_factor_start = m_interpextrap->get_value(start_date);
    const double discount_factor_end = m_interpextrap->get_value(end_date);
    const double forecast_factor = discount_factor_end / discount_factor_start;
    return forecast_factor;
  }

  double Curve::get_forward(string index, date::FlowFrequency frequency, date::Date start_date, date::Date end_date) const
  {
    return 0.0;
  }
  void Curve::dump(ostream & os) const
  {
    Marketdata::dump(os);
    os << "\n";
    for (int i = 0; i < m_dates.size(); i++)
    {
      os << m_dates[i] << " " << m_discount_factors[i] << "\n";
    }
  }
}
