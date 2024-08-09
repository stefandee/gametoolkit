#ifndef COMMON_HELPERSWIN32_H
#define COMMON_HELPERSWIN32_H
#pragma once

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "ddraw.h"

// tmn:  Why require anything higher than we really need.
// karg: now for alpha-blending ;)

// I hardly believe some will want to use DirectX5...
#if (DIRECTDRAW_VERSION <= 0x0500)
  typedef DDSURFACEDESC        DDSURFACEDESC_t;
  typedef DDSCAPS              DDSCAPS_t;
  typedef LPDIRECTDRAWSURFACE3 LPDIRECTDRAWSURFACE_t;
  typedef IDirectDrawSurface3  DIRECTDRAWSURFACE_t;
  typedef LPDIRECTDRAW2        LPDIRECTDRAW_t;
  typedef DDSCAPS              DDSCAPS_t;
#endif

#if (DIRECTDRAW_VERSION > 0x0500)
  #if (DIRECTDRAW_VERSION >= 0x0900)
//  #pragma message("DIRECTX 9")
    typedef DDSURFACEDESC2 DDSURFACEDESC_t;
    typedef DDSCAPS2       DDSCAPS_t;
    typedef LPDIRECTDRAWSURFACE7 LPDIRECTDRAWSURFACE_t;
    typedef IDirectDrawSurface7  DIRECTDRAWSURFACE_t;
    typedef LPDIRECTDRAW7        LPDIRECTDRAW_t;
  #else
    #if (DIRECTDRAW_VERSION >= 0x0700)
//#pragma message("DIRECTX 7")
      typedef DDSURFACEDESC2 DDSURFACEDESC_t;
      typedef DDSCAPS2       DDSCAPS_t;
      typedef LPDIRECTDRAWSURFACE7 LPDIRECTDRAWSURFACE_t;
      typedef IDirectDrawSurface7  DIRECTDRAWSURFACE_t;
      typedef LPDIRECTDRAW7        LPDIRECTDRAW_t;
    #else
      typedef DDSURFACEDESC2 DDSURFACEDESC_t;
      typedef DDSCAPS2       DDSCAPS_t;
      typedef LPDIRECTDRAWSURFACE5 LPDIRECTDRAWSURFACE_t;
      typedef IDirectDrawSurface5  DIRECTDRAWSURFACE_t;
      typedef LPDIRECTDRAW5        LPDIRECTDRAW_t;
    #endif
  #endif
#endif


struct TPoint : POINT {};
//struct TRect : RECT {};


#endif	// COMMON_HELPERSWIN32_H
