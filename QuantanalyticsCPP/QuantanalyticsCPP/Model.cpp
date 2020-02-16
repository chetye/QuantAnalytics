#include "Model.h"

#include <sstream>
using namespace std;

namespace model
{
  Model::Model(ModelType type)
    : m_type(type)
  {
  }

  Model::~Model()
  {
  }

  ModelType Model::get_type() const
  {
    return m_type;
  }

  void Model::dump(ostream & os) const
  {
    // should never come here
    ostringstream error_string;
    error_string << "Model::dump() is not available";
    std::exception(error_string.str().c_str());
  }

  boost::shared_ptr<marketdata::Marketdata> Model::get_marketdata(marketdata::MarketdataType market_type) const
  {
    // should never come here
    ostringstream error_string;
    error_string << "Model::get_marketdata() is not available";
    std::exception(error_string.str().c_str());
    return boost::shared_ptr<marketdata::Marketdata>(new marketdata::Marketdata(date::Date()));
  }

  ostream & operator<<(ostream & os, const Model & model)
  {
    model.dump(os);
    return os;
  }
}
