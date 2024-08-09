#include "MotDistributionVector.h"
#include <stdlib.h>
#include <math.h>

CVector3D CMotDistributionVectorCone::GetValue()
{
  double lMod = mMin + drand48() * (mMax - mMin);
  double lAngle = (-mAngle / 2 + mAngle * drand48()) * (PI / 180);
  
  CVector3D lDir;
  
  lDir.SetX(mAxis.GetX() * cos(lAngle) - mAxis.GetY() * sin(lAngle)); 
  lDir.SetY(mAxis.GetY() * cos(lAngle) + mAxis.GetX() * sin(lAngle));

  lDir.Normalize();
  
  return lDir * lMod;	    
}

CVector3D CMotDistributionVectorConstant::GetValue()
{
  double lMod = mMin + drand48() * (mMax - mMin);
  
  return mDir * lMod;
}

       
