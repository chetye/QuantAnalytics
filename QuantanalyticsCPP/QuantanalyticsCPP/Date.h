#pragma once
#include <string>
#include <set>
#include <vector>
#include <map>
#include <sstream>
using namespace std;

#include "Constants.h"

namespace date
{
  enum DayCountConvention
  {
    NONE_CONVENTION,
    ACT_360,
    ACT_365,
    D30_360
  };

  ostream& operator<<(ostream& os, DayCountConvention dc);

  enum HolidayCenter
  {
    NONE_HOLIDAY = 0,
    TOKYO = 1,
    NEW_YORK = 2,
    LONDON = 3
  };

  ostream& operator<<(ostream& os, HolidayCenter hc);

  enum TenorUnit
  {
    NONE_TENOR,
    DAY,
    WEEK,
    MONTH,
    YEAR
  };

  ostream& operator<<(ostream& os, TenorUnit dc);

  enum DateRollConvention
  {
    MODIFIED_PRECEEDING = -1,
    NONE_DATEROLL = 0,
    MODIFIED_FOLLOWING = 1,
    FOLLOWING,
    PRECEEDING,
  };

  ostream& operator<<(ostream& os, DateRollConvention dr);

  enum FlowFrequency
  {
    NONE_FREQUENCY,
    DAILY = 1,
    MONTHLY = 30,
    QUARTERLY = 90,
    SEMI_ANNUAL = 180,
    ANNUAL = 365
  };

  ostream& operator<<(ostream& os, FlowFrequency ff);

  class Date;
  class Holidays
  {
  private:
    set<HolidayCenter> m_holidays;
    static map<HolidayCenter, set<Date> > holiday_registry; // this shared registry is created on first access to Holidays
  public:
    Holidays();
    Holidays(HolidayCenter h, ...); // function with variadic arguments 
    bool is_holiday(Date d) const;
    bool is_weekday(Date d) const;
  };

  class Tenor
  {
  private:
    string m_tenor_string;
    long m_multiplier;
    TenorUnit m_unit;
    Holidays m_holidays;
    DateRollConvention m_dateroll;
  public:
    Tenor(string tenor_string, Holidays holidays, DateRollConvention dateroll);
    Tenor(FlowFrequency fq, Holidays holidays, DateRollConvention dateroll);
    Date get_start_date(const Date& start_date) const;
    Date get_end_date(const Date& start_date) const;
  };

  class Date
  {
  private:
    long m_day;
    long m_month;
    long m_year;

    // private utilities
    long days_in_month(long month, long year);

  public:
    Date();
    Date(const string& date_string);
    
    // getter methods
    long day() const { return m_day; }
    long month() const { return m_month; }
    long year() const { return m_year; }

    // date operations
    Date operator+(const long & days_to_add) const;
    Date operator-(const long & days_to_subtract) const;
    Date operator+(const Tenor& tenor_to_add) const;
    Date operator-(const Tenor& tenor_to_subtract) const;
    long operator-(const Date& d) const;
    bool operator<(const Date& d) const;
    bool operator>(const Date& d) const;
    bool operator<=(const Date& d) const;
    bool operator>=(const Date& d) const;
    bool operator==(const Date& d) const;

    // utility functions
    double accrual_factor(Date expiry, DayCountConvention dc) const;
    void dump(ostream& os) const;

    ~Date();
  };

  // dump details
  ostream& operator<<(ostream& os, const Date& d);
}

