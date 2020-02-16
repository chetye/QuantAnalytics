#include "Deposit.h"
#include "Pricer.h"
#include "Curve.h"

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

  double Deposit::payoff(const pricer::Pricer& pricer) const
  {
    return 0.0;
  }

  double Deposit::par_rate(const pricer::Pricer& pricer) const
  {
    double par_rate = 0;

    boost::shared_ptr<marketdata::Marketdata> p_marketdata = pricer.get_model()->get_marketdata(marketdata::CURVE);
    boost::shared_ptr<marketdata::Curve> p_curve = boost::static_pointer_cast<marketdata::Curve>(p_marketdata);

    const double discount_factor = p_curve->get_discount_factor(m_bullet_flow.get_end_date());
    par_rate = std::pow(discount_factor, -1 / m_bullet_flow.get_accrual()) - 1; // assuming annual rate

    return par_rate;
  }

  double Deposit::price(const pricer::Pricer& pricer) const
  {
    double price = 0;
    price = par_rate(pricer) - m_bullet_flow.get_rate();
    return price;
  }
}
