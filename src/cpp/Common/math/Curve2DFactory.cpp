#pragma hdrstop
//---------------------------------------------------------------------------

#include "CurveLinear.h"
#include "CurveElipsoid.h"
#include "CurveScript.h"
#include "CurvePolynome.h"
#include "Curve2DFactory.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)

Curve2D* Curve2DFactory::Get(CPString _className)
{
  TemplateParamList<double> lParamList;

  if (_className == CPString("Curve2DLinear"))
  {
    return new CurveLinear(lParamList);
  }

  if (_className == CPString("Curve2DElipsoid"))
  {
    return new CurveElipsoid(lParamList);
  }

  if (_className == CPString("Curve2DPolynome"))
  {
    return new CurvePolynome(lParamList);
  }

  if (_className == CPString("Curve2DScript"))
  {
    return new CurveScript(lParamList);
  }

  return 0;
}
//---------------------------------------------------------------------------

