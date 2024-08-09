#ifndef PP_SURFACE_H
#define PP_SURFACE_H

#ifdef PPT_USE_SDL
#include "SDL.h"
#else
#include "DDraw.h"
#endif
#include "PP_Graphic.h"
#include "PP_basetypes.h"


class CPGISurface
{
public:
	typedef PP_uint32_t color_t;

	CPGISurface();
	~CPGISurface();

	unsigned int Width() const;
	unsigned int Height() const;
	unsigned int Bpp() const;

	CPRect GetClipRect() const;
	bool   SetClipRect(const CPRect& cliprect);
	bool   ResetClipRect();

    bool isPixelTransparent(int x, int y);

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

#ifdef PPT_USE_SDL
	SDL_Surface* pimpl;
#else
	DIRECTDRAWSURFACE_t*	pimpl;
#endif

private:
	CPRect					mClipRect;
	void*					m_pBits;
	unsigned int			m_pitch;
};


#endif	// PP_SURFACE_H
