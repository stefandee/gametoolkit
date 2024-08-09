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

 description: Class Piron Component Interface - Edit
 last modify: 20 01 01
        by grabX
---------------------------------------------------------------------------*/
//---------------------------------------------------------------------------
//#include <vcl.h>
#pragma hdrstop

#include "PP_CEdit.h"

CPIEdit::CPIEdit(CPIWinControl *Parent) : CPIEditControl(Parent)
{
}
CPIEdit::~CPIEdit()
{
}

void CPIEdit::Paint()
{
   SetClipRegion();
   GetGraphicInstance()->SetColor(0xFFFFFFFF);
   GetGraphicInstance()->Rectangle(GetAbsoluteBoundsRect());
   CPIEditControl::Paint();
}

//---------------------------------------------------------------------------
#ifdef __BORLANDC__
#pragma package(smart_init)
#endif
