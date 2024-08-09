//---------------------------------------------------------------------------

#pragma hdrstop

#include "CurveLinear.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

std::vector<double> CurveLinear::Compute(TemplateParamList<double>& _paramList)
{
  TemplateParamList<double>& lParamList = GetParamList();

  std::vector<double> lComputed;

  if (lParamList.GetName() == CPString("Implicit"))
  {
    double lA = lParamList.GetParam("A");
    double lB = lParamList.GetParam("B");
    double lC = lParamList.GetParam("C");

    double lX = _paramList.GetParam("X");

    if (lB != 0 && GetDefInterval().IsIn(lX))
    {
      lComputed.push_back(lX);

      double lY = -lC - lA * lX / lB;

      lComputed.push_back(lY);
    }
  }


  /*
  if (mA != 0)
  {
    lYVector.push_back(-mC - mB * 0 / mA);
  }
  */

  return lComputed;
}
//---------------------------------------------------------------------------

