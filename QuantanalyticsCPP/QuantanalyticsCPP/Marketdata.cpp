#include "Marketdata.h"

namespace marketdata
{
  Marketdata::Marketdata(date::Date market_date, MarketdataType type)
    : m_type(type), m_market_date(market_date)
  {
  }

  date::Date Marketdata::get_market_date() const
  {
    return m_market_date;
  }

  void Marketdata::set_market_date(date::Date market_date)
  {
    m_market_date = market_date;
  }

  MarketdataType Marketdata::get_type() const
  {
    return m_type;
  }

  void Marketdata::dump(ostream & os) const
  {
    os << m_type << std::endl;
    os << m_market_date << std::endl;
  }

  ostream & operator<<(ostream & os, const Marketdata & marketdata)
  {
    marketdata.dump(os);
    return os;
  }
}
