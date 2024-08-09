#include <math.h>
#include <stdlib.h>

#include "MotDistributionPos.h"

TPos3D CMotDistributionEllipse::GetPos()
{
  double lAngle = 2 * PI * drand48();

  TPos3D lPos = {mRadiusX * cos(lAngle), mRadiusY * sin(lAngle), 0.};
  
  return lPos;
}

TPos3D CMotDistributionSegment::GetPos()
{
  double lDelta = drand48();

  TPos3D lPos = {mStart.x + (mEnd.x - mStart.x) * lDelta,
                 mStart.y + (mEnd.y - mStart.y) * lDelta,
		 mStart.z + (mEnd.z - mStart.z) * lDelta
		};
		 
  return lPos;		 
}

TPos3D CMotDistributionDisc::GetPos()
{
  double lAngle = 2 * PI * drand48();
  double lRadiusX = mRadiusX * drand48();
  double lRadiusY = mRadiusY * drand48();

  TPos3D lPos = {lRadiusX * cos(lAngle), lRadiusY * sin(lAngle), 0.};
  
  return lPos;
}

