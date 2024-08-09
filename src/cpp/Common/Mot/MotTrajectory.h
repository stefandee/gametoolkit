#ifndef MOTTRAJECTORY
#define MOTTRAJECTORY

#include "basic3d.h"

class CMotTrajectory
{
  public:
    CMotTrajectory() { mTime = 0.0; }
    virtual ~CMotTrajectory() {}
    
  public:
    virtual TPos3D GetPos() = 0;
    virtual void Update() { mTime += 0.05; }
    
  public: // get -set  
    double GetTime() { return mTime; }
    void SetTime(double _v) { mTime = _v; }
    
  private:
    double mTime;	    
};

class CMotTrajectoryCircle : public CMotTrajectory
{
  public:
    CMotTrajectoryCircle(double _r) { mRadius = _r; }
  
    virtual TPos3D GetPos();  
    
  private:
    double mRadius;  
};

class CMotTrajectorySin : public CMotTrajectory
{
  public:
    CMotTrajectorySin(double _r) { mRadius = _r; }
  
    virtual TPos3D GetPos();  
    
  private:
    double mRadius;  
};

class CMotTrajectoryCos : public CMotTrajectory
{
  public:
    CMotTrajectoryCos(double _r) { mRadius = _r; }
  
    virtual TPos3D GetPos();  
    
  private:
    double mRadius;  
};

class CMotTrajectoryAstroid : public CMotTrajectory
{
  public:
    CMotTrajectoryAstroid(double _r) { mRadius = _r; }
  
    virtual TPos3D GetPos();  
    
  private:
    double mRadius;  
};

#endif
