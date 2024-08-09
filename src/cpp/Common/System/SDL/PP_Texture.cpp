//-----------------------------------------------------------------------------
// This file is part of PPTactical Engine.                                   //
//                                                                           //
// PPTactical Engine - engine for tactical/strategy games                    //
// Copyright (C) 1998-2017 Stefan Dicu                                       //
//                                                                           //
// PPTactical Engine is free software; you can redistribute it               //
// and/or modify it under the terms of the GNU Lesser General Public License //
// as published by the Free Software Foundation; either version 2.1 of the   //
// License, or (at your option) any later version.                           //
//                                                                           //
// PPTactical Engine is distributed in the hope that it will be useful, but  //
// WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY//
// or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public   //
// License for more details.                                                 //
//                                                                           //
// You should have received a copy of the GNU Lesser General Public License  //
// along with PPTactical Engine; if not, write to the Free Software          //
// Foundation Inc. 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA   //
//-----------------------------------------------------------------------------
#pragma hdrstop
#include "PP_Texture.h"

CPGITexture::CPGITexture()
{
    //ctor
    impl = NULL;
}
//---------------------------------------------------------------------------

CPGITexture::~CPGITexture()
{
    //dtor
    if (impl != NULL)
    {
        SDL_DestroyTexture(impl);
        impl = NULL;
    }
}
//---------------------------------------------------------------------------

int CPGITexture::Width() const
{
    int w, h;
    SDL_QueryTexture(impl, NULL, NULL, &w, &h);

    return w;
}
//---------------------------------------------------------------------------

int CPGITexture::Height() const
{
    int w, h;
    SDL_QueryTexture(impl, NULL, NULL, &w, &h);

    return h;
}
//---------------------------------------------------------------------------

unsigned int CPGITexture::Format() const
{
    unsigned int format;
    SDL_QueryTexture(impl, &format, NULL, NULL, NULL);

    return format;
}
//---------------------------------------------------------------------------

CPGITexture::TextureLocker::TextureLocker(CPGITexture& texture)
    :	mTexture(texture)
{
    SDL_LockTexture(texture.impl, NULL, mTexture.m_pBits, &mTexture.m_pitch);
}
//---------------------------------------------------------------------------

CPGITexture::TextureLocker::~TextureLocker()
{
    mTexture.m_pBits = NULL;
    mTexture.m_pitch = 0;
    SDL_UnlockTexture(mTexture.impl);
}
//---------------------------------------------------------------------------

