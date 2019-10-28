#include <iostream>
#include <sstream>
using namespace std;

#include "Date.h"
using namespace date;

int main()
{
	cout << "This is the Quantitative Analytics Library" << endl;

  Date d("31-12-2018");
  cout << d << endl;

  for (int i = 0; i < 30; i++)
  {
    int add_subtract = i*100;
    d = d - add_subtract;
    cout << "-" << add_subtract << "D : " << d << " | ";
    d = d + add_subtract;
    cout << "+" << add_subtract << "D : " << d << endl;
  }

	system("pause");
	return 0;
}