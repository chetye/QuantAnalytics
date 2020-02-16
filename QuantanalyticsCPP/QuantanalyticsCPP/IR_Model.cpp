#include "IR_Model.h"

namespace model
{
  IR_Model::IR_Model(const boost::shared_ptr<marketdata::Marketdata> p_curve)
    : Model(INTEREST_RATE), m_p_curve(p_curve)
  {
  }

  IR_Model::~IR_Model()
  {
  }

  void model::IR_Model::dump(ostream & os) const
  {
    os << m_type << endl;
    os << *m_p_curve;
  }

  boost::shared_ptr<marketdata::Marketdata> model::IR_Model::get_marketdata(marketdata::MarketdataType market_type) const
  {
    return m_p_curve; // has only one marketdata in this model, so return it
  }
}
