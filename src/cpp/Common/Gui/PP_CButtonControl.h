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

 description: Class Piron Interface
              this is an abstract class and cannot be instantiated directly 
 last modify: 09 02 01
        by grabX
---------------------------------------------------------------------------*/
//---------------------------------------------------------------------------
#ifndef PP_CButtonControlH
#define PP_CButtonControlH

#include "PP_CControl.h"
#include "PP_CWinControl.h"

/*
#define BTNCTRL_NORMAL     0
#define BTNCTRL_DOWN       1
#define BTNCTRL_SENSITIVE  2
*/

class CPIButtonControl: public CPIControl
{
   protected:
      CPIButtonControl(CPIWinControl *Parent = NULL);
      virtual ~CPIButtonControl();
      virtual CPString ClassName() {return PICN_BUTTON;}
      virtual bool IsDerivedFrom(CPString ClassName);

      virtual void Paint();

      virtual void OnLeftMouseDown(int X, int Y);
      virtual void OnMouseMove(int X, int Y);
      virtual void OnLeftMouseUp(int X, int Y);
      virtual void OnLeftMouseClick();

   public:
      bool GetMouseSensitive();
      void SetMouseSensitive(bool value);
      int  GetMouseDownAnim();
      void SetMouseDownAnim(int value);
      int  GetMouseSensitiveAnim();
      void SetMouseSensitiveAnim(int value);
      int  GetOnLeftClickSound();
      void SetOnLeftClickSound(int value);

   private:
/*      int  mButtonState;            //the state of the button:
                                    //   0 - normal
                                    //   1 - down
                                    //   2 - sensitive
*/
      bool mMouseSensitive;         //if it's sensitive to mouse move

      int  mMouseDownAnim;          //when mouse is down and inside
      int  mMouseSensitiveAnim;     //when mouse is moving

      int  mOnLeftClickSound;
};


/////////////////////////////////////////////////////////////////////////////
//GET-SET (INLINE) SECTION
/////////////////////////////////////////////////////////////////////////////
inline bool CPIButtonControl::GetMouseSensitive()
{
   return mMouseSensitive;
}

inline void CPIButtonControl::SetMouseSensitive(bool value)
{
   mMouseSensitive = value;
}

inline int CPIButtonControl::GetMouseDownAnim()
{
   return mMouseDownAnim;
}

inline void CPIButtonControl::SetMouseDownAnim(int value)
{
   mMouseDownAnim = value;
}

inline int CPIButtonControl::GetMouseSensitiveAnim()
{
   return mMouseSensitiveAnim;
}

inline void CPIButtonControl::SetMouseSensitiveAnim(int value)
{
   mMouseSensitiveAnim = value;
}

inline int  CPIButtonControl::GetOnLeftClickSound()
{
   return mOnLeftClickSound;
}

inline void CPIButtonControl::SetOnLeftClickSound(int value)
{
   mOnLeftClickSound = value;
}
//---------------------------------------------------------------------------

#endif
