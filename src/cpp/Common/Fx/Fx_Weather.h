//---------------------------------------------------------------------------
#ifndef Fx_WeatherH
#define Fx_WeatherH
//---------------------------------------------------------------------------

#include "Fx.h"

struct TFxWeatherParticle
{
  int mX, mY, mEndY;
  int mSpeedX, mSpeedY;
  int mParams[2];
};

class CFx_Weather : public CFx
{
  private:

  protected:
    TFxWeatherParticle* mParticles;
    int                 mParticlesCount;
    int                 mWindSpeed;

    virtual void GenerateParticle(int _index);

  public:
    CFx_Weather();

    virtual void Update();

    virtual ~CFx_Weather();

    virtual void SetParticleCount(int _v);
    virtual int  GetParticleCount() { return mParticlesCount; };

    virtual void SetWidth(int _v);
    virtual void SetHeight(int _v);

    virtual void SetWindSpeed(int _v) { mWindSpeed = _v; };
    virtual int  GetWindSpeed() { return mWindSpeed; };
};

#endif
