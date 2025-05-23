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

 description: Class Piron Component Interface - Label
 last modify: 21 04 00
        by grabX
---------------------------------------------------------------------------*/
//---------------------------------------------------------------------------
#pragma hdrstop

#include "PP_CLabel.h"
#include "PP_Graphic.h"
#include "Paths.h"
#include "basepath.h"
#include "easylogging++.h"
/////////////////////////////////////////////////////////////////////////////
//PUBLIC SECTION
/////////////////////////////////////////////////////////////////////////////
/*---------------------------------------------------------------------------
 description: constructor

---------------------------------------------------------------------------*/
CPILabel::CPILabel(CPIWinControl *Parent) : CPIControl(Parent)
{
#if PPT_USE_SDL
   mFont = new CPFont(PATH_FONTS + FILE_FONT_SMALL, 12, true);
#else
   mFont = new CPFont(PATH_FONTS + FILE_FONT_SMALL);
   mFont->SetSize(12);
#endif // PPT_USE_SDL
   SetColor(0x00FFFFFF);
   SetAutoSize(false);
   SetWidth(100);
   SetHeight(13);
   SetTextAllignment(PPDT_LEFT);

   mMultiline = false;
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: destructor
---------------------------------------------------------------------------*/
CPILabel::~CPILabel()
{
   delete mFont;
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: return true if the className is a parent of this object
 parameters : className - className wanted
---------------------------------------------------------------------------*/
bool CPILabel::IsDerivedFrom(CPString className)
{
   return ((className == ClassName()) || (CPIControl::IsDerivedFrom(className)));
}
//---------------------------------------------------------------------------

void CPILabel::SetColor(TColor32 _color)
{
   if (mFont)
   {
     mFont->SetColor(_color);
   }

   CPIControl::SetColor(_color);
}
//---------------------------------------------------------------------------

void CPILabel::SetWidth (int value)
{
  if (!mFont)
  {
    return;
  }

  if (mAutoSize)
  {
    CPIControl::SetWidth(mFont->GetTextWidth(GetText()));
    return;
  }

  CPIControl::SetWidth(value);
}
//---------------------------------------------------------------------------

void CPILabel::SetHeight (int value)
{
  if (!mFont)
  {
    return;
  }

  if (mAutoSize)
  {
    CPIControl::SetHeight(mFont->GetTextHeight(GetText()));
    return;
  }

  CPIControl::SetHeight(value);
}
//---------------------------------------------------------------------------

void CPILabel::SetText (CPString value)
{
  if (!mFont)
  {
    return;
  }

  CPIControl::SetText(value);

  if (mAutoSize)
  {
    // it doesnt matter what we send to these methods if autosize is true;
    SetWidth(0);
    SetHeight(0);
  }
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: update function
---------------------------------------------------------------------------*/
void CPILabel::Update()
{
   CPIControl::Update();
}
//---------------------------------------------------------------------------

CPRect CPILabel::GetClipRegion()
{
    CPRect result = CPIControl::GetClipRegion();

    if (mTextAllignment & PPDT_CENTER)
    {
        int halfWidth = GetWidth() / 2;
        result.left -= halfWidth;
        result.right -= halfWidth;
    }

    return result;
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: paint method
 others     :
---------------------------------------------------------------------------*/
void CPILabel::Paint()
{
   if (!GetVisible())
   {
      return;
   }

   VLOG(9) << "CPILabel::Paint";

   /*
   if (GetText() == CPString("Attack"))
   {
     CPRect lR = GetClipRegion();
     logWriteLn(CPString(lR.left) + "," + CPString(lR.top) + "->" + CPString(lR.right) + "," + CPString(lR.bottom));
   }
   */

   SetClipRegion();

   GetGraphicInstance()->SetFont(GetFont());
   //lGSI->SetColor(GetColor());

   if (mMultiline)
   {
#if PPT_USE_SDL
        GetGraphicInstance()->TextOutWrapped(GetAbsoluteLeft(), GetAbsoluteTop(), GetText(), GetWidth());
#endif
   }
   else
   {
        GetGraphicInstance()->TextOut2(GetAbsoluteLeft(), GetAbsoluteTop(), GetText(), mTextAllignment);
   }
   //GetGraphicInstance()->TextOut(0, GetLeft(), GetTop(), GetText());

   //logWrite("Current hint: "); logWrite(GetText() + "..."); logWriteI(GetAbsoluteLeft()); logWrite(","); logWriteILn(GetAbsoluteTop());
   //logWrite("CPILabel::Paint - "); logWriteLn(GetText().c_str());

   VLOG(9) << "CPILabel::Paint - over";
}
//---------------------------------------------------------------------------

#ifdef __BORLANDC__
#pragma package(smart_init)
#endif

