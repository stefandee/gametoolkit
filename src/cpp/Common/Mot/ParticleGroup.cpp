#include "ParticleGroup.h"
#include <iostream>
using namespace std;

void CParticleGroup::Update()
{
  for(int i = 0; i < mElements.size(); i++)
  {
    //cout << i << endl;
  
    mElements[i]->Update();
  }
}

void CParticleGroup::Paint(SDL_Surface* _surface, TPos2D _origin)
{
  TPos2D lP;
  
  lP.x = GetX() + _origin.x;
  lP.y = GetY() + _origin.y;

  for(int i = 0; i < mElements.size(); i++)
  {
    mElements[i]->Paint(_surface, lP);
  }
}

