#include "Instrument_Helpers.h"

namespace instrument
{
  ostream & operator>>(ostream & os, const InstrumentType & instrument_type)
  {
    switch (instrument_type)
    {
    case NONE_INSTRUMENT:
    {
      os << "NONE_INSTRUMENT";
      break;
    }
    case DEPOSIT:
    {
      os << "DEPOSIT";
      break;
    }
    case FRA:
    {
      os << "FRA";
      break;
    }
    case FUTURE:
    {
      os << "FUTURE";
      break;
    }
    case SWAP:
    {
      os << "SWAP";
      break;
    }
    case CAPLET:
    {
      os << "CAPLET";
      break;
    }
    case FLOORLET:
    {
      os << "FLOORLET";
      break;
    }
    case CAP:
    {
      os << "CAP";
      break;
    }
    case FLOOR:
    {
      os << "FLOOR";
      break;
    }
    case SWAPTION:
    {
      os << "DEPOSIT";
      break;
    }
    }

    return os;
  }
}
