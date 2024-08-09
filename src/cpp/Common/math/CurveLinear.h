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
//  Unit              : Line in 2D
//
//  Version           : 1.0
//
//  Description       :
//    * a simple line
//    * for the moment, only the implicit form is implemented (A * x + B * y + C = 0)
//
//  History           :
//    [04.11.2004] - [Karg] - created
//-----------------------------------------------------------------------------

#ifndef CurveLinearH
#define CurveLinearH
//---------------------------------------------------------------------------

#include "Curve2D.h"
#include "basic2d.h"

class CurveLinear : public Curve2D
{
  public:
    CurveLinear(TemplateParamList<double> _paramList) : Curve2D(_paramList) {}
    CurveLinear(Interval _defInterval, TemplateParamList<double> _paramList) : Curve2D(_defInterval, _paramList) {}
    virtual CPString ClassName () {return "Curve2DLinear"; }
    virtual Curve2D* Clone() { return new CurveLinear(*this); }

  public:
    virtual std::vector<double> Compute(TemplateParamList<double>& _paramList);

  protected:

  private:
};

#endif
