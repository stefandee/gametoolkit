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
//  Unit              : Sensitive scroll
//
//  Version           : 1.0
//
//  Description       :
//    * sensitive control
//
//  History           :
//    [24.04.2000] - [grabX] - first work
//    [01.12.2002] - [Tmn]   - bug in Check - call to GetPos without parameter
//                             (used default value which was 0)
//-----------------------------------------------------------------------------

//---------------------------------------------------------------------------
#ifndef PP_CSensitiveScrollH
#define PP_CSensitiveScrollH

#include "PP_CControl.h"
#include "PP_CWinControl.h"
#include "easylogging++.h"

class CPISensitiveScroll: public CPIControl
{
   public:
      CPISensitiveScroll(CPIWinControl *Parent = NULL);
      virtual ~CPISensitiveScroll();

      virtual void OnMouseOn(int X, int Y);
      virtual void OnMouseMove(int X, int Y);

      virtual void OnChange(CPIObject *Sender);

      virtual void OnScroll(CPIObject *Sender);

      virtual void Check();
   public:
      int  GetDir(int index = 0);
      void SetDir(int value, int index = 0);

      int  GetMin(int index = 0);
      void SetMin(int value, int index = 0);

      int  GetMax(int index = 0);
      void SetMax(int value, int index = 0);

      int  GetPos(int index = 0);
      void SetPos(int value, int index = 0);
      void SetPosWithNoChange(int value, int index = 0);

      int  GetStep(int index = 0);
      void SetStep(int value, int index = 0);

      int  GetDimCount();
      void SetDimCount(int value);

      virtual void SetHandleOnChange(StandardGUICallback callback);
      //virtual void SetHandleOnChange(PP_voidret_callback_t callback);
      //virtual void SetHandleOnChange (void (__closure* value)(CPIObject *Sender));

   private:
      // Karg: was previously 10, but it crashes on linux...
      int mDir[10];
      int mMin[10];
      int mMax[10];
      int mPos[10];
      int mStep[10];
      int mDimCount;

      StandardGUICallback mHandleOnChange; // on change event
      //PP_voidret_callback_t mHandleOnChange; // on change event
      //void (__closure* mHandleOnChange)(CPIObject *Sender); //on change event
};

/////////////////////////////////////////////////////////////////////////////
//GET-SET (INLINE) SECTION
/////////////////////////////////////////////////////////////////////////////
/*---------------------------------------------------------------------------
 description: Dir
---------------------------------------------------------------------------*/
inline int  CPISensitiveScroll::GetDir(int index)
{
   return mDir[index];
}
inline void CPISensitiveScroll::SetDir(int value, int index)
{
   mDir[index] = value;
}
/*---------------------------------------------------------------------------
 description: Min
---------------------------------------------------------------------------*/
inline int  CPISensitiveScroll::GetMin(int index)
{
   if (index < 0 || index >= 10)
   {
     LOG(ERROR) << "index is out of bounds: " << index;

     return 0;
   }

   return mMin[index];
}
inline void CPISensitiveScroll::SetMin(int value, int index)
{
   mMin[index] = value;
}
/*---------------------------------------------------------------------------
 description: Max
---------------------------------------------------------------------------*/
inline int  CPISensitiveScroll::GetMax(int index)
{
   return mMax[index];
}
inline void CPISensitiveScroll::SetMax(int value, int index)
{
   mMax[index] = value;
}

inline void CPISensitiveScroll::SetPos(int value, int index)
{
   if (value < GetMin(index))
      value = GetMin(index);
   else if (value >= GetMax(index))
      value = GetMax(index);
   if ( mPos[index] != value)
   {
      mPos[index]= value;
      OnChange(this);
   }
}

/*---------------------------------------------------------------------------
 description: Step
---------------------------------------------------------------------------*/
inline int  CPISensitiveScroll::GetStep(int index)
{
   return mStep[index];
}
inline void CPISensitiveScroll::SetStep(int value, int index)
{
   mStep[index]= value;
}
/*---------------------------------------------------------------------------
 description: DimCount
---------------------------------------------------------------------------*/
inline int  CPISensitiveScroll::GetDimCount()
{
   return mDimCount;
}

inline void CPISensitiveScroll::SetDimCount(int value)
{
   mDimCount = value;
}
/*---------------------------------------------------------------------------
 description: HandleOnChange
---------------------------------------------------------------------------*/
//inline void CPISensitiveScroll::SetHandleOnChange(PP_voidret_callback_t callback)
inline void CPISensitiveScroll::SetHandleOnChange(StandardGUICallback callback)
{
   mHandleOnChange = callback;
}

/*
void CPISensitiveScroll::SetHandleOnChange (void (__closure* value)(CPIObject *Sender))
{
   mHandleOnChange = value;
}
*/

//---------------------------------------------------------------------------
#endif

