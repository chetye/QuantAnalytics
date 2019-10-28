#include "Date.h"
#include <sstream>
using namespace std;

namespace date
{
  long Date::days_in_month(long month, long year)
  {
    if (year % 4 == 0 && year % 100 != 0 || year % 400 == 0)
    {
      switch (month)
      {
      case 1:
      case 3:
      case 5:
      case 7:
      case 8:
      case 10:
      case 12:
      {
        return 31;
        break;
      }
      case 4:
      case 6:
      case 9:
      case 11:
      {
        return 30;
        break;
      }
      case 2:
      {
        return 28;
        break;
      }
      }
    }
    else
    {
      switch (month)
      {
      case 1:
      case 3:
      case 5:
      case 7:
      case 8:
      case 10:
      case 12:
      {
        return 31;
        break;
      }
      case 4:
      case 6:
      case 9:
      case 11:
      {
        return 30;
        break;
      }
      case 2:
      {
        return 29;
        break;
      }
      }
    }
  }

  Date::Date()
  {
    m_day = m_month = m_year = 0;
  }

  Date::Date(const string& date_string)
  {
    if (date_string.find('-') != string::npos)
    {
      stringstream day_string;
      day_string << date_string.substr(0, 2);
      day_string >> m_day;

      stringstream month_string;
      month_string << date_string.substr(3, 2);
      month_string >> m_month;

      stringstream year_string;
      year_string << date_string.substr(6, 4);
      year_string >> m_year;
    }
    else
    {
      // assuming date in DDMMYYYY format
      try
      {
        stringstream day_string;
        day_string << date_string.substr(0, 2);
        day_string >> m_day;

        stringstream month_string;
        month_string << date_string.substr(2, 2);
        month_string >> m_month;

        stringstream year_string;
        year_string << date_string.substr(4, 4);
        year_string >> m_year;
      }
      catch (...)
      {
        string error_string = date_string + " does not seem to be in DDMMYYYY format";
        std::exception(error_string.c_str());
      }
    }
  }

  Date::~Date()
  {
  }

  // date utils
  Date Date::operator+(const long& days_to_add)
  {
    // Algo
    // 1. Add the days to m_days
    // 2. While m_days > days_in_month() and m_days - days_in_month() > 0
    //    m_days -= days_in_month()
    //    m_month = m_month + 1 > 12 ? 1 : m_month + 1
    //    m_year = m_month == 1 ? m_year + 1 : m_year
    
    m_day = m_day + days_to_add;
    
    while (m_day > days_in_month(m_month, m_year) && m_day - days_in_month(m_month, m_year) > 0)
    {
      m_day -= days_in_month(m_month, m_year);
      m_month = m_month + 1 > 12 ? 1 : m_month + 1;
      m_year = m_month == 1 ? m_year + 1 : m_year;
    };

    return *this;
  }

  Date Date::operator-(const long& days_to_subtract)
  {
    // Algo
    // 1. Subtract the days from m_days
    // 2. While m_days < 1
    //    m_days += days_in_month()
    //    m_month = m_month - 1 > 0 ? m_month - 1 : 12
    //    m_year = m_month == 12 ? m_year - 1 : m_year
    
    m_day = m_day - days_to_subtract;

    while (m_day < 1)
    {
      m_month = m_month - 1 > 0 ? m_month - 1 : 12;
      m_year = m_month == 12 ? m_year - 1 : m_year;
      m_day += days_in_month(m_month, m_year);
    };

    return *this;
  }

  ostream & operator<<(ostream & os, const Date & d)
  {
    // TODO: insert return statement here
    os << d.day() << "-" << d.month() << "-" << d.year();
    return os;
  }
}
