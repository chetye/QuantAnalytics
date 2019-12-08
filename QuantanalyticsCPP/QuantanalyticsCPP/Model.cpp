#include "Model.h"

namespace model
{
  Model::Model()
  {
  }


  Model::~Model()
  {
  }

  ModelType Model::get_type() const
  {
    return m_type;
  }

  ostream & operator<<(ostream & os, const Model & model)
  {
    model.dump(os);
    return os;
  }
}
