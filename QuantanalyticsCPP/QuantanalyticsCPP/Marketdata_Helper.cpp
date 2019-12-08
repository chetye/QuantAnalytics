#include "Marketdata_Helper.h"

namespace marketdata
{
  ostream & operator>>(ostream & os, const MarketdataType & marketdata_type)
  {
    switch (marketdata_type)
    {
    case NONE_MARKETDATA:
    {
      os << "NONE_MARKETDATA";
      break;
    }
    case CURVE:
    {
      os << "INTEREST_RATE_CURVE";
      break;
    }
    case SPOT:
    {
      os << "SPOT";
      break;
    }
    /*case SWAPTION_VOLATILITY:
    {
      os << "SWAPTION_VOLATILITY";
      break;
    }
    case CAPLET_VOLATILITY:
    {
      os << "CAPLET_VOLATILITY";
      break;
    }*/
    }

    return os;
  }
}
