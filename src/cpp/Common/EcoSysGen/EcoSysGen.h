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
//  Unit              : Eco System Generator Base Class
//
//  Version           : 1.0
//
//  Description       :
//    * abstract class for all ecosys generators
//    * we must use a pointer to EcoSysItemIn for the input items because
//      various algorithms require different (but based on the base class) input data
//
//  History           :
//    [10.12.2004] - [Karg] - created
//-----------------------------------------------------------------------------

#ifndef EcoSysGenH
#define EcoSysGenH
//---------------------------------------------------------------------------

#include "EcoSysItem.h"
#include <vector>

class EcoSysGen
{
  public:
    EcoSysGen() {}
    virtual ~EcoSysGen();

  public:
    virtual std::vector<EcoSysItemOut> Generate() = 0;

    void SetItems(std::vector<EcoSysItemIn*> _items);
    std::vector<EcoSysItemIn*> GetItems();

    void SetZone(CPRect _v) { mZone = _v; }
    CPRect GetZone() { return mZone; }

  protected:
    void CleanItems();

  private:
    std::vector<EcoSysItemIn*> mItems;
    CPRect mZone;
};

#endif
