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
//  Unit              : Planar Curve
//
//  Version           : 1.0
//
//  Description       :
//    * abstract class - a curve in plane
//    * a curve may be defined by implicit equation (ax+by+c = 0 for a line)
//      explicit (y=ax+b) or parametric (x(u) = x0 + u * a, y(u) = y0 + u * b)
//    * when you set the parameters for a curve (a TemplateParameterList),
//      make sure you set the name of the list with the one the curve supports
//    * in Compute, there are checks for the parameter list name and the computation
//      goes accordingly to this name
//    * also make sure you send the right number and names for the values you want
//      the curve to take into account
//    * a curve also has an interval - to be defined its functionality (some curves take
//      into account, some other dont...)
//    * must also add serialization/deserialization
//
//  History           :
//    [01.11.2004] - [Karg] - created
//-----------------------------------------------------------------------------

#ifndef Curve2DH
#define Curve2DH
//---------------------------------------------------------------------------

#include "Interval.h"
#include "TemplateParamList.h"
#include "Stream.h"
#include <vector>

class Curve2D
{
  public:
    Curve2D() : mName("") { }
    Curve2D(const Interval& _defInterval) : mDefInterval(_defInterval), mName("Nonamed") {}
    Curve2D(Interval _defInterval, TemplateParamList<double> _paramList) : mDefInterval(_defInterval), mName("Nonamed") { SetParamList(_paramList); }
	Curve2D(TemplateParamList<double> _paramList) : mName("Nonamed") { SetParamList(_paramList); }
    virtual ~Curve2D() {}

    virtual CPString ClassName () {return "Curve2D"; }
    virtual Curve2D* Clone() = 0;

  public:
    virtual void  SetDefInterval(Interval _v) { mDefInterval = _v; }
    virtual Interval& GetDefInterval() { return mDefInterval; }

    virtual TemplateParamList<double>& GetParamList() { return mParamList; }
    virtual void SetParamList(TemplateParamList<double> _v) { mParamList = _v; }

    virtual void SetName(CPString _name) { mName = _name; }
    virtual CPString GetName() const { return mName; }

  public:
    virtual std::vector<double> Compute(TemplateParamList<double>& _paramList) = 0;

  public: // ser-deser
    virtual void        Serialize(PP::Stream &a);
    virtual void        DeSerialize(PP::Stream &a);

  private:
    Interval                  mDefInterval;
    TemplateParamList<double> mParamList;
    CPString                  mName;
};

#endif
