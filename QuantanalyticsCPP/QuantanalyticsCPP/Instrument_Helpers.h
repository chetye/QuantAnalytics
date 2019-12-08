#pragma once
#include <iostream>
using namespace std;

namespace instrument
{
  enum InstrumentType
  {
    NONE_INSTRUMENT,
    DEPOSIT,
    FRA,
    FUTURE,
    SWAP,
    CAPLET,
    FLOORLET,
    CAP,
    FLOOR,
    SWAPTION
  };

  ostream& operator>>(ostream& os, const InstrumentType& instrument_type);
}

