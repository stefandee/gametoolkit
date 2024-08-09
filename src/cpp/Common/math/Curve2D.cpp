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

#include "Curve2D.h"
#include "Serializable.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

void  Curve2D::Serialize(PP::Stream &a)
{
  CSerString(mName).Serialize(a);
  mDefInterval.Serialize(a);

  CSerString(mParamList.GetName()).Serialize(a);

  std::vector<std::string> lKeys = mParamList.GetKeys();

  int lSize = lKeys.size();

  a.writeInt(&lSize);

  for(unsigned int i = 0; i < lKeys.size(); i++)
  {
    CSerString(CPString(lKeys[i].c_str())).Serialize(a);

    double lValue = mParamList.GetParam(CPString(lKeys[i].c_str()));

    a.writeDouble(&lValue);
  }
}
//---------------------------------------------------------------------------

void  Curve2D::DeSerialize(PP::Stream &a)
{
  CSerString lSerName, lSerListName;

  lSerName.DeSerialize(a);
  mDefInterval.DeSerialize(a);

  //
  // read the param list name
  //
  lSerListName.DeSerialize(a);
  mParamList.SetName(lSerListName.Get());

  mParamList.Clear();

  //
  // read the number of (key,value) pairs
  //
  int lSize;
  a.readInt(&lSize);

  //
  // read the (key, value) pairs and add them in the list
  //
  for(int i = 0; i < lSize; i++)
  {
    CSerString lKey;
    double     lValue;

    lKey.DeSerialize(a);
    a.readDouble(&lValue);

    mParamList.AddParam(lKey.Get(), lValue);
  }

  mName = lSerName.Get();
}
//---------------------------------------------------------------------------


