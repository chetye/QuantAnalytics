#pragma once
#include <string>
#include <set>
#include <vector>
#include <map>
using namespace std;

namespace date
{
  enum DayCountConvention
  {
    NONE_CONVENTION,
    ACTUAL_360,
    ACTUAL_365,
    D30_360,
    D30_365
  };

  enum HolidayCenter
  {
    NONE_HOLIDAY = 0,
    TOKYO = 1,
    NEW_YORK = 2,
    LONDON = 3
  };

  enum TenorUnit
  {
    NONE_TENOR,
    DAY,
    WEEK,
    MONTH,
    YEAR
  };

  enum DateRollConvention
  {
    NONE_DATEROLL = 0,
    FOLLOWING = 1,
    PRECEEDING = -1,
    MODIFIED_FOLLOWING = 1,
    MODIFIED_PRECEEDING = -1
  };

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
    bool operator<(const Date& d) const;

    ~Date();
  };

  // dump details
  ostream& operator<<(ostream& os, const Date& d);
}

