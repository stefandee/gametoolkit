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

 description: Class Piron String List
 last modify: 24 06 00
        by grabX
---------------------------------------------------------------------------*/

//---------------------------------------------------------------------------
//#include <vcl.h>
#pragma hdrstop

#include "PP_StringList.h"
#include <algorithm>

/*---------------------------------------------------------------------------
 description: adds a string to the list
---------------------------------------------------------------------------*/
void CPStringList::Add(CPString *String)
{
  m_list.push_back(String);
}
/*---------------------------------------------------------------------------
 description: removes the string from list
---------------------------------------------------------------------------*/
void CPStringList::Remove(CPString* String)
{
  std::vector<CPString*>::iterator it = std::find(m_list.begin(), m_list.end(), String);
  if (it != m_list.end()) {
	  m_list.erase(it);
  }
//  m_list.remove(String);
}
/*---------------------------------------------------------------------------
 description: gets the index element
---------------------------------------------------------------------------*/
CPString* CPStringList::GetString(int Index)
{
   return m_list.at(Index);
}
/*---------------------------------------------------------------------------
 description: gets the index element
---------------------------------------------------------------------------*/
CPString *CPStringList::operator[](int Index)
{
   return m_list[Index];
}


//---------------------------------------------------------------------------
#ifdef __BORLANDC__
#pragma package(smart_init)
#endif
