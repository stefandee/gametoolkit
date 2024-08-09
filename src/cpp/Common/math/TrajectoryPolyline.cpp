//---------------------------------------------------------------------------

#include <math.h>
#pragma hdrstop

#include "TrajectoryPolyline.h"
#include "MathConstants.h"

//---------------------------------------------------------------------------
// ----------------------------------------------------------------------------
// construct a PolylinePathway given the number of points (vertices),
// an array of points, and a path radius.


TrajectoryPolyline::TrajectoryPolyline (const int _pointCount,
                                  const CPPoint3D _points[],
                                  const float _radius,
                                  const bool _cyclic)
{
    initialize (_pointCount, _points, _radius, _cyclic);
}


// ----------------------------------------------------------------------------
// utility for constructors


void TrajectoryPolyline::initialize (const int _pointCount,
                                        const CPPoint3D _points[],
                                        const float _radius,
                                        const bool _cyclic)
{
    // set data members, allocate arrays
    radius = _radius;
    cyclic = _cyclic;
    pointCount = _pointCount;
    totalPathLength = 0;
    if (cyclic) pointCount++;
    lengths = new float    [pointCount];
    points  = new CPPoint3D [pointCount];
    normals = new CVector3D [pointCount];

    // loop over all points
    for (int i = 0; i < pointCount; i++)
    {
        // copy in point locations, closing cycle when appropriate
        const bool closeCycle = cyclic && (i == pointCount-1);
        const int j = closeCycle ? 0 : i;
        points[i] = _points[j];

        // for the end of each segment
        if (i > 0)
        {
            // compute the segment length
            normals[i] = CVector3D(points[i], points[i-1]);
            lengths[i] = normals[i].GetModul();

            // find the normalized vector parallel to the segment
            normals[i].Normalize();

            // keep running total of segment lengths
            totalPathLength += lengths[i];
        }
    }
}


// ----------------------------------------------------------------------------
// Given an arbitrary point ("A"), returns the nearest point ("P") on
// this path.  Also returns, via output arguments, the path tangent at
// P and a measure of how far A is outside the Pathway's "tube".  Note
// that a negative distance indicates A is inside the Trajectory.
CPPoint3D TrajectoryPolyline::mapPointToPath (const CPPoint3D& point,
                                            CVector3D& tangent,
                                            float& outside)
{
    float d;
    float minDistance = PLUS_INFINITY_FLOAT;
    CPPoint3D onPath;

    // loop over all segments, find the one nearest to the given point
    for (int i = 1; i < pointCount; i++)
    {
        segmentLength = lengths[i];
        segmentNormal = normals[i];
        d = pointToSegmentDistance (point, points[i-1], points[i]);
        if (d < minDistance)
        {
            minDistance = d;
            onPath = chosen;
            tangent = segmentNormal;
        }
    }

    // measure how far original point is outside the Trajectory's "tube"
    outside = onPath.Distance(point) - radius;

    // return point on path
    return onPath;
}


// ----------------------------------------------------------------------------
// given an arbitrary point, convert it to a distance along the path


float TrajectoryPolyline::mapPointToPathDistance (const CPPoint3D& point)
{
    float d;
    float minDistance = PLUS_INFINITY_FLOAT;
    float segmentLengthTotal = 0;
    float pathDistance = 0;

    for (int i = 1; i < pointCount; i++)
    {
        segmentLength = lengths[i];
        segmentNormal = normals[i];
        d = pointToSegmentDistance (point, points[i-1], points[i]);
        if (d < minDistance)
        {
            minDistance = d;
            pathDistance = segmentLengthTotal + segmentProjection;
        }
        segmentLengthTotal += segmentLength;
    }

    // return distance along path of onPath point
    return pathDistance;
}


// ----------------------------------------------------------------------------
// given a distance along the path, convert it to a point on the path


CPPoint3D TrajectoryPolyline::mapPathDistanceToPoint (float pathDistance)
{
    // clip or wrap given path distance according to cyclic flag
    float remaining = pathDistance;
    if (cyclic)
    {
        remaining = (float) fmod (pathDistance, totalPathLength);
    }
    else
    {
        if (pathDistance < 0) return points[0];
        if (pathDistance >= totalPathLength) return points [pointCount-1];
    }

    // step through segments, subtracting off segment lengths until
    // locating the segment that contains the original pathDistance.
    // Interpolate along that segment to find 3d point value to return.
    CPPoint3D result;
    
    for (int i = 1; i < pointCount; i++)
    {
        segmentLength = lengths[i];
        if (segmentLength < remaining)
        {
            remaining -= segmentLength;
        }
        else
        {
            float ratio = remaining / segmentLength;

            // TODO: insert interpolate
            result = points[i-1].Interpolate(ratio, points[i]);
            break;
        }
    }
    return result;
}


// ----------------------------------------------------------------------------
// computes distance from a point to a line segment
//
// (I considered moving this to the vector library, but its too
// tangled up with the internal state of the TrajectoryPolyline instance)


float TrajectoryPolyline::pointToSegmentDistance (const CPPoint3D& point,
                                                    const CPPoint3D& ep0,
                                                    const CPPoint3D& ep1)
{
    // convert the test point to be "local" to ep0
    local                = CVector3D(ep0, point);
    CVector3D segmentDir = CVector3D(ep0, ep1);
    segmentDir.Normalize();

    // find the projection of "local" onto "segmentNormal"
    segmentProjection = segmentDir.Dot (local);

    // handle boundary cases: when projection is not on segment, the
    // nearest point is one of the endpoints of the segment
    if (segmentProjection < 0)
    {
        chosen = ep0;
        segmentProjection = 0;
        return point.Distance(ep0);
    }
    if (segmentProjection > segmentLength)
    {
        chosen = ep1;
        segmentProjection = segmentLength;
        return point.Distance(ep1);
    }

    // otherwise nearest point is projection point on segment
    CVector3D scaled = segmentDir * segmentProjection;

    chosen   = ep0;
    chosen.x += scaled.GetX();
    chosen.y += scaled.GetY();
    chosen.z += scaled.GetZ();

    return point.Distance(chosen);
}

#pragma package(smart_init)
