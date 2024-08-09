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
/*---------------------------------------------------------------------------
 application: Pure Power

 description: Class Counter
 last modify: 13 06 00
        by grabX
---------------------------------------------------------------------------*/

//---------------------------------------------------------------------------
#pragma hdrstop

#include "PP_Counter.h"

/*---------------------------------------------------------------------------
 description: constructor
---------------------------------------------------------------------------*/
CPCounter::CPCounter()
:  mCounter(0),
   mMinValue(0),
   mMaxValue(1),
   mCircular(true),
   mMaxReached(false),
   mMinReached(true),
   mInfinite(false)
{
	/*
   mCounter    = 0;
   mMinValue   = 0;
   mMaxValue   = 1;
   mMinReached = true;
   mMaxReached = false;
   mCircular   = true;
   mInfinite   = false;
   */
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: custom constructor
---------------------------------------------------------------------------*/
CPCounter::CPCounter(int maxValue)
:  mCounter(maxValue),
   mMinValue(0),
   mMaxValue(maxValue),
   mCircular(true),
   mMaxReached(false),
   mMinReached(true),
   mInfinite(false)
{
	/*
   mCounter    = 0;
   mMaxValue   = maxValue;
   mMinReached = true;
   mMaxReached = false;
   mCircular   = true;
   */
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: increments and returns thios object
---------------------------------------------------------------------------*/
CPCounter& CPCounter::operator++()
{
   Inc();
   return *this;
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: increments and returns thios object
---------------------------------------------------------------------------*/
CPCounter& CPCounter::operator++(int redundant)
{
   Inc();
   return *this;
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: decrements and returns thios object
---------------------------------------------------------------------------*/
CPCounter& CPCounter::operator--()
{
   Dec();
   return *this;
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: decrements and returns thios object
---------------------------------------------------------------------------*/
CPCounter& CPCounter::operator--(int redundant)
{
   Dec();
   return *this;
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: returns true if the counters value are the equal
---------------------------------------------------------------------------*/
int CPCounter::operator==(CPCounter& toCompare)
{
   return (GetCounter()==toCompare.GetCounter());
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: returns true if the counter equals value
---------------------------------------------------------------------------*/
int CPCounter::operator==(int value)
{
   return (GetCounter()==value);
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: sets the counter value
---------------------------------------------------------------------------*/
CPCounter& CPCounter::operator=(const CPCounter& toInit)
{
   /*SetMaxValue (toInit.GetMaxValue());
   SetMinValue (toInit.GetMinValue());
   SetCircular (toInit.GetCircular());
   SetCounter  (toInit.GetCounter());*/
   SetMaxValue (toInit.mMaxValue);
   SetMinValue (toInit.mMinValue);
   SetCircular (toInit.mCircular);
   SetCounter  (toInit.mCounter);
   return *this;
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: sets the counter value
---------------------------------------------------------------------------*/
CPCounter& CPCounter::operator=(int value)
{
   SetCounter(value);
   return *this;
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: gets the counter value
---------------------------------------------------------------------------*/
CPCounter::operator int ()
{
   return GetCounter();
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: gets the counter value
---------------------------------------------------------------------------*/
int CPCounter::operator()()
{
   return GetCounter();
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: resets the counter
 others     : does not set the min reached flag
---------------------------------------------------------------------------*/
CPCounter& CPCounter::Reset()
{
   SetMinReached(false);
   SetMaxReached(false);
   mCounter = GetMaxValue();
//   SetCounter(GetMinValue());
   return *this;
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: sets the counter value
---------------------------------------------------------------------------*/
CPCounter& CPCounter::Set(int count)
{
   SetCounter(count);
   return *this;
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: increments the counter with the value
---------------------------------------------------------------------------*/
CPCounter& CPCounter::Inc(int count)
{
   SetCounter(GetCounter() + count);
   return *this;
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: decrements the counter with the value
---------------------------------------------------------------------------*/
CPCounter& CPCounter::Dec(int count)
{
   SetCounter(GetCounter() - count);
   return *this;
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: increments/decrements the counter value towards the wanted value
 others     : still to be completed (if the step is not 1)
---------------------------------------------------------------------------*/
CPCounter& CPCounter::ToValue(int wantedValue, int step)
{
   if (GetCounter() - wantedValue == 0)
      return *this;
   int lTmp;
   lTmp = wantedValue - GetCounter();
   if (((lTmp>=GetMinValue())&&(lTmp<=GetValueInterval()/2)) || (lTmp<-GetValueInterval()/2))
      Inc(step);
   else
      Dec(step);
   return *this;
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: returns true if the max value was reached by the previous iteration
---------------------------------------------------------------------------*/
bool CPCounter::MaxReached()
{
   return GetMaxReached();
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: returns true if the min value was reached by the previous iteration
---------------------------------------------------------------------------*/
bool CPCounter::MinReached()
{
   return GetMinReached();
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: gets the difference between max and min values
---------------------------------------------------------------------------*/
int CPCounter::GetValueInterval()
{
   return GetMaxValue() - GetMinValue();
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: gets the object
---------------------------------------------------------------------------*/
//CPCounter* CPCounter::GetObject()
CPCounter* CPCounter::GetThis()
{
   return this;
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
#ifdef __BORLANDC__
#pragma package(smart_init)
#endif
