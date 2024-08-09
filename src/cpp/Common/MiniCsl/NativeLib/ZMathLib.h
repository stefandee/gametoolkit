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

//-----------------------------------------------------------------------------
//  Unit              :
//
//  Version           :
//
//  Description       :
//    *
//  History           :
//    [] - [] -
//-----------------------------------------------------------------------------

#ifndef ZMathLibH
#define ZMathLibH
//---------------------------------------------------------------------------

#include "ZLibrary.h"
#include "ZCallback.h"

class ZMathLib : public ZLibrary
{
  private:
    // callback handlers
    // callback handlers
    ZCsl_callback_t mHandleAbs;
    ZCsl_callback_t mHandleAcos;
    ZCsl_callback_t mHandleAsin;
    ZCsl_callback_t mHandleAtan;
    ZCsl_callback_t mHandleCeil;
    ZCsl_callback_t mHandleCos;
    ZCsl_callback_t mHandleCosh;
    ZCsl_callback_t mHandleExp;
    ZCsl_callback_t mHandleFloor;
    ZCsl_callback_t mHandleLog;
    ZCsl_callback_t mHandleLog10;
    ZCsl_callback_t mHandleMax;
    ZCsl_callback_t mHandleMin;
    ZCsl_callback_t mHandlePow;
    ZCsl_callback_t mHandleSqrt;
    ZCsl_callback_t mHandleSin;
    ZCsl_callback_t mHandleSinh;
    ZCsl_callback_t mHandleTan;
    ZCsl_callback_t mHandleTanh;
    ZCsl_callback_t mHandleRandom;
    ZCsl_callback_t mHandleDist2D;

    /*
    ZString (__closure* mHandleAbs)(ZCsl* aCsl);
    ZString (__closure* mHandleAcos)(ZCsl* aCsl);
    ZString (__closure* mHandleAsin)(ZCsl* aCsl);
    ZString (__closure* mHandleAtan)(ZCsl* aCsl);
    ZString (__closure* mHandleCeil)(ZCsl* aCsl);
    ZString (__closure* mHandleCos)(ZCsl* aCsl);
    ZString (__closure* mHandleCosh)(ZCsl* aCsl);
    ZString (__closure* mHandleExp)(ZCsl* aCsl);
    ZString (__closure* mHandleFloor)(ZCsl* aCsl);
    ZString (__closure* mHandleLog)(ZCsl* aCsl);
    ZString (__closure* mHandleLog10)(ZCsl* aCsl);
    ZString (__closure* mHandleMax)(ZCsl* aCsl);
    ZString (__closure* mHandleMin)(ZCsl* aCsl);
    ZString (__closure* mHandlePow)(ZCsl* aCsl);
    ZString (__closure* mHandleSqrt)(ZCsl* aCsl);
    ZString (__closure* mHandleSin)(ZCsl* aCsl);
    ZString (__closure* mHandleSinh)(ZCsl* aCsl);
    ZString (__closure* mHandleTan)(ZCsl* aCsl);
    ZString (__closure* mHandleTanh)(ZCsl* aCsl);
    ZString (__closure* mHandleRandom)(ZCsl* aCsl);
    ZString (__closure* mHandleDist2D)(ZCsl* aCsl);
    */

    // the actual implementation
    ZString mathAbs(ZCsl* csl);
    ZString mathAcos(ZCsl* csl);
    ZString mathAsin(ZCsl* csl);
    ZString mathAtan(ZCsl* csl);
    ZString mathCeil(ZCsl* csl);
    ZString mathCos(ZCsl* csl);
    ZString mathCosh(ZCsl* csl);
    ZString mathExp(ZCsl* csl);
    ZString mathFloor(ZCsl* csl);
    ZString mathLog(ZCsl* csl);
    ZString mathLog10(ZCsl* csl);
    ZString mathMax(ZCsl* csl);
    ZString mathMin(ZCsl* csl);
    ZString mathPow(ZCsl* csl);
    ZString mathSqrt(ZCsl* csl);
    ZString mathSin(ZCsl* csl);
    ZString mathSinh(ZCsl* csl);
    ZString mathTan(ZCsl* csl);
    ZString mathTanh(ZCsl* csl);
    ZString mathRandom(ZCsl* csl);
    ZString mathDist2D(ZCsl* csl);

  public:
   ZMathLib(                     // constructor
      ZCsl* _Parent,             // parent
      const ZString& _Name      // library name
      //ZLibrary* _Prev            // previous library
      );

   virtual void InitLibrary();
};


#endif
