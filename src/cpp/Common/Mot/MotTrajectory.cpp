#include <math.h>
#include "MotTrajectory.h"

TPos3D CMotTrajectoryCircle::GetPos()
{
  TPos3D lP;
  
  lP.x = mRadius * sin(GetTime());
  lP.y = mRadius * cos(GetTime());
  lP.z = 0;
  
  return lP;
}

TPos3D CMotTrajectorySin::GetPos()
{
  TPos3D lP = {mRadius * sin(GetTime()), 0., 0.};
  
  return lP;
}

TPos3D CMotTrajectoryCos::GetPos()
{
  TPos3D lP;
  
  lP.x = mRadius * cos(GetTime());
  lP.y = 0.;
  lP.z = 0.;
  
  return lP;
}

TPos3D CMotTrajectoryAstroid::GetPos()
{
  TPos3D lP = { mRadius * sin(GetTime()) * sin(GetTime()) * sin(GetTime()),
                mRadius * cos(GetTime()) * cos(GetTime()) * cos(GetTime()),
		0. };
  
  return lP;
}
