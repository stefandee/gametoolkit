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

 description: Class Piron Component Interface - Action List
 last modify: 07 05 00
        by grabX
---------------------------------------------------------------------------*/
//---------------------------------------------------------------------------
#pragma hdrstop

#include "PP_CActionList.h"

/*---------------------------------------------------------------------------
 description: constructor
---------------------------------------------------------------------------*/
//CPIActionList::CPIActionList() : TList()
CPIActionList::CPIActionList()
{
   mEnabled = true;
   mList    = new CPList<CPIAction>(8); 
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: handles the tick event
 others     : it calls the update methods from all the actions
---------------------------------------------------------------------------*/
void CPIActionList::Update()
{
   /*
   if (mEnabled == true)
      for (int i=0; i<Count; i++)
         ((CPIAction *)Items[i])->Update();
   */      
   if (mEnabled == true)
      for (int i = 0; i < mList->Count(); i++)
         (mList->Get(i))->Update();
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: adds an action to the list
---------------------------------------------------------------------------*/
void CPIActionList::Add(CPIAction *Action)
{
   //TList::Add(Action);
   mList->Add(Action);
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: removes the Action from the action list
---------------------------------------------------------------------------*/
void CPIActionList::Remove(CPIAction *Action)
{
   //TList::Remove(Action);
   for(int i = 0; i < mList->Count(); i++)
   {
     if (Action == mList->Get(i))
     {
       mList->Remove(i);
       return;
     }
   }
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: gets the action with Index
 parameters : Index - index of trhe wanted action
---------------------------------------------------------------------------*/
CPIAction *CPIActionList::GetAction(int Index)
{
   //return ((CPIAction *)Items[Index]);
   return mList->Get(Index);
}
//---------------------------------------------------------------------------

/*---------------------------------------------------------------------------
 description: gets the action with Index
 parameters : Index - index of trhe wanted action
---------------------------------------------------------------------------*/
CPIAction *CPIActionList::operator[](int Index)
{
   //return ((CPIAction *)Items[Index]);
   return mList->Get(Index);
}
//---------------------------------------------------------------------------

CPIActionList::~CPIActionList()
{
  delete mList;
}
//---------------------------------------------------------------------------

#ifdef __BORLANDC__
#pragma package(smart_init)
#endif

