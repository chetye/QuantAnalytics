#pragma once
#include "Instrument.h"
#include "Flow.h"

namespace instrument
{
  class Swap : public Instrument
  {
  protected:
    date::FlowTable m_firstleg_flow;
    date::FlowTable m_secondleg_flow;
    string m_firstleg_currency;
    string m_firstleg_index;
    string m_secondleg_currency;
    string m_secondleg_index;

  public:
    Swap(string firstleg_currency, string firstleg_index, date::FlowTable firstleg_flow, 
      string secondleg_currency, string secondleg_index, date::FlowTable secondleg_flow, double notional = 1.0);
    ~Swap();

    // utilities
    virtual void dump(ostream& os) const {}

    // pricing functions
    virtual double payoff(const pricer::Pricer& pricer) const;
    virtual double par_rate(const pricer::Pricer& pricer) const;
    virtual double price(const pricer::Pricer& pricer) const;
  };

  // support typedefs
  typedef boost::shared_ptr<Swap> Swap_Ptr;
}
