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

    // pricing functions
    virtual double payoff(const pricer::Pricer& pricer) const;
    virtual double par_rate(const pricer::Pricer& pricer) const;
    virtual double price(const pricer::Pricer& pricer) const;
  };

  // support typedefs
  typedef boost::shared_ptr<Deposit> Deposit_Ptr;
}
