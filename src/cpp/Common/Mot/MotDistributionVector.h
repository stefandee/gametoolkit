#ifndef MOTDISTRIBUTIONVECTOR
#define MOTDISTRIBUTIONVECTOR

#include "basic3d.h"

class CMotDistributionVector
{
  public:
    CMotDistributionVector() {}
    virtual ~CMotDistributionVector() {}
    
  public:
    virtual CVector3D GetValue() = 0;    
};

class CMotDistributionVectorCone : public CMotDistributionVector
{
  public:
    CMotDistributionVectorCone(CVector3D _axis, double _angle, double _min, double _max) : mAxis(_axis), mAngle(_angle), mMin(_min), mMax(_max) {}
  
    virtual CVector3D GetValue();   
    
  private:
    CVector3D mAxis;
    double    mAngle, mMin, mMax;   

};

class CMotDistributionVectorConstant : public CMotDistributionVector
{
  public:
    CMotDistributionVectorConstant(CVector3D _dir, double _min, double _max) : mDir(_dir), mMin(_min), mMax(_max) { mDir.Normalize(); }
    
    virtual CVector3D GetValue();   
    
  private:
    CVector3D mDir;
    double    mMin, mMax;       
};  

#endif // MOTDISTRIBUTIONVECTOR
