#include <iostream>
#include <sstream>
using namespace std;

#include "Date.h"
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

  // Date Maths
  Holidays japan_holidays(TOKYO, NEW_YORK);
  Tenor tenor("2D", japan_holidays, MODIFIED_FOLLOWING);
  Date expiry_date = start_date + tenor;
  cout << expiry_date << endl;
  //d.accrual_factor(expiry, DAYCOUNT_30_360)

	system("pause");
	return 0;
}