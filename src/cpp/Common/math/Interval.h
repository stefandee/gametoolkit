//---------------------------------------------------------------------------

#ifndef IntervalH
#define IntervalH
//---------------------------------------------------------------------------

#include "MathConstants.h"
#include <algorithm>
#include "Stream.h"

class Interval
{
  public:
    Interval() : mMin(MINUS_INFINITY_DOUBLE), mMax (PLUS_INFINITY_DOUBLE) {}
    Interval(double _min, double _max) : mMin(_min), mMax(_max) {}

  public:
    void SetInterval(double _min, double _max) { mMin = _min; mMax = _max; Verify(); }

    void SetMax(double _v) { mMax = _v; Verify(); }
    void SetMin(double _v) { mMin = _v; Verify(); }

    double GetMax() { return mMax; }
    double GetMin() { return mMin; }

    bool IsIn(double _v) { return (mMin <= _v) && (_v <= mMax); }

  public: // ser-deser  
    virtual void Serialize(PP::Stream &a);
    virtual void DeSerialize(PP::Stream &a);
    
  protected:
    void Verify() { double temp = mMin; mMin = std::min(mMin, mMax); mMax = std::max(mMax, temp); }

  private:
    double mMin, mMax;
};

#endif
