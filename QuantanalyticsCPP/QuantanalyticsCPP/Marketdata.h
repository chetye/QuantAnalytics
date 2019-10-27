#pragma once
#include <sstream>
#include <string>
using namespace std;

namespace Marketdata
{
	enum MarketdataType
	{
		INTEREST_RATE_CURVE,
		SWAPTION_VOLATILITY,
		CAPLET_VOLATILITY, 
    NONE
	};

	class Marketdata
	{
  private:
    MarketdataType m_type;
	public:
    virtual MarketdataType get_type() { return NONE; };
		virtual void read(const string marketdata_stream) = 0;
    virtual void dump(string& marketdata_stream) = 0;

		Marketdata();
		virtual ~Marketdata();
	};
}

