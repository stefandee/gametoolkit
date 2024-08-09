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
//---------------------------------------------------------------------------
#pragma hdrstop

#include "PP_CImage.h"
#include "Paths.h"

//---------------------------------------------------------------------------
#ifdef __BORLANDC__
#pragma package(smart_init)
#endif

CPIImage::CPIImage(CPIWinControl *Parent) : CPIControl(Parent)
{
  mCenter  = false;
  mStretch = false;
}
//---------------------------------------------------------------------------

void CPIImage::LoadFromFile(CPString _fileName)
{
  delete GetSprite();

  // setting a sprite obtained by a call to graphic class
  // could create leaks (because we can't delete the sprite, as
  // it is shared by several objects)
  //SetSprite(GetGraphicInstance()->AddSprite(_fileName, 1, 1));
  SetSprite(new CPGISprite(_fileName, 1, 1));

  if (GetAutoSizeToSprite() == true)
  {
    SetAutoSizeToSprite(true);
  }
}
//---------------------------------------------------------------------------

void CPIImage::Paint()
{
  int  lL = GetAbsoluteLeft(), lT = GetAbsoluteTop();

   if (GetSprite() != NULL)
   {
     SetClipRegion();

     /*
     TRect lRect = GetBoundsRect();

     logWrite("Rect : ");
     logWriteI(lRect.left); logWrite(","); logWriteI(lRect.top); logWrite(",");
     logWriteI(lRect.right); logWrite(","); logWriteILn(lRect.bottom);
     */

     if (GetAutoSizeToSprite())
     {
       GetGraphicInstance()->PaintSprite(lL, lT, GetSprite(), 0, 0);
     }
     else
     {
       if (GetCenter())
       {
         GetGraphicInstance()->PaintSprite(
           lL + GetWidth() / 2 - GetSprite()->GetAnimWidth() / 2,
           lT + GetHeight() / 2 - GetSprite()->GetAnimHeight() / 2,
           GetSprite(),
           0, 0);
       }
       else
       {
         GetGraphicInstance()->PaintSprite(lL, lT, GetSprite(), 0, 0);
       }
    }
  }

  //CPIControl::Paint();
}
//---------------------------------------------------------------------------

CPIImage::~CPIImage()
{
}
//---------------------------------------------------------------------------

