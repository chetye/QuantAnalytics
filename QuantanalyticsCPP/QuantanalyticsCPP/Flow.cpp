#include "Flow.h"

namespace date
{
  Flow::Flow(Date start_date, Date end_date, FlowType flow_type)
    : m_start_date(start_date), m_end_date(end_date), m_type(flow_type)
  {
  }

  Date Flow::get_start_date() const
  {
    return m_start_date;
  }

  Date Flow::get_end_date() const
  {
    return m_end_date;
  }

  void Flow::set_start_date(const Date & start_date)
  {
    m_start_date = start_date;
  }

  void Flow::set_end_date(const Date & end_date)
  {
    m_end_date = end_date;
  }

  FlowType Flow::get_type() const
  {
    return m_type;
  }

  BulletFlow::BulletFlow(Date start_date, Date end_date,
    const Holidays& holidays, DayCountConvention daycount, DateRollConvention dateroll,
    double rate, double margin)
    : Flow(start_date, end_date, BULLET_FLOW)
  {
    m_frequency = NONE_FREQUENCY;

    m_holidays = holidays;
    m_daycount = daycount;
    m_dateroll = dateroll;

    m_fixing_start_date = start_date;
    m_fixing_end_date = end_date;
    m_fixing_holidays = holidays;
    m_fixing_daycount = daycount;
    m_fixing_dateroll = dateroll;

    m_rate = rate;
    m_margin = margin;

    generate_flow();
  }

  BulletFlow::BulletFlow(Date start_date, Date end_date,
    Date fixing_start_date, Date fixing_end_date,
    const Holidays& holidays, DayCountConvention daycount, DateRollConvention dateroll,
    const Holidays& fixing_holidays, DayCountConvention fixing_daycount, DateRollConvention fixing_dateroll,
    double rate, double margin)
    : Flow(start_date, end_date, BULLET_FLOW)
  {
    m_frequency = NONE_FREQUENCY;

    m_holidays = holidays;
    m_daycount = daycount;
    m_dateroll = dateroll;

    m_fixing_start_date = fixing_start_date;
    m_fixing_end_date = fixing_end_date;
    m_fixing_holidays = fixing_holidays;
    m_fixing_daycount = fixing_daycount;
    m_fixing_dateroll = fixing_dateroll;

    m_rate = rate;
    m_margin = margin;

    generate_flow();
  }

  DayCountConvention BulletFlow::get_daycount() const
  {
    return m_daycount;
  }

  Holidays BulletFlow::get_holidays() const
  {
    return m_holidays;
  }

  DateRollConvention BulletFlow::get_dateroll() const
  {
    return m_dateroll;
  }

  Date BulletFlow::get_fixing_start_date() const
  {
    return m_fixing_start_date;
  }

  Date BulletFlow::get_fixing_end_date() const
  {
    return m_fixing_start_date;
  }

  DayCountConvention BulletFlow::get_fixing_daycount() const
  {
    return m_fixing_daycount;
  }

  Holidays BulletFlow::get_fixing_holidays() const
  {
    return m_fixing_holidays;
  }

  DateRollConvention BulletFlow::get_fixing_dateroll() const
  {
    return m_fixing_dateroll;
  }

  double BulletFlow::get_rate() const
  {
    return m_rate;
  }

  double BulletFlow::get_margin() const
  {
    return m_margin;
  }

  void BulletFlow::set_daycount(DayCountConvention daycount)
  {
    m_daycount = daycount;
  }

  void BulletFlow::set_holidays(const Holidays & holidays)
  {
    m_holidays = holidays;
  }

  void BulletFlow::set_dateroll(DateRollConvention dateroll)
  {
    m_dateroll = dateroll;
  }

  void BulletFlow::set_fixing_start_date(const Date & fixing_start_date)
  {
    m_fixing_start_date = fixing_start_date;
  }

  void BulletFlow::set_fixing_end_date(const Date & fixing_end_date)
  {
    m_fixing_end_date = fixing_end_date;
  }

  void BulletFlow::set_fixing_daycount(DayCountConvention fixing_daycount)
  {
    m_fixing_daycount = fixing_daycount;
  }

  void BulletFlow::set_fixing_holidays(const Holidays & fixing_holidays)
  {
    m_fixing_holidays = fixing_holidays;
  }

  void BulletFlow::set_fixing_dateroll(DateRollConvention fixing_dateroll)
  {
    m_fixing_dateroll = fixing_dateroll;
  }

  void BulletFlow::set_rate(double rate)
  {
    m_rate = rate;
  }

  void BulletFlow::set_margin(double margin)
  {
    m_margin = margin;
  }

