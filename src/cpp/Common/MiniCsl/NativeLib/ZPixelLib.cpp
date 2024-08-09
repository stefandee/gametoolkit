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
#include "ZPixelLib.h"
#include "ZCsl.hpp"
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

ZPixelLib::ZPixelLib(ZCsl* _Parent, const ZString& _Name/*, ZLibrary* _Prev*/) : ZLibrary(_Parent, _Name/*, _Prev*/)
{
  //mNative = true;
}
//---------------------------------------------------------------------------

void ZPixelLib::InitLibrary()
{
   ZString iModule("ZPixelLib");
   // this is native library - no external dependencies
   ZString init(ZString(
      "const pixelVersion = '1.0.0';\n"
      "const pixelCompiler = '") + PPT_COMPILER +"';\n"
      "const pixelLibtype = 'NATIVE';\n"
      "const pixelBuilt = '"+ZString(__DATE__)+" "+__TIME__+"';\n"
   );

#if PPT_COMPILER_MICROSOFT
   std::istrstream str((char*)init);
#else
   std::istringstream str((char *) init);
#endif
   
   GetParent()->loadScript(iModule, &str);

   // handler assignment
   mHandleGetR     .set(this, &ZPixelLib::GetR);
   mHandleGetG     .set(this, &ZPixelLib::GetG);
   mHandleGetB     .set(this, &ZPixelLib::GetB);
   mHandleGetA     .set(this, &ZPixelLib::GetA);
   mHandleBuildRGB .set(this, &ZPixelLib::BuildRGB);
   mHandleBuildARGB .set(this, &ZPixelLib::BuildARGB);

   // add functions
   GetParent()->addFunc(iModule, "GetR(const pixel)",          mHandleGetR);
   GetParent()->addFunc(iModule, "GetG(const pixel)",          mHandleGetG);
   GetParent()->addFunc(iModule, "GetB(const pixel)",          mHandleGetB);
   GetParent()->addFunc(iModule, "GetA(const pixel)",          mHandleGetA);
   GetParent()->addFunc(iModule, "BuildRGB(const r, const g, const b)",          mHandleBuildRGB);
   GetParent()->addFunc(iModule, "BuildARGB(const a, const r, const g, const b)",          mHandleBuildARGB);
}
//---------------------------------------------------------------------------

ZString ZPixelLib::GetR(ZCsl* csl)
{
   int pixel = csl->get("pixel").asInt();

   return (pixel & 0x00FF0000) >> 16;

} // ZPixelLib::GetR
//---------------------------------------------------------------------------

ZString ZPixelLib::GetG(ZCsl* csl)
{
   int pixel = csl->get("pixel").asInt();

   return (pixel & 0x0000FF00) >> 8;

} // ZPixelLib::GetG
//---------------------------------------------------------------------------

ZString ZPixelLib::GetB(ZCsl* csl)
{
   int pixel = csl->get("pixel").asInt();

   return (pixel & 0x000000FF);

} // ZPixelLib::GetB
//---------------------------------------------------------------------------

ZString ZPixelLib::GetA(ZCsl* csl)
{
   int pixel = csl->get("pixel").asInt();

   return (pixel & 0xFF000000) >> 24;

} // ZPixelLib::GetR
//---------------------------------------------------------------------------

ZString ZPixelLib::BuildRGB(ZCsl* csl)
{
   int r = csl->get("r").asInt();
   int g = csl->get("g").asInt();
   int b = csl->get("b").asInt();

   return (r << 16) | (g << 8) | b;

} // ZPixelLib::BuildRGB
//---------------------------------------------------------------------------

ZString ZPixelLib::BuildARGB(ZCsl* csl)
{
   int a = csl->get("a").asInt();
   int r = csl->get("r").asInt();
   int g = csl->get("g").asInt();
   int b = csl->get("b").asInt();

   return (a << 24) | (r << 16) | (g << 8) | b;

} // ZPixelLib::BuildRGB
//---------------------------------------------------------------------------
