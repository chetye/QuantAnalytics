#pragma once

#include "Instrument.h"
#include "Model.h"

#include <boost/shared_ptr.hpp>

namespace pricer
{
  class Pricer
  {
  private:
    boost::shared_ptr<instrument::Instrument> m_p_instrument;
    boost::shared_ptr<model::Model> m_p_model;

  public:
    Pricer(const boost::shared_ptr<instrument::Instrument> p_instrument, const boost::shared_ptr<model::Model> p_model);
    ~Pricer();

    // utilities
    boost::shared_ptr<model::Model> get_model() const;
    boost::shared_ptr<instrument::Instrument> get_instrument() const;
  };
}

