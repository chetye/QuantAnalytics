#include "IR_Pricer.h"

namespace pricer
{

  IR_Pricer::IR_Pricer(const boost::shared_ptr<instrument::Instrument> p_instrument, const boost::shared_ptr<model::Model> p_model)
    : Pricer(p_instrument, p_model)
  {
  }


  IR_Pricer::~IR_Pricer()
  {
  }
}
