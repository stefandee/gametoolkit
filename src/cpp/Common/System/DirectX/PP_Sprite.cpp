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
//#include <vcl.h>
#pragma hdrstop

#include "PP_Sprite.h"
#include "PP_Graphic.h"
#include "logfile.h"
#include "GBmpLoad.h"
#include "GJpgLoad.h"
#include "GPcxLoad.h"
#include "GPngLoad.h"

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
   mSurface = 0;

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
HRESULT CSprite::ReleaseSurface()
{
   HRESULT lHRet;
   if (mSurface != NULL)
   {
      lHRet = mSurface->Release();
      mSurface=NULL;
   }
   return lHRet;
}
//---------------------------------------------------------------------------

//--------------------------------------------------------------------------
// Description : reface Sprite-ul
// Returned val: HRESULT
//--------------------------------------------------------------------------
HRESULT CSprite::Restore()
{
   HRESULT lHRet = E_FAIL;

   if (mSurface)
   {
      // try to restore the surface
      lHRet = mSurface->Restore();

      if (lHRet != DD_OK)
      {
         // restoring went ok, reload the data from file
         if (LoadGraphicFile16(mFileName) == true)
         {
           lHRet = DD_OK;
         }
      }
      else
      {
        // it seems that the memory surface was totally lost
        // release the surface (just to make mSurface NULL, otherwise
        // loadgraphicfile16 would just fail) and reload from disk
        ReleaseSurface();

        if (LoadGraphicFile16(mFileName) == true)
        {
          lHRet = DD_OK;
        }
        else
        {
          mValid = false;
        }
      }
   }
   else
   {
      if (LoadGraphicFile16(mFileName) == true)
         lHRet = DD_OK;
   }      

   return lHRet;
}
//---------------------------------------------------------------------------

/////////////////////////////////////////////////////////////////////////////
//PRIVATE SECTION
/////////////////////////////////////////////////////////////////////////////
//--------------------------------------------------------------------------
// Description : creaza lpoSurface
// Param       : width, height - dimensiunea suprafetei dorite
//--------------------------------------------------------------------------
HRESULT CSprite::CreateSurface(long width, long height)
{
   CPGIGraphicSystem *lGSI;   //adica local Graphic System Instance

   try
   {
      lGSI = lGSI->Instance();
   }
   catch(...)
   {
      logWriteLn("CSprite::CreateSurface - Unable to create surface.");
      return E_FAIL;
   }

   mSurface = lGSI->CreateSurface(width, height);
   //aici se scriu informatiile in Sprite
   mWidth = width;
   mHeight = height;
   mValid = true;

//   ddrval = DDSetColorKey(lpSurface, RGB(0, 0, 0));
   return DD_OK;
}
//---------------------------------------------------------------------------

bool CSprite::LoadGraphicFile16(CPString filename)
{
  //GJpgLoad       lBmpLoad;
  //GJpgLoad       lJpgLoad;
  TColorFormat   lColorFormat;
  int            lWidth, lHeight;
  //DDSURFACEDESC2 lDDSD;
  DDSURFACEDESC_t lDDSD;
  HRESULT        lHRet;

  GLoad* lLoader = GetLoader(filename);

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

  lLoader->GetInfo(&lWidth, &lHeight, &lColorFormat);

  // Create a DirectDrawSurface for this bitmap
  if (mSurface == 0)
  {
    if (CreateSurface(lWidth, lHeight) != DD_OK)
    {
      delete lLoader;
      return false;
    }
  }

  // Lock surface buffer
  lDDSD.dwSize = sizeof(lDDSD);
  lHRet = mSurface->Lock(NULL, &lDDSD, DDLOCK_WAIT, NULL);
  if (lHRet != DD_OK)
  {
    logWriteLn("CSprite::LoadGraphicFile16 - error getting surface lock, LOGDET_LOWEST");
    delete lLoader;
    return false;
  }

  // Load the image from file
  if (!lLoader->LoadAligned((unsigned char*)lDDSD.lpSurface, lWidth, lHeight, lDDSD.lPitch, RGB_565))
  {
    logWriteLn("CSprite::LoadGraphicFile16 - failed to load from graphic file", LOGDET_LOWEST);
    mSurface->Unlock(NULL);
    delete lLoader;
    return false;
  }

  mSurface->Unlock(NULL);

  delete lLoader;
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

  if (lFileExt3 == CPString("PNG"))
  {
    return new GPngLoad();
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

   if (CreateSurface(_width, _height) != DD_OK)
   {
      mValid = false;
      throw CSpriteError("Cannot create surface!");
   }
}
//---------------------------------------------------------------------------


