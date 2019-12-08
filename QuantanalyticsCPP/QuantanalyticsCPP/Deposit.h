#pragma once
#include "Instrument.h"
#include "Date.h"
#include "Flow.h"

namespace instrument
{
  class Deposit : public Instrument
  {
  protected:
    date::BulletFlow m_bullet_flow;
    string m_currency;

  public:
    Deposit(date::BulletFlow flow, string currency, double notional = 1.0);
    ~Deposit();

    // utilities
    virtual void dump(ostream& os) const;
  };
}
