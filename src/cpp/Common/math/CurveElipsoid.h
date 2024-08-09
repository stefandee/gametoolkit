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
//  Unit              : Ellipsoid
//
//  Version           : 1.0
//
//  Description       :
//    * a curve for an ellipsoid and its particular brother, the circle
//    * only the parametric form is supported: x(u) = R * sin(u), y(u) = R * cos(u)
//
//  History           :
//    [04.11.2004] - [Karg] - created
//-----------------------------------------------------------------------------

#ifndef CurveElipsoidH
#define CurveElipsoidH
//---------------------------------------------------------------------------

#include "Curve2D.h"
#include "basic2d.h"

class CurveElipsoid : public Curve2D
{
  public:
    CurveElipsoid(TemplateParamList<double> _paramList) : Curve2D(_paramList) {}
    CurveElipsoid(Interval _defInterval, TemplateParamList<double> _paramList) : Curve2D(_defInterval, _paramList) {}
    virtual CPString ClassName () {return "Curve2DElipsoid"; }
    virtual Curve2D* Clone() { return new CurveElipsoid(*this); }

  public:
    virtual std::vector<double> Compute(TemplateParamList<double>& _paramList);

  protected:

  private:
};

#endif
