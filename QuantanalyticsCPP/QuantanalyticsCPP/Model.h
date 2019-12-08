#pragma once

#include "Model_Helpers.h"

namespace model
{
  class Model
  {
  protected:
    ModelType m_type;
  public:
    // constructors
    Model();
    ~Model();

    // utilities
    ModelType get_type() const;
    virtual void dump(ostream& os) const = 0;
  };

  ostream& operator<<(ostream& os, const Model& model);
}

