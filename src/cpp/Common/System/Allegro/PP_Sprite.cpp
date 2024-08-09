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
/*---------------------------------------------------------------------------
 application: Pure Power

 descrition : Class Sprite
              suporta si links
 last modify: 21 04 00
        by grabX
 - Modificari :
   Karg - 19 oct   2001, 0300  - graphic files now load using GLoad classes
   Karg - 29 iulie 1999, 1500  - am adaugat functia loadGraphicFile16
                               - am modificat constructorul
                               - am sters functia loadSpriteFromFile (oricum nu facea ce zicea)
   Karg - 30 iulie 1999, 1900  - am adaugat al doilea constructor
--------------------------------------------------------------------------*/
#pragma hdrstop

#include "PP_Sprite.h"
#include "PP_Graphic.h"
#include "logfile.h"
#include "GBmpLoad.h"
#include "GJpgLoad.h"
#include "GPcxLoad.h"
#include "Stream_ANSI_C.h"

//---------------------------------------------------------------------------
#ifdef __BORLANDC__
#pragma package(smart_init)
#endif


/////////////////////////////////////////////////////////////////////////////
//PUBLIC SECTION
/////////////////////////////////////////////////////////////////////////////
//--------------------------------------------------------------------------
// Description : Constructor
// Param       : numele bitmapului, si dim. celulei
//--------------------------------------------------------------------------
/*CSprite::CSprite(CPString filename, long cellWidth, long cellHeight)
{
   mReferences = 1;
   mFileName = filename;

   mCWidth  = cellWidth;
   mCHeight = cellHeight;

   if (!LoadGraphicFile16(filename))
      PostQuitMessage(0);

   mCount  = (long) mWidth / mCWidth;
}
*/
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Descriere   : al doilea constructor, apelat din SpriteList
// Param       : ca si mai sus; redundant - nu se foloseste
// Rezultat    : nimic
// Comentarii  : ca sa nu se modifice tot ce a facut Grab,
//               si sa pot adauga loaderul de fisiere de PCX, TGA, BMP
//---------------------------------------------------------------------------
CSprite::CSprite(CPString filename, long wcells, long hcells)
{
   mReferences = 1;
   mFileName = filename;

   logWriteLn("CSprite::CSprite - BEGIN", LOGDET_LOWEST);
   logWrite("Sprite file name : ",LOGDET_LOWEST);
   logWriteLn(filename,LOGDET_LOWEST);
   if (!LoadGraphicFile16(filename))
   {
      //exceptie
      throw CSpriteError("Cannot load from file!");
   }

   // loadGraphicFile a completat cimpurile Width si Height, de fapt apelul la createSprite
   mHCells = hcells;
   logWrite("CSprite::CSprite - HCells  =", LOGDET_LOWEST);
   logWriteILn(mHCells, LOGDET_LOWEST);
   mWCells = wcells;
   logWrite("CSprite::CSprite - WCells  =", LOGDET_LOWEST);
   logWriteILn(mWCells, LOGDET_LOWEST);
   mCWidth = mWidth / mWCells;
   logWrite("CSprite::CSprite - CWidth  =", LOGDET_LOWEST);
   logWriteILn(mCWidth, LOGDET_LOWEST);
   mCHeight= mHeight / mHCells;
   logWrite("CSprite::CSprite - CHeight = ", LOGDET_LOWEST);
   logWriteILn(mCHeight, LOGDET_LOWEST);
   mCount  = (long) mWidth / mCWidth;
   logWriteLn("CSprite::CSprite - OVER", LOGDET_LOWEST);
}
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
// Description : Destructor
//--------------------------------------------------------------------------
CSprite::~CSprite()
{
   logWriteLn("CSprite::~CSprite - BEGIN", LOGDET_LOWEST);
   logWriteLn(mFileName.c_str(), LOGDET_LOWEST);
   ReleaseSurface();
   logWriteLn("CSprite::~CSprite - OVER", LOGDET_LOWEST);
}
//---------------------------------------------------------------------------

//--------------------------------------------------------------------------
// Description : release-uieste suprafata
// Others      : folosita pentru schimbarea rezolutiei : se sterge si se reincarca
//--------------------------------------------------------------------------
bool CSprite::ReleaseSurface()
{
   if (mSurface != NULL)
   {
      delete mSurface;
      mSurface = 0;
   }
   return true;
}
//---------------------------------------------------------------------------

//--------------------------------------------------------------------------
// Description : reface Sprite-ul
// Returned val: HRESULT
//--------------------------------------------------------------------------
bool CSprite::Restore()
{
#ifdef PPT_USE_SDL	// TMN: Disabled during SDL conversion

   return false;

#else	// PPT_USE_SDL

   HRESULT lHRet = E_FAIL;

   if (mSurface)
   {
      lHRet = mSurface->pimpl->Restore();
      if (lHRet != DD_OK)
         if (LoadGraphicFile16(mFileName) == true)
            lHRet = DD_OK;
   }
   else
   {
      if (LoadGraphicFile16(mFileName) == true) {
         lHRet = DD_OK;
      }
   }
   return lHRet == DD_OK;

#endif	// PPT_USE_SDL

}
//---------------------------------------------------------------------------

