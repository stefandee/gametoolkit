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

 description: Class Piron Component Interface - Screen
              it should allow only one instance (Singleton Pattern)
 last modify: 19 04 00
        by grabX
---------------------------------------------------------------------------*/
//---------------------------------------------------------------------------
#pragma hdrstop

#include "PP_CScreen.h"
#include "PP_Graphic.h"
#include "PPIG_Cursor.h"
#include "PP_CForm.h"
#include "PP_Keyboard.h"
#include "easylogging++.h"
#include "PP_Stdlib.h"

/////////////////////////////////////////////////////////////////////////////
//PROTECTED SECTION
/////////////////////////////////////////////////////////////////////////////
/*---------------------------------------------------------------------------
 description: default constructor
---------------------------------------------------------------------------*/
CPIScreen::CPIScreen()
{
   mDesktop                = 0;
   mLastControlLeftDown    = 0;
   mLastControlRightDown   = 0;
   mKeyboardCaptureControl = 0;
   mMouseCaptureControl    = 0;
   mActions                = 0;
   mHintControl            = new CPILabel(0);
   mHintControl->SetAutoSize(true);
   mHintControl->SetColor(0xFFB0B0B0);
   mShowHint               = true;
   mControlShowHint        = false;
   mModalDialog            = 0;

   //mHintControl->GetFont()->SetUnderline(true);

   //mHandleOnHint           = NULL;
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: destructor
---------------------------------------------------------------------------*/
CPIScreen::~CPIScreen()
{
   if (GetDesktop() != NULL)
      delete GetDesktop();
   if (GetHintControl() != NULL)
      delete GetHintControl();
}
//---------------------------------------------------------------------------

/////////////////////////////////////////////////////////////////////////////
//PUBLIC SECTION
/////////////////////////////////////////////////////////////////////////////
/*---------------------------------------------------------------------------
 description: here are events generated
 others     : overwrite this metod to get acces to the tick event,
              this method should be called in this case
---------------------------------------------------------------------------*/
void CPIScreen::Update()
{
   if (GetDesktop() != NULL)
   {
      //Update the desktop or the modal dialog
      if (!mModalDialog)
      {
        VLOG(5) << "Desktop Update begins ...";
        GetDesktop()->Update();
      }
      else
      {
        mModalDialog->Update();
      }

      //Events
      UpdateEvents(); //private method

      //Paint
      VLOG(5) << "Desktop Paint begins ...";
      try
      {
         GetDesktop()->Paint();

         if (GetHintControl() != NULL && GetControlShowHint() == true)
         {
            GetHintControl()->Paint();
         }
      }
      catch(...)
      {
         LOG(FATAL) << "Screen::Update - UNABLE TO PAINT DESKTOP !!!!!!!!!!";
         throw;
      }
      try
      {
         CPGIGraphicSystem *lGSI;
         lGSI = GetGraphicInstance();
#if PPT_USE_SDL
         lGSI->ResetClipRegion();
#else
         lGSI->ResetClipRegion(NULL);
#endif // PPT_USE_SDL
      }
      catch(...)
      {
         LOG(FATAL) << "Screen::Update - UNABLE TO QUERY GRAPHICS !!!!!!!!!!";
         throw;
      }

      if (!mModalDialog)
      {
        if (GetActions() != NULL)
           GetActions()->Update();
      }
   }
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: sets the mouse capturer control
 parameters : Control - the one that captures
---------------------------------------------------------------------------*/
void CPIScreen::SetMouseCaptureControl (CPIControl* Control)
{
  mMouseCaptureControl    = Control;
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: sets the keyb capturer control
 parameters : Control - the one that captures
---------------------------------------------------------------------------*/
void CPIScreen::SetKeyboardCaptureControl (CPIControl* Control)
{
   mKeyboardCaptureControl = Control;
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: releases the mouse capture
---------------------------------------------------------------------------*/
void CPIScreen::ReleaseMouseCapture ()
{
   mMouseCaptureControl    = NULL;
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: releases the keyb capture
---------------------------------------------------------------------------*/
void CPIScreen::ReleaseKeyboardCapture ()
{
   mKeyboardCaptureControl = NULL;
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: On hint event
 parameters : Sender - the object that sends the hint msg
---------------------------------------------------------------------------*/
void CPIScreen::OnHint(CPIObject *Sender)
{
   if (mHandleOnHint)
      mHandleOnHint(Sender);
}
//---------------------------------------------------------------------------

/*
void CPIScreen::OnHint(CPIObject *Sender)
{
   if (mHandleOnHint != NULL)
      mHandleOnHint(Sender);
}
//---------------------------------------------------------------------------
*/

/////////////////////////////////////////////////////////////////////////////
//PRIVATE SECTION
/////////////////////////////////////////////////////////////////////////////
/*---------------------------------------------------------------------------
 description: updates the events
 others     : check the mouse and the keyboard and creates their events
---------------------------------------------------------------------------*/
void CPIScreen::UpdateEvents()
{
   VLOG(5) << "Events update begins ...";
   CPIICursor *lCI;
   try
   {
      lCI = GetCursorInstance();
   }
   catch(...)
   {
      LOG(FATAL) << "Screen::Update - UNABLE TO QUERY MOUSE !!!!!!!!!!";
      throw;
   }

   CPIControl *lControl;
   if (mMouseCaptureControl != NULL)
   {
      lControl = mMouseCaptureControl;
   }
   else if (mKeyboardCaptureControl != NULL)
   {
      lControl = mKeyboardCaptureControl;
   }
   else
   {
      if (mModalDialog)
      {
        lControl = mModalDialog->GetControlInPoint(lCI->GetPosX(), lCI->GetPosY());
      }
      else
      {
        lControl = GetDesktop()->GetControlInPoint(lCI->GetPosX(), lCI->GetPosY());
      }
   }

   if (lControl != NULL)
   {
      lCI->SetType(lControl->GetMouseType());
      //Left mouse test
      if (lCI->IsLeftDown())
      {
         lControl->OnLeftMouseDown(lCI->GetPosX(), lCI->GetPosY());
         if (lControl->IsPointInside(CPPoint(lCI->GetPosX(), lCI->GetPosY())) == true)
            mLastControlLeftDown = lControl;  //the condiion is necessary because it is possible
                                              //that the mouse is captured
         else
            mLastControlLeftDown = NULL;
      }
      else if (lCI->IsLeftUp())
      {
         lControl->OnLeftMouseUp(lCI->GetPosX(), lCI->GetPosY());
         if (lControl == mLastControlLeftDown && lControl->IsPointInside(CPPoint(lCI->GetPosX(), lCI->GetPosY())) == true)
            lControl->OnLeftMouseClick();
         mLastControlLeftDown = NULL;
      }
      //Left mouse test - over
      //Right mouse test
      if (lCI->IsRightDown())
      {
         lControl->OnRightMouseDown(lCI->GetPosX(), lCI->GetPosY());
         if (lControl->IsPointInside(CPPoint(lCI->GetPosX(), lCI->GetPosY())) == true)
            mLastControlRightDown = lControl;  //the condiion is necessary because it is possible
                                              //that the mouse is captured
         else
            mLastControlRightDown = NULL;
      }
      else if (lCI->IsRightUp())
      {
         lControl->OnRightMouseUp(lCI->GetPosX(), lCI->GetPosY());
         if (lControl == mLastControlRightDown && lControl->IsPointInside(CPPoint(lCI->GetPosX(), lCI->GetPosY())) == true)
            lControl->OnRightMouseClick();
         mLastControlRightDown = NULL;
      }
      //Right mouse test - over

      if (lCI->IsMoved())
      {
         lControl->OnMouseMove(lCI->GetPosX(), lCI->GetPosY());
      }
      else
         lControl->OnMouseOn(lCI->GetPosX(), lCI->GetPosY());

      // fly the keyboard messages
      lControl->OnKeyPressed(GetKeyboardInstance()->GetFirstKeyPressed());

      //hint
      SetControlShowHint(lControl->GetShowHint() && GetShowHint());
      //GetHintControl()->SetVisible(true);
      //GetHintControl()->SetEnabled(true);
      //SetControlShowHint(true);
      GetHintControl()->SetText(lControl->GetHint());

      //logWriteLn(lControl->GetHint());

      // until the control coordinates "mess" clarifies, we have to set the
      // hint label coordinates like this
      int hintX = lCI->GetPosX();
      int hintY = lCI->GetPosY() + 16;

      hintX = clamp(hintX, 0, GetDesktop()->GetWidth() - GetHintControl()->GetWidth());
      hintY = clamp(hintY, 0, GetDesktop()->GetHeight() - GetHintControl()->GetHeight());

      /*
      if (hintX + GetHintControl()->GetWidth() > GetDesktop()->GetWidth())
      {
          hintX = GetDesktop()->GetWidth() - GetHintControl()->GetWidth();
      }

      if (hintY + GetHintControl()->GetHeight() > GetDesktop()->GetHeight())
      {
          hintY = GetDesktop()->GetHeight() - GetHintControl()->GetHeight();
      }
      */

      GetHintControl()->SetAbsoluteLeft(hintX);
      GetHintControl()->SetAbsoluteTop(hintY);
      GetHintControl()->SetLeft(hintX);
      GetHintControl()->SetTop(hintY);
      //GetHintControl()->SetAbsoluteLeft(0);
      //GetHintControl()->SetAbsoluteTop(0);
   }
   VLOG(5) << "Events update over";
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#ifdef __BORLANDC__
#pragma package(smart_init)
#endif

