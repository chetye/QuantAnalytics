#include "Curve.h"

namespace marketdata
{
  Curve::Curve(date::Date market_date, string currency, string index, vector<instrument::Instrument> instruments,
    curve::InterpolationType interp, curve::ExtrapolationType extrap)
    : Marketdata(market_date) ,m_instruments(instruments)
    , m_interpextrap(curve::InterpolationExtrapolation2D<date::Date, double>(vector<date::Date>(), vector<double>(), interp, extrap))
  { 
    for (size_t i = 0; i < instruments.size(); i++)
    {
      m_dates.push_back(instruments[i].get_end_date());
      m_discount_factors.push_back(0.0);
    }
  }

  Curve::Curve(date::Date market_date, string currency, string index, curve::InterpolationType interp, curve::ExtrapolationType extrap)
    : Marketdata(market_date), 
    m_interpextrap(curve::InterpolationExtrapolation2D<date::Date, double>(vector<date::Date>(), vector<double>(), interp, extrap))
  {
  }

  bool Curve::add_curve(curve::CurveType curve_type, string index, vector<instrument::Instrument> instruments)
  {
    Curve c(get_market_date(), m_currency, index, instruments, m_interpextrap.get_interpolation(), m_interpextrap.get_extrapolation());
    ostringstream key;
    key << m_currency << "_" << curve_type;
    m_sub_curve[key.str()] = c;

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
    // bootstrapping logic

    m_bootstrapped = true;
    return m_bootstrapped;
  }

  double Curve::get_discount_factor(date::Date date) const
  {
    const double discount_factor = m_interpextrap.get_value(date);
    return discount_factor;
  }

  double Curve::get_forecast_factor(string index, date::Date start_date, date::Date end_date) const
  {
    const double discount_factor_start = m_interpextrap.get_value(start_date);
    const double discount_factor_end = m_interpextrap.get_value(end_date);
    const double forecast_factor = discount_factor_end / discount_factor_start;
    return forecast_factor;
  }

  double Curve::get_forward(string index, date::FlowFrequency frequency, date::Date start_date, date::Date end_date) const
  {
    return 0.0;
  }
}
