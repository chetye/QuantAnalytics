#include "Swap.h"
#include "Pricer.h"
#include "Curve.h"

namespace instrument
{
  Swap::Swap(string firstleg_currency, string firstleg_index, date::FlowTable firstleg_flow, 
    string secondleg_currency, string secondleg_index, date::FlowTable secondleg_flow, double notional)
    : Instrument(firstleg_flow.get_start_date(0), firstleg_flow.get_end_date(firstleg_flow.get_end_dates().size()-1), SWAP, notional),
    m_firstleg_currency(firstleg_currency), m_firstleg_index(firstleg_index), m_firstleg_flow(firstleg_flow), 
    m_secondleg_currency(secondleg_currency), m_secondleg_index(secondleg_index), m_secondleg_flow(secondleg_flow)
  {
    if ((m_firstleg_currency == m_secondleg_currency) && (m_firstleg_index == m_secondleg_index) &&
      m_firstleg_flow.get_frequency() == m_secondleg_flow.get_frequency())
    {
      throw std::runtime_error("Invalid Swap. Currency, Index and Frequency for both legs are same");
    }
  }

  Swap::~Swap()
  {
  }

  double Swap::payoff(const pricer::Pricer & pricer) const
  {
    return 0.0;
  }

  double Swap::par_rate(const pricer::Pricer& pricer) const
  {
    boost::shared_ptr<marketdata::Marketdata> p_marketdata = pricer.get_model()->get_marketdata(marketdata::CURVE);
    boost::shared_ptr<marketdata::Curve> p_curve = boost::static_pointer_cast<marketdata::Curve>(p_marketdata);

    double first_leg_price = 0;
    if (m_firstleg_index == "")
    {
      // Fixed Flow
      for (int i = 0; i < m_firstleg_flow.get_end_dates().size(); i++)
      {
        double discount_factor = p_curve->get_discount_factor(m_firstleg_flow.get_end_date(i));
        double accrual = m_firstleg_flow.get_fixing_accrual(i);
        double rate = m_firstleg_flow.get_rate(i);
        double margin = m_firstleg_flow.get_margin(i);
         
        first_leg_price += (rate + margin) * accrual * discount_factor;
      }
    }
    else
    {
      // Floating Flow
      for (int i = 0; i < m_firstleg_flow.get_end_dates().size(); i++)
      {
        double discount_factor = p_curve->get_discount_factor(m_firstleg_flow.get_end_date(i));
        double accrual = m_firstleg_flow.get_fixing_accrual(i);
        date::Date start_date = m_firstleg_flow.get_start_date(i);
        date::Date end_date = m_firstleg_flow.get_end_date(i);
        date::FlowFrequency frequency = m_firstleg_flow.get_frequency();
        double forward = p_curve->get_forward(m_firstleg_index, frequency, start_date, end_date);
        double margin = m_firstleg_flow.get_margin(i);

        first_leg_price += (forward + margin) * accrual * discount_factor;
      }
    }

    double second_leg_price = 0;
    if (m_secondleg_index == "")
    {
      // Fixed Flow
      for (int i = 0; i < m_secondleg_flow.get_end_dates().size(); i++)
      {
        double discount_factor = p_curve->get_discount_factor(m_secondleg_flow.get_end_date(i));
        double accrual = m_secondleg_flow.get_fixing_accrual(i);
        double rate = m_secondleg_flow.get_rate(i);
        double margin = m_secondleg_flow.get_margin(i);

        second_leg_price += (rate + margin) * accrual * discount_factor;
      }
    }
    else
    {
      for (int i = 0; i < m_secondleg_flow.get_end_dates().size(); i++)
      {
        // Floating Flow
        double discount_factor = p_curve->get_discount_factor(m_secondleg_flow.get_end_date(i));
        double accrual = m_secondleg_flow.get_fixing_accrual(i);
        date::Date start_date = m_secondleg_flow.get_start_date(i);
        date::Date end_date = m_secondleg_flow.get_end_date(i);
        date::FlowFrequency frequency = m_secondleg_flow.get_frequency();
        double forward = p_curve->get_forward(m_secondleg_index, frequency, start_date, end_date);
        double margin = m_secondleg_flow.get_margin(i);

        second_leg_price += (forward + margin) * accrual * discount_factor;
      }
    }

    double par_rate = first_leg_price - second_leg_price;

    return par_rate;
  }

  double Swap::price(const pricer::Pricer& pricer) const
  {
    double price = 0;
    price = par_rate(pricer);
    return price;
  }

}
