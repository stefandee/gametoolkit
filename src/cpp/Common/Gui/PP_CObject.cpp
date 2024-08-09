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

 description: Class Piron Component Interface Object
              is an interface and cannot be instantiated
 last modify: 17 04 00
        by grabX
---------------------------------------------------------------------------*/
//---------------------------------------------------------------------------
//#include <mem.h>
#pragma hdrstop

#include "PP_CObject.h"

/////////////////////////////////////////////////////////////////////////////
//PROTECTED SECTION
/////////////////////////////////////////////////////////////////////////////
/*---------------------------------------------------------------------------
 description: constructor (protected)
---------------------------------------------------------------------------*/
CPIObject::CPIObject()
{
   mName    = "NONAME";
   mTag     = 0;
   mVoidTag = 0;
}

/*---------------------------------------------------------------------------
 description: destructor (protected)
---------------------------------------------------------------------------*/
CPIObject::~CPIObject()
{
}

/////////////////////////////////////////////////////////////////////////////
//PUBLIC SECTION
/////////////////////////////////////////////////////////////////////////////
/*---------------------------------------------------------------------------
 description: return true if the className is a parent of this object
 parameters : className - className wanted
---------------------------------------------------------------------------*/
bool CPIObject::IsDerivedFrom(CPString className)
{
  return (className == ClassName()) != 0;
}

/*---------------------------------------------------------------------------
 description: called at every tick
 others     : overwrite this to handle update
---------------------------------------------------------------------------*/
void CPIObject::Update ()
{
}

//---------------------------------------------------------------------------
#ifdef __BORLANDC__
#pragma package(smart_init)
#endif

