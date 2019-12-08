#pragma once
#include <iostream>
using namespace std;

namespace model
{
  enum ModelType
  {
    NONE_MODEL,
    INTEREST_RATE,
    BLACK_SCHOLES,
    VASICHEK,
    COX_INGERSOL_ROSS,
    HULL_AND_WHITE_I,
    HULL_AND_WHITE_II,
    LIBOR_MARKET_MODEL
  };

  ostream& operator>>(ostream& os, const ModelType& model_type);
}

