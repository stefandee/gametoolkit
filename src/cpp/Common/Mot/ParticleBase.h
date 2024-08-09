// this is a physical object that has physical properties (position, 
// speed, accel, mass)

#ifndef PARTICLEBASEH
#define PARTICLEBASEH

#include <vector>
#include "basic3d.h"
//#include "ParticleAction.h"
#include "SDL.h"

class CParticleBase
{
  public: //c-d
    //CParticleBase() {};
    //CParticleBase(const CParticleBase&) {};
    //virtual ~CParticleBase() {};
    
  //public: //operators
    //CParticleGroup& operator=(const CParticleGroup&) = 0;
    
  public: // ops
    virtual void Update() = 0;
    virtual void Paint(SDL_Surface* _surface, TPos2D _origin) = 0;
      
  public: // get-set
    TPos3D GetPos() { return mPos; }
    float  GetX() { return mPos.x; }     
    float  GetY() { return mPos.y; }     
    float  GetZ() { return mPos.z; }
     
    void SetPos(const TPos3D& _v) { mPos = _v; }
    void SetX(float _v) { mPos.x = _v; } 
    void SetY(float _v) { mPos.y = _v; } 
    void SetZ(float _v) { mPos.z = _v; }
    
    CVector3D GetSpeed() { return mSpeed; }
    void      SetSpeed(const CVector3D& _v) { mSpeed = _v; }  
	      
    CVector3D GetAccel() { return mAccel; }
    void      SetAccel(const CVector3D& _v) { mAccel = _v; }  
    
    float GetMass() { return mMass; }
    void  SetMass(float _v) { mMass = _v; }
    
  protected:
    int random(int n);
    
  private:
    TPos3D    mPos;
    CVector3D mSpeed;
    CVector3D mAccel;
    float     mMass;	    
};

#endif

