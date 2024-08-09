//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "Fx_Weather.h"
//#include "logfile.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

CFx_Weather::CFx_Weather()
{
  mParticles      = NULL;
  mParticlesCount = 0;
  mWindSpeed      = 0;
}
//---------------------------------------------------------------------------

void CFx_Weather::Update()
{
  for(int i = 0; i < mParticlesCount; i++)
  {
    mParticles[i].mX += mParticles[i].mSpeedX;
    mParticles[i].mY += mParticles[i].mSpeedY;

    if (mParticles[i].mY > mParticles[i].mEndY)
    {
      GenerateParticle(i);
    }
  }
}
//---------------------------------------------------------------------------

void CFx_Weather::SetParticleCount(int _v)
{
  if (_v < 0)
  {
    return;
  }

  mParticlesCount = _v;

  if (mParticles)
  {
    delete [] mParticles;
  }

  try
  {
    mParticles = new TFxWeatherParticle[mParticlesCount];
  }
  catch(...)
  {
    mParticlesCount = 0;
    mParticles     = NULL;
    return;
  }

  for(int i = 0; i < mParticlesCount; i++)
  {
    GenerateParticle(i);
  }
}
//---------------------------------------------------------------------------

void CFx_Weather::GenerateParticle(int _index)
{
  // do nothing
}
//---------------------------------------------------------------------------

void CFx_Weather::SetWidth(int _v)
{
  CFx::SetWidth(_v);

  for(int i = 0; i < mParticlesCount; i++)
  {
    GenerateParticle(i);
  }
}
//---------------------------------------------------------------------------

void CFx_Weather::SetHeight(int _v)
{
  CFx::SetHeight(_v);

  for(int i = 0; i < mParticlesCount; i++)
  {
    GenerateParticle(i);
  }
}
//---------------------------------------------------------------------------

CFx_Weather::~CFx_Weather()
{
  delete [] mParticles;
}
//---------------------------------------------------------------------------