  void BulletFlow::generate_flow()
  {
    m_accrual = m_start_date.accrual_factor(m_end_date, m_daycount);
    m_fixing_accrual = m_fixing_start_date.accrual_factor(m_fixing_end_date, m_fixing_daycount);
  }

  void BulletFlow::dump(ostream & os) const
  {
    os.precision(MAX_PRECISION);
    os << m_type << std::endl;
    os << "SCHEDULE " << m_frequency << " " << m_daycount << " " << m_dateroll << std::endl;
    os << "FIXING " << m_frequency << " " << m_fixing_daycount << " " << m_fixing_dateroll << std::endl;
    os << "1" << std::endl;
    os << "1 " << m_start_date << " " << m_end_date << " " << m_accrual << " "
      << m_fixing_start_date << " " << m_fixing_end_date << " " << m_fixing_accrual << " "
      << m_rate << " " << m_margin;
  }

  void FlowTable::initialize(size_t size)
  {
    m_start_dates.resize(size);
    m_end_dates.resize(size);
    m_table_accrual.resize(size);
    m_fixing_start_dates.resize(size);
    m_fixing_end_dates.resize(size);
    m_fixing_accrual.resize(size);
    m_rates.resize(size);
    m_margins.resize(size);
  }

  FlowTable::FlowTable(Date start_date, Date end_date, FlowFrequency frequency, 
    const Holidays& table_holidays, DayCountConvention table_daycount, DateRollConvention table_dateroll,
    double rate, double margin)
    : Flow(start_date, end_date, FLOW_TABLE)
  {
    // This function assumes fixing conventions are same as table conventions
    m_frequency = frequency;
    m_table_holidays = table_holidays;
    m_table_daycount = table_daycount;
    m_table_dateroll = table_dateroll;
    m_fixing_holidays = table_holidays;
    m_fixing_daycount = table_daycount;
    m_fixing_dateroll = table_dateroll;

    generate_flow();
  }

  FlowTable::FlowTable(Date start_date, Date end_date, FlowFrequency frequency, 
    const Holidays& table_holidays, DayCountConvention table_daycount, DateRollConvention table_dateroll,
    const Holidays& fixing_holidays, DayCountConvention fixing_daycount, DateRollConvention fixing_dateroll,
    double rate, double margin)
    : Flow(start_date, end_date, FLOW_TABLE)
  {
    m_frequency = frequency;
    m_table_holidays = table_holidays;
    m_table_daycount = table_daycount;
    m_table_dateroll = table_dateroll;
    m_fixing_holidays = fixing_holidays;
    m_fixing_daycount = fixing_daycount;
    m_fixing_dateroll = fixing_dateroll;

    generate_flow();
  }

  vector<Date> FlowTable::get_start_dates() const
  {
    return m_start_dates;
  }

  vector<Date> FlowTable::get_end_dates() const
  {
    return m_end_dates;
  }

  vector<Date> FlowTable::get_fixing_start_dates() const
  {
    return m_fixing_start_dates;
  }

  vector<Date> FlowTable::get_fixing_end_dates() const
  {
    return m_fixing_end_dates;
  }

  vector<double> FlowTable::get_rates() const
  {
    return m_rates;
  }

  vector<double> FlowTable::get_margins() const
  {
    return m_margins;
  }

  FlowFrequency FlowTable::get_frequency() const
  {
    return m_frequency;
  }

  DayCountConvention FlowTable::get_table_daycount() const
  {
    return m_table_daycount;
  }

  DayCountConvention FlowTable::get_fixing_daycount() const
  {
    return m_fixing_daycount;
  }

  Holidays FlowTable::get_table_holidays() const
  {
    return m_table_holidays;
  }

  Holidays FlowTable::get_fixing_holidays() const
  {
    return m_fixing_holidays;
  }

  DateRollConvention FlowTable::get_table_dateroll() const
  {
    return m_table_dateroll;
  }

  DateRollConvention FlowTable::get_fixing_dateroll() const
  {
    return m_fixing_dateroll;
  }

  Date FlowTable::get_start_date(size_t index) const
  {
    if (index < m_start_dates.size())
    {
      ostringstream error_string;
      error_string << "Index " << index << " is greater than start dates size " << m_start_dates.size() << std::endl;
      throw(error_string.str().c_str());
    }
    return m_start_dates[index];
  }

  Date FlowTable::get_end_date(size_t index) const
  {
    if (index < m_end_dates.size())
    {
      ostringstream error_string;
      error_string << "Index " << index << " is greater than end dates size " << m_end_dates.size() << std::endl;
      throw(error_string.str().c_str());
    }

    return m_end_dates[index];
  }

