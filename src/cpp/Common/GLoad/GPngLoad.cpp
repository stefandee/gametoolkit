//-----------------------------------------------------------------------------
// Copyright (C) 2002 Mike Nordell                                           //
// This file is part of PPTactical Engine.                                   //
//                                                                           //
// PPTactical Engine - engine for tactical/strategy games                    // 
//                                                                           //
// PPTactical Engine is free software; you can redistribute it               //
// and/or modify it under the terms of the GNU General Public License as     // 
// published by the Free Software Foundation; either version 2 of the        //
// License, or (at your option) any later version.                           //
//                                                                           //  
// PPTactical Engine is distributed in the hope that it will be useful, but  //
// WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY// 
// or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License  //
// for more details.                                                         //
//                                                                           // 
// You should have received a copy of the GNU General Public License         //
// along with PPTactical Engine; if not, write to the Free Software          //
// Foundation Inc. 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA   // 
//-----------------------------------------------------------------------------
//---------------------------------------------------------------------------
// History:
//    22 Mar 2002 - TMN: Created
//---------------------------------------------------------------------------

#include "Platform.h"
#ifdef PPT_PLATFORM_WIN
#include "GPngLoad.h"
#include "logfile.h"

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif


#if defined(__BORLANDC__) || defined (_MSC_VER)
typedef unsigned __int16 local_16bits_t;
#else
#error Unsupported compiler. Please fix.
#endif

typedef struct
{
  unsigned char blue;
  unsigned char green;
  unsigned char red;
} xTRgb;

GPngLoad::GPngLoad()
{
	memset(&mInfo, 0, sizeof(mInfo));
	mInfo.structsize = sizeof(PNGD_P2DINFO);
}
//---------------------------------------------------------------------------

// Description : 
// Parameters  : 
// Returns     : 
// Comment     : 
// History: 
bool GPngLoad::Load(unsigned char* data, int width, int height, TColorFormat dataBpp)
{
	return LoadAligned(data, width, height, -1, dataBpp);
}
//---------------------------------------------------------------------------

// Description : 
// Parameters  : 
// Returns     : 
// Comment     : 
// History: 
bool GPngLoad::LoadAligned(unsigned char* data, int width, int height, int bytesPitch, TColorFormat dataBpp)
{
	if (!data || dataBpp != RGB_565)
	{
		return false;
	}

	switch(info.bpp)
	{
    case RGB_8 :
		return Load8Bits(data, width, height, bytesPitch, dataBpp);
		
    case RGB_24 :
		return Load24Bits(data, width, height, bytesPitch, dataBpp);
		
    default :
		// pe aici nu o sa se ajunga vreodata, dar sa fim siguri
		return false;
	}
}
//---------------------------------------------------------------------------

// Description : 
// Parameters  : 
// Returns     : 
// Comment     : 
// History: 
bool GPngLoad::ReadHeader()
{
	if (!rewind()) 
	{
		return false;
	}
	
	PP::Stream* pStream = GetStream();
	if (!pStream)
	{
		return false;
	}
	if (read_png_to_dib(&mInfo, *pStream) != PNGD_E_SUCCESS)
	{
		return false;
	}

	// TMN: Yikes! Changing parent class data!
	info.width  = mInfo.lpdib->biWidth;
	info.height = mInfo.lpdib->biHeight;

	switch(mInfo.lpdib->biBitCount)
	{
	case 8 :
		info.bpp = RGB_8;
		break;
    case 24 :
		info.bpp = RGB_24;
		break;
	default :
		return false;
	}
	
	return true;
}
//---------------------------------------------------------------------------

// Description : 
// Parameters  : 
// Returns     : 
// Comment     : 
// History: 
bool GPngLoad::Load8Bits (unsigned char* data, int width, int height, int bytesPitch, TColorFormat dataBpp)
{
	int            j, i; // [Kafka] 
	local_16bits_t refPal[256];
	
	if (!data || dataBpp != RGB_565)
	{
		return false;
	}

	const int nColors = mInfo.palette_colors;
	const RGBQUAD* srcPal = mInfo.palette;

	// create 16-bit 565 colors from the RGB palette
	for(i = 0; i < nColors; i++)
	{
		refPal[i] =
			(local_16bits_t)
			(((srcPal[i].rgbRed >> 3) << 11)  |
			((srcPal[i].rgbGreen >> 2) << 5) |
			(srcPal[i].rgbBlue >> 3));
	}

	unsigned char* startPtr = NULL;

	// pentru optimizare; avem 2 octeti/pixel in formatul 565
	if (bytesPitch == -1)
	{
		startPtr   = data + (width * 2) * (height - 1);
		bytesPitch = (width * 2);
	}
	else
	{
		if (bytesPitch / 2 < width) 
		{
			return false;
		}
		startPtr = data + bytesPitch * (height - 1);
	}
	// startPtr = data + (width << 1) * (height - 1);
	
	// conversiile intre suprafete sint mai lente
	for(i = 0; i < height; i++)
	{
		unsigned char* srcBits = &((unsigned char*)mInfo.lpbits)[mInfo.lpdib->biWidth * i];
		
		// conversie 256 culori -> 565
		for(j = 0; j < width ; j++)
		{
			*((local_16bits_t*)startPtr + j) = refPal[srcBits[j]];
		}
		
		// modific scan line start
		startPtr -= bytesPitch;
	}
	
	return true;
}
//---------------------------------------------------------------------------

// Description : 
// Parameters  : 
// Returns     : 
// Comment     : 
// History: 
bool GPngLoad::Load24Bits (unsigned char* data, int width, int height, int bytesPitch, TColorFormat dataBpp)
{
	if (!data || dataBpp != RGB_565)
	{
		return false;
	}

	unsigned char* startPtr;

	if (bytesPitch == -1)
	{
		startPtr = data + (width << 1) * (height - 1);
		bytesPitch = (width << 1);
	}  
	else
	{
		if (bytesPitch / 2 < width) 
		{
			return false;
		}
		startPtr = data + bytesPitch * (height - 1);
	}

	const size_t srcPitch = ((mInfo.lpdib->biWidth * 3) + 3) & ~3;;

	for(int i = 0; i < height; i++)
	{
		unsigned char* srcBits = &((unsigned char*)mInfo.lpbits)[srcPitch * i];
		local_16bits_t* pDest = (local_16bits_t*)startPtr;

		// conversion 24 -> 565
		for(int j = 0; j < width ; j++)
		{
			const local_16bits_t val =
				(local_16bits_t)((((local_16bits_t)srcBits[j*3+2] >> 3) << 11) |
						         (((local_16bits_t)srcBits[j*3+1] >> 2) << 5)  |
							                      (srcBits[j*3] >> 3)); // [Kafka]
			pDest[j] = val;
		}

		// modific scan line start
		startPtr -= bytesPitch;
	}

	return true;
}
//---------------------------------------------------------------------------

// Description : 
// Parameters  : 
// Returns     : 
// Comment     : 
// History: 
GPngLoad::~GPngLoad()
{
	if (Close() != true)
	{
		// do nothing
	}

	if (mInfo.lpdib) {
		GlobalFree(mInfo.lpdib);
	}
}
//---------------------------------------------------------------------------
#endif // PPT_PLATFORM_WIN
