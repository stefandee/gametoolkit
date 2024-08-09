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
//---------------------------------------------------------------------------
#include <math.h>
#pragma hdrstop

#include "basic2d.h"
//---------------------------------------------------------------------------

double CPPoint::Distance(const CPPoint& _p) const
{
  return sqrt(pow((double)(x - _p.x), 2) + pow((double)(y - _p.y), 2));
}
//---------------------------------------------------------------------------

bool CPRect::IsPointInside(const CPPoint& _point) const
{
  return (_point.x <= right) && (_point.x >= left) && (_point.y >= top) && (_point.y <= bottom);
}
//---------------------------------------------------------------------------

bool CPRect::IsPointInside(int x, int y) const
{
  return (x <= right) && (x >= left) && (y >= top) && (y <= bottom);
}
//---------------------------------------------------------------------------

bool CPRect::Intersects(const CPRect& _rect) const
{
    if ((right < _rect.left) || (bottom < _rect.top) || (_rect.bottom < top) || (_rect.right < left))
    {
      return false;
    }

    return true;
}
//---------------------------------------------------------------------------

bool CPRect::Contains(const CPRect& _rect) const
{
  return (IsPointInside(_rect.left,  _rect.top) && IsPointInside(_rect.right, _rect.bottom));
}
//---------------------------------------------------------------------------

#ifdef __BORLANDC__
#pragma package(smart_init)
#endif
