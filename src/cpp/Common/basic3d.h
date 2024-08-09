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
#ifndef basic3dH
#define basic3dH
//---------------------------------------------------------------------------

#define PI 3.14159265358979

#define SIGN(X) (X<0?-1:1)

typedef struct
{
  float x, y;
} TPos2D;

typedef struct
{
  float x, y, z;
} TPos3D;

// point in 3D
class CPPoint3D
{
  public:
    float x, y, z;

    //CPPoint() {}
    CPPoint3D() : x(0.), y(0.), z(0.) {} // TMN: Initialize for debugging purposes
    CPPoint3D(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}
    CPPoint3D(const CPPoint3D& p) { x = p.x; y = p.y; z = p.z; }

    float Distance(const CPPoint3D& _p) const;
    CPPoint3D Interpolate(float ratio, const CPPoint3D& _p);
};

// clasa care implementeaza un vector 3D
class CVector3D
{
  private:
    float mCx, mCy, mCz, mModul;

  protected:

  public:
    CVector3D(float _cx = 1.0, float _cy = 1.0, float cz = 1.0);
    CVector3D(const CPPoint3D& _start, const CPPoint3D& _end);
    ~CVector3D();

    void  Normalize();
    float GetModul() const;
    float GetX() const;
    float GetY() const;
    float GetZ() const;

    CVector3D Cross(const CVector3D& _v) const;
    float Dot(const CVector3D& _vector) const;
    float Cos(const CVector3D& _vector) const;

    void Sub(const CVector3D& _vector);
    void ScalarMultiply(float v);

    CVector3D operator* (const float _v) const { return CVector3D (_v * mCx, _v * mCy, _v * mCz); }
};

float  Dist3D(TPos3D _p1, TPos3D _p2);
int    CenterAngle(float _x, float _y);
float  CenterRadianAngle(float x, float y);

template<class T> inline T interpolate (float alpha, const T& x0, const T& x1)
{
    return x0 + ((x1 - x0) * alpha);
}

#endif