/////////////////////////////////////////////////////////////////////////////
//PRIVATE SECTION
/////////////////////////////////////////////////////////////////////////////
//--------------------------------------------------------------------------
// Description : creaza lpoSurface
// Param       : width, height - dimensiunea suprafetei dorite
//--------------------------------------------------------------------------
bool CSprite::CreateSurface(long width, long height)
{
   CPGIGraphicSystem *lGSI;   //adica local Graphic System Instance

   try
   {
      lGSI = CPGIGraphicSystem::Instance();
   }
   catch(...)
   {
      return false;
   }

   mSurface = lGSI->CreateSurface(width, height);
   //aici se scriu informatiile in Sprite
   mWidth = width;
   mHeight = height;
   mValid = true;

//   ddrval = DDSetColorKey(lpSurface, RGB(0, 0, 0));
   return true;
}
//---------------------------------------------------------------------------

bool CSprite::LoadGraphicFile16(CPString filename)
{
  //set_color_conversion(COLORCONV_24_TO_16);

  mSurface = new CPGISurface();

  filename.Replace('/', '\\');
  logWriteLn(filename.c_str());

  //set_color_conversion(COLORCONV_24_TO_16);

  mSurface->pimpl = load_bitmap(filename.c_str(), 0);
  mWidth = mSurface->Width();
  mHeight = mSurface->Height();

  if (!mSurface->pimpl)
  {
      logWriteLn("Loading of graphic file failed.");
      return false;
  }

  for (int y = 0; y < mSurface->pimpl->h; y++)
	 for (int x = 0; x < mSurface->pimpl->w; x++)
	    if (getpixel(mSurface->pimpl, x, y) == 0)
	       putpixel(mSurface->pimpl, x, y, bitmap_mask_color(mSurface->pimpl));

  set_clip_state(mSurface->pimpl, 1);

  //GBmpLoad        lBmpLoad;
  /*
  TColorFormat    lColorFormat;
  int             lWidth, lHeight;
  GLoad*          lLoader; // should be an auto_ptr

  lLoader = GetLoader(filename);

  if (!lLoader)
  {
    return false;
  }

  // deschid imaginea - citesc headerul
  if (!lLoader->Open(filename.c_str()))
  {
    delete lLoader;
    return false;
  }

  //logWriteLn("bmp get info");
  lLoader->GetInfo(&lWidth, &lHeight, &lColorFormat);

  // Create a DirectDrawSurface for this bitmap
  if (!CreateSurface(lWidth, lHeight))
  {
    delete lLoader;
    return false;
  }

  try
  {
    // Lock surface buffer
    CPGISurface::SurfaceLocker locker(*mSurface);

    logWriteLn(CPString("surface pitch: ") + CPString(lWidth) + "x" + CPString(lHeight));

    // TODO: Load the image from file
    if (!lLoader->LoadAligned((unsigned char*)mSurface->Bits(), lWidth, lHeight, mSurface->Pitch(), RGB_565))
    {
      logWriteLn("CSprite::LoadGraphicFile16 - failed to load from graphic file", LOGDET_LOWEST);
      delete lLoader;
      return false;
    }
  }
  catch (...)
  {
    logWriteLn("CSprite::LoadGraphicFile16 - exception loading the image file.", LOGDET_LOWEST);
    delete lLoader;
    return false;
  }

  delete lLoader;
  */

  return true;
}
//---------------------------------------------------------------------------

//
// a factory method to retrive a loader class based on file extension
//
GLoad* CSprite::GetLoader(CPString _filename)
{
  CPString lFileExt3 = _filename.SubString(_filename.Length() - 3, 3);
  lFileExt3.ToUpperCase();

  if (lFileExt3 == CPString("JPG"))
  {
    return new GJpgLoad();
  }

  if (lFileExt3 == CPString("BMP"))
  {
    return new GBmpLoad();
  }

  if (lFileExt3 == CPString("PCX"))
  {
    return new GPcxLoad();
  }

  return 0;
}
//---------------------------------------------------------------------------

CSpriteCustom::CSpriteCustom(int _width, int _height, int _wcells, int _hcells)
{
   mWidth  = _width;
   mHeight = _height;
   mHCells = _hcells;
   mWCells = _wcells;
   mCWidth = mWidth / mWCells;
   mCHeight= mHeight / mHCells;
   mCount  = (long) mWidth / mCWidth;

   if (!CreateSurface(_width, _height))
   {
      mValid = false;
      throw CSpriteError("Cannot create surface!");
   }
}
//---------------------------------------------------------------------------


