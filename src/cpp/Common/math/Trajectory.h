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

// ----------------------------------------------------------------------------
// OpenSteer -- Steering Behaviors for Autonomous Characters
//
// Copyright (c) 2002-2003, Sony Computer Entertainment America
// Original author: Craig Reynolds <craig_reynolds@playstation.sony.com>
//
// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
// ----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//  Unit              : Trajectory
//
//  Version           : 0.8.2
//
//  Description       :
//    * a pure virtual base class for an abstract pathway in space, as for
//      example would be used in path following.
//
//  History           :
//    [07.12.2004] - [Karg] - ripped from OpenSteer, class renamed and integrated
//    [08.12.2004] - [Karg] - replaced types that were vec3 type with CPPoint where
//                            they stood for points
//    [08.12.2004] - [Karg] - replaced vec3 with CVector3D where types required a vector
//-----------------------------------------------------------------------------

#ifndef TRAJECTORYH
#define TRAJECTORYH

#include "basic3d.h"

class Trajectory
{
public:
    // Given an arbitrary point ("A"), returns the nearest point ("P") on
    // this path.  Also returns, via output arguments, the path tangent at
    // P and a measure of how far A is outside the Pathway's "tube".  Note
    // that a negative distance indicates A is inside the Pathway.
    virtual CPPoint3D mapPointToPath (const CPPoint3D& point,
                                 CVector3D& tangent,
                                 float& outside) = 0;

    // given a distance along the path, convert it to a point on the path
    virtual CPPoint3D mapPathDistanceToPoint (float pathDistance) = 0;

    // Given an arbitrary point, convert it to a distance along the path.
    virtual float mapPointToPathDistance (const CPPoint3D& point) = 0;

    // is the given point inside the path tube?
    bool isInsidePath (const CPPoint3D& point)
    {
        float outside;
        CVector3D tangent;

        mapPointToPath (point, tangent, outside);
        return outside < 0;
    }

    // how far outside path tube is the given point?  (negative is inside)
    float howFarOutsidePath (const CPPoint3D& point)
    {
        float outside;
        CVector3D tangent;

        mapPointToPath (point, tangent, outside);
        return outside;
    }
};

// ----------------------------------------------------------------------------
#endif // TRAJECTORYH
