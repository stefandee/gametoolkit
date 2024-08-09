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

 description: Class Piron Component Interface - Track Bar
 last modify: 21 04 00
        by grabX
---------------------------------------------------------------------------*/
//---------------------------------------------------------------------------
#ifndef PP_CTrackBarH
#define PP_CTrackBarH

#include "PP_CControl.h"
#include "PP_CWinControl.h"
#include "PP_GuiDef.h"

class CPITrackBar: public CPIControl
{
   public:
      CPITrackBar(CPIWinControl *Parent = NULL);
      virtual ~CPITrackBar();
      virtual CPString ClassName() {return PICN_TRACKBAR;}
      virtual bool IsDerivedFrom(CPString className);

      virtual void Paint();

      virtual void OnMouseMove(int X, int Y);
      virtual void OnLeftMouseDown(int X, int Y);
      virtual void OnLeftMouseUp(int X, int Y);

      virtual void OnChange(int oldValue);
   private:
      int  mCurrentValue;
      int  mMaxValue;

      GUIObjectIntCallback mHandleOnChange;
      //void (__closure *mHandleOnChange)(CPIObject *Sender, int oldValue);

   public:
      virtual int  GetCurrentValue();
      virtual void SetCurrentValue(int value);

      virtual int  GetMaxValue();
      virtual void SetMaxValue(int value);

      float CurrentAsNormalized() { return (float)mCurrentValue / mMaxValue; }
      int CurrentAsPercent() { return 100 * mCurrentValue / mMaxValue; }

      virtual void SetHandleOnChange(GUIObjectIntCallback callback);
      //virtual void SetHandleOnChange (void (__closure* value)(CPIObject *Sender, int oldValue));
};

/////////////////////////////////////////////////////////////////////////////
//GET-SET (INLINE) SECTION
/////////////////////////////////////////////////////////////////////////////
/*---------------------------------------------------------------------------
 description: CurrentValue
---------------------------------------------------------------------------*/
inline int CPITrackBar::GetCurrentValue()
{
   return mCurrentValue;
}

inline void CPITrackBar::SetCurrentValue(int value)
{
   int lTmpValue = mCurrentValue;
   mCurrentValue = value;
   if (mCurrentValue < 0)
      mCurrentValue = 0;
   else if (mCurrentValue >= GetMaxValue())
      mCurrentValue = GetMaxValue()-1;
   if (mCurrentValue != lTmpValue)
      OnChange(lTmpValue);
}
/*---------------------------------------------------------------------------
 description: MaxValue
---------------------------------------------------------------------------*/
inline int CPITrackBar::GetMaxValue()
{
   return mMaxValue;
}
inline void CPITrackBar::SetMaxValue(int value)
{
   mMaxValue = value;
}

/*---------------------------------------------------------------------------
 description: HandleOnChange
---------------------------------------------------------------------------*/
inline void CPITrackBar::SetHandleOnChange(GUIObjectIntCallback callback)
{
   mHandleOnChange = callback;
}

/*
void CPITrackBar::SetHandleOnChange (void (__closure* value)(CPIObject *Sender, int oldValue))
{
   mHandleOnChange = value;
}
*/

//---------------------------------------------------------------------------
#endif