  Date FlowTable::get_fixing_start_date(size_t index) const
  {
    if (index < m_fixing_start_dates.size())
    {
      ostringstream error_string;
      error_string << "Index " << index << " is greater than fixing start dates size " << m_fixing_start_dates.size() << std::endl;
      throw(error_string.str().c_str());
    }

    return m_fixing_start_dates[index];
  }

  Date FlowTable::get_fixing_end_date(size_t index) const
  {
    if (index < m_fixing_end_dates.size())
    {
      ostringstream error_string;
      error_string << "Index " << index << " is greater than fixing end dates size " << m_fixing_end_dates.size() << std::endl;
      throw(error_string.str().c_str());
    }

    return m_fixing_end_dates[index];
  }

  double FlowTable::get_rate(size_t index) const
  {
    if (index < m_rates.size())
    {
      ostringstream error_string;
      error_string << "Index " << index << " is greater than rates size " << m_rates.size() << std::endl;
      throw(error_string.str().c_str());
    }

    return m_rates[index];
  }

  double FlowTable::get_margin(size_t index) const
  {
    if (index < m_margins.size())
    {
      ostringstream error_string;
      error_string << "Index " << index << " is greater than margins size " << m_margins.size() << std::endl;
      throw(error_string.str().c_str());
    }

    return m_margins[index];
  }

  void FlowTable::set_start_dates(const vector<Date>& start_dates)
  {
    m_start_dates = start_dates;
  }

  void FlowTable::set_end_dates(const vector<Date>& end_dates)
  {
    m_end_dates = end_dates;
  }

  void FlowTable::set_fixing_start_dates(const vector<Date>& fixing_start_dates)
  {
    m_fixing_start_dates = fixing_start_dates;
  }

  void FlowTable::set_fixing_end_dates(const vector<Date>& fixing_end_dates)
  {
    m_fixing_end_dates = fixing_end_dates;
  }

  void FlowTable::set_rates(const vector<double>& rates)
  {
    m_rates = rates;
  }

  void FlowTable::set_margins(const vector<double>& margins)
  {
    m_margins = margins;
  }

  void FlowTable::set_frequency(const FlowFrequency frequency)
  {
    m_frequency = frequency;
  }

  void FlowTable::set_table_daycount(const DayCountConvention table_daycount)
  {
    m_table_daycount = table_daycount;
  }

  void FlowTable::set_fixing_daycount(const DayCountConvention fixing_daycount)
  {
    m_fixing_daycount = fixing_daycount;
  }

  void FlowTable::set_table_holidays(const Holidays table_holidays)
  {
    m_table_holidays = table_holidays;
  }

  void FlowTable::set_fixing_holidays(const Holidays fixing_holidays)
  {
    m_fixing_holidays = fixing_holidays;
  }

  void FlowTable::set_table_dateroll(DateRollConvention table_dateroll)
  {
    m_table_dateroll = table_dateroll;
  }

  void FlowTable::set_fixing_dateroll(DateRollConvention fixing_dateroll)
  {
    m_fixing_dateroll = fixing_dateroll;
  }

  void FlowTable::set_start_date(size_t index, const Date start_date)
  {
    if (index < m_start_dates.size())
    {
      ostringstream error_string;
      error_string << "Index " << index << " is greater than start dates size " << m_start_dates.size() << std::endl;
      throw(error_string.str().c_str());
    }

    m_start_dates[index] = start_date;
  }

  void FlowTable::set_end_date(size_t index, const Date end_date)
  {
    if (index < m_end_dates.size())
    {
      ostringstream error_string;
      error_string << "Index " << index << " is greater than end dates size " << m_end_dates.size() << std::endl;
      throw(error_string.str().c_str());
    }

    m_end_dates[index] = end_date;
  }

  void FlowTable::set_fixing_start_date(size_t index, const Date fixing_start_date)
  {
    if (index < m_fixing_start_dates.size())
    {
      ostringstream error_string;
      error_string << "Index " << index << " is greater than fixing start dates size " << m_fixing_start_dates.size() << std::endl;
      throw(error_string.str().c_str());
    }

    m_fixing_start_dates[index] = fixing_start_date;
  }

  void FlowTable::set_fixing_end_date(size_t index, const Date fixing_end_date)
  {
    if (index < m_fixing_end_dates.size())
    {
      ostringstream error_string;
      error_string << "Index " << index << " is greater than fixing end dates size " << m_fixing_end_dates.size() << std::endl;
      throw(error_string.str().c_str());
    }

    m_fixing_end_dates[index] = fixing_end_date;
  }

