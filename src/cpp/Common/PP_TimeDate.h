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
//-----------------------------------------------------------------------------
//  Unit              : Pure Power Date unit (PPDate.h)
//
//  Versiune          : 1.0
//
//  Descriere         :
//    * clasa pentru manipularea datei (zile, luni, ani)
//
//  Istorie           :
//    [Karg] - [8.04.2000] - a fost creat unitul si clasa
//-----------------------------------------------------------------------------

//---------------------------------------------------------------------------
#ifndef PP_TimeDateH
#define PP_TimeDateH
//---------------------------------------------------------------------------

#include "PP_String.h"

class CPPTimeDate
{
private:
  int mDay, mMonth, mYear;
  int mHour, mMinute, mSecond;

  bool IsAfter   (CPPTimeDate _td) const;
  bool IsBetween (CPPTimeDate _tdStart, CPPTimeDate _tdEnd) const;
  bool IsBefore  (CPPTimeDate _td) const;
  bool IsEqual   (CPPTimeDate _td) const;

protected:

public:
  CPPTimeDate ();
  CPPTimeDate (int _day, int _month, int _year, int _hour, int _minute, int _second);
  CPPTimeDate (CPPTimeDate& _date);

  void SetDate   (int _day, int _month, int _year, int _hour, int _minute, int _second);
  int  GetDay    () const { return mDay;    };
  int  GetMonth  () const { return mMonth;  };
  int  GetYear   () const { return mYear;   };
  int  GetHour   () const { return mHour;   };
  int  GetMinute () const { return mMinute; };
  int  GetSecond () const { return mSecond; };
  CPString GetStringDate() const;

  virtual int   Serialize(char* _buffer) const;
  virtual int   DataSize() const;
  virtual bool  DeSerialize(char* _buffer);

  int         operator-(CPPTimeDate&  _td);
  CPPTimeDate operator+(int _hours);
  int         operator==(CPPTimeDate& _td) const;
  int         operator!=(CPPTimeDate& _td) const;
  int         operator>(CPPTimeDate& _td) const;
  int         operator<(CPPTimeDate& _td) const;
  int         operator>=(CPPTimeDate& _td) const;
  int         operator<=(CPPTimeDate& _td) const;
};

#endif
