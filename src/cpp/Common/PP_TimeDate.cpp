//-----------------------------------------------------------------------------
// This file is part of PPTactical Engine.                                   //
//                                                                           //
// PPTactical Engine - engine for tactical/strategy games                    // 
// Copyright (C) 1998, 1999, 2000, 2001 Stefan Dicu & Tudor Girba            //
//                                                                           //
// PPTactical Engine is free software; you can redistribute it               //
// and/or modify it under the terms of the GNU Lesser General Public License as     // 
// published by the Free Software Foundation; either version 2.1 of the        //
// License, or (at your option) any later version.                           //
//                                                                           //  
// PPTactical Engine is distributed in the hope that it will be useful, but  //
// WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY// 
// or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License  //
// for more details.                                                         //
//                                                                           // 
// You should have received a copy of the GNU Lesser General Public License         //
// along with PPTactical Engine; if not, write to the Free Software          //
// Foundation Inc. 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA   // 
//-----------------------------------------------------------------------------
//---------------------------------------------------------------------------
#include <string.h>
#pragma hdrstop

#include "PP_TimeDate.h"

//---------------------------------------------------------------------------
#ifdef __BORLANDC__
#pragma package(smart_init)
#endif

CPPTimeDate::CPPTimeDate()
{
  mDay    = 0;
  mMonth  = 0;
  mYear   = 0;
  mHour   = 0;
  mMinute = 0;
  mSecond = 0;
}
//---------------------------------------------------------------------------

CPPTimeDate::CPPTimeDate(int _day, int _month, int _year, int _hour, int _minute, int _second)
{
  mDay    = _day;
  mMonth  = _month;
  mYear   = _year;
  mHour   = _hour;
  mMinute = _minute;
  mSecond = _second;
}
//---------------------------------------------------------------------------

CPPTimeDate::CPPTimeDate (CPPTimeDate& _date)
{
  mDay    = _date.mDay;
  mMonth  = _date.mMonth;
  mYear   = _date.mYear;
  mHour   = _date.mHour;
  mMinute = _date.mMinute;
  mSecond = _date.mSecond;
}
//---------------------------------------------------------------------------

void CPPTimeDate::SetDate(int _day, int _month, int _year, int _hour, int _minute, int _second)
{
  mDay    = _day;
  mMonth  = _month;
  mYear   = _year;
  mHour   = _hour;
  mMinute = _minute;
  mSecond = _second;
}
//---------------------------------------------------------------------------

bool CPPTimeDate::IsEqual(CPPTimeDate _td) const
{
  if ((mSecond == _td.mSecond) &&
      (mMinute == _td.mMinute) &&
      (mHour   == _td.mHour)   &&
      (mDay    == _td.mDay)    &&
      (mMonth  == _td.mMonth)  &&
      (mYear   == _td.mYear))
  {
    return true;
  }

  return false;
}
//---------------------------------------------------------------------------

bool CPPTimeDate::IsAfter(CPPTimeDate _td) const
{
  return !IsBefore(_td);
}
//---------------------------------------------------------------------------

bool CPPTimeDate::IsBetween(CPPTimeDate _tdStart, CPPTimeDate _tdEnd) const
{
  if (IsAfter(_tdStart) && IsBefore(_tdEnd))
  {
    return true;
  }

  return false;
}
//---------------------------------------------------------------------------

bool CPPTimeDate::IsBefore(CPPTimeDate _td) const
{
  if (_td.mYear < mYear)
  {
    return false;
  }
  else
  {
    if (_td.mYear == mYear)
      if (_td.mMonth < mMonth)
      {
        return false;
      }
      else
      {
        if (_td.mMonth == mMonth)
          if (_td.mDay < mDay)
          {
            return false;
          }
      }
  }

  return true;
}
//---------------------------------------------------------------------------

