#include "Swap.h"

namespace instrument
{
  Swap::Swap(string firstleg_currency, string firstleg_index, date::FlowTable firstleg_flow, 
    string secondleg_currency, string secondleg_index, date::FlowTable secondleg_flow, double notional)
    : Instrument(firstleg_flow.get_start_date(0), firstleg_flow.get_end_date(firstleg_flow.get_end_dates().size()-1), SWAP, notional),
    m_firstleg_currency(firstleg_currency), m_firstleg_index(firstleg_index), m_firstleg_flow(firstleg_flow), 
    m_secondleg_currency(secondleg_currency), m_secondleg_index(secondleg_index), m_secondleg_flow(secondleg_flow)
  {
  }

  Swap::~Swap()
  {
  }

}
