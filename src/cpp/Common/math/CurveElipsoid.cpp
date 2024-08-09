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

#include <math.h>
#pragma hdrstop

#include "CurveElipsoid.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

std::vector<double> CurveElipsoid::Compute(TemplateParamList<double>& _paramList)
{
  TemplateParamList<double>& lParamList = GetParamList();

  std::vector<double> lComputed;

  if (lParamList.GetName() == CPString("Parametric"))
  {
    double lU = _paramList.GetParam("U");

    //
    // Test if the u parameter is in the definition interval
    //
    if (GetDefInterval().IsIn(lU))
    {
      double lA = lParamList.GetParam("A");
      double lB = lParamList.GetParam("B");

      lComputed.push_back(lA * cos(lU));
      lComputed.push_back(lB * sin(lU));
    }
  }

  return lComputed;
}
