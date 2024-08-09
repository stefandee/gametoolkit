//-----------------------------------------------------------------------------
// This file is part of PPTactical Engine.                                   //
//                                                                           //
// PPTactical Engine - engine for tactical/strategy games                    //
// Copyright (C) 1998-2004 Stefan Dicu & Tudor Girba                         //
//                                                                           //
// PPTactical Engine is free software; you can redistribute it               //
// and/or modify it under the terms of the GNU Lesser General Public License //
// as published by the Free Software Foundation; either version 2.1 of the   //
// License, or (at your option) any later version.                           //
//                                                                           //
// PPTactical Engine is distributed in the hope that it will be useful, but  //
// WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY//
// or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public    //
// License for more details.                                                 //
//                                                                           //
// You should have received a copy of the GNU Lesser General Public License  //
// along with PPTactical Engine; if not, write to the Free Software          //
// Foundation Inc. 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA   //
//-----------------------------------------------------------------------------

#pragma hdrstop

#include "PP_CCheckBox.h"
#include "basepath.h"
#include "Paths.h"
//---------------------------------------------------------------------------
#ifdef __BORLANDC__
#pragma package(smart_init)
#endif


CPICheckBox::CPICheckBox(CPIWinControl *Parent) : CPIWinControl(Parent)
{
  mTextAllignment = PPDT_RIGHT;

  mFont = new CPFont(PATH_FONTS + FILE_FONT_SMALL, 12, true);

  SetSprite(GetGraphicInstance()->AddSprite(CPString(PATH_GUI_GFX) + "checkbox.png", 4, 1));

  mSpacing = 5;

  mChecked = true;
}
//---------------------------------------------------------------------------

CPICheckBox::~CPICheckBox()
{
  delete mFont;
  mFont = 0;
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: return true if the className is a parent of this object
 parameters : className - className wanted
---------------------------------------------------------------------------*/
bool CPICheckBox::IsDerivedFrom(CPString className)
{
   return ((className == ClassName()) || (CPIWinControl::IsDerivedFrom(className)));
}
//---------------------------------------------------------------------------

void CPICheckBox::SetTextAllignment(int value)
{
   mTextAllignment = value;
}
//---------------------------------------------------------------------------

int CPICheckBox::GetTextAllignment()
{
   return mTextAllignment;
}
//---------------------------------------------------------------------------

CPFont* CPICheckBox::GetFont()
{
   return mFont;
}
//---------------------------------------------------------------------------

void  CPICheckBox::SetFont(CPFont* value)
{
   delete mFont;
   mFont = value;
}
//---------------------------------------------------------------------------

void CPICheckBox::SetColor(TColor32 _color)
{
   if (mFont)
   {
     mFont->SetColor(_color);
   }

   CPIControl::SetColor(_color);
}
//---------------------------------------------------------------------------

void CPICheckBox::Paint()
{
  SetClipRegion();

  GetGraphicInstance()->SetFont(mFont);
  GetGraphicInstance()->SetColor(GetColor());

  int cell = 0;

  if (GetChecked())
  {
    if (GetEnabled())
    {
      cell = 1;
    }
    else
    {
      cell = 3;
    }
  }
  else
  {
    if (GetEnabled())
    {
      cell = 0;
    }
    else
    {
      cell = 2;
    }
  }

  // only two values are legal for text alignment
  if (mTextAllignment == PPDT_LEFT)
  {
    GetGraphicInstance()->TextOut2(GetAbsoluteLeft(), GetAbsoluteTop(), GetText());//, mTextAllignment);

    GetGraphicInstance()->PaintSprite(
      GetAbsoluteLeft() + mFont->GetTextWidth(GetText()) + mSpacing,
      GetAbsoluteTop() + mFont->GetTextHeight(GetText()) / 2 - GetSprite()->GetAnimHeight() / 2,
      GetSprite(),
      cell, 0);
  }
  else
  {
    GetGraphicInstance()->TextOut2(GetAbsoluteLeft() + GetSprite()->GetAnimWidth() + mSpacing, GetAbsoluteTop(), GetText());//, mTextAllignment);

    GetGraphicInstance()->PaintSprite(
      GetAbsoluteLeft(),
      GetAbsoluteTop() + mFont->GetTextHeight(GetText()) / 2 - GetSprite()->GetAnimHeight() / 2,
      GetSprite(),
      cell, 0);
  }
}
//---------------------------------------------------------------------------

void CPICheckBox::OnLeftMouseClick()
{
  SetChecked(!GetChecked());

  CPIWinControl::OnLeftMouseClick();
}
//---------------------------------------------------------------------------

