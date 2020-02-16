#include "Instrument.h"

namespace instrument
{
  Instrument::Instrument(date::Date start_date, date::Date end_date, InstrumentType type, double notional)
    : m_type(type), m_start_date(start_date), m_end_date(end_date), m_notional(1.0)
  {
  }

  Instrument::~Instrument()
  {
  }

  double Instrument::get_notional() const
  {
    return m_notional;
  }

  void Instrument::set_notional(double d)
  {
    m_notional = d;
  }

  InstrumentType Instrument::get_type() const
  {
    return m_type;
  }

  void Instrument::dump(ostream & os) const
  {
    // should never come here
    ostringstream error_string;
    error_string << "Instrument::dump() is not available";
    std::exception(error_string.str().c_str());
  }

  date::Date Instrument::get_start_date() const
  {
    return m_start_date;
  }

  date::Date Instrument::get_end_date() const
  {
    return m_end_date;
  }

  double Instrument::payoff(const pricer::Pricer& pricer) const
  {
    // should never come here
    ostringstream error_string;
    error_string << "Instrument::payoff() is not available";
    std::exception(error_string.str().c_str());

    return 0;
  }

  double Instrument::par_rate(const pricer::Pricer& pricer) const
  {
    // should never come here
    ostringstream error_string;
    error_string << "Instrument::par_rate() is not available";
    std::exception(error_string.str().c_str());

    return 0;
  }

  double Instrument::price(const pricer::Pricer& pricer) const
  {
    // should never come here
    ostringstream error_string;
    error_string << "Instrument::price() is not available";
    std::exception(error_string.str().c_str());

    return 0;
  }

  ostream & operator<<(ostream & os, Instrument & instrument)
  {
    instrument.dump(os);
    return os;
  }
}
