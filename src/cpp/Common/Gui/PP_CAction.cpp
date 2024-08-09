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

 description: Class Piron Component Interface - Action
 last modify: 06 05 00
        by grabX
---------------------------------------------------------------------------*/
//---------------------------------------------------------------------------
//#include <mem.h>
#pragma hdrstop

#include "PP_CAction.h"
#include "PP_Keyboard.h" //it's not entirely right
#include "easylogging++.h"

/*---------------------------------------------------------------------------
 description: constructor
---------------------------------------------------------------------------*/
CPIAction::CPIAction()
{
   //mHandleOnActivate = NULL;
   mKeysPressed      = 0;
   mKeysDown         = 0;
   mKeysUp           = 0;
   mEnabled          = true;
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: destructor
---------------------------------------------------------------------------*/
CPIAction::~CPIAction()
{

}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: handles the tick event
---------------------------------------------------------------------------*/
void CPIAction::Update()
{
   if (GetEnabled() == false)
      return;
   if (RequiredKeysArePressed() && RequiredKeysAreDown() && RequiredKeysAreUp())
      OnActivate();
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: on activate event handler
---------------------------------------------------------------------------*/
/*
void CPIAction::OnActivate ()
{
   if (mHandleOnActivate != NULL)
      mHandleOnActivate(this );
}
*/

void CPIAction::OnActivate ()
{
   if (mHandleOnActivate)
   {
      mHandleOnActivate(this);
   }
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: returns true if the required pressed keys are pressed
---------------------------------------------------------------------------*/
bool CPIAction::RequiredKeysArePressed()
{
   CPGIKeyboard *lKI;
   try
   {
      lKI = GetKeyboardInstance();
   }
   catch(...)
   {
      LOG(FATAL) << "CPIAction::Update - UNABLE TO QUERY KEYBOARD !!!!!!!!!!";
      throw;
   }

   if (GetKeysPressed() != 0)
   {
      return lKI->IsPressed(GetKeysPressed());
   }
   else
      return true;
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: returns true if the required down keys are down
---------------------------------------------------------------------------*/
bool CPIAction::RequiredKeysAreDown()
{
   CPGIKeyboard *lKI;
   try
   {
      lKI = GetKeyboardInstance();
   }
   catch(...)
   {
      LOG(FATAL) << "CPIAction::Update - UNABLE TO QUERY KEYBOARD !!!!!!!!!!";
      throw;
   }
   if (GetKeysDown() != 0)
   {
      return lKI->IsDown(GetKeysDown());
   }
   else
      return true;
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: returns true if the required up keys are up
---------------------------------------------------------------------------*/
bool CPIAction::RequiredKeysAreUp()
{
   CPGIKeyboard *lKI;
   try
   {
      lKI = GetKeyboardInstance();
   }
   catch(...)
   {
      LOG(FATAL) << "CPIAction::Update - UNABLE TO QUERY KEYBOARD !!!!!!!!!!";
      throw;
   }
   if (GetKeysUp() != 0)
   {
      return lKI->IsUp(GetKeysUp());
   }
   else
      return true;
}
//---------------------------------------------------------------------------

#ifdef __BORLANDC__
#pragma package(smart_init)
#endif

