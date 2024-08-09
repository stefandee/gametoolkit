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


namespace // unique namespace
{
  inline void clamp(int& val, int minval, int maxval)
  {
          if (val < minval)
          {
            val = minval;
          }
          else if (val > maxval)
          {
            val = maxval;
          }
  }
}	// unique namespace
//---------------------------------------------------------------------------

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
		// free the surface
		destroy_bitmap(pimpl);
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
  return bitmap_color_depth(pimpl);
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
  const unsigned int width  = Width();
  const unsigned int height = Height();

  clamp(cliprect.left, 0, width);
  clamp(cliprect.top, 0, height);
  clamp(cliprect.right, cliprect.left, width);
  clamp(cliprect.bottom, cliprect.top, height);

  mClipRect = cliprect;

  set_clip_rect(pimpl, cliprect.left, cliprect.top, cliprect.Width(), cliprect.Height());

  return true;
}
//---------------------------------------------------------------------------

bool CPGISurface::ResetClipRect()
{
  set_clip_rect(pimpl, 0, 0, Width(), Height());
  return true;
  // TODO: set the clip rectangle
  //return SetClipRect(CPRect(0, 0, Width(), Height())) && SDL_SetClipRect(pimpl, NULL);
}
//---------------------------------------------------------------------------

void CPGISurface::FrameRect(const CPRect& rc, color_t color)
{
   //SurfaceLocker locker(*this);

   /*
   const CPRect rcClip = getGlobalClipRect();
   CPRect rcSurfClip = GetClipRect();
   --rcSurfClip.right;
   --rcSurfClip.bottom;
   setGlobalClipRect(rcSurfClip);
   */

   rect(pimpl, rc.left, rc.top, rc.right, rc.bottom, makecol(gfGetRValue(color), gfGetGValue(color), gfGetBValue(color)));

   /*
   frameRect(static_cast<char*>(Bits()),
             Pitch(),
             rc.left,
             rc.top,
             rc.right - 1,
             rc.bottom - 1,
             color);
   */

   //setGlobalClipRect(rcClip);
}
//---------------------------------------------------------------------------

void CPGISurface::FillRect(const CPRect& rc, color_t color)
{
  // TODO: should we lock the surface?
  rectfill(pimpl, rc.left, rc.top, rc.right, rc.bottom, makecol(gfGetRValue(color), gfGetGValue(color), gfGetBValue(color)));
}
//---------------------------------------------------------------------------

void CPGISurface::FillSurface(color_t color)
{
  FillRect(CPRect(0, 0, Width(), Height()), color);
}
//---------------------------------------------------------------------------

void CPGISurface::DrawSelRect(const CPRect& rc, unsigned int Length, color_t color)
{
   SurfaceLocker locker(*this);

   /*
   const CPRect rcClip = getGlobalClipRect();
   CPRect rcSurfClip = GetClipRect();
   --rcSurfClip.right;
   --rcSurfClip.bottom;
   setGlobalClipRect(rcSurfClip);
   */

   // TODO: temporary
   rect(pimpl, rc.left, rc.top, rc.right, rc.bottom, makecol(gfGetRValue(color), gfGetGValue(color), gfGetBValue(color)));
   /*
   selRect(static_cast<char*>(Bits()),
           Pitch(),
           rc.left,
           rc.top,
           rc.right,
           rc.bottom,
           Length,
           color);
   */

   //setGlobalClipRect(rcClip);
}
//---------------------------------------------------------------------------

void CPGISurface::Line(const CPPoint& ptStart, const CPPoint& ptEnd, color_t color)
{
  //SurfaceLocker locker(*this);

  /*
  const CPRect rcClip = getGlobalClipRect();
  CPRect rcSurfClip = GetClipRect();
  --rcSurfClip.right;
  --rcSurfClip.bottom;
  setGlobalClipRect(rcSurfClip);
  */

  line(pimpl, ptStart.x, ptStart.y, ptEnd.x, ptEnd.y, makecol(gfGetRValue(color), gfGetGValue(color), gfGetBValue(color)));

  //setGlobalClipRect(rcClip);
}
//---------------------------------------------------------------------------

