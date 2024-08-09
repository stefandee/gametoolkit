//-----------------------------------------------------------------------------
// This file is part of PPTactical Engine.                                   //
//                                                                           //
// PPTactical Engine - engine for tactical/strategy games                    //
// Copyright (C) 1998-2002 Stefan Dicu & Tudor Girba                         //
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

#include "PP_Surface.h"
#include "PP_Stdlib.h"

CPGISurface::CPGISurface()
    :	pimpl(0),
      mClipRect(0, 0, 0, 0),
      m_pBits(0),
      m_pitch(0)
{
}
//---------------------------------------------------------------------------

CPGISurface::~CPGISurface()
{
    if (pimpl)
    {
        SDL_FreeSurface(pimpl);
        pimpl = 0;
    }
}
//---------------------------------------------------------------------------

unsigned int CPGISurface::Width() const
{
    return pimpl->w;
}
//---------------------------------------------------------------------------

unsigned int CPGISurface::Height() const
{
    return pimpl->h;
}
//---------------------------------------------------------------------------

unsigned int CPGISurface::Bpp() const
{
    return pimpl->format->BitsPerPixel;
}
//---------------------------------------------------------------------------

CPRect CPGISurface::GetClipRect() const
{
    return mClipRect;
}
//---------------------------------------------------------------------------

bool CPGISurface::SetClipRect(const CPRect& cliprect_)
{
    CPRect cliprect = cliprect_;
    int width  = Width();
    int height = Height();

    cliprect.left = clamp(cliprect.left, 0, width);
    cliprect.top = clamp(cliprect.top, 0, height);
    cliprect.right = clamp(cliprect.right, cliprect.left, width);
    cliprect.bottom = clamp(cliprect.bottom, cliprect.top, height);

    mClipRect = cliprect;

    SDL_Rect rc = { cliprect.left, cliprect.top,
                    cliprect.Width(), cliprect.Height()
                  };
    SDL_SetClipRect(pimpl, &rc);
    return true;
}
//---------------------------------------------------------------------------

bool CPGISurface::ResetClipRect()
{
    return SetClipRect(CPRect(0, 0, Width(), Height())) && SDL_SetClipRect(pimpl, NULL);
}
//---------------------------------------------------------------------------

void* CPGISurface::Bits()
{
    return m_pBits;
}
//---------------------------------------------------------------------------

unsigned int CPGISurface::Pitch() const
{
    return m_pitch;
}
//---------------------------------------------------------------------------

bool CPGISurface::isPixelTransparent(int x, int y)
{
    if (x < 0 || y < 0 || x >= Width() || y >= Height())
    {
        return false;
    }

    SurfaceLocker locker(*this);

    char* p = static_cast<char*>(Bits());

    Uint32 pixel = *((Uint32*)(p + y*Pitch() + (x << 2)));

    Uint8 r, g, b, a;

    SDL_GetRGBA(pixel, pimpl->format, &r, &g, &b, &a);

    return (a == 0);
}

//////////////////////////////////////////////////////////////////
CPGISurface::SurfaceLocker::SurfaceLocker(CPGISurface& surface)
    :	mSurface(surface)
{
    SDL_LockSurface(surface.pimpl);
    surface.m_pBits = surface.pimpl->pixels;
    surface.m_pitch = surface.pimpl->pitch;
}
//---------------------------------------------------------------------------

CPGISurface::SurfaceLocker::~SurfaceLocker()
{
    mSurface.m_pBits = 0;
    mSurface.m_pitch = 0;
    SDL_UnlockSurface(mSurface.pimpl);
}
//---------------------------------------------------------------------------

