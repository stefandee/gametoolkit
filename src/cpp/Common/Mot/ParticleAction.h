#ifndef MOTACTIONH
#define MOTACTIONH

#include <vector>

#include "Particle.h"

class CMotAction
{
  public:
  
  public:
    virtual void Update() = 0;
    
    void SetParticles(std::vector<CParticle>* _v) { mParticles = _v; }
    std::vector<CParticle>* GetParticles() { return mParticles; }
    
  private:
    std::vector<CParticle>* mParticles;  
};

class CMotActionMatchColor : public CMotAction
{
  public:
    CMotActionMatchColor(int _targetR, int _targetG, int _targetB, int _scale);
    
    void Update();
  
  private:
    int mTargetR, mTargetB, mTargetG, mScale;    	    
};

// not fully implemented
class CMotActionSpeedLimit : public CMotAction
{
  public:
    CMotActionSpeedLimit(double _min, double _max);
    
    void Update();
  
  private:
    int mMin, mMax;    	    
};

class CMotActionFollow : public CMotAction
{
  public:
    CMotActionFollow(double _scale, double _radius);
    
    void Update();
  
  private:
    double mRadius, mScale;    	    
};

class CMotActionOrbitPoint : public CMotAction
{
  public:
    CMotActionOrbitPoint(double _scale, double _radius, TPos3D _center) : mScale(_scale), mCenter(_center), mRadius(_radius) {}
    
    void Update();
  
  private:
    double mScale, mRadius;
    TPos3D mCenter;    	    
};

class CMotActionAccelerate : public CMotAction
{
  public:
    CMotActionAccelerate(CVector3D _a) : mAccel(_a) {}
    
    void Update();
    
  private:
    CVector3D mAccel;  
};

#endif
