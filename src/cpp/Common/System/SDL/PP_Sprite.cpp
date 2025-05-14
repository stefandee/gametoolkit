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
 - History :
   Karg - 17.08.2017 - load image files using SDL_image library, removed dependencies to GLoad
   Karg - 19 oct   2001  - graphic files now load using GLoad classes
   Karg - 29 iulie 1999  - am adaugat functia loadGraphicFile16
                               - am modificat constructorul
                               - am sters functia loadSpriteFromFile (oricum nu facea ce zicea)
   Karg - 30 iulie 1999  - am adaugat al doilea constructor
--------------------------------------------------------------------------*/
#include "PP_Graphic.h"
#include "PP_Sprite.h"
#include "SDL_image.h"
#include "easylogging++.h"
#pragma hdrstop

#include "PP_Texture.h"

//---------------------------------------------------------------------------
#ifdef __BORLANDC__
#pragma package(smart_init)
#endif

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
    mValid = false;

    VLOG(9) << "CSprite::CSprite - BEGIN";
    VLOG(9) << "Sprite file name : " << filename;

    if (!LoadFromFile(filename))
    {
        LOG(FATAL) << "Cannot load: " << filename;

        throw CSpriteError("Cannot load from file!");
    }

    // loadGraphicFile a completat cimpurile Width si Height, de fapt apelul la createSprite
    mHCells = hcells;
    VLOG(9) << "CSprite::CSprite - HCells  =" << mHCells;

    mWCells = wcells;
    VLOG(9) << "CSprite::CSprite - WCells  =" << mWCells;

    mCWidth = mWidth / mWCells;
    VLOG(9) << "CSprite::CSprite - CWidth  =" << mCWidth;

    mCHeight= mHeight / mHCells;
    VLOG(9) << "CSprite::CSprite - CHeight = " << mCHeight;

    mCount  = (long) mWidth / mCWidth;
    VLOG(9) << "CSprite::CSprite - Count = " << mCount;

    mValid = true;

    VLOG(9) << "CSprite::CSprite - OVER";
}
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
// Description : Destructor
//--------------------------------------------------------------------------
CSprite::~CSprite()
{
    VLOG(9) << "CSprite::~CSprite - BEGIN " << mFileName.c_str();

    ReleaseTexture();
    mValid = false;

    VLOG(9) << "CSprite::~CSprite - OVER";
}
//---------------------------------------------------------------------------

//--------------------------------------------------------------------------
// Description : release-uieste suprafata
// Others      : folosita pentru schimbarea rezolutiei : se sterge si se reincarca
//--------------------------------------------------------------------------
bool CSprite::ReleaseTexture()
{
    if (mTexture != NULL)
    {
        delete mTexture;
        mTexture = NULL;
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
    return false;
}
//---------------------------------------------------------------------------

/////////////////////////////////////////////////////////////////////////////
//PRIVATE SECTION
/////////////////////////////////////////////////////////////////////////////
//--------------------------------------------------------------------------
// Description : creaza lpoSurface
// Param       : width, height - dimensiunea suprafetei dorite
//--------------------------------------------------------------------------
bool CSprite::CreateTexture(long width, long height)
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

    mTexture = lGSI->CreateTexture(width, height);

    mWidth = width;
    mHeight = height;
    mValid = true;

    return true;
}
//---------------------------------------------------------------------------

bool CSprite::LoadFromFile(CPString filename)
{
    mTexture = CPGIGraphicSystem::Instance()->CreateTextureFromFile(filename);
    mValid = false;

    if (mTexture == NULL || mTexture->impl == NULL )
    {
        return false;
    }

    SDL_SetTextureBlendMode(mTexture->impl, SDL_BLENDMODE_BLEND);

    mWidth = mTexture->Width();
    mHeight = mTexture->Height();
    mValid = true;

    return true;
}
//---------------------------------------------------------------------------

CPRect CSprite::GetPaintRect(CPPoint _where, int animCount, int animType, int zoomFactor)
{
    CPRect lSource = GetAnimRect(animCount, animType);

    return CPRect(_where.x, _where.y, _where.x + lSource.right - lSource.left, _where.y + lSource.bottom - lSource.top);
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

    if (!CreateTexture(_width, _height))
    {
        mValid = false;
        throw CSpriteError("Cannot create surface!");
    }
}
//---------------------------------------------------------------------------
