#ifndef PP_SURFACE_H
#define PP_SURFACE_H

#include "logfile.h"
#include "PP_Graphic.h"
#include "PP_basetypes.h"
#include <allegro.h>

class CPGISurface
{
public:
	typedef PP_uint16_t color_t;

	CPGISurface();
	~CPGISurface();

	unsigned int Width() const;
	unsigned int Height() const;
	unsigned int Bpp() const;

	CPRect GetClipRect() const;
	bool   SetClipRect(const CPRect& cliprect);
	bool   ResetClipRect();

	void FrameRect(const CPRect& rc, color_t color);
	void FillRect(const CPRect& rc, color_t color);
	void FillSurface(color_t color);
	void DrawSelRect(const CPRect& rc, unsigned int Length, color_t color);
    void Line(const CPPoint& ptStart, const CPPoint& ptEnd, color_t color);
    void HLine(int xStart, int xEnd, int y, color_t color);
    void VLine(int x, int yStart, int yEnd, color_t color);
    void Circle(const CPPoint& ptCenter, int _radius, color_t color);
    void FillTriangle(const CPPoint& pt1, const CPPoint& pt2, const CPPoint& pt3, color_t color);

	void*        Bits();		// Only valid if the surface is locked!
	unsigned int Pitch() const;	// Only valid if the surface is locked!

	class SurfaceLocker
	{
	public:
	   SurfaceLocker(CPGISurface& surface);
	   ~SurfaceLocker();

	private:
	   SurfaceLocker(const SurfaceLocker&);     // no impl.
	   void operator=(const SurfaceLocker&);    // no impl.

	   CPGISurface&    mSurface;
	};

private:
	CPGISurface(const CPGISurface& rhs);	// no impl
	void operator=(const CPGISurface& rhs);	// no impl

	friend class SurfaceLocker;
	friend class CPGIGraphicSystem;
	friend class CSprite;

public:
    // surface
    BITMAP* pimpl;

private:
	CPRect					mClipRect;
	char*					m_pBits;
	unsigned int			m_pitch;
};


#endif	// PP_SURFACE_H
