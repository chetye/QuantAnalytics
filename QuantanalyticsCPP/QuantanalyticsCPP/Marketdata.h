#pragma once
#include <sstream>
#include <string>
using namespace std;

#include "Constants.h"
#include "Marketdata_Helper.h"
#include "Date.h"

namespace marketdata
{
	class Marketdata
	{
  protected:
    MarketdataType m_type;
    date::Date m_market_date;

	public:
    // constructor
    Marketdata(date::Date market_date, MarketdataType type = NONE_MARKETDATA);

    // getter methods
    date::Date get_market_date() const;

    // setter methods
    void set_market_date(date::Date market_date);

    // utilities
    virtual MarketdataType get_type() const;
    void dump(ostream& os) const;
	};

  ostream& operator<<(ostream& os, const Marketdata& marketdata);
}

