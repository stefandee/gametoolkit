//---------------------------------------------------------------------------
#include <math.h>
#include <stdlib.h>
#pragma hdrstop

#include "CurvePolynome.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

std::vector<double> CurvePolynome::Compute(TemplateParamList<double>& _paramList)
{
  TemplateParamList<double>& lParamList = GetParamList();

  std::vector<double> lComputed;

  if (lParamList.GetName() == CPString("Explicit"))
  {
    double lN = lParamList.GetParam("N");
    double lY = 0.;
    double lX = _paramList.GetParam("X");
    double lPowX = 1;

    for(int i = 0; i <= (int)lN; i++)
    {
      //double lAi = lParamList.GetParam(CPString("A") + itoa(i, lStringBuffer, 10));
      double lAi = lParamList.GetParam(CPString("A") + CPString(i));

      lY += lPowX * lAi;

      lPowX = lPowX * lX;
    }

    lComputed.push_back(lX);
    lComputed.push_back(lY);
  }

  return lComputed;
}
//---------------------------------------------------------------------------

