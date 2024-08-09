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

 description: Class Piron Component Interface - Edit Control
 last modify: 19 01 01
        by grabX
---------------------------------------------------------------------------*/
//---------------------------------------------------------------------------
#ifndef PP_CEditControlH
#define PP_CEditControlH

#include "PP_CControl.h"
//#include "PP_InterfaceDef.h"
#include "PP_Counter.h"

class CPIEditControl: public CPIControl
{
   public:
      virtual CPString ClassName() {return PICN_EDITCONTROL;}
      virtual bool IsDerivedFrom(CPString className);
      virtual void Update();
      virtual void Paint();

      virtual void OnLeftMouseClick();
      virtual void OnLeftMouseDown(int X, int Y);
      virtual void OnKeyPressed(int key);

      virtual CPFont* GetFont();
      virtual void  SetFont(CPFont* value);

      virtual void Focus();
      virtual void DeFocus();

   protected:
      CPIEditControl(CPIWinControl *Parent = NULL);
      virtual ~CPIEditControl();

   public: //get sets
      void SetTextAllignment(int value);
      int GetTextAllignment();

      void SetMaxTextLength(int value);
      int GetMaxTextLength();

      void SetTextLengthLimited(bool value);
      bool GetTextLengthLimited();

      //void SetRepeatDelay(int value) { mRepeatDelay = value; }
      //void SetRepeatRate(int value) { mRepeatRate = value; }

   private:
      CPFont     *mFont;

      bool        mHasFocus;

      CPCounter   mBlinkCounter;

      // time in miliseconds
      //int         mRepeatDelay;
      int         mLastDelayTime;

      // time in miliseconds between two repeats of the same key
      //int         mRepeatRate;
      int         mLastRateTime;
      int         mPrevKey;

      int         mTextAllignment;
      int         mMaxTextLength;
      bool        mTextLengthLimited;
};
/*---------------------------------------------------------------------------
 description: Font
---------------------------------------------------------------------------*/
inline CPFont* CPIEditControl::GetFont()
{
   return mFont;
}
inline void  CPIEditControl::SetFont(CPFont* value)
{
   delete mFont;
   mFont = value;
}

/*---------------------------------------------------------------------------
 description: text allignemt
---------------------------------------------------------------------------*/
inline void CPIEditControl::SetTextAllignment(int value)
{
   mTextAllignment = value;
}

inline int CPIEditControl::GetTextAllignment()
{
   return mTextAllignment;
}

/*---------------------------------------------------------------------------
 description: max text length
---------------------------------------------------------------------------*/
inline void CPIEditControl::SetMaxTextLength(int value)
{
   mMaxTextLength = value;
}

inline int CPIEditControl::GetMaxTextLength()
{
   return mMaxTextLength;
}

/*---------------------------------------------------------------------------
 description: text length limited
---------------------------------------------------------------------------*/
inline void CPIEditControl::SetTextLengthLimited(bool value)
{
   mTextLengthLimited = value;
}

inline bool CPIEditControl::GetTextLengthLimited()
{
   return mTextLengthLimited;
}
//---------------------------------------------------------------------------

#endif
