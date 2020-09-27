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
    double par_rate = 0.0;
    double margin = 0.0;

    boost::shared_ptr<marketdata::Marketdata> p_marketdata = pricer.get_model()->get_marketdata(marketdata::CURVE);

    // First leg
    boost::shared_ptr<marketdata::Interest_Rate_Curve_System> p_curve = boost::static_pointer_cast<marketdata::Interest_Rate_Curve_System>(p_marketdata);
    boost::shared_ptr<marketdata::Interest_Rate_Curve_System> p_firstleg_curve;
    if (p_curve->get_currency() == m_firstleg_currency)
    {
      p_firstleg_curve = p_curve;
    }
    else if (p_curve->get_external_curve_system() != NULL && p_curve->get_external_curve_system()->get_currency() == m_firstleg_currency)
    {
      p_firstleg_curve = p_curve->get_external_curve_system();
    }
    else
    {
      throw std::runtime_error("Curve for " + m_firstleg_currency + " leg not found in the curve system");
    }

    boost::shared_ptr<marketdata::Interest_Rate_Curve_System> p_secondleg_curve;
    if (p_curve->get_currency() == m_secondleg_currency)
    {
      p_secondleg_curve = p_curve;
    }
    else if (p_curve->get_external_curve_system() != NULL && p_curve->get_external_curve_system()->get_currency() == m_secondleg_currency)
    {
      p_secondleg_curve = p_curve->get_external_curve_system();
    }
    else
    {
      throw std::runtime_error("Curve for " + m_secondleg_currency + " leg not found in the curve system");
    }

    if (m_firstleg_index == "" && m_secondleg_index == "") // Fix vs Fix
    {
      double first_leg_annuity = 0;
      // Fixed Flow
      for (int i = 0; i < m_firstleg_flow.get_end_dates().size(); i++)
      {
        double discount_factor = p_firstleg_curve->get_discount_factor(m_firstleg_flow.get_end_date(i));
        double accrual = m_firstleg_flow.get_fixing_accrual(i);
        margin = m_firstleg_flow.get_margin(i);

        first_leg_annuity += accrual * discount_factor;
      }

      double second_leg_price = 0;
      // Fixed Flow
      for (int i = 0; i < m_secondleg_flow.get_end_dates().size(); i++)
      {
        double discount_factor = p_secondleg_curve->get_discount_factor(m_secondleg_flow.get_end_date(i));
        double accrual = m_secondleg_flow.get_fixing_accrual(i);
        double rate = m_secondleg_flow.get_rate(i);
        double margin = m_secondleg_flow.get_margin(i);

        second_leg_price += (rate + margin) * accrual * discount_factor;
      }

      par_rate = second_leg_price / first_leg_annuity - margin; // margin from first leg
    }
    else if (m_firstleg_index == "" && m_secondleg_index != "") // Fix vs Floating
    {
      double first_leg_annuity = 0;
      // Fixed Flow
      for (int i = 0; i < m_firstleg_flow.get_end_dates().size(); i++)
      {
        double discount_factor = p_firstleg_curve->get_discount_factor(m_firstleg_flow.get_end_date(i));
        double accrual = m_firstleg_flow.get_fixing_accrual(i);
        margin = m_firstleg_flow.get_margin(i);

        first_leg_annuity += accrual * discount_factor;
      }

      double second_leg_price = 0;
      // Floating Flow
      for (int i = 0; i < m_firstleg_flow.get_end_dates().size(); i++)
      {
        double discount_factor = p_secondleg_curve->get_discount_factor(m_firstleg_flow.get_end_date(i));
        double accrual = m_firstleg_flow.get_fixing_accrual(i);
        date::Date start_date = m_firstleg_flow.get_start_date(i);
        date::Date end_date = m_firstleg_flow.get_end_date(i);
        date::FlowFrequency frequency = m_firstleg_flow.get_frequency();
        double forward = p_secondleg_curve->get_forward_rate(m_firstleg_index, frequency, start_date, end_date);
        double margin = m_firstleg_flow.get_margin(i);

        second_leg_price += (forward + margin) * accrual * discount_factor;
      }

      par_rate = second_leg_price / first_leg_annuity - margin;
    }
    else if (m_firstleg_index != "" && m_secondleg_index == "") // Floating vs Fix
    {
      double first_leg_price = 0;
      for (int i = 0; i < m_firstleg_flow.get_end_dates().size(); i++)
      {
        double discount_factor = p_firstleg_curve->get_discount_factor(m_firstleg_flow.get_end_date(i));
        double accrual = m_firstleg_flow.get_fixing_accrual(i);
        date::Date start_date = m_firstleg_flow.get_start_date(i);
        date::Date end_date = m_firstleg_flow.get_end_date(i);
        date::FlowFrequency frequency = m_firstleg_flow.get_frequency();
        double forward = p_firstleg_curve->get_forward_rate(m_firstleg_index, frequency, start_date, end_date);
        double margin = m_firstleg_flow.get_margin(i);

        first_leg_price += (forward + margin) * accrual * discount_factor;
      }

      double second_leg_annuity = 0;
      // Fixed Flow
      for (int i = 0; i < m_secondleg_flow.get_end_dates().size(); i++)
      {
        double discount_factor = p_secondleg_curve->get_discount_factor(m_secondleg_flow.get_end_date(i));
        double accrual = m_secondleg_flow.get_fixing_accrual(i);
        margin = m_secondleg_flow.get_margin(i);

        second_leg_annuity += accrual * discount_factor;
      }

      par_rate = first_leg_price / second_leg_annuity - margin;
    }
    else // Floating vs Floating
    {
      double first_leg_price = 0;
      double first_leg_annuity = 0;
      for (int i = 0; i < m_firstleg_flow.get_end_dates().size(); i++)
      {
        double discount_factor = p_firstleg_curve->get_discount_factor(m_firstleg_flow.get_end_date(i));
        double accrual = m_firstleg_flow.get_fixing_accrual(i);
        date::Date start_date = m_firstleg_flow.get_start_date(i);
        date::Date end_date = m_firstleg_flow.get_end_date(i);
        date::FlowFrequency frequency = m_firstleg_flow.get_frequency();
        double forward = p_firstleg_curve->get_forward_rate(m_firstleg_index, frequency, start_date, end_date);

        first_leg_price += forward * accrual * discount_factor;
        first_leg_annuity += accrual * discount_factor;
      }

      double second_leg_price = 0;
      // Floating Flow
      for (int i = 0; i < m_firstleg_flow.get_end_dates().size(); i++)
      {
        double discount_factor = p_secondleg_curve->get_discount_factor(m_firstleg_flow.get_end_date(i));
        double accrual = m_firstleg_flow.get_fixing_accrual(i);
        date::Date start_date = m_firstleg_flow.get_start_date(i);
        date::Date end_date = m_firstleg_flow.get_end_date(i);
        date::FlowFrequency frequency = m_firstleg_flow.get_frequency();
        double forward = p_secondleg_curve->get_forward_rate(m_firstleg_index, frequency, start_date, end_date);
        double margin = m_firstleg_flow.get_margin(i);

        second_leg_price += (forward + margin) * accrual * discount_factor;
      }

      par_rate = (second_leg_price - first_leg_price) / first_leg_annuity;
    }

    return par_rate;
  }

  double Swap::price(const pricer::Pricer& pricer) const
  {
    double price = 0.0;

    boost::shared_ptr<marketdata::Marketdata> p_marketdata = pricer.get_model()->get_marketdata(marketdata::CURVE);

    // First leg
    boost::shared_ptr<marketdata::Interest_Rate_Curve_System> p_curve = boost::static_pointer_cast<marketdata::Interest_Rate_Curve_System>(p_marketdata);
    boost::shared_ptr<marketdata::Interest_Rate_Curve_System> p_firstleg_curve;
    if (p_curve->get_currency() == m_firstleg_currency)
    {
      p_firstleg_curve = p_curve;
    }
    else if (p_curve->get_external_curve_system() != NULL && p_curve->get_external_curve_system()->get_currency() == m_firstleg_currency)
    {
      p_firstleg_curve = p_curve->get_external_curve_system();
    }
    else 
    {
      throw std::runtime_error("Curve for " + m_firstleg_currency + " leg not found in the curve system");
    }

    double first_leg_price = 0;
    if (m_firstleg_index == "")
    { 
      // Fixed Flow
      for (int i = 0; i < m_firstleg_flow.get_end_dates().size(); i++)
      {
        double discount_factor = p_firstleg_curve->get_discount_factor(m_firstleg_flow.get_end_date(i));
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
        double discount_factor = p_firstleg_curve->get_discount_factor(m_firstleg_flow.get_end_date(i));
        double accrual = m_firstleg_flow.get_fixing_accrual(i);
        date::Date start_date = m_firstleg_flow.get_start_date(i);
        date::Date end_date = m_firstleg_flow.get_end_date(i);
        date::FlowFrequency frequency = m_firstleg_flow.get_frequency();
        double forward = p_firstleg_curve->get_forward_rate(m_firstleg_index, frequency, start_date, end_date);
        double margin = m_firstleg_flow.get_margin(i);

        first_leg_price += (forward + margin) * accrual * discount_factor;
      }
    }

    // Second leg
    boost::shared_ptr<marketdata::Interest_Rate_Curve_System> p_secondleg_curve;
    if (p_curve->get_currency() == m_secondleg_currency)
    {
      p_secondleg_curve = p_curve;
    }
    else if (p_curve->get_external_curve_system() != NULL && p_curve->get_external_curve_system()->get_currency() == m_secondleg_currency)
    {
      p_secondleg_curve = p_curve->get_external_curve_system();
    }
    else
    {
      throw std::runtime_error("Curve for " + m_secondleg_currency + " leg not found in the curve system");
    }

    double second_leg_price = 0;
    if (m_secondleg_index == "")
    {
      // Fixed Flow
      for (int i = 0; i < m_secondleg_flow.get_end_dates().size(); i++)
      {
        double discount_factor = p_secondleg_curve->get_discount_factor(m_secondleg_flow.get_end_date(i));
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
        double discount_factor = p_secondleg_curve->get_discount_factor(m_secondleg_flow.get_end_date(i));
        double accrual = m_secondleg_flow.get_fixing_accrual(i);
        date::Date start_date = m_secondleg_flow.get_start_date(i);
        date::Date end_date = m_secondleg_flow.get_end_date(i);
        date::FlowFrequency frequency = m_secondleg_flow.get_frequency();
        double forward = p_secondleg_curve->get_forward_rate(m_secondleg_index, frequency, start_date, end_date);
        double margin = m_secondleg_flow.get_margin(i);

        second_leg_price += (forward + margin) * accrual * discount_factor;
      }
    }

    price = first_leg_price - second_leg_price;

    return price;
  }

}
