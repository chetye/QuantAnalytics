#include <iostream>
#include <sstream>
using namespace std;

#include "Date.h"
#include "Flow.h"
using namespace date;

int main()
{
	cout << "This is the Quantitative Analytics Library" << endl;

  Date start_date("31-12-2018");
  cout << start_date << endl;
  
  if (false)
  {
    Date end_date;
    for (int i = 0; i < 30; i++)
    {
      int days = i * 100;
      end_date = start_date - days;
      cout << "Start Date " << start_date << " -" << days << " = " << end_date << " | ";
      end_date = end_date + days;
      cout << "+" << days << " = " << end_date << endl;
    }
  }

  if (false)
  {
    // Date Maths
    Holidays japan_holidays(TOKYO, NEW_YORK);
    Tenor tenor("T/N", japan_holidays, MODIFIED_FOLLOWING);
    Date expiry_date = start_date + tenor;
    cout << expiry_date << endl;
    start_date.accrual_factor(expiry_date, D30_360);
  }
  
  // Flow Maths
  Holidays japan_holidays(TOKYO, NEW_YORK);
  Tenor tenor_9m("95D", japan_holidays, MODIFIED_FOLLOWING);
  Date end_date = start_date + tenor_9m;
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
    
	system("pause");
	return 0;
}