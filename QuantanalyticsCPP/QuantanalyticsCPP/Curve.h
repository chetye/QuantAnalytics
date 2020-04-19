#pragma once
#include <vector>
#include <map>
using namespace std;

#include "Marketdata.h"
#include "Instrument.h"
#include "Curve_Helper.h"

#include <dlib/optimization.h>
#include <dlib/global_optimization.h>
#include <boost/optional.hpp>

namespace marketdata
{
  class Curve
  {
  protected:
    vector<date::Date> m_dates;
    vector<double> m_discount_factors;
    boost::shared_ptr<curve::InterpolationExtrapolation2D<date::Date, double> > m_interpextrap;

  public:
    Curve(vector<date::Date> dates, vector<double> discount_factors, curve::InterpolationType interp, curve::ExtrapolationType extrap);

    // utilities
    virtual double get_discount_factor(date::Date date) const;
    virtual double get_forecast_factor(date::Date start_date, date::Date end_date) const;
    virtual void set_discount_factors(vector<double> discount_factors);

    virtual void dump(ostream& os) const;
  };

  // support typedefs
  typedef boost::shared_ptr<Curve> Curve_Ptr;

  class Interest_Rate_Curve : public Curve
  {
  protected:
    vector<instrument::Instrument_Ptr> m_instruments;

  public:
    Interest_Rate_Curve(vector<instrument::Instrument_Ptr>, curve::InterpolationType interp, curve::ExtrapolationType extrap);

    // utilities
    vector<instrument::Instrument_Ptr> get_instruments() const { return m_instruments; }
    vector<double> get_discount_factors(size_t from_index = 1) const;
  };

  // support typedefs
  typedef boost::shared_ptr<Interest_Rate_Curve> Interest_Rate_Curve_Ptr;

  class Curve_System : public Marketdata
  {
  protected:
    std::string m_currency;

  public:
    Curve_System(date::Date market_date, std::string currency);

    // utilities
    std::string get_currency() const { return m_currency; }
    virtual double get_discount_factor(date::Date date) const = 0;
    virtual double get_forecast_factor(date::Date start_date, date::Date end_date) const = 0;
    virtual double get_forward_rate(string index, date::FlowFrequency frequency, date::Date start_date, date::Date end_date,
      date::DayCountConvention dc = date::ACT_365) const = 0;

    virtual std::vector<instrument::Instrument_Ptr> get_instruments() const = 0;
    virtual std::vector<double> get_discount_factors() const = 0;
    virtual void set_discount_factors(vector<double> discount_factors) = 0;

    virtual void bootstrap() = 0;

    virtual void dump(ostream& os) const = 0;
  };

  // support typedefs
  typedef boost::shared_ptr<Curve_System> Curve_System_Ptr;

  class Interest_Rate_Curve_System : public Curve_System
  {
  protected:
    std::map<std::string, Interest_Rate_Curve_Ptr> m_curves;
    boost::shared_ptr<Interest_Rate_Curve_System> m_external_curve_system;

  public:
    Interest_Rate_Curve_System(date::Date market_date, std::string currency);
    virtual void add_curve(std::string type, Interest_Rate_Curve_Ptr curve);
    virtual void add_external_curve_system(boost::shared_ptr<Interest_Rate_Curve_System> external_curve_system);
    virtual void add_external_curve_system(Interest_Rate_Curve_System& external_curve_system);

    // utilities
    virtual double get_discount_factor(date::Date date) const;
    virtual double get_forecast_factor(date::Date start_date, date::Date end_date) const;
    virtual double get_forward_rate(string index, date::FlowFrequency frequency, date::Date start_date, date::Date end_date,
      date::DayCountConvention dc = date::ACT_365) const;

    virtual std::vector<instrument::Instrument_Ptr> get_instruments() const;
    virtual std::vector<double> get_discount_factors() const;
    virtual void set_discount_factors(vector<double> discount_factors);

    virtual void bootstrap();

    virtual void dump(ostream& os) const;
  };

  // support typedefs
  typedef boost::shared_ptr<Interest_Rate_Curve_System> Interest_Rate_Curve_System_Ptr;

  /*
  class Curve
  {
  private:
    bool m_bootstrapped;

  protected:
    vector<instrument::Instrument_Ptr> m_instruments;
    vector<date::Date> m_dates;
    vector<double> m_discount_factors;
    map<string, boost::shared_ptr<Curve> > m_sub_curves;
    boost::shared_ptr<Curve> m_external_curve;

    boost::shared_ptr<curve::InterpolationExtrapolation2D<date::Date, double> > m_interpextrap;
    string m_index;
    string m_currency;

  public:
    Curve() : Marketdata(date::Date()) {}
    Curve(date::Date market_date, string currency, string index, vector<instrument::Instrument_Ptr> instruments,
      curve::InterpolationType interp, curve::ExtrapolationType extrap);
    Curve(date::Date market_date, string currency, string index, curve::InterpolationType interp, curve::ExtrapolationType extrap);

    // main functions
    bool bootstrap();

    // getter and setter methods
    string get_index() const { return m_index; }
    string get_currency() const { return m_currency; }
    vector<instrument::Instrument_Ptr> get_instruments() const { return m_instruments; }
    void update_discount_factors(vector<double> discount_factors);

    // utilities
    double get_discount_factor(date::Date date) const;
    double get_forecast_factor(date::Date start_date, date::Date end_date) const;
    double get_forward(string index, date::FlowFrequency frequency, date::Date start_date, date::Date end_date,
      date::DayCountConvention dc = date::ACT_365) const;

    void dump(ostream& os) const;
  };

  // support typedefs
  boost::shared_ptr<Curve> Curve_Ptr;
  */

}

