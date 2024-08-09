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
#pragma hdrstop

#include "PP_CButtonControl.h"
#include "PP_Sound.h"
#include "easylogging++.h"
//#include "PPC_CScreen.h"

/////////////////////////////////////////////////////////////////////////////
//PROTECTED SECTION
/////////////////////////////////////////////////////////////////////////////
/*---------------------------------------------------------------------------
 description: consutrc
---------------------------------------------------------------------------*/
CPIButtonControl::CPIButtonControl(CPIWinControl *Parent) : CPIControl(Parent)
{
   mMouseSensitive      = true;
   mMouseDownAnim       = 1;
   mMouseSensitiveAnim  = 1;
   SetLeftMouseCaptureEnabled(true);
   mOnLeftClickSound    = -1; //means no sound
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: destructor
---------------------------------------------------------------------------*/
CPIButtonControl::~CPIButtonControl()
{
}
//---------------------------------------------------------------------------

/////////////////////////////////////////////////////////////////////////////
//PUBLIC SECTION
/////////////////////////////////////////////////////////////////////////////
/*---------------------------------------------------------------------------
 description: return true if the className is a parent of this object
 parameters : className - className wanted
---------------------------------------------------------------------------*/
bool CPIButtonControl::IsDerivedFrom(CPString className)
{
   return ((className == ClassName()) || (CPIControl::IsDerivedFrom(className)));
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: paint method
---------------------------------------------------------------------------*/
void CPIButtonControl::Paint()
{
   CPIControl::Paint();
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: On left mouse down event (overwritten)
 parameters : X, Y - position of the cursor in screen coord
---------------------------------------------------------------------------*/
void CPIButtonControl::OnLeftMouseDown(int X, int Y)
{
   SetAnimType(GetMouseDownAnim());
   CPIControl::OnLeftMouseDown(X, Y);
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: On left mouse move event (overwritten)
 parameters : X, Y - position of the cursor in screen coord
---------------------------------------------------------------------------*/
void CPIButtonControl::OnMouseMove(int X, int Y)
{
   if (GetWasLeftMouseDown() == true)
      if (IsPointInside(CPPoint(X, Y)) == true)
         SetAnimType(GetMouseDownAnim());
      else
         SetAnimType(0);
   else
   {//mouse was not down
/*      if (GetMouseSensitive() == true)
      {
         CPIScreen *lMSI;
         try
         {
            lMSI = GetMainScreenInstance();
         }
         catch(...)
         {
            throw;
         }
         if (IsPointInside(CPPoint(X, Y)) == true)
         {
            lMSI->SetMouseCaptureControl(this);
            SetAnimType(GetMouseSensitiveAnim());
         }
         else
         {
            lMSI->ReleaseMouseCapture();
            SetAnimType(0);
         }
      }
*/
   }
   CPIControl::OnMouseMove(X, Y);
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: On left mouse up event (overwritten)
 parameters : X, Y - position of the cursor in screen coord
---------------------------------------------------------------------------*/
void CPIButtonControl::OnLeftMouseUp(int X, int Y)
{
   if (GetWasLeftMouseDown() == true)
   {
      SetAnimType(0);
   }
   CPIControl::OnLeftMouseUp(X, Y);
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: on left mouse click event
---------------------------------------------------------------------------*/
void CPIButtonControl::OnLeftMouseClick()
{
   if (GetOnLeftClickSound() != -1)
   {//means that there is a sound
      CPSISoundSystem *lSSI;
      try
      {
         lSSI = GetSoundInstance();
      }
      catch(...)
      {
         LOG(FATAL) << "CPIButtonControl::OnLeftMouseClick - UNABLE TO QUERY SOUND !!!!!!!!!!";
         throw;
      }
      lSSI->Play(GetOnLeftClickSound(), 0);
   }
   CPIControl::OnLeftMouseClick();
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#ifdef __BORLANDC__
#pragma package(smart_init)
#endif
