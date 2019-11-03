#include "Date.h"
#include <sstream>
#include <cstdarg>
using namespace std;

namespace date
{
  map<HolidayCenter, set<Date> > Holidays::holiday_registry;

  Holidays::Holidays()
  {
  }

  Holidays::Holidays(HolidayCenter h, ...)
  {
    if (holiday_registry.empty())
    {
      // populating the registry the first time
      // Tokyo holidays
      set<Date> tokyo_holidays;
      tokyo_holidays.insert(Date("01-01-2019"));
      holiday_registry.insert(pair<HolidayCenter, set<Date> >(TOKYO, tokyo_holidays));

      // New York holidays
      set<Date> newyork_holidays;
      newyork_holidays.insert(Date("02-01-2019"));
      holiday_registry.insert(pair<HolidayCenter, set<Date> >(NEW_YORK, newyork_holidays));
    }

    // get the variadic arguments
    va_list arguments;
    m_holidays.clear(); // clear if any previous one is present
    va_start(arguments, h);
    for (; h <= 4 && h >= 1; h = va_arg(arguments, HolidayCenter) )
    {
      m_holidays.insert(h);
    }

    va_end(arguments);
  }

  bool Holidays::is_holiday(Date d) const
  {
    set<HolidayCenter>::iterator holiday_iter = m_holidays.begin();
    for (; holiday_iter != m_holidays.end(); holiday_iter++)
    {
      map<HolidayCenter, set<Date> >::iterator iter = holiday_registry.find(*holiday_iter);
      if (iter != holiday_registry.end())
      {
        set<Date> holidays = iter->second;
        if (holidays.find(d) != holidays.end())
        {
          return true;
        }
      }
    }
    return false;
  }

  Tenor::Tenor(string tenor_string, Holidays holidays, DateRollConvention dateroll)
  {
    try
    {
      m_tenor_string = tenor_string;
      m_dateroll = dateroll;
      m_holidays = holidays;
      int has_day_label = m_tenor_string.find("D");
      int has_week_label = m_tenor_string.find("W");
      int has_month_label = m_tenor_string.find("M");
      int has_year_label = m_tenor_string.find("Y");
      if (m_tenor_string == "O/N")
      {
        m_unit = DAY;
        m_multiplier = 1; // should be later changed to currency specific convention
      }
      else if (m_tenor_string == "T/N")
      {
        m_unit = DAY;
        m_multiplier = 2; // should be later changed to currency specific convention
      }
      else if (m_tenor_string == "S/N")
      {
        m_unit = DAY;
        m_multiplier = 3; // should be later changed to currency specific convention
      }
      else if (has_day_label != string::npos)
      {
        m_unit = DAY;

        string multiplier_string = m_tenor_string.substr(0, has_day_label);
        stringstream os;
        os << multiplier_string;
        os >> m_multiplier;
      }
      else if (has_week_label != string::npos)
      {
        m_unit = WEEK;

        string multiplier_string = m_tenor_string.substr(0, has_week_label);
        stringstream os;
        os << multiplier_string;
        os >> m_multiplier;
      }
      else if (has_month_label != string::npos)
      {
        m_unit = MONTH;

        string multiplier_string = m_tenor_string.substr(0, has_month_label);
        stringstream os;
        os << multiplier_string;
        os >> m_multiplier;
      }
      else if (has_year_label != string::npos)
      {
        m_unit = YEAR;

        string multiplier_string = m_tenor_string.substr(0, has_year_label);
        stringstream os;
        os << multiplier_string;
        os >> m_multiplier;
      }
      else
      {
        ostringstream error_string;
        error_string << tenor_string << " is invalid tenor";
        throw std::exception(error_string.str().c_str());
      }
    }
    catch (...)
    {
      ostringstream error_string;
      error_string << tenor_string << " is invalid tenor";
      throw std::exception(error_string.str().c_str());
    }
  }

  Date Tenor::get_start_date(const Date& start_date) const
  {
    Date d = start_date;

    if (m_tenor_string == "T/N")
    {
      d = d + 1; // should be later changed to currency specific convention
    }
    else if (m_tenor_string == "S/N")
    {
      d = d + 2; // should be later changed to currency specific convention
    }

    // Check if the start date is holiday
    if (m_dateroll == MODIFIED_FOLLOWING || m_dateroll == MODIFIED_PRECEEDING)
    {
      while (m_holidays.is_holiday(d))
      {
        d = d + m_dateroll;
      };
    }

    return d;
  }

