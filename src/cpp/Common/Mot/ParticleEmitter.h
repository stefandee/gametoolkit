#ifndef PARTICLEEMMITERH
#define PARTICLEEMMITERH

#include "ParticleBase.h"
#include "ParticleDistribution.h"
#include "ParticleBase.h"
#include "Particle.h"
#include "ParticleAction.h"
#include "MotDistributionPos.h"
#include "MotDistributionVector.h"

class CParticleEmitter : public CParticleBase
{
  public: //c-d
    CParticleEmitter(int _count, CParticleDistribution* _lifed, CDistributionColor* _colord, CMotDistributionPos* _posd, CMotDistributionVector* _speedd);
    //CParticleGroup(const CParticleGroup&);
    virtual ~CParticleEmitter();
    
  //public: //operators
    //CParticleGroup& operator=(const CParticleGroup&);
    
  public: // ops
    virtual void Update();
    virtual void Paint(SDL_Surface* _surface, TPos2D _origin);
    
    void AddAction(CMotAction* _action) { _action->SetParticles(&mParticles); mActionsList.push_back(_action); }    
  
  public: // get-set  
    
  protected:
    CParticleEmitter();
    CParticleEmitter(const CParticleEmitter&);
    CParticleEmitter& operator = (const CParticleEmitter&);
  
  private:
    std::vector<CParticle> mParticles;
    
    void GenerateParticle(int i);
    
    CDistributionColor*    mColorDistribution;
    CMotDistributionPos*   mPosDistribution;
    CParticleDistribution* mLifeDistribution;
    CMotDistributionVector* mSpeedDistribution;
    
    std::vector<CMotAction*> mActionsList;    
};

#endif
