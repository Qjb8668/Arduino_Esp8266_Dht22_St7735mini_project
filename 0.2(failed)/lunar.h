#ifndef _LUNAR_H
#define _LUNAR_H

#include "Arduino.h"

//Lunar dates
/*
  bit23 bit22 bit21 bit20:means the leap month this year
						  while 0 means there is no leap month this year
  bit19 bit18 bit17 bit16 bit15 bit14 bit13 bit12 bit11 bit10 bit9 bit8 bit7
	1     2     3     4     5     6     7     8     9     10    11   12   13
  the amount of this month while 1 means 30 days and 0 means 29 days
  bit6 bit5                         bit4 bit3 bit2 bit1 bit0
  solor month of Spring Fesital   solor dates of Spring Festival
*/
extern unsigned int lunar200y[199];

extern const int MonthsCount[13];

class Lunar {
private:
	int getLunarMonthDays(int _year, int _monthIndex);
public:
	int syear, smonth, sdate;
	int year, month, date;
	bool isLeap;
	Lunar();
	Lunar(int _year, int _month, int _day);
};

#endif // !_LUNAR_H
