#pragma once
#include "Model.h"
#include "Marketdata.h"

namespace model
{
  class IR_Model : public Model
  {
  private:
    boost::shared_ptr<marketdata::Marketdata> m_p_curve;

  public:
    IR_Model(const boost::shared_ptr<marketdata::Marketdata> p_curve);
    ~IR_Model();

    virtual void dump(ostream& os) const;
    virtual boost::shared_ptr<marketdata::Marketdata> get_marketdata(marketdata::MarketdataType market_type) const;
  };

  //// support functions
  //ostream& operator<<(ostream& os, const IR_Model& ir_model);

  // support typedefs
  typedef boost::shared_ptr<IR_Model> IR_Model_Ptr;
}

