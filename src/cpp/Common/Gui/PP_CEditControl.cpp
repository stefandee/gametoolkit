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
//#include <vcl.h>
#pragma hdrstop

#include "easylogging++.h"

#include "PP_CEditControl.h"
#include "PPIG_CScreen.h"
#include "PP_KeyConvertor.h"
#include "PP_Keyboard.h"
#include "PP_Timer.h"
#include "PP_Input.h"
#include "PP_Cursor.h"
#include "Paths.h"
#include "basepath.h"
#include "PlayerProfile.h"

/*---------------------------------------------------------------------------
 PROTECTED SECTION
---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------
 description: constructor
---------------------------------------------------------------------------*/
CPIEditControl::CPIEditControl(CPIWinControl *Parent) : CPIControl(Parent)
{
   mFont = new CPFont(PATH_FONTS + FILE_FONT_SMALL);

   SetColor(0xffffffff);
   mBlinkCounter.SetMaxValue(3);

   mHasFocus = false;
   mTextAllignment = PPDT_LEFT;
   mMaxTextLength = 0;
   mTextLengthLimited = false;

   //mRepeatDelay = 0;
   mLastDelayTime = GetTimerInstance()->GetTime();

   //mRepeatRate = 0;
   mLastRateTime = GetTimerInstance()->GetTime();
}

/*---------------------------------------------------------------------------
 description: destructor
---------------------------------------------------------------------------*/
CPIEditControl::~CPIEditControl()
{
   delete mFont;
}

/*---------------------------------------------------------------------------
 PROTECTED SECTION
---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------
 description: return true if the className is a parent of this object
 parameters : className - className wanted
---------------------------------------------------------------------------*/
bool CPIEditControl::IsDerivedFrom(CPString className)
{
   return ((className == ClassName()) || (CPIControl::IsDerivedFrom(className)));
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: update function
---------------------------------------------------------------------------*/
void CPIEditControl::Update()
{
   CPIControl::Update();
   mBlinkCounter++;
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: paint method
 others     :
---------------------------------------------------------------------------*/
void CPIEditControl::Paint()
{
   SetClipRegion();
   CPGIGraphicSystem *lGSI;
   try
   {
      lGSI = GetGraphicInstance();
   }
   catch(...)
   {
      LOG(FATAL) << "CPILabel::Paint - UNABLE TO QUERY GRAPHICS !!!!!!!!!!!!!";
      throw;
   }
   lGSI->SetFont(GetFont());
   lGSI->SetColor(GetColor());

   int lL = GetAbsoluteLeft(), lT = GetAbsoluteTop();

   if ((mBlinkCounter.MaxReached() == true) && mHasFocus)
      lGSI->TextOut2(lL, lT, GetText() + "|", mTextAllignment);
   else
      lGSI->TextOut2(lL, lT, GetText(), mTextAllignment);
}

/*---------------------------------------------------------------------------
 description: on left mouse down may lose focus
---------------------------------------------------------------------------*/
void CPIEditControl::OnLeftMouseDown(int X, int Y)
{
   if (IsPointInside(CPPoint(X, Y)) == false && mHasFocus == true)
      DeFocus();
}

/*---------------------------------------------------------------------------
 description: on left mouse click get the focus
---------------------------------------------------------------------------*/
void CPIEditControl::OnLeftMouseClick()
{
   Focus();
   CPIControl::OnLeftMouseClick();
}

/*---------------------------------------------------------------------------
 description: on key pressed event
 parameters : key - the dik
---------------------------------------------------------------------------*/
void CPIEditControl::OnKeyPressed(int key)
{
   key = GetKeyboardInstance()->GetFirstKeyPressed(/*true*/);

   if (key == -1)
   {
     return;
   }

   if (GetTimerInstance()->GetTime() - mLastDelayTime < GetProfileInstance()->GetRepeatDelay())
   {
     return;
   }

   mLastDelayTime = GetTimerInstance()->GetTime();

   if (mPrevKey == key)
   {
     if (GetTimerInstance()->GetTime() - mLastRateTime < GetProfileInstance()->GetRepeatRate())
     {
       return;
     }
   }

   mLastRateTime = GetTimerInstance()->GetTime();
   mPrevKey = key;

   switch (key)
   {
      case PPK_ESCAPE:
        DeFocus();
        break;

      case PPK_BKSPACE:
        SetText(GetText().SubString(0, GetText().Length() - 1));
        break;

      default:
        if (GetTextLengthLimited() == true)
        {
          if (GetMaxTextLength() > GetText().Length())
              SetText(GetText() + ConvertKey(GetKeyboardInstance()->IsPressed(PPK_LSHIFT) || GetKeyboardInstance()->IsPressed(PPK_RSHIFT), key));
        }
        else
        {
          SetText(GetText() + ConvertKey(GetKeyboardInstance()->IsPressed(PPK_LSHIFT) || GetKeyboardInstance()->IsPressed(PPK_RSHIFT), key));
        }
        break;
   }

   CPIControl::OnKeyPressed(key);
}


/*---------------------------------------------------------------------------
 description: sets the focus to the control
---------------------------------------------------------------------------*/
void CPIEditControl::Focus()
{
   GetMainScreenInstance()->SetKeyboardCaptureControl(this);
   GetMainScreenInstance()->SetMouseCaptureControl(this);
   mHasFocus = true;
   GetCursorInstance()->SetVisible(false);
   GetCursorInstance()->SetEnabled(false);
}
/*---------------------------------------------------------------------------
 description: forces the control to lose the focus if it has it
 parameters :
---------------------------------------------------------------------------*/
void CPIEditControl::DeFocus()
{
   GetMainScreenInstance()->SetKeyboardCaptureControl(NULL);
   GetMainScreenInstance()->SetMouseCaptureControl(NULL);
   mHasFocus = false;

   GetCursorInstance()->SetVisible(true);
   GetCursorInstance()->SetEnabled(true);
}

//---------------------------------------------------------------------------
#ifdef __BORLANDC__
#pragma package(smart_init)
#endif
