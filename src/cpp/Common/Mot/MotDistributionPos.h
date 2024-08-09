#ifndef MOTDISTRIBUTIONPOSH
#define MOTDISTRIBUTIONPOSH

#include "basic3d.h"

class CMotDistributionPos
{
  public:
    CMotDistributionPos() {}
    virtual ~CMotDistributionPos() {}
    
  public:
    virtual TPos3D GetPos() = 0;  
};

class CMotDistributionEllipse : public CMotDistributionPos
{
  public:
    CMotDistributionEllipse(double _radiusX, double _radiusY, TPos3D _center) : mRadiusX(_radiusX), mRadiusY(_radiusY), mCenter(_center) {}
    virtual ~CMotDistributionEllipse() {}

  public:
    virtual TPos3D GetPos();
    
  private:
    double mRadiusX, mRadiusY;
    TPos3D mCenter;
};

class CMotDistributionSegment : public CMotDistributionPos
{
  public:
    CMotDistributionSegment(TPos3D _start, TPos3D _end) : mStart(_start), mEnd(_end) {}
    virtual ~CMotDistributionSegment() {}
    
  public:
    virtual TPos3D GetPos();  
    
  private:
    TPos3D mStart, mEnd;  

};

class CMotDistributionDisc : public CMotDistributionPos
{
  public:
    CMotDistributionDisc(double _radiusX, double _radiusY, TPos3D _center) : mRadiusX(_radiusX), mRadiusY(_radiusY), mCenter(_center) {}
    virtual ~CMotDistributionDisc() {}

  public:
    virtual TPos3D GetPos();
    
  private:
    double mRadiusX, mRadiusY;
    TPos3D mCenter;
};


#endif // MOTDISTRIBUTIONPOSH
