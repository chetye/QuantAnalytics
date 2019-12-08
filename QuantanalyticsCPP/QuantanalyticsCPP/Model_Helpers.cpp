#include "Model_Helpers.h"

namespace model
{

  ostream & operator>>(ostream & os, const ModelType & model_type)
  {
    switch (model_type)
    {
    case NONE_MODEL:
    {
      os << "NONE_MODEL";
      break;
    }
    case INTEREST_RATE:
    {
      os << "INTEREST_RATE";
      break;
    }
    case BLACK_SCHOLES:
    {
      os << "BLACK_SCHOLES";
      break;
    }
    case VASICHEK:
    {
      os << "VASICHEK";
      break;
    }
    case COX_INGERSOL_ROSS:
    {
      os << "COX_INGERSOL_ROSS";
      break;
    }
    case HULL_AND_WHITE_I:
    {
      os << "HULL_AND_WHITE_I";
      break;
    }
    case HULL_AND_WHITE_II:
    {
      os << "HULL_AND_WHITE_II";
      break;
    }
    case LIBOR_MARKET_MODEL:
    {
      os << "LIBOR_MARKET_MODEL";
      break;
    }
    }

    return os;
  }
}