  void FlowTable::set_rate(size_t index, const double rate)
  {
    if (index < m_rates.size())
    {
      ostringstream error_string;
      error_string << "Index " << index << " is greater than rates size " << m_rates.size() << std::endl;
      throw(error_string.str().c_str());
    }

    m_rates[index] = rate;
  }

  void FlowTable::set_margin(size_t index, const double margin)
  {
    if (index < m_margins.size())
    {
      ostringstream error_string;
      error_string << "Index " << index << " is greater than margin size " << m_margins.size() << std::endl;
      throw(error_string.str().c_str());
    }

    m_margins[index] = margin;
  }

  void FlowTable::generate_flow()
  {
    size_t days = m_end_date - m_start_date;
    size_t number_of_flows = days / m_frequency;
    Tenor table_tenor(m_frequency, m_table_holidays, m_table_dateroll);
    Tenor fixing_tenor(m_frequency, m_fixing_holidays, m_fixing_dateroll);

    // Resize the flow vectors
    initialize(number_of_flows);

    Date date = m_start_date;

    for (int i = 0; i < number_of_flows; i++)
    {
      if (i == 0)
      {
        m_start_dates[i] = date;
        m_fixing_start_dates[i] = date;
      }
      else
      {
        m_start_dates[i] = m_end_dates[i-1];
        m_fixing_start_dates[i] = m_fixing_end_dates[i-1];
      }

      m_end_dates[i] = m_start_dates[i] + table_tenor;
      m_end_dates[i] = m_end_dates[i] < m_end_date ? m_end_dates[i] : m_end_date;
      m_table_accrual[i] = m_start_dates[i].accrual_factor(m_end_dates[i], m_table_daycount);

      m_fixing_end_dates[i] = m_fixing_start_dates[i] + fixing_tenor;
      m_fixing_accrual[i] = m_fixing_start_dates[i].accrual_factor(m_fixing_end_dates[i], m_table_daycount);
      
      m_rates[i] = 0;
      m_margins[i] = 0;
    }

    // Remember the last flow is any
    if (number_of_flows > 0 && m_end_dates[number_of_flows - 1] < m_end_date)
    {
      m_start_dates.push_back(m_end_dates[number_of_flows - 1]);
      m_fixing_start_dates.push_back(m_fixing_end_dates[number_of_flows - 1]);

      m_end_dates.push_back(m_end_date);
      m_table_accrual.push_back(m_start_dates[number_of_flows].accrual_factor(m_end_dates[number_of_flows], m_table_daycount));

      m_fixing_end_dates.push_back(m_end_date);
      m_fixing_accrual.push_back(m_fixing_start_dates[number_of_flows].accrual_factor(m_fixing_end_dates[number_of_flows], m_table_daycount));

      m_rates.push_back(0.0);
      m_margins.push_back(0.0);
    }
    else if(number_of_flows == 0)
    {
      m_start_dates.push_back(m_start_date);
      m_fixing_start_dates.push_back(m_start_date);

      m_end_dates.push_back(m_end_date);
      m_table_accrual.push_back(m_start_date.accrual_factor(m_end_date, m_table_daycount));

      m_fixing_end_dates.push_back(m_end_date);
      m_fixing_accrual.push_back(m_start_date.accrual_factor(m_end_date, m_table_daycount));

      m_rates.push_back(0.0);
      m_margins.push_back(0.0);
    }
  }

  void FlowTable::dump(ostream & os) const
  {
    os.precision(MAX_PRECISION);
    os << m_type << std::endl;
    os << "SCHEDULE " << m_frequency << " " << m_table_daycount << " " << m_table_dateroll << std::endl;
    os << "FIXING " << m_frequency << " " << m_fixing_daycount << " " << m_fixing_dateroll << std::endl;
    os << m_start_dates.size() << std::endl;
    for (int i = 0; i < m_start_dates.size(); i++)
    {
      os << i+1 << " " << m_start_dates[i] << " " << m_end_dates[i] << " " << m_table_accrual[i] << " "
        << m_fixing_start_dates[i] << " " << m_fixing_end_dates[i] << " " << m_fixing_accrual[i] << " "
        << m_rates[i] << " " << m_margins[i] << std::endl;
    }
  }
  ostream & operator<<(ostream & os, FlowType ft)
  {
    switch (ft)
    {
    case NONE_FLOW:
    {
      os << "NONE_FLOW";
      break;
    }
    case BULLET_FLOW:
    {
      os << "BULLET_FLOW";
      break;
    }
    case FLOW_TABLE:
    {
      os << "FLOW_TABLE";
      break;
    }
    }

    return os;
  }

  ostream & operator<<(ostream & os, const Flow & f)
  {
    f.dump(os);
    return os;
  }
}
