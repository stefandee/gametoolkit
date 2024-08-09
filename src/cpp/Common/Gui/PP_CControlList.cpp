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

 description: Class Piron Component Interface - Control List
 last modify: 19 04 00
        by grabX
---------------------------------------------------------------------------*/
//---------------------------------------------------------------------------
#pragma hdrstop

#include "PP_CControlList.h"

/////////////////////////////////////////////////////////////////////////////
//PUBLIC SECTION
/////////////////////////////////////////////////////////////////////////////
/*---------------------------------------------------------------------------
 description: default constructor
---------------------------------------------------------------------------*/
//CPIControlList::CPIControlList() : TList()
CPIControlList::CPIControlList()
{
  mList = new CPList<CPIControl>(8);
}

/*---------------------------------------------------------------------------
 description: gets the control
 parameters : Index - index of the control
---------------------------------------------------------------------------*/
CPIControl* CPIControlList::operator[](int _index)
{
//   return (CPIControl*)(Items[index]);
  return mList->Get(_index);
}

/*---------------------------------------------------------------------------
 description: gets the index control
 parameters : Index - index of the control
---------------------------------------------------------------------------*/
CPIControl* CPIControlList::GetControl(int _index)
{
//   return (CPIControl*)(Items[Index]);
  return mList->Get(_index);
}

/*---------------------------------------------------------------------------
 description: hides the Add method for security reasons (doesn't allow user to add other pointers)
 parameters : Control - control to add
---------------------------------------------------------------------------*/
int CPIControlList::Add(CPIControl* _control)
{
//   return TList::Add(Control);
  return mList->Add(_control);
}
//---------------------------------------------------------------------------

void CPIControlList::Remove(int _index)
{
  mList->Remove(_index);
}
//---------------------------------------------------------------------------

void CPIControlList::Delete(int _index)
{
  mList->Delete(_index);
}
//---------------------------------------------------------------------------

void CPIControlList::Insert(CPIControl* _control, int _index)
{
  mList->Insert(_control, _index);

  //for(int i = 0; i < Count(); i++)
  //{
  //  logWriteLn(GetControl(i)->ClassName());
  //}
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: returns true if the control is in this list
 parameters : Control - control to look for
---------------------------------------------------------------------------*/
bool CPIControlList::Contain(CPIControl* _control)
{
   if (_control == NULL)
   {
      return false;
   }

   return (mList->Contain(_control));
}
//---------------------------------------------------------------------------

int CPIControlList::Count()
{
  return mList->Count();
}
//---------------------------------------------------------------------------

CPIControlList::~CPIControlList()
{
  delete mList;
}
//---------------------------------------------------------------------------

#ifdef __BORLANDC__
#pragma package(smart_init)
#endif

