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
    BulletFlow bullet_flow1(start_date, end_date, japan_holidays, ACT_365
      , MODIFIED_FOLLOWING);
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

    if (false)
    {
      Holidays japan_holidays(TOKYO, NEW_YORK);
      
      string jpy_ccy = "JPY";

      vector<Instrument_Ptr> instruments;
      for (int i = 1; i < 11; i++)
      {
        ostringstream tenor_stream;
        tenor_stream << i << "Y";
        Tenor tenor(tenor_stream.str(), japan_holidays, MODIFIED_FOLLOWING);
        Date end_date = start_date + tenor;
        BulletFlow bullet_flow(start_date, end_date, japan_holidays, ACT_365, MODIFIED_FOLLOWING, i / 100.0);
        Deposit_Ptr deposit_ptr(new Deposit(bullet_flow, jpy_ccy));

        instruments.push_back(deposit_ptr);
      }

      // jpy single curve system
      Interest_Rate_Curve_Ptr single_jpy_curve(new Interest_Rate_Curve(instruments, LINEAR_INTERPOLATION, FLAT_EXTRAPOLATION));

      Interest_Rate_Curve_System single_jpy_curve_system(start_date, "JPY");

      single_jpy_curve_system.add_curve("DISCOUNTING", "", NONE_FREQUENCY, single_jpy_curve);

      single_jpy_curve_system.bootstrap();

      cout << "JPY Single Curve" << endl;
      single_jpy_curve_system.dump(cout);
    }

    if (true)
    {
      Holidays japan_holidays(TOKYO, NEW_YORK);

      vector<Instrument_Ptr> jpy_discounting_instruments;
      string jpy_ccy = "JPY";
      for (int i = 1; i < 11; i++)
      {
        ostringstream tenor_stream;
        tenor_stream << i << "Y";
        Tenor tenor(tenor_stream.str(), japan_holidays, MODIFIED_FOLLOWING);
        Date end_date = start_date + tenor;
        BulletFlow bullet_flow(start_date, end_date, japan_holidays, ACT_365, MODIFIED_FOLLOWING, i / 100.0);
        Deposit_Ptr deposit_ptr(new Deposit(bullet_flow, jpy_ccy));

        jpy_discounting_instruments.push_back(deposit_ptr);
      }

      vector<Instrument_Ptr> jpy_sa_forecasting_instruments;
      for (int i = 1; i < 11; i++)
      {
        ostringstream tenor_stream;

        tenor_stream << i << "Y";
        Tenor tenor(tenor_stream.str(), japan_holidays, MODIFIED_FOLLOWING);
        Date end_date = start_date + tenor;
        FlowTable flow_table_leg1(start_date, end_date, SEMI_ANNUAL, japan_holidays, ACT_365, MODIFIED_FOLLOWING, (double)i / 2 / 100.0);
        FlowTable flow_table_leg2(start_date, end_date, SEMI_ANNUAL, japan_holidays, ACT_365, MODIFIED_FOLLOWING, 0, (double)i / 2 * 0.0001);

        Swap_Ptr swap_ptr(new Swap(jpy_ccy, "", flow_table_leg1, jpy_ccy, "LIBOR", flow_table_leg2));
        jpy_sa_forecasting_instruments.push_back(swap_ptr);
      }

      // jpy multi curve system
      Interest_Rate_Curve_Ptr discounting_curve(new Interest_Rate_Curve(jpy_discounting_instruments, LINEAR_INTERPOLATION, FLAT_EXTRAPOLATION));
      Interest_Rate_Curve_Ptr forecasting_sa_curve(new Interest_Rate_Curve(jpy_sa_forecasting_instruments, LINEAR_INTERPOLATION, FLAT_EXTRAPOLATION));

      Interest_Rate_Curve_System jpy_curve_system(start_date, "JPY");
      jpy_curve_system.add_curve("DISCOUNTING", "", NONE_FREQUENCY, discounting_curve);
      jpy_curve_system.add_curve("FORECASTING", "LIBOR", SEMI_ANNUAL, forecasting_sa_curve);

      // boostrap the curve
      jpy_curve_system.bootstrap();

      cout << "JPY Discounting and Forecasting SA Curve details" << endl;
      cout << jpy_curve_system << endl;
    }

    if (false)
    {
      Holidays japan_holidays(TOKYO, NEW_YORK);

      vector<Instrument_Ptr> jpy_discounting_instruments;
      string jpy_ccy = "JPY";
      for (int i = 1; i < 11; i++)
      {
        ostringstream tenor_stream;
        tenor_stream << i << "Y";
        Tenor tenor(tenor_stream.str(), japan_holidays, MODIFIED_FOLLOWING);
        Date end_date = start_date + tenor;
        BulletFlow bullet_flow(start_date, end_date, japan_holidays, ACT_365, MODIFIED_FOLLOWING);
        Deposit_Ptr deposit_ptr(new Deposit(bullet_flow, jpy_ccy));

        jpy_discounting_instruments.push_back(deposit_ptr);
      }

      vector<Instrument_Ptr> jpy_sa_forecasting_instruments;
      for (int i = 0; i < 11; i++)
      {
        ostringstream tenor_stream;

        tenor_stream << i << "Y";
        Tenor tenor(tenor_stream.str(), japan_holidays, MODIFIED_FOLLOWING);
        Date end_date = start_date + tenor;
        FlowTable flow_table_leg1(start_date, end_date, SEMI_ANNUAL, japan_holidays, ACT_365, MODIFIED_FOLLOWING, i / 2 / 100.0);
        FlowTable flow_table_leg2(start_date, end_date, SEMI_ANNUAL, japan_holidays, ACT_365, MODIFIED_FOLLOWING, 0, i / 2 * 0.0001);

        Swap_Ptr swap_ptr(new Swap(jpy_ccy, "", flow_table_leg1, jpy_ccy, "LIBOR", flow_table_leg2));
        jpy_sa_forecasting_instruments.push_back(swap_ptr);
      }

      vector<Instrument_Ptr> jpy_qu_forecasting_instruments;
      for (int i = 0; i < 11; i++)
      {
        ostringstream tenor_stream;
        tenor_stream << i << "Y";
        Tenor tenor(tenor_stream.str(), japan_holidays, MODIFIED_FOLLOWING);
        Date end_date = start_date + tenor;

        FlowTable flow_table_leg1(start_date, end_date, QUARTERLY, japan_holidays, ACT_365, MODIFIED_FOLLOWING, i / 4 / 100.0);
        FlowTable flow_table_leg2(start_date, end_date, QUARTERLY, japan_holidays, ACT_365, MODIFIED_FOLLOWING, 0, i / 4 * 0.0001);

        Swap_Ptr swap_ptr(new Swap(jpy_ccy, "", flow_table_leg1, jpy_ccy, "LIBOR", flow_table_leg2));
        jpy_qu_forecasting_instruments.push_back(swap_ptr);
      }

      // jpy multi curve system
      Interest_Rate_Curve_Ptr discounting_curve(new Interest_Rate_Curve(jpy_discounting_instruments, LINEAR_INTERPOLATION, FLAT_EXTRAPOLATION));
      Interest_Rate_Curve_Ptr forecasting_sa_curve(new Interest_Rate_Curve(jpy_sa_forecasting_instruments, LINEAR_INTERPOLATION, FLAT_EXTRAPOLATION));
      Interest_Rate_Curve_Ptr forecasting_qu_curve(new Interest_Rate_Curve(jpy_qu_forecasting_instruments, LINEAR_INTERPOLATION, FLAT_EXTRAPOLATION));

      Interest_Rate_Curve_System jpy_curve_system(start_date, "JPY");
      jpy_curve_system.add_curve("DISCOUNTING", "", NONE_FREQUENCY, discounting_curve);
      jpy_curve_system.add_curve("FORECASTING", "LIBOR", SEMI_ANNUAL, forecasting_sa_curve);
      jpy_curve_system.add_curve("FORECASTING", "LIBOR", QUARTERLY, forecasting_qu_curve);

      // boostrap the curve
      jpy_curve_system.bootstrap();

      cout << "JPY Discounting, Forecasting SA and Forecasting QU details" << endl;
      cout << jpy_curve_system << endl;

      if (false)
      {
        Holidays us_holidays(NEW_YORK);

        vector<Instrument_Ptr> usd_discounting_instruments;
        for (int i = 1; i < 11; i++)
        {
          ostringstream tenor_stream;
          tenor_stream << i << "Y";
          Tenor tenor(tenor_stream.str(), us_holidays, MODIFIED_FOLLOWING);
          Date end_date = start_date + tenor;
          BulletFlow bullet_flow(start_date, end_date, us_holidays, ACT_365, MODIFIED_FOLLOWING, i * 2 / 100.0);
          Deposit_Ptr deposit_ptr(new Deposit(bullet_flow, "USD"));

          usd_discounting_instruments.push_back(deposit_ptr);
        }

        vector<Instrument_Ptr> usd_qu_forecasting_instruments;
        for (int i = 0; i < 11; i++)
        {
          ostringstream tenor_stream;
          tenor_stream << i << "Y";
          Tenor tenor(tenor_stream.str(), japan_holidays, MODIFIED_FOLLOWING);
          Date end_date = start_date + tenor;

          FlowTable flow_table_leg1(start_date, end_date, QUARTERLY, japan_holidays, ACT_365, MODIFIED_FOLLOWING, i * 4 / 100);
          FlowTable flow_table_leg2(start_date, end_date, QUARTERLY, japan_holidays, ACT_365, MODIFIED_FOLLOWING, 0, i * 4 * 0.0001);

          Swap_Ptr swap_ptr(new Swap("USD", "", flow_table_leg1, "USD", "LIBOR", flow_table_leg2));
          usd_qu_forecasting_instruments.push_back(swap_ptr);
        }

        // jpy xccy curve system 
        Interest_Rate_Curve_System jpy_xccy_curve_system = jpy_curve_system;

        // usd curve system
        Interest_Rate_Curve_System usd_curve_system(start_date, "USD");

        Interest_Rate_Curve_Ptr usd_discounting_curve(new Interest_Rate_Curve(usd_discounting_instruments, LINEAR_INTERPOLATION, FLAT_EXTRAPOLATION));
        Interest_Rate_Curve_Ptr usd_forecasting_qu_curve(new Interest_Rate_Curve(usd_qu_forecasting_instruments, LINEAR_INTERPOLATION, FLAT_EXTRAPOLATION));

        usd_curve_system.add_curve("DISCOUNTING", "", NONE_FREQUENCY, usd_discounting_curve);
        usd_curve_system.add_curve("FORECASTING", "LIBOR", QUARTERLY, usd_forecasting_qu_curve);

        jpy_xccy_curve_system.add_external_curve_system(usd_curve_system);

        // bootstrap the curve
        jpy_xccy_curve_system.bootstrap();

        cout << "JPY XCCY Curve details" << endl;
        cout << jpy_xccy_curve_system << endl;
      }
    }
  }
    
	system("pause");
	return 0;
}