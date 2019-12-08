#pragma once

#include "Instrument_Helpers.h"
#include "Date.h"

namespace instrument
{
  class Instrument
  {
  protected:
    InstrumentType m_type;
    double m_notional;
    date::Date m_start_date;
    date::Date m_end_date;
    
  public:
    // Constructors
    Instrument(date::Date start_date, date::Date end_date, InstrumentType type = NONE_INSTRUMENT, double notional = 1.0);
    ~Instrument();

    // getter methods
    double get_notional() const;

    // setter methods
    void set_notional(double d);

    // utilities
    InstrumentType get_type() const;
    virtual void dump(ostream& os) const;
    date::Date get_start_date() const;
    date::Date get_end_date() const;
  };

  // support functions
  ostream& operator<<(ostream& os, Instrument& instrument);
}