  Date Tenor::get_end_date(const Date& start_date) const
  {
    Date d = start_date;
    
    if (m_tenor_string == "O/N")
    {
      d = d + 1; // should be later changed to currency specific convention
    }
    else if (m_tenor_string == "T/N")
    {
      d = d + 2; // should be later changed to currency specific convention
    }
    else if (m_tenor_string == "S/N")
    {
      d = d + 3; // should be later changed to currency specific convention
    }
    else if (m_unit == DAY)
    {
      d = d + 1 * m_multiplier; // should be later changed to currency specific convention
    }
    else if (m_unit == WEEK)
    {
      d = d + 7 * m_multiplier; // should be later changed to currency specific convention
    }
    else if (m_unit == MONTH)
    {
      d = d + 31 * m_multiplier; // should be later changed to currency specific convention
    }
    else if (m_unit == YEAR)
    {
      d = d + 365 * m_multiplier; // should be later changed to currency specific convention
    }
    
    // Check if the start date is holiday
    if (m_dateroll == MODIFIED_FOLLOWING || m_dateroll == MODIFIED_PRECEEDING)
    {
      while (m_holidays.is_holiday(d))
      {
        d = d + m_dateroll;
      };
    }

    return d;
  }

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
  Date Date::operator+(const long& days_to_add) const
  {
    // Algo
    // 0. If days_to_add is negetive, call the - operator
    // 1. Add the days to m_days
    // 2. While m_days > days_in_month() and m_days - days_in_month() > 0
    //    m_days -= days_in_month()
    //    m_month = m_month + 1 > 12 ? 1 : m_month + 1
    //    m_year = m_month == 1 ? m_year + 1 : m_year
    Date d = *this;

    if (days_to_add >= 0) // do this only in case the days to add are positive
    {
      d.m_day = d.m_day + days_to_add;

      while (d.m_day > d.days_in_month(d.m_month, d.m_year) && d.m_day - d.days_in_month(d.m_month, d.m_year) > 0)
      {
        d.m_day -= d.days_in_month(d.m_month, d.m_year);
        d.m_month = d.m_month + 1 > 12 ? 1 : d.m_month + 1;
        d.m_year = d.m_month == 1 ? d.m_year + 1 : d.m_year;
      };
    }
    else
    {
      d - abs(days_to_add);
    }

    return d;
  }

  Date Date::operator-(const long& days_to_subtract) const
  {
    // Algo
    // 1. Subtract the days from m_days
    // 2. While m_days < 1
    //    m_days += days_in_month()
    //    m_month = m_month - 1 > 0 ? m_month - 1 : 12
    //    m_year = m_month == 12 ? m_year - 1 : m_year
    Date d = *this;

    if (days_to_subtract >= 0)
    {
      d.m_day = d.m_day - days_to_subtract;

      while (d.m_day < 1)
      {
        d.m_month = d.m_month - 1 > 0 ? d.m_month - 1 : 12;
        d.m_year = d.m_month == 12 ? d.m_year - 1 : d.m_year;
        d.m_day += d.days_in_month(d.m_month, d.m_year);
      };
    }
    else
    {
      d + abs(days_to_subtract);
    }


    return d;
  }

  Date Date::operator+(const Tenor & tenor_to_add) const
  {
    return tenor_to_add.get_end_date(*this);
  }

  Date Date::operator-(const Tenor & tenor_to_subtract) const
  {
    Date d = *this;



    return Date();
  }

  bool Date::operator<(const Date & d) const
  {
    if (m_year < d.m_year)
    {
      return true;
    }
    else if (m_month < d.m_month)
    {
      return true;
    }
    else if (m_day < d.m_day)
    {
      return true;
    }
    return false;
  }

  ostream & operator<<(ostream & os, const Date & d)
  {
    // TODO: insert return statement here
    os << d.day() << "-" << d.month() << "-" << d.year();
    return os;
  }
}
