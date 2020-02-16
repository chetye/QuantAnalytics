#pragma once
#include "Pricer.h"

namespace pricer
{
  class IR_Pricer : public Pricer
  {
  public:
    IR_Pricer(const boost::shared_ptr<instrument::Instrument> p_instrument, const boost::shared_ptr<model::Model> p_model);
    ~IR_Pricer();
  };
}