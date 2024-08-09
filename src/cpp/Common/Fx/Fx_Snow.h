//---------------------------------------------------------------------------
#ifndef Fx_SnowH
#define Fx_SnowH
//---------------------------------------------------------------------------

#include "Fx_Weather.h"

class CFx_Snow : public CFx_Weather
{
  private:

  protected:
    int mGrad;

    virtual void GenerateParticle(int _index);

  public:
    CFx_Snow();

    virtual void Update();
    virtual void Paint();

    virtual ~CFx_Snow() {};
};

#endif
