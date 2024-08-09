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
/*---------------------------------------------------------------------------
 application: Pure Power

 description: 3D & angles utilities
 last modify: 12 06 00
        by Karg & grabX
---------------------------------------------------------------------------*/

//---------------------------------------------------------------------------
#include <math.h>
#pragma hdrstop

#include "basic3d.h"

//---------------------------------------------------------------------------
#ifdef __BORLANDC__
#pragma package(smart_init)
#endif

// Descriere   :  calculeaza distanta intre 2 puncte in spatiu
// Param       :  p1, p2 - punctele
// Rezultat    :  distanta
// Comentarii  :  none
float Dist3D(TPos3D _p1, TPos3D _p2)
{
  return (float)sqrt(pow((_p1.x - _p2.x), 2) + pow((_p1.y - _p2.y), 2) + pow((_p1.z - _p2.z), 2));
}

// Descriere   :  unghi la centru
// Param       :  x, y - coordonatele punctului
// Rezultat    :  unghiul
// Comentarii  :  none
int CenterAngle(float _x, float _y)
{
  // cazurile banale
  if ((_x == 0) && (_y == 0)) return 0;

  if (_y == 0)
  {
    if (_x < 0) return 180;
    else return 0;
  }

  if (_x == 0)
  {
    if (_y < 0) return 270;
    else return 90;
  }

  return ((int)(((atan2(_y, _x) + PI) * 180.0) / PI) + 180) % 360;
}
//---------------------------------------------------------------------------

// Descriere   :  unghi la centru - in radiani
// Param       :  x, y - coordonatele punctului
// Rezultat    :  unghiul in radiani
// Comentarii  :  none
float CenterRadianAngle(float x, float y)
{
   /*
   float tmp = 0;

   if (x == 0)
   {
      if (y == 0)
         return 0;
      else
      if (y > 0)
         return PI/2;
      else
         return 3*PI/2;
   }
   else
      tmp = atan2(y,x);

   //if (tmp < 0)
      tmp = tmp + 2*PI;

   if (tmp >= 2*PI)
      tmp = 2*PI - tmp;

   return tmp;
   */
   return (float)(CenterAngle(x, y) * PI / 180.0);
}
//---------------------------------------------------------------------------

CVector3D::CVector3D(float _cx, float _cy, float _cz)
{
  mCx = _cx;
  mCy = _cy;
  mCz = _cz;
}
//---------------------------------------------------------------------------

CVector3D::CVector3D(const CPPoint3D& _start, const CPPoint3D& _end)
{
  mCx = _end.x - _start.x;
  mCy = _end.y - _start.y;
  mCz = _end.z - _start.z;
}
//---------------------------------------------------------------------------

void CVector3D::Normalize()
{
  mModul = GetModul();
  mCx = mCx / mModul;
  mCy = mCy / mModul;
  mCz = mCz / mModul;
}
//---------------------------------------------------------------------------

float CVector3D::GetX() const
{
  return mCx;
}
//---------------------------------------------------------------------------

float CVector3D::GetY() const
{
  return mCy;
}
//---------------------------------------------------------------------------

float CVector3D::GetZ() const
{
  return mCz;
}
//---------------------------------------------------------------------------

float CVector3D::GetModul() const
{
  //logWrite("("); logWriteI(mCx * 1000); logWrite(","); logWriteI(mCy * 1000); logWrite(","); logWriteI(mCz * 1000); logWriteLn(")");
  return (float)sqrt(mCx * mCx + mCy * mCy + mCz * mCz);
}
//---------------------------------------------------------------------------

float CVector3D::Cos(const CVector3D& _vector) const
{
  float lModul1, lModul2;

  lModul1 = GetModul();
  lModul2 = _vector.GetModul();

  if ((lModul1 == 0.0) ||
     (lModul2 == 0.0))
  {
    return -1.0;
  }

  return Dot(_vector) / (lModul1 * lModul2);
}
//---------------------------------------------------------------------------

float CVector3D::Dot(const CVector3D& _vector) const
{
  return mCx * _vector.mCx + mCy * _vector.mCy + mCz * _vector.mCz;
}
//---------------------------------------------------------------------------

CVector3D CVector3D::Cross(const CVector3D& _v) const
{
  return CVector3D ((mCy * _v.mCz) - (mCz * _v.mCy),
                (mCz * _v.mCx) - (mCx * _v.mCz),
                (mCx * _v.mCy) - (mCy * _v.mCx));
}
//---------------------------------------------------------------------------

void CVector3D::Sub(const CVector3D& _vector)
{
    mCx -= _vector.mCx;
    mCy -= _vector.mCy;
    mCz -= _vector.mCz;
}
//---------------------------------------------------------------------------

void CVector3D::ScalarMultiply(float v)
{
    mCx *= v;
    mCy *= v;
    mCz *= v;
}
//---------------------------------------------------------------------------

CVector3D::~CVector3D()
{
}
//---------------------------------------------------------------------------

float CPPoint3D::Distance(const CPPoint3D& _p) const
{
  return sqrt(pow((x - _p.x), 2) + pow((y - _p.y), 2) + pow((z - _p.z), 2));
}
//---------------------------------------------------------------------------

CPPoint3D CPPoint3D::Interpolate(float ratio, const CPPoint3D& _p)
{
  return CPPoint3D(x + ratio * (_p.x - x), y + ratio * (_p.y - y), z + ratio * (_p.z - z));
}
//---------------------------------------------------------------------------

