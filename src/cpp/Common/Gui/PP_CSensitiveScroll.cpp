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

 description: Class Piron Componnet Interface - Sensitive Scroll
 last modify: 24 04 00
        by grabX
---------------------------------------------------------------------------*/
//---------------------------------------------------------------------------
#pragma hdrstop

#include "PP_CSensitiveScroll.h"

/////////////////////////////////////////////////////////////////////////////
//PUBLIC SECTION
/////////////////////////////////////////////////////////////////////////////
/*---------------------------------------------------------------------------
 description: constructor
---------------------------------------------------------------------------*/
CPISensitiveScroll::CPISensitiveScroll(CPIWinControl *Parent):CPIControl(Parent)
{
   for (int i=0; i<10; i++)
   {
      mDir[i]  = 0;
      mMin[i]  = 0;
      mMax[i]  = 0;
      mPos[i]  = 0;
      mStep[i] = 0;
   }
   mDimCount       = 1;
   //mHandleOnChange = NULL;
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: destructor
---------------------------------------------------------------------------*/
CPISensitiveScroll::~CPISensitiveScroll()
{
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: on mouse on event
 parameters : X, Y - mouse coord on screen
 others     :
---------------------------------------------------------------------------*/
void CPISensitiveScroll::OnMouseOn(int X, int Y)
{
   OnScroll(this);
   CPIControl::OnMouseOn(X, Y);
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: on mouse move event
 parameters : X, Y - mouse coord on screen
 others     :
---------------------------------------------------------------------------*/
void CPISensitiveScroll::OnMouseMove(int X, int Y)
{
   OnScroll(this);
   CPIControl::OnMouseMove(X, Y);
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: on change event
---------------------------------------------------------------------------*/
void CPISensitiveScroll::OnChange(CPIObject *Sender)
{
  /*if (mHandleOnChange.valid()) {
    mHandleOnChange.callback(this);
  }*/
  if (mHandleOnChange)
  {
    mHandleOnChange(this);
  }
}
//---------------------------------------------------------------------------

/*
void CPISensitiveScroll::OnChange(CPIObject *Sender)
{
   if (mHandleOnChange != NULL)
      mHandleOnChange(this);
}
//---------------------------------------------------------------------------
*/

/*---------------------------------------------------------------------------
 description: on scroll event
---------------------------------------------------------------------------*/
void CPISensitiveScroll::OnScroll(CPIObject *Sender)
{
   for (int i=0; i<GetDimCount(); i++)
      SetPos(GetPos(i) + GetDir(i)*GetStep(i), i);
}

/*---------------------------------------------------------------------------
 description: checks if the pos is in range
---------------------------------------------------------------------------*/
void CPISensitiveScroll::Check()
{
   for (int i=0; i<GetDimCount(); i++)
      SetPos(GetPos(i), i);
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: Pos
---------------------------------------------------------------------------*/
int  CPISensitiveScroll::GetPos(int index)
{
   return mPos[index];
}
//---------------------------------------------------------------------------

void CPISensitiveScroll::SetPosWithNoChange(int value, int index)
{
   if (value < GetMin(index))
      value = GetMin(index);
   else if (value >= GetMax(index))
      value = GetMax(index);
   mPos[index]= value;
}
//---------------------------------------------------------------------------

#ifdef __BORLANDC__
#pragma package(smart_init)
#endif

