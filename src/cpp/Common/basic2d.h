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
//-----------------------------------------------------------------------------
//  Unit              : Basic 2D Definitions and Functions
//
//  Versiune          : 1.0
//
//  Descriere         :
//    * contine tipuri si functii pentru lucrul cu date elementare 2d (puncte, box'uri, vextori 2d)
//
//  Istorie           :
//    [20.10.2001] - [Karg] - created this unit
//-----------------------------------------------------------------------------
//---------------------------------------------------------------------------

#ifndef basic2dH
#define basic2dH
//---------------------------------------------------------------------------

//types definitions, for future compatibility
//typedef TRect     CPRect;
//typedef TPoint    CPPoint;

typedef struct CPPoint
{
  int x, y;

  CPPoint() : x(0), y(0) {} // TMN: Initialize for debugging purposes
  CPPoint(int _x, int _y) : x(_x), y(_y) {}
  CPPoint(const CPPoint& p) { x = p.x; y = p.y; }

  double Distance(const CPPoint& _p) const;

} CPPoint;

// Structura Rectangle - Copy/Paste din Windoze
typedef struct CPRect
{
  int left, right, top, bottom;

  CPRect() : left(0), right(0), top(0), bottom(0) {}
  CPRect(const CPPoint& TL, const CPPoint& BR) { left=TL.x; top=TL.y; right=BR.x; bottom=BR.y; }
  CPRect(int l, int t, int r, int b) { left=l; top=t; right=r; bottom=b; }
  /*
  CPRect(RECT& r)
  {
    left    = r.left;
    top     = r.top;
    right   = r.right;
    bottom  = r.bottom;
  }
  */
  int Width () const { return right  - left; }
  int Height() const { return bottom - top ; }

  bool operator ==(const CPRect& rc) const
  {
    return left ==  rc.left  && top==rc.top &&
           right == rc.right && bottom==rc.bottom;
  }
  bool operator !=(const CPRect& rc) const
  {
    return !(rc==*this);
  }

  bool IsPointInside(const CPPoint& _point) const;
  bool IsPointInside(int x, int y) const;
  bool Intersects(const CPRect& _rect) const;
  bool Contains(const CPRect& _rect) const;
  /*
  __property long Left    = { read=left,   write=left   };
  __property long Top     = { read=top,    write=top };
  __property long Right   = { read=right,  write=right  };
  __property long Bottom  = { read=bottom, write=bottom };
  */

} CPRect;


#endif
