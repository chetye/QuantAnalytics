#pragma once
#include <vector>
#include <map>
using namespace std;

#include "Marketdata.h"
#include "Instrument.h"
#include "Curve_Helper.h"

namespace marketdata
{
  class Curve : public Marketdata
  {
  private:
    bool m_bootstrapped;

  protected:
    vector<instrument::Instrument> m_instruments;
    vector<date::Date> m_dates;
    vector<double> m_discount_factors;
    map<string,Curve> m_sub_curve;
    curve::InterpolationExtrapolation2D<date::Date, double> m_interpextrap;
    string m_index;
    string m_currency;

  public:
    Curve() : Marketdata(date::Date()) , m_interpextrap(curve::InterpolationExtrapolation2D<date::Date, double>(vector<date::Date>(), vector<double>(), curve::NONE_INTERPOLATION, curve::NONE_EXTRAPOLATION)) {}
    Curve(date::Date market_date, string currency, string index, vector<instrument::Instrument> instruments, 
      curve::InterpolationType interp, curve::ExtrapolationType extrap);
    Curve(date::Date market_date, string currency, string index, curve::InterpolationType interp, curve::ExtrapolationType extrap);

    // SubCurves
    bool add_curve(curve::CurveType curve_type, string index, vector<instrument::Instrument> instruments);
    bool add_curve(curve::CurveType curve_type, Curve curve);

    // main functions
    bool bootstrap();

    // getter and setter methods
    string get_index() const {}
    string get_currency() const {}

    // utilities
    double get_discount_factor(date::Date date) const;
    double get_forecast_factor(string index, date::Date start_date, date::Date end_date) const;
    double get_forward(string index, date::FlowFrequency frequency, date::Date start_date, date::Date end_date) const;
  };
}

