#pragma once
#include <iostream>
using namespace std;

namespace marketdata
{
  enum MarketdataType
  {
    NONE_MARKETDATA,
    CURVE,
    SPOT
  };

  ostream& operator>>(ostream& os, const MarketdataType& marketdata_type);
}

