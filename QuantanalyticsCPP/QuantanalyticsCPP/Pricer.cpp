#include "Pricer.h"

namespace pricer
{
  Pricer::Pricer(const boost::shared_ptr<instrument::Instrument> p_instrument, const boost::shared_ptr<model::Model> p_model)
    : m_p_instrument(p_instrument), m_p_model(p_model)
  {
  }

  Pricer::~Pricer()
  {
  }

  boost::shared_ptr<model::Model> Pricer::get_model() const
  {
    return m_p_model;
  }

  boost::shared_ptr<instrument::Instrument> Pricer::get_instrument() const
  {
    return m_p_instrument;
  }
}