#pragma once
#include <string>
using namespace std;

namespace date
{
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
    Date operator+(const long & days_to_add);
    Date operator-(const long & days_to_subtract);

    ~Date();
  };

  // dump details
  ostream& operator<<(ostream& os, const Date& d);
}

