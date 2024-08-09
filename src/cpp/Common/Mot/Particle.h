#ifndef PARTICLEH
#define PARTICLEH

#include "ParticleBase.h"
#include <stdlib.h>

class CParticle : public CParticleBase
{
  public:
    CParticle() { mAge = 0.0; mMaxAge = 1.0; mId = 0;/*drand48() * 32;*/ }
    CParticle(double _maxAge);
    ~CParticle();
    
  public:
    virtual void Update();
    virtual void Paint(SDL_Surface* _surface, TPos2D _origin);
    
  public: // get-set
    int       GetColor() { return mColor; }
    void      SetColor(int _v) { mColor = _v; }
    
    bool IsDead() { return mAge > mMaxAge; }
    
    int Id() { return mId; }
            
  protected:
  
  private:
    int    mColor;
    double mAge, mMaxAge;        
    int    mId;
    
};

#endif
