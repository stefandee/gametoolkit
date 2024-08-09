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

 description: Class Piron Component Interface - Check Button
 last modify: 21 04 00
        by grabX
---------------------------------------------------------------------------*/
//---------------------------------------------------------------------------
#pragma hdrstop

#include "PP_CCheckButton.h"
#include "basepath.h"
#include "Paths.h"

/////////////////////////////////////////////////////////////////////////////
//PUBLIC SECTION
/////////////////////////////////////////////////////////////////////////////
/*---------------------------------------------------------------------------
 description: constructor
---------------------------------------------------------------------------*/
CPICheckButton::CPICheckButton(CPIWinControl *Parent) : CPIButtonControl(Parent)
{
//   SetMouseSensitive(true);
   mGroupIndex = 1;
   mCheckedAnim = 1;

   //mIconSprite = GetGraphicInstance()->AddSprite((getGamePath() + PATH_GUI_GFX + "cell.bmp").c_str(), 1, 2);

   mTextAllignment = PPDT_RIGHT | PPDT_VCENTER | PPDT_SINGLELINE;

   SetAutoSizeToSprite(false);

   mFont = new CPFont(PATH_FONTS + FILE_FONT_SMALL);
   GetFont()->SetSize(7);

   SetColor(0xffffff);
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: destructor
---------------------------------------------------------------------------*/
CPICheckButton::~CPICheckButton()
{
  delete mFont;
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: return true if the className is a parent of this object
 parameters : className - className wanted
---------------------------------------------------------------------------*/
bool CPICheckButton::IsDerivedFrom(CPString className)
{
   return ((className == ClassName()) || (CPIButtonControl::IsDerivedFrom(className)));
}
//---------------------------------------------------------------------------

void CPICheckButton::Paint()
{
   SetClipRegion();
   if (GetSprite() != NULL)
      GetGraphicInstance()->PaintSprite(GetAbsoluteLeft(), GetAbsoluteTop(), GetSprite(), GetAnimCounter(), GetAnimType());
   if (GetIconSprite() != NULL)
   {
      int ldH = (GetHeight() - GetIconSprite()->GetAnimHeight())/2;   //adica delta h
      int ldL = (GetWidth() - GetIconSprite()->GetAnimWidth())/2;   //adica delta h

      switch (mTextAllignment)
      {
         case PPDT_LEFT |PPDT_VCENTER | PPDT_SINGLELINE:
            GetGraphicInstance()->PaintSprite(GetAbsoluteLeft() + GetWidth() - ldL*3/2, GetAbsoluteTop() + ldH, GetIconSprite(), 0, 0);
            break;
         case PPDT_CENTER | PPDT_VCENTER | PPDT_SINGLELINE:
            GetGraphicInstance()->PaintSprite(GetAbsoluteLeft() + ldL, GetAbsoluteTop() + ldH, GetIconSprite(), 0, 0);
            break;

         default: //right
            GetGraphicInstance()->PaintSprite(GetAbsoluteLeft() + ldL/2, GetAbsoluteTop() + ldH, GetIconSprite(), 0, 0);
            break;
      }

      if (GetText().isEmpty() == false)
      {
         GetGraphicInstance()->SetFont(mFont);
         GetGraphicInstance()->SetColor(GetColor());
         //GetGraphicInstance()->DrawText(NULL, GetText(), GetAbsoluteBoundsRect(), mTextAllignment);

         GetGraphicInstance()->TextOut2(GetAbsoluteLeft(), GetAbsoluteTop(), GetText());//, mTextAllignment);
      }
   }
   else
      if (GetText().isEmpty() == false)
      {
         GetGraphicInstance()->SetFont(mFont);
         GetGraphicInstance()->SetColor(GetColor());

         GetGraphicInstance()->TextOut2(GetAbsoluteLeft(), GetAbsoluteTop(), GetText(), PPDT_CENTER | PPDT_VCENTER | PPDT_SINGLELINE);
         //GetGraphicInstance()->DrawText(NULL, GetText(), GetAbsoluteBoundsRect(), PPDT_CENTER | PPDT_VCENTER | PPDT_SINGLELINE);
      }
}

/*---------------------------------------------------------------------------
 description: on left mouse click event (overwritten)
 others     : here is reversed the check state
---------------------------------------------------------------------------*/
void CPICheckButton::OnLeftMouseClick()
{
   SetChecked(!GetChecked());
   if (GetGroupIndex() != 0 && GetParent() != NULL && GetChecked() == true)
   {
      for (int i=0; i<GetParent()->GetControls()->Count(); i++)
      {
         if (GetParent()->GetControls()->GetControl(i)->ClassName() == ClassName() &&
             GetParent()->GetControls()->GetControl(i) != this &&
             ((CPICheckButton *)GetParent()->GetControls()->GetControl(i))->GetGroupIndex() == GetGroupIndex())
            ((CPICheckButton *)GetParent()->GetControls()->GetControl(i))->SetChecked(false);
      }
   }
   CPIButtonControl::OnLeftMouseClick();
}
//---------------------------------------------------------------------------

#ifdef __BORLANDC__
#pragma package(smart_init)
#endif


