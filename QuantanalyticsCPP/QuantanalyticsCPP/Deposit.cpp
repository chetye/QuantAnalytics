#include "Deposit.h"

namespace instrument
{
  Deposit::Deposit(date::BulletFlow flow, string currency, double notional)
    : Instrument(flow.get_start_date(), flow.get_end_date(), DEPOSIT, notional), m_bullet_flow(flow), m_currency(currency)
  {
  }

  Deposit::~Deposit()
  {
  }

  void Deposit::dump(ostream & os) const
  {
    os << m_type << std::endl;
    os << "Notional " << m_notional << std::endl;
    os << m_bullet_flow;
  }
}
