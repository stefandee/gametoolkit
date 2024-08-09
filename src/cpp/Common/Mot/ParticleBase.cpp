#include "ParticleBase.h"

int CParticleBase::random(int n)
{
  return (int)(((float)(n-1) * rand()) / (float)RAND_MAX);
}
  

