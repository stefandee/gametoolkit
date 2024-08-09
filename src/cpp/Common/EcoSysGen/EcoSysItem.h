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

//-----------------------------------------------------------------------------
//  Unit              : Eco System Item
//
//  Version           : 1.0
//
//  Description       :
//    * an item in the ecosystem (like a tree or rock)
//
//  History           :
//    [10.12.2004] - [Karg] - created
//-----------------------------------------------------------------------------

#ifndef EcoSysItemH
#define EcoSysItemH
//---------------------------------------------------------------------------

#include "basic2d.h"
#include "PP_String.h"

class EcoSysItemIn
{
  public:
    EcoSysItemIn() {}
    ~EcoSysItemIn() {}

    virtual EcoSysItemIn* Clone() { return new EcoSysItemIn(*this); }

  public: // for now, public
    int      mColorCode;
    CPPoint  mSize;
    int      mCode;
    CPString mFilePath;

  protected:

};

class EcoSysItemOut
{
  public:
    EcoSysItemOut() {}
    ~EcoSysItemOut() {}

  public: // for now, public
    int      mColorCode;
    CPPoint  mLocation, mSize;
    int      mCode;
    CPString mFilePath;

  protected:

};

#endif
