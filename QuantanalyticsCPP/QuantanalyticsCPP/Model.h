#pragma once

#include "Model_Helpers.h"
#include "Marketdata.h"

#include <boost/shared_ptr.hpp>

namespace model
{
  class Model
  {
  protected:
    ModelType m_type;

  public:
    // constructors
    Model(ModelType type = NONE_MODEL);
    ~Model();

    // utilities
    ModelType get_type() const;
    virtual void dump(ostream& os) const;
    virtual boost::shared_ptr<marketdata::Marketdata> get_marketdata(marketdata::MarketdataType market_type) const;
  };

  // support functions
  ostream& operator<<(ostream& os, const Model& model);

  // support typedefs
  typedef boost::shared_ptr<Model> Model_Ptr;

}

