//---------------------------------------------------------------------------

#ifndef CurveCompositeH
#define CurveCompositeH
//---------------------------------------------------------------------------

#include "Curve2D.h"

class CurveComposite : public Curve2D
{
  public:
    CurveComposite() {}
    //CurveComposite(Interval _defInterval) : Curve2D(_defInterval) {}
    // should implement copy ctor and assignment op

    virtual Curve2D* Clone() { return new CurveComposite(*this); }

  public:
    void AddCurve(Curve2D* _curve) { if (_curve) mCurves.push_back(_curve); }  

  public:
    virtual std::vector<double> Compute(TemplateParamList<double>& _paramList);

  protected:

  private:
    std::vector<Curve2D*> mCurves;
};

#endif
