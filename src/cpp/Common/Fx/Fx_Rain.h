//---------------------------------------------------------------------------
#ifndef Fx_RainH
#define Fx_RainH
//---------------------------------------------------------------------------
#include "Fx_Weather.h"

class CFx_Rain : public CFx_Weather
{
  private:

  protected:
    virtual void GenerateParticle(int _index);

  public:
    CFx_Rain();

    virtual void Update();
    virtual void Paint();

    virtual ~CFx_Rain() {};
};

#endif
