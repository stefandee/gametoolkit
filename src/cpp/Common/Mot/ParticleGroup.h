#ifndef PARTICLEGROUPH
#define PARTICLEGROUPH

#include "ParticleBase.h"

class CParticleGroup : public CParticleBase
{
  public:
    virtual void Update();
    virtual void Paint(SDL_Surface* _surface, TPos2D _origin);
    
    void Add(CParticleBase* _e) { mElements.push_back(_e); }
    
  private:
    std::vector<CParticleBase*> mElements;  
};

#endif
