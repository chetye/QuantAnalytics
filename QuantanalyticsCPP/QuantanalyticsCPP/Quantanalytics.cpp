#include <iostream>
#include <sstream>
using namespace std;

#include "Date.h"
#include "Flow.h"
#include "Instrument.h"
#include "Deposit.h"
#include "Swap.h"
#include "Curve.h"
using namespace date;

int main()
{
	cout << "This is the Quantitative Analytics Library" << endl;

  Date start_date("31-12-2018");
  cout << start_date << endl;

  if (false)
  {
    // Date Maths
    cout << "Date Mathematics" << endl;

    Date end_date;
    for (int i = 0; i < 30; i++)
    {
      int days = i * 100;
      end_date = start_date - days;
      cout << "Start Date " << start_date << " -" << days << " = " << end_date << " | ";
      end_date = end_date + days;
      cout << "+" << days << " = " << end_date << endl;
    }

    Holidays japan_holidays(TOKYO, NEW_YORK);
    Tenor tenor("T/N", japan_holidays, MODIFIED_FOLLOWING);
    Date expiry_date = start_date + tenor;
    cout << expiry_date << endl;
    start_date.accrual_factor(expiry_date, D30_360);
  }
  
  if (false)
  {
    // Flow Maths
    cout << "Flow Mathematics" << endl;
    Holidays japan_holidays(TOKYO, NEW_YORK);
    Tenor tenor_9y("9Y", japan_holidays, MODIFIED_FOLLOWING);
    Date end_date = start_date + tenor_9y;
    BulletFlow bullet_flow1(start_date, end_date, japan_holidays, ACT_365, MODIFIED_FOLLOWING);
    cout << bullet_flow1 << endl;
    BulletFlow bullet_flow2(start_date, end_date, start_date - 1, end_date - 1,
      japan_holidays, ACT_365, MODIFIED_FOLLOWING,
      japan_holidays, ACT_365, MODIFIED_FOLLOWING);
    cout << bullet_flow2 << endl;

    FlowTable flow_table1(start_date, end_date, QUARTERLY, japan_holidays, ACT_365, MODIFIED_FOLLOWING);
    cout << flow_table1 << endl;
    FlowTable flow_table2(start_date, end_date, QUARTERLY, japan_holidays, ACT_365, MODIFIED_FOLLOWING,
      japan_holidays, ACT_365, MODIFIED_FOLLOWING);
    cout << flow_table2 << endl;
  }

  if (false)
  {
    using namespace instrument;

    // Instrument Maths
    cout << "Instrument Mathematics" << endl;
    Holidays japan_holidays(TOKYO, NEW_YORK);
    Tenor tenor_9y("9Y", japan_holidays, MODIFIED_FOLLOWING);
    Date end_date = start_date + tenor_9y;
    BulletFlow bullet_flow(start_date, end_date, japan_holidays, ACT_365, MODIFIED_FOLLOWING);
    Deposit deposit(bullet_flow, "JPY");
    cout << deposit << endl;
  }

  if (true)
  {
    using namespace instrument;
    using namespace marketdata;
    using namespace marketdata::curve;

    if (true)
    {
      Holidays japan_holidays(TOKYO, NEW_YORK);
      
      vector<Instrument> instruments;
      for (int i = 1; i < 11; i++)
      {
        ostringstream tenor_stream;
        tenor_stream << i << "Y";
        Tenor tenor(tenor_stream.str(), japan_holidays, MODIFIED_FOLLOWING);
        Date end_date = start_date + tenor;
        BulletFlow bullet_flow(start_date, end_date, japan_holidays, ACT_365, MODIFIED_FOLLOWING);
        Deposit deposit(bullet_flow,"JPY");

        instruments.push_back(deposit);
      }

      // jpy single curve system
      Curve single_jpy_curve(start_date, "JPY", "", instruments, LINEAR_INTERPOLATION, FLAT_EXTRAPOLATION);

      single_jpy_curve.bootstrap();

      cout << "JPY Single Curve" << endl;
      cout << single_jpy_curve << endl;
    }

    if (false)
    {
      Holidays japan_holidays(TOKYO, NEW_YORK);

      vector<Instrument> jpy_discounting_instruments;
      string jpy_ccy = "JPY";
      for (int i = 1; i < 11; i++)
      {
        ostringstream tenor_stream;
        tenor_stream << i << "Y";
        Tenor tenor(tenor_stream.str(), japan_holidays, MODIFIED_FOLLOWING);
        Date end_date = start_date + tenor;
        BulletFlow bullet_flow(start_date, end_date, japan_holidays, ACT_365, MODIFIED_FOLLOWING);
        Deposit deposit(bullet_flow, jpy_ccy);

        jpy_discounting_instruments.push_back(deposit);
      }

      vector<Instrument> jpy_sa_forecasting_instruments;
      for (int i = 0; i < 11; i++)
      {
        ostringstream tenor_stream;
        tenor_stream << i << "Y";
        Tenor tenor(tenor_stream.str(), japan_holidays, MODIFIED_FOLLOWING);
        Date end_date = start_date + tenor;
        FlowTable flow_table_leg1(start_date, end_date, SEMI_ANNUAL, japan_holidays, ACT_365, MODIFIED_FOLLOWING, i/100);
        FlowTable flow_table_leg2(start_date, end_date, SEMI_ANNUAL, japan_holidays, ACT_365, MODIFIED_FOLLOWING, i/100-0.0001*i);

        Swap swap(jpy_ccy, "", flow_table_leg1, jpy_ccy, "LIBOR", flow_table_leg2);
        jpy_sa_forecasting_instruments.push_back(swap);
      }

      vector<Instrument> jpy_qu_forecasting_instruments;
      for (int i = 0; i < 11; i++)
      {
        ostringstream tenor_stream;
        tenor_stream << i << "Y";
        Tenor tenor(tenor_stream.str(), japan_holidays, MODIFIED_FOLLOWING);
        Date end_date = start_date + tenor;

        FlowTable flow_table_leg1(start_date, end_date, QUARTERLY, japan_holidays, ACT_365, MODIFIED_FOLLOWING, i*3/4/100);
        FlowTable flow_table_leg2(start_date, end_date, QUARTERLY, japan_holidays, ACT_365, MODIFIED_FOLLOWING, i*3/4/100-0.0001*i);

        Swap swap(jpy_ccy, "", flow_table_leg1, jpy_ccy, "LIBOR", flow_table_leg2);
        jpy_sa_forecasting_instruments.push_back(swap);
      }

      // jpy multi curve system
      string index = "LIBOR";
      Curve jpy_curve(start_date, "JPY", "", LINEAR_INTERPOLATION, FLAT_EXTRAPOLATION);
      jpy_curve.add_curve(DISCOUNTING, index, jpy_discounting_instruments);
      jpy_curve.add_curve(FORECASTING_SEMIANNUALY, index, jpy_sa_forecasting_instruments);
      jpy_curve.add_curve(FORECASTING_QUARTERLY, index, jpy_qu_forecasting_instruments);

      // boostrap the curve
      jpy_curve.bootstrap();

      cout << "JPY Curve details" << endl;
      cout << jpy_curve << endl;

      if (false)
      {
        Holidays us_holidays(NEW_YORK);

        vector<Instrument> usd_discounting_instruments;
        for (int i = 1; i < 11; i++)
        {
          ostringstream tenor_stream;
          tenor_stream << i << "Y";
          Tenor tenor(tenor_stream.str(), us_holidays, MODIFIED_FOLLOWING);
          Date end_date = start_date + tenor;
          BulletFlow bullet_flow(start_date, end_date, us_holidays, ACT_365, MODIFIED_FOLLOWING);
          Deposit deposit(bullet_flow, "USD");

          usd_discounting_instruments.push_back(deposit);
        }

        vector<Instrument> usd_qu_forecasting_instruments;
        for (int i = 0; i < 11; i++)
        {
          ostringstream tenor_stream;
          tenor_stream << i << "Y";
          Tenor tenor(tenor_stream.str(), japan_holidays, MODIFIED_FOLLOWING);
          Date end_date = start_date + tenor;

          FlowTable flow_table_leg1(start_date, end_date, QUARTERLY, japan_holidays, ACT_365, MODIFIED_FOLLOWING, i * 3 / 4 / 100);
          FlowTable flow_table_leg2(start_date, end_date, QUARTERLY, japan_holidays, ACT_365, MODIFIED_FOLLOWING, i * 3 / 4 / 100 - 0.0001*i);

          Swap swap("USD", "", flow_table_leg1, "USD", "LIBOR", flow_table_leg2);
          usd_qu_forecasting_instruments.push_back(swap);
        }

        // jpy xccy curve system 
        Curve jpy_xccy_curve = jpy_curve;
        string index = "LIBOR";

        // usd curve system
        Curve usd_curve(start_date, "USD", "", LINEAR_INTERPOLATION, FLAT_EXTRAPOLATION);
        usd_curve.add_curve(DISCOUNTING, index, usd_discounting_instruments);
        usd_curve.add_curve(FORECASTING_QUARTERLY, index, usd_qu_forecasting_instruments);
        jpy_xccy_curve.add_curve(CROSS_CURRENCY, usd_curve);

        // bootstrap the curve
        jpy_xccy_curve.bootstrap();

        cout << "JPY XCCY Curve details" << endl;
        cout << jpy_xccy_curve << endl;
      }
    }
  }
    
	system("pause");
	return 0;
}