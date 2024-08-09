//-----------------------------------------------------------------------------
// This file is part of PPTactical Engine.                                   //
//                                                                           //
// PPTactical Engine - engine for tactical/strategy games                    // 
// Copyright (C) 1998, 1999, 2000, 2001 Stefan Dicu & Tudor Girba            //
//                                                                           //
// PPTactical Engine is free software; you can redistribute it               //
// and/or modify it under the terms of the GNU Lesser General Public License as     // 
// published by the Free Software Foundation; either version 2.1 of the        //
// License, or (at your option) any later version.                           //
//                                                                           //  
// PPTactical Engine is distributed in the hope that it will be useful, but  //
// WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY// 
// or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License  //
// for more details.                                                         //
//                                                                           // 
// You should have received a copy of the GNU Lesser General Public License         //
// along with PPTactical Engine; if not, write to the Free Software          //
// Foundation Inc. 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA   // 
//-----------------------------------------------------------------------------

#pragma hdrstop
#include "Platform.h"
#include "ZMathLib.h"
#include "ZCsl.hpp"
#include <stdlib.h>
#include <math.h>
#include <errno.h>
#include "PP_Stdlib.h"
#include "ZBase.hpp"

#ifndef PPT_COMPILER_MICROSOFT
#include <sstream>
#else
#include <strstream>	// TMN: Note use of REALLY OLD header !!!
#endif

using namespace std;


//---------------------------------------------------------------------------
#ifdef PPT_COMPILER_BORLAND
#pragma package(smart_init)
#endif

ZMathLib::ZMathLib(ZCsl* _Parent, const ZString& _Name/*, ZLibrary* _Prev*/) : ZLibrary(_Parent, _Name/*, _Prev*/)
{
  //mNative = true;
}
//---------------------------------------------------------------------------

void ZMathLib::InitLibrary()
{
   ZString iModule("ZMathLib");
   // this is native library - no external dependencies
   ZString init(ZString(
      "const mathVersion = '4.03';\n"
      "const mathCompiler = '") + PPT_COMPILER +"';\n"
      "const mathLibtype = 'NATIVE';\n"
      "const mathBuilt = '"+ZString(__DATE__)+" "+__TIME__+"';\n"
      /*
      "// functions in alfabetical order\n"
      "var abs(const x);                  // return absolute value\n"
      "var acos(const x);                 // calculate arccosine\n"
      "var asin(const x);                 // calculate arcsine\n"
      "var atan(const x);                 // calculate arctangent\n"
      "var ceil(const x);                 // find integer >= x\n"
      "var cos(const x);                  // calculate cosine\n"
      "var cosh(const x);                 // calculate hyperbolic cosine\n"
      "var exp(const x);                  // calculate exponential function\n"
      "var floor(const x);                // find integer <= x\n"
      "var log(const x);                  // calculate natural logarithm\n"
      "var log10(const x);                // calculate base 10 logarithm\n"
      "var max(const x, const y);         // return larger of x and y\n"
      "var min(const x, const y);         // return lesser of x and y\n"
      "var pow(const x, const y);         // compute power\n"
      "var sqrt(const x);                 // calculate square root\n"
      "var sin(const x);                  // calculate sine\n"
      "var sinh(const x);                 // calculate hyperbolic sine\n"
      "var tan(const x);                  // calculate tangent\n"
      "var tanh(const x);                 // calculate hyperbolic tangent\n"
      */
   );

#if PPT_COMPILER_MICROSOFT
   std::istrstream str((char*)init);
#else
   std::istringstream str((char *) init);
#endif
   
   GetParent()->loadScript(iModule, &str);

   // handler assignment
   mHandleAbs    .set(this, &ZMathLib::mathAbs);
   mHandleAcos   .set(this, &ZMathLib::mathAcos);
   mHandleAsin   .set(this, &ZMathLib::mathAsin);
   mHandleAtan   .set(this, &ZMathLib::mathAtan);
   mHandleCeil   .set(this, &ZMathLib::mathCeil);
   mHandleCos    .set(this, &ZMathLib::mathCos);
   mHandleCosh   .set(this, &ZMathLib::mathCosh);
   mHandleExp    .set(this, &ZMathLib::mathExp);
   mHandleFloor  .set(this, &ZMathLib::mathFloor);
   mHandleLog    .set(this, &ZMathLib::mathLog);
   mHandleLog10  .set(this, &ZMathLib::mathLog10);
   mHandleMax    .set(this, &ZMathLib::mathMax);
   mHandleMin    .set(this, &ZMathLib::mathMin);
   mHandlePow    .set(this, &ZMathLib::mathPow);
   mHandleSqrt   .set(this, &ZMathLib::mathSqrt);
   mHandleSin    .set(this, &ZMathLib::mathSin);
   mHandleSinh   .set(this, &ZMathLib::mathSinh);
   mHandleTan    .set(this, &ZMathLib::mathTan);
   mHandleTanh   .set(this, &ZMathLib::mathTanh);
   mHandleRandom .set(this, &ZMathLib::mathRandom);
   mHandleDist2D .set(this, &ZMathLib::mathDist2D);
   /*
   mHandleAbs    = mathAbs;
   mHandleAcos   = mathAcos;
   mHandleAsin   = mathAsin;
   mHandleAtan   = mathAtan;
   mHandleCeil   = mathCeil;
   mHandleCos    = mathCos;
   mHandleCosh   = mathCosh;
   mHandleExp    = mathExp;
   mHandleFloor  = mathFloor;
   mHandleLog    = mathLog;
   mHandleLog10  = mathLog10;
   mHandleMax    = mathMax;
   mHandleMin    = mathMin;
   mHandlePow    = mathPow;
   mHandleSqrt   = mathSqrt;
   mHandleSin    = mathSin;
   mHandleSinh   = mathSinh;
   mHandleTan    = mathTan;
   mHandleTanh   = mathTanh;
   mHandleRandom = mathRandom;
   mHandleDist2D = mathDist2D;
   */

   // add functions
   GetParent()->addFunc(iModule, "abs(const x)",          mHandleAbs);
   GetParent()->addFunc(iModule, "acos(const x)",         mHandleAcos);
   GetParent()->addFunc(iModule, "asin(const x)",         mHandleAsin);
   GetParent()->addFunc(iModule, "atan(const x)",         mHandleAtan);
   GetParent()->addFunc(iModule, "ceil(const x)",         mHandleCeil);
   GetParent()->addFunc(iModule, "cos(const x)",          mHandleCos);
   GetParent()->addFunc(iModule, "cosh(const x)",         mHandleCosh);
   GetParent()->addFunc(iModule, "exp(const x)",          mHandleExp);
   GetParent()->addFunc(iModule, "floor(const x)",        mHandleFloor);
   GetParent()->addFunc(iModule, "log(const x)",          mHandleLog);
   GetParent()->addFunc(iModule, "log10(const x)",        mHandleLog10);
   GetParent()->addFunc(iModule, "max(const x, const y)", mHandleMax);
   GetParent()->addFunc(iModule, "min(const x, const y)", mHandleMin);
   GetParent()->addFunc(iModule, "pow(const x, const y)", mHandlePow);
   GetParent()->addFunc(iModule, "sqrt(const x)",         mHandleSqrt);
   GetParent()->addFunc(iModule, "sin(const x)",          mHandleSin);
   GetParent()->addFunc(iModule, "sinh(const x)",         mHandleSinh);
   GetParent()->addFunc(iModule, "tan(const x)",          mHandleTan);
   GetParent()->addFunc(iModule, "tanh(const x)",         mHandleTanh);

   GetParent()->addFunc(iModule, "random(const x)",       mHandleRandom);
   GetParent()->addFunc(iModule, "dist2D(const x1, const y1, const x2, const y2)", mHandleDist2D);
}
//---------------------------------------------------------------------------

ZString ZMathLib::mathAbs(ZCsl* csl)
{
   double x(csl->get("x").asDouble());
   if (x < 0) x = -x;
   return x;
} // ZMathLib::mathAbs
//---------------------------------------------------------------------------

ZString ZMathLib::mathAcos(ZCsl* csl)
{
   double x(csl->get("x").asDouble());
   if (x < -1 || x > 1)
      ZTHROWEXC("x must be in -1...1");
   return acos(x);
} // ZMathLib::mathAcos
//---------------------------------------------------------------------------

ZString ZMathLib::mathAsin(ZCsl* csl)
{
   double x(csl->get("x").asDouble());
   if (x < -1 || x > 1)
      ZTHROWEXC("x must be in -1...1");
   return asin(x);
} // ZMathLib::mathAsin
//---------------------------------------------------------------------------

ZString ZMathLib::mathAtan(ZCsl* csl)
{
   return atan(csl->get("x").asDouble());
} // ZMathLib::mathAtan
//---------------------------------------------------------------------------

ZString ZMathLib::mathCeil(ZCsl* csl)
{
   return ceil(csl->get("x").asDouble());
} // ZMathLib::mathCeil
//---------------------------------------------------------------------------

ZString ZMathLib::mathCos(ZCsl* csl)
{
   return cos(csl->get("x").asDouble());
} // ZMathLib::mathCos
//---------------------------------------------------------------------------

ZString ZMathLib::mathCosh(ZCsl* csl)
{
   return cosh(csl->get("x").asDouble());
} // ZMathLib::mathCosh
//---------------------------------------------------------------------------

ZString ZMathLib::mathExp(ZCsl* csl)
{
   return exp(csl->get("x").asDouble());
} // ZMathLib::mathExp
//---------------------------------------------------------------------------

ZString ZMathLib::mathFloor(ZCsl* csl)
{
   return floor(csl->get("x").asDouble());
} // ZMathLib::mathFloor
//---------------------------------------------------------------------------

ZString ZMathLib::mathLog(ZCsl* csl)
{
   double x(csl->get("x").asDouble());
   if (x <= 0)
      ZTHROWEXC("x must be positive");
   return log(x);
} // ZMathLib::mathLog
//---------------------------------------------------------------------------

ZString ZMathLib::mathLog10(ZCsl* csl)
{
   double x(csl->get("x").asDouble());
   if (x <= 0)
      ZTHROWEXC("x must be positive");
   return log10(x);
} // ZMathLib::mathLog10
//---------------------------------------------------------------------------

ZString ZMathLib::mathMax(ZCsl* csl)
{
   double x(csl->get("x").asDouble());
   double y(csl->get("y").asDouble());
   return y > x ? y : x;
} // ZMathLib::mathMax
//---------------------------------------------------------------------------

ZString ZMathLib::mathMin(ZCsl* csl)
{
   double x(csl->get("x").asDouble());
   double y(csl->get("y").asDouble());
   return y < x ? y : x;
} // ZMathLib::mathMin
//---------------------------------------------------------------------------

ZString ZMathLib::mathPow(ZCsl* csl)
{
   errno = 0;
   double p(pow(
      csl->get("x").asDouble(),
      csl->get("y").asDouble()
   ));
   if ( errno == EDOM )
      ZTHROWEXC("invalid arguments for x and y");
   if ( errno == ERANGE )
      ZTHROWEXC("result underrun/overrun");
   return p;
} // ZMathLib::mathPow
//---------------------------------------------------------------------------

ZString ZMathLib::mathSqrt(ZCsl* csl)
{
   double x(csl->get("x").asDouble());
   if (x < 0)
      ZTHROWEXC("x must be >= 0");
   return sqrt(x);
} // ZMathLib::mathSqrt
//---------------------------------------------------------------------------

ZString ZMathLib::mathSin(ZCsl* csl)
{
   return sin(csl->get("x").asDouble());
} // ZMathLib::mathSin
//---------------------------------------------------------------------------

ZString ZMathLib::mathSinh(ZCsl* csl)
{
   return sinh(csl->get("x").asDouble());
} // ZMathLib::mathSinh
//---------------------------------------------------------------------------

ZString ZMathLib::mathTan(ZCsl* csl)
{
   return tan(csl->get("x").asDouble());
} // ZMathLib::mathTan
//---------------------------------------------------------------------------

ZString ZMathLib::mathTanh(ZCsl* csl)
{
   return tanh(csl->get("x").asDouble());
} // ZMathLib::mathTanh
//---------------------------------------------------------------------------

ZString ZMathLib::mathRandom(ZCsl* csl)
{
  return gfRandom(csl->get("x").asInt());
} // ZMathLib::mathRandom
//---------------------------------------------------------------------------

ZString ZMathLib::mathDist2D(ZCsl* csl)
{
  double xd = (csl->get("x1").asDouble() - csl->get("x2").asDouble());
  double yd = (csl->get("y1").asDouble() - csl->get("y2").asDouble());

  return sqrt(xd*xd + yd*yd);
} // ZMathLib::mathDist2D
//---------------------------------------------------------------------------