void CPGISurface::HLine(int xStart, int xEnd, int y, color_t color)
{
  /*
  const CPRect rcClip = getGlobalClipRect();
  CPRect rcSurfClip = GetClipRect();
  --rcSurfClip.right;
  --rcSurfClip.bottom;
  setGlobalClipRect(rcSurfClip);
  */

  hline(pimpl, xStart, y, xEnd, makecol(gfGetRValue(color), gfGetGValue(color), gfGetBValue(color)));

  //setGlobalClipRect(rcClip);
}
//---------------------------------------------------------------------------

void CPGISurface::VLine(int x, int yStart, int yEnd, color_t color)
{
  /*
  const CPRect rcClip = getGlobalClipRect();
  CPRect rcSurfClip = GetClipRect();
  --rcSurfClip.right;
  --rcSurfClip.bottom;
  setGlobalClipRect(rcSurfClip);
  */

  vline(pimpl, x, yStart, yEnd, makecol(gfGetRValue(color), gfGetGValue(color), gfGetBValue(color)));

  //setGlobalClipRect(rcClip);
}
//---------------------------------------------------------------------------

void CPGISurface::Circle(const CPPoint& ptCenter, int _radius, color_t color)
{
  //SurfaceLocker locker(*this);

  /*
  const CPRect rcClip = getGlobalClipRect();
  CPRect rcSurfClip = GetClipRect();
  --rcSurfClip.right;
  --rcSurfClip.bottom;
  setGlobalClipRect(rcSurfClip);
  */

  circle(pimpl, ptCenter.x, ptCenter.y, _radius, makecol(gfGetRValue(color), gfGetGValue(color), gfGetBValue(color)));

  /*
  circle
   (
     static_cast<char*>(Bits()),
     Pitch(),
     ptCenter.x,
     ptCenter.y,
     _radius,
     color
   );
   */

  //setGlobalClipRect(rcClip);
}
//---------------------------------------------------------------------------

void CPGISurface::FillTriangle(const CPPoint& pt1, const CPPoint& pt2, const CPPoint& pt3, color_t color)
{
  //SurfaceLocker locker(*this);

  /*
  const CPRect rcClip = getGlobalClipRect();
  CPRect rcSurfClip = GetClipRect();
  --rcSurfClip.right;
  --rcSurfClip.bottom;
  setGlobalClipRect(rcSurfClip);
  */

  triangle(pimpl,
     pt1.x,
     pt1.y,
     pt2.x,
     pt2.y,
     pt3.x,
     pt3.y,
     makecol(gfGetRValue(color), gfGetGValue(color), gfGetBValue(color))
     );

  /*
  fillTriangle
   (
     static_cast<char*>(Bits()),
     Pitch(),
     pt1.x,
     pt1.y,
     pt2.x,
     pt2.y,
     pt3.x,
     pt3.y,
     color
   );
   */

  //setGlobalClipRect(rcClip);
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

//////////////////////////////////////////////////////////////////
CPGISurface::SurfaceLocker::SurfaceLocker(CPGISurface& surface)
:	mSurface(surface)
{
  // lock the surface for RAII
  acquire_bitmap(surface.pimpl);

  surface.m_pBits = (char*)surface.pimpl->line[0];
  surface.m_pitch = 2 * (surface.pimpl->line[1] - surface.pimpl->line[0]);
}
//---------------------------------------------------------------------------

CPGISurface::SurfaceLocker::~SurfaceLocker()
{
  mSurface.m_pBits = 0;
  mSurface.m_pitch = 0;

  // unlock the surface/RAII
  release_bitmap(mSurface.pimpl);
}
//---------------------------------------------------------------------------

