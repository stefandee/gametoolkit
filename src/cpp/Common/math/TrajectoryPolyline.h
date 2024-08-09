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
//  Unit              : TrajectoryPolyline
//
//  Version           : 0.8.2
//
//  Description       :
//    * a simple implementation of the Trajectory protocol.  The path
//      is a "polyline" a series of line segments between specified points.  A
//      radius defines a volume for the path which is the union of a sphere at each
//      point and a cylinder along each segment.
//
//  History           :
//    [07.12.2004] - [Karg] - unit integrated, class renamed from PolylinePathway
//-----------------------------------------------------------------------------

#ifndef TrajectoryPolylineH
#define TrajectoryPolylineH
//---------------------------------------------------------------------------

#include "Trajectory.h"

class TrajectoryPolyline: public virtual Trajectory
{
public:

    int pointCount;
    CPPoint3D* points;
    float radius;
    bool cyclic;

    TrajectoryPolyline (void) {}

    // construct a TrajectoryPolyline given the number of points (vertices),
    // an array of points, and a path radius.
    TrajectoryPolyline (const int _pointCount,
                     const CPPoint3D _points[],
                     const float _radius,
                     const bool _cyclic);

    // utility for constructors in derived classes
    void initialize (const int _pointCount,
                     const CPPoint3D _points[],
                     const float _radius,
                     const bool _cyclic);

    // Given an arbitrary point ("A"), returns the nearest point ("P") on
    // this path.  Also returns, via output arguments, the path tangent at
    // P and a measure of how far A is outside the Trajectory's "tube".  Note
    // that a negative distance indicates A is inside the Trajectory.
    CPPoint3D mapPointToPath (const CPPoint3D& point, CVector3D& tangent, float& outside);


    // given an arbitrary point, convert it to a distance along the path
    float mapPointToPathDistance (const CPPoint3D& point);

    // given a distance along the path, convert it to a point on the path
    CPPoint3D mapPathDistanceToPoint (float pathDistance);

    // utility methods

    // compute minimum distance from a point to a line segment
    float pointToSegmentDistance (const CPPoint3D& point,
                                  const CPPoint3D& ep0,
                                  const CPPoint3D& ep1);

    // accessor for total path length;
    float getTotalPathLength (void) {return totalPathLength;};

// XXX removed the "private" because it interfered with derived
// XXX classes later this should all be rewritten and cleaned up
// private:

    // xxx shouldn't these 5 just be local variables?
    // xxx or are they used to pass secret messages between calls?
    // xxx seems like a bad design
    float segmentLength;
    float segmentProjection;
    CVector3D local;
    CPPoint3D chosen;
    CVector3D segmentNormal;

    float* lengths;
    CVector3D* normals;
    float totalPathLength;
};

#endif
