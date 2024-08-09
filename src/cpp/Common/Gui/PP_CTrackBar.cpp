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
#pragma hdrstop

#include "PP_CTrackBar.h"
#include "PP_Graphic.h"
#include "easylogging++.h"

/*---------------------------------------------------------------------------
 description: constructor
---------------------------------------------------------------------------*/
CPITrackBar::CPITrackBar(CPIWinControl *Parent) : CPIControl(Parent)
{
   mCurrentValue = 1;
   mMaxValue     = 1;

   //mHandleOnChange = NULL;

   SetLeftMouseCaptureEnabled(true);
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: destructor
---------------------------------------------------------------------------*/
CPITrackBar::~CPITrackBar()
{
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: return true if the className is a parent of this object
 parameters : className - className wanted
---------------------------------------------------------------------------*/
bool CPITrackBar::IsDerivedFrom(CPString className)
{
   return (ClassName() == className || CPIControl::IsDerivedFrom(className));
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: paint method
---------------------------------------------------------------------------*/
void CPITrackBar::Paint()
{
   if (GetSprite() != NULL)
   {
      SetClipRegion();
      CPGIGraphicSystem *lGSI;   //local Graphic System Instance
      try
      {
         lGSI = GetGraphicInstance();
      }
      catch(...)
      {
         LOG(FATAL) << "CPITrackBar::Paint - UNABLE TO QUERY GRAPHICS !!!!!!!!!!";
         throw;
      }
      for (int i=0; i<GetMaxValue(); i++)
      {
         if (i<= mCurrentValue)
            lGSI->PaintSprite(GetAbsoluteLeft() + i*GetSprite()->GetAnimWidth(), GetAbsoluteTop(), GetSprite(), GetAnimCounter(), 1);
         else
            lGSI->PaintSprite(GetAbsoluteLeft() + i*GetSprite()->GetAnimWidth(), GetAbsoluteTop(), GetSprite(), GetAnimCounter(), 0);
      }
   }
}

/*---------------------------------------------------------------------------
 description: on mouse move
 parameters : X, Y - mouse position on the screen
---------------------------------------------------------------------------*/
void CPITrackBar::OnMouseMove(int X, int Y)
{
   if (GetWasLeftMouseDown() == true)
   {
      CPPoint lTmp = CoordToScreen(CPPoint(0, 0));
      SetCurrentValue((X - lTmp.x) / GetSprite()->GetAnimWidth());
   }
   CPIControl::OnMouseMove(X, Y);
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: on left mouse down
 parameters : X, Y - mouse position on the screen
---------------------------------------------------------------------------*/
void CPITrackBar::OnLeftMouseDown(int X, int Y)
{
   CPPoint lTmp = CoordToScreen(CPPoint(0, 0));
   SetCurrentValue((X - lTmp.x) / GetSprite()->GetAnimWidth());
   CPIControl::OnLeftMouseDown(X, Y);
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: on left mouse up
 parameters : X, Y - mouse position on the screen
---------------------------------------------------------------------------*/
void CPITrackBar::OnLeftMouseUp(int X, int Y)
{
   CPIControl::OnLeftMouseUp(X, Y);
}

/*---------------------------------------------------------------------------
 description: on value change
 parameters : the old value
---------------------------------------------------------------------------*/
void CPITrackBar::OnChange(int oldValue)
{
   if (mHandleOnChange)
      mHandleOnChange(this, oldValue);
}

/*
void CPITrackBar::OnChange(int oldValue)
{
   if (mHandleOnChange != NULL)
      mHandleOnChange(this, oldValue);
}
*/
//---------------------------------------------------------------------------

#ifdef __BORLANDC__
#pragma package(smart_init)
#endif

