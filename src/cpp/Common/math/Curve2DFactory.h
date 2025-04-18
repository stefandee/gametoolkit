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
//  Unit              : Curve Factory
//
//  Version           : 1.0
//
//  Description       :
//    * class that creates curves
//    * it is also used to load and save curves to streams (that is, first read/write
//      its type - class name, id, whatever - then it uses its factory capabilities
//      to create the appropriate curve and ask the newly created curve to serialize/
//      deserialize)
//
//  History           :
//    [18.11.2004] - [Karg] - unit created
//-----------------------------------------------------------------------------
//---------------------------------------------------------------------------

#ifndef Curve2DFactoryH
#define Curve2DFactoryH
//---------------------------------------------------------------------------

#include "PP_String.h"
#include "Curve2D.h"

class Curve2DFactory
{
  public:
    Curve2DFactory() {}
    ~Curve2DFactory() {}

  public:
    Curve2D* Get(CPString _className);

  protected:

  private:

};
//---------------------------------------------------------------------------

#endif
