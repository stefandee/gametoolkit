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

 description: Class Piron Component Interface - Button
 last modify: 21 04 00
        by grabX
---------------------------------------------------------------------------*/
//---------------------------------------------------------------------------
#pragma hdrstop

#include "PP_CButton.h"
#include "PP_Graphic.h"
#include "easylogging++.h"

/////////////////////////////////////////////////////////////////////////////
//PUBLIC SECTION
/////////////////////////////////////////////////////////////////////////////
/*---------------------------------------------------------------------------
 description: consutrc
---------------------------------------------------------------------------*/
CPIButton::CPIButton(CPIWinControl *Parent) : CPIButtonControl(Parent)
{
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: destructor
---------------------------------------------------------------------------*/
CPIButton::~CPIButton()
{
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: return true if the className is a parent of this object
 parameters : className - className wanted
---------------------------------------------------------------------------*/
bool CPIButton::IsDerivedFrom(CPString className)
{
   return ((className == ClassName()) || (CPIControl::IsDerivedFrom(className)));
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: paint method
---------------------------------------------------------------------------*/
void CPIButton::Paint()
{
   VLOG(9) << "CPIButton::Paint";

   if (GetSprite() != NULL)
   {
      SetClipRegion();
      GetGraphicInstance()->PaintSprite(GetAbsoluteLeft(), GetAbsoluteTop(), GetSprite(), GetAnimCounter(), GetAnimType());
   }

   VLOG(9) << "CPIButton::Paint - over";
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#ifdef __BORLANDC__
#pragma package(smart_init)
#endif

