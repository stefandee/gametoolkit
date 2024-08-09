//---------------------------------------------------------------------------

#ifndef EcoSysGenRandomH
#define EcoSysGenRandomH
//---------------------------------------------------------------------------

#include "EcoSysGen.h"

class EcoSysGenRandom : public EcoSysGen
{
  public:
    EcoSysGenRandom() : mRepeatOnCollision(false), mSteps(50) {}
    ~EcoSysGenRandom() {}

  public:
    std::vector<EcoSysItemOut> Generate();

  public:
    void SetSteps(int _v) { mSteps = _v; }
    int  GetSteps() { return mSteps; }

    void SetRepeat(bool _v) { mRepeatOnCollision = _v; }
    bool GetRepeat() { return mRepeatOnCollision; }

  private:
    bool mRepeatOnCollision;
    int  mSteps;
};

#endif