CPString CPPTimeDate::GetStringDate() const
{
  CPString lDayStr, lMonthStr;
  int        lDay, lMonth;

  lDay = GetDay();
  if (lDay < 9)
  {
    lDayStr = "0";                                                              
  }
  lDayStr = lDayStr + CPString(lDay);

  lMonth = GetMonth();
  if (lMonth < 9)
  {
    lMonthStr = "0";
  }
  lMonthStr = lMonthStr + CPString(lMonth);

  return lDayStr + "." + lMonthStr + "." + CPString(GetYear());
}
//---------------------------------------------------------------------------

int   CPPTimeDate::Serialize(char* _buffer) const
{
  strcpy(_buffer, "CPPTimeDate");

  *((int*)(_buffer + 24) + 0) = mDay;
  *((int*)(_buffer + 24) + 1) = mMonth;
  *((int*)(_buffer + 24) + 2) = mYear;
  *((int*)(_buffer + 24) + 3) = mHour;
  *((int*)(_buffer + 24) + 4) = mMinute;
  *((int*)(_buffer + 24) + 5) = mSecond;

  return DataSize();
}
//---------------------------------------------------------------------------

int   CPPTimeDate::DataSize() const
{
  return 24 + 6 * sizeof(int);
}
//---------------------------------------------------------------------------

bool  CPPTimeDate::DeSerialize(char* _buffer)
{
  if (strcmp(_buffer, "CPPTimeDate") != 0)
  {
    return false;
  }

  mDay    = *((int*)(_buffer + 24) + 0);
  mMonth  = *((int*)(_buffer + 24) + 1);
  mYear   = *((int*)(_buffer + 24) + 2);
  mHour   = *((int*)(_buffer + 24) + 3);
  mMinute = *((int*)(_buffer + 24) + 4);
  mSecond = *((int*)(_buffer + 24) + 5);

  return true;
}
//---------------------------------------------------------------------------

// Descriere   : returneaza diferenta in zile intre doua date
// Param       : data
// Rezultat    : evident
// Comentarii  : n/a
int CPPTimeDate::operator-(CPPTimeDate& _td)
{
  return ((mYear * 12 * 30 + mMonth * 30 + mDay) -
         (_td.mYear * 12 * 30 + _td.mMonth * 30 + _td.mDay)) * 24 + mHour - _td.mHour;
}
//---------------------------------------------------------------------------

CPPTimeDate CPPTimeDate::operator+(int _hours)
{
  CPPTimeDate lTd = *this;

  lTd.mYear  += _hours/(360*24);
  _hours  %= 360*24;

  lTd.mMonth += _hours/(30*12);
  _hours %= (30*12);

  lTd.mDay += _hours/24;
  _hours %= 24;

  lTd.mHour += _hours;

  /*
  if (lTd.mHour > 24)
  {
    lTd.mHour %= 24;
    lTd.mDay += (lTd.mHour / 24);
  }
  */

  if (lTd.mDay > 30)
  {
    lTd.mDay %= 30;
    lTd.mMonth++;
  }

  if (lTd.mMonth > 12)
  {
    lTd.mMonth %= 12;
    mYear++;
  }

  return lTd;
}
//---------------------------------------------------------------------------

int CPPTimeDate::operator==(CPPTimeDate& _td) const
{
  return IsEqual(_td);
}
//---------------------------------------------------------------------------

int  CPPTimeDate::operator!=(CPPTimeDate& _td) const
{
  return !IsEqual(_td);
}
//---------------------------------------------------------------------------

int CPPTimeDate::operator>(CPPTimeDate& _td) const
{
  return IsAfter(_td);
}
//---------------------------------------------------------------------------

int CPPTimeDate::operator<(CPPTimeDate& _td) const
{
  return IsBefore(_td);
}
//---------------------------------------------------------------------------

int CPPTimeDate::operator>=(CPPTimeDate& _td) const
{
  return IsAfter(_td) || IsEqual(_td);
}
//---------------------------------------------------------------------------

int CPPTimeDate::operator<=(CPPTimeDate& _td) const
{
  return IsBefore(_td) || IsEqual(_td);
}
//---------------------------------------------------------------------------

