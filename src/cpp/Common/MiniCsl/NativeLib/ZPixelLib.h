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
//  Hystory           :
//    [] - [] -
//-----------------------------------------------------------------------------

#ifndef ZPixelLibH
#define ZPixelLibH
//---------------------------------------------------------------------------

#include "ZLibrary.h"
#include "GenericCallback.h"

class ZPixelLib : public ZLibrary
{
  private:
    // callback handlers
    ZCsl_callback_t mHandleGetR;
    ZCsl_callback_t mHandleGetG;
    ZCsl_callback_t mHandleGetB;
    ZCsl_callback_t mHandleGetA;
    ZCsl_callback_t mHandleBuildRGB;
    ZCsl_callback_t mHandleBuildARGB;
    
    // the actual implementation
    ZString GetR(ZCsl* csl);
    ZString GetG(ZCsl* csl);
    ZString GetB(ZCsl* csl);
    ZString GetA(ZCsl* csl);
    ZString BuildRGB(ZCsl* csl);
    ZString BuildARGB(ZCsl* csl);

  public:
   ZPixelLib(                     // constructor
      ZCsl* _Parent,             // parent
      const ZString& _Name      // library name
      //ZLibrary* _Prev            // previous library
      );

   virtual void InitLibrary();
};


#endif
