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
//---------------------------------------------------------------------------
// History:
//    28-Jan-2000 [Kafka]
//      (i've "castrat" some implicit conversions to explicitomes (warnings))
//      (at "if" instructions i've put a block in place of one instruction)
//		(removed method Open (moved in base class))
//	  29-Jan-2000 [Kafka]
//      (replaced "delete buffer" with "delete [] buffer")
//      (declared int i at method begining not in for(;;))
//---------------------------------------------------------------------------
//#include <io.h>
#include <sys/stat.h>
//#include <fcntl.h>
#include <string.h>
#pragma hdrstop

#include "easylogging++.h"
#include "GBmpLoad.h"
#include "BitOp.h"

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

//#include <memory>
//using namespace std;

//---------------------------------------------------------------------------
#ifdef __BORLANDC__
#pragma package(smart_init)
#endif

// Descriere   : constructor
// Param       : n/a
// Rezultat    : n/a
// Comentarii  : 
// History     : 30-Jan-2000 [Kafka] initialization of member variables
GBmpLoad::GBmpLoad()
{
	memset(&fileHeader, 0x00, sizeof(TBmpFileHeader)); // [Kafka]
	memset(&infoHeader, 0x00, sizeof(TBmpInfoHeader)); // [Kafka]
}
//---------------------------------------------------------------------------
// Descriere   : incarca efectiv fisierul grafic intr'o suprafata (zona de memorie)
//               data de user
// Param       : data - zona de memorie, width - latime, height - inaltime, dataBpp - formatul zonei de memorie
// Rezultat    : true/false
// Comentarii  : metoda poate face conversii de color format
bool GBmpLoad::Load(unsigned char* data, int width, int height, TColorFormat dataBpp)
{
	// verificari
	if (!data) 
	{
		return false;
	}

	// nu se poate incarca in suprafete (char*) mai putin de 16 bpp
	switch(dataBpp)
	{
    case RGB_4 :
    case RGB_8 :
		return false;
	}
	
	// aleg metoda de incarcare
	switch(info.bpp)
	{
    case RGB_4 :
		return false;
		
    case RGB_8 :
		if (infoHeader.compressionType == 0) 
		{
			return Load8Bits(data, width, height, -1, dataBpp);
		}
		else 
		{
			return false;
		}
		
    case RGB_16 :
		return Load16Bits(data, width, height, -1, dataBpp);
		
    case RGB_24 :
		return Load24Bits(data, width, height, -1, dataBpp);
		
    default :
		// pe aici nu o sa se ajunga vreodata, dar sa fim siguri
		return false;
	}
}
//---------------------------------------------------------------------------

// Descriere   : incarca efectiv fisierul grafic intr'o suprafata (zona de memorie)
//               data de user
// Param       : data - zona de memorie, width - latime, height - inaltime, dataBpp - formatul zonei de memorie, bytesPitch - aliniere pe orizontala
// Rezultat    : true/false
// Comentarii  : metoda poate face conversii de color format
bool GBmpLoad::LoadAligned(unsigned char* data, int width, int height, int bytesPitch, TColorFormat dataBpp)
{
	// verificari
	if (!data) 
	{
		return false;
	}

	// nu se poate incarca in suprafete (char*) mai putin de 16 bpp
	switch(dataBpp)
	{
    case RGB_4 :
    case RGB_8 :
		return false;
	}
	
	// aleg metoda de incarcare
	switch(info.bpp)
	{
    case RGB_4 :
		return false;
		
    case RGB_8 :
		if (infoHeader.compressionType == 0) 
		{
			return Load8Bits(data, width, height, bytesPitch, dataBpp);
		}
		else 
		{
			return false;
		}
		
    case RGB_16 :
		return Load16Bits(data, width, height, bytesPitch, dataBpp);
		
    case RGB_24 :
		return Load24Bits(data, width, height, bytesPitch, dataBpp);
		
    default :
		// pe aici nu o sa se ajunga vreodata, dar sa fim siguri
		return false;
	}
}
//---------------------------------------------------------------------------

// Descriere   : incarca headerele fisierului .BMP, si verifica daca este un fisier valid
//               verifica daca id'ul e BMP_ID
// Param       : n/a
// Rezultat    : n/a
// Comentarii  : ar putea fi facuta mai stricta verificarea, dar nu cred ca are rost
// History: 29-Jan-2000 [Kafka] removed & at fileHeader.id (function read)
//          30-Jan-2000 [Kafka] intercepting return values of all "read" functions
bool GBmpLoad::ReadHeader()
{
	int nResult = -1;

	if (!rewind())
	{
		return false;
	}

	// din cauza align la word/quad

//#if !defined(WIN32) && !defined(_WIN32) && !defined(__WIN32__)
	// TMN: Reading one entry at the time was dog-slow

	// file header
	if ((nResult = read(fileHeader.id, 2*sizeof(char))) == -1)
	{
		return false;
	}

	if ((nResult = readInt(&fileHeader.fileSize)) == -1)
	{
		return false;
	}

	if ((nResult = readInt(&fileHeader.reserved)) == -1)
	{
		return false;
	}

	if ((nResult = readInt(&fileHeader.bmpFileOffset)) == -1)
	{
		return false;
	}


	// info header
	if ((nResult = readInt(&infoHeader.headerSize)) == -1)
	{
		return false;
	}

	if ((nResult = readInt(&infoHeader.imgWidth)) == -1)
	{
		return false;
	}

	if ((nResult = readInt(&infoHeader.imgHeight)) == -1)
	{
		return false;
	}

	if ((nResult = readShort(&infoHeader.planes)) == -1)
	{
		return false;
	}

	if ((nResult = readShort(&infoHeader.bpp)) == -1)
	{
		return false;
	}

	if ((nResult = readInt(&infoHeader.compressionType)) == -1)
	{
		return false;
	}

	if ((nResult = readInt(&infoHeader.imgSize)) == -1)
	{
		return false;
	}

	if ((nResult = readInt(&infoHeader.xPelsPerMeter)) == -1)
	{
		return false;
	}

	if ((nResult = readInt(&infoHeader.yPelsPerMeter)) == -1)
	{
		return false;
	}

	if ((nResult = readInt(&infoHeader.usedColors)) == -1)
	{
		return false;
	}

	if ((nResult = readInt(&infoHeader.importantColors)) == -1)
	{
		return false;
	}
	
	// headerul.id e ok ? (avem un bitmap)
	if (strncmp(BMP_ID, fileHeader.id, strlen(BMP_ID)) != 0) 
	{
		return false;
	}

/*
#else
	BITMAPFILEHEADER bfh;
	nResult = read(&bfh, sizeof(bfh));
	if (nResult == -1) {
		return false;
	}
	if (strncmp(reinterpret_cast<const char*>(&bfh.bfType), "BM", 2) != 0) {
		return false;
	}
	fileHeader.id[0]         = 'B';
	fileHeader.id[1]         = 'M';
	fileHeader.fileSize      = bfh.bfSize;
	fileHeader.bmpFileOffset = bfh.bfOffBits;
	BITMAPINFOHEADER bih;
	nResult = read(&bih, sizeof(bih));
	if (nResult == -1) {
		return false;
	}
	infoHeader.headerSize      = bih.biSize;
	infoHeader.imgWidth        = bih.biWidth;
	infoHeader.imgHeight       = bih.biHeight;
	infoHeader.planes          = bih.biPlanes;
	infoHeader.bpp             = bih.biBitCount;
	infoHeader.compressionType = bih.biCompression;
	infoHeader.imgSize         = bih.biSizeImage;
	infoHeader.xPelsPerMeter   = bih.biXPelsPerMeter;
	infoHeader.yPelsPerMeter   = bih.biYPelsPerMeter;
	infoHeader.usedColors      = bih.biClrUsed;
	infoHeader.importantColors = bih.biClrImportant;
#endif
*/
	
	// completez o structura TGInfo
	info.width  = infoHeader.imgWidth;
	info.height = infoHeader.imgHeight;
	switch(infoHeader.bpp)
	{
		case 4 :
		info.bpp = RGB_4;
		case 8 :
		info.bpp = RGB_8;
		break;
    case 16 :
		info.bpp = RGB_16;
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


// Descriere   : incarca un fisier BMP 256 culori intr'o suprafata cu formatul dat de dataBpp
// Param       : data - unde se incarca, pointer ce trebuie alocat de user;
//               width, height - dimensiunile suprafetei;
//               dataBpp - formatul de culoare al suprafetei
// Rezultat    : true/false
// Comentarii  : inca trebuie testata
// History     : 28-Jan-2000 [Kafka] added explicit conversion to eliminate warnings
bool GBmpLoad::Load8Bits (unsigned char* data, int width, int height, int bytesPitch, TColorFormat dataBpp)
{
	int              result, j, i; // [Kafka] 
	unsigned char*   buffer = NULL, *startPtr = NULL;
	unsigned __int8  tempPal[256][4];
	TRgb             pallete[256];
	unsigned __int16 refPal[256];
	int              skipSize = (width%4 == 0) ? 0 : 4 - width%4;
	
	if (!data) 
	{
		return false;
	}
	
	// nu suportam bitmap'uri 8 bit compresate
	if (infoHeader.compressionType != 0) 
	{
		return false;
	}
	
	// merg la inceputul paletei
	if (!seek(FILE_HEADER_SIZE + INFO_HEADER_SIZE)) return false;
	
	// citesc informatiile de paleta, 256 de culori QuadRgb
	if (read(tempPal, 1024) == -1) return false;
	for(i = 0; i < 256; i++)
	{
		pallete[i].blue   = tempPal[i][0];
		pallete[i].green  = tempPal[i][1];
		pallete[i].red    = tempPal[i][2];
	}
	
	// merg la inceputul datelor
	if (!seek(fileHeader.bmpFileOffset)) return false;
	
	// nu se poate incarca in suprafete (char*) mai putin de 16 bpp
	// pare in plus verificarea, pentru ca functia e interna si eu stiu
	// sa o apelez, dar...
	switch(dataBpp)
	{
    case RGB_4 :
    case RGB_8 :
		return false;
		
    case RGB_565 :
		// fac o paleta de referinta, pentru viteza
		for(i = 0; i < 256; i++)
		{
			refPal[i] =
				(unsigned __int16)
				(((pallete[i].red >> 3) << 11)  |
				((pallete[i].green >> 2) << 5) |
				(pallete[i].blue >> 3));
		}
		
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
		
		buffer = new unsigned char[width];
		if (!buffer) 
		{
			return false;
		}
		
		// conversiile intre suprafete sint mai lente
		for(i = 0; i < height; i++)
		{
			result = read(buffer, width);
			if (result == -1)
			{
				delete [] buffer;
				return false;
			}
			if (result == 0) break;
			
			// conversie 256 culori -> 565
			for(j = 0; j < width ; j++)
			{
				*((__int16*)startPtr + j) = refPal[buffer[j]];
			}
			
			// aliniere la 32-bit boundary
			if (!skip(skipSize))
			{
				delete [] buffer;
				return false;
			}
			
			// modific scan line start
			startPtr -= bytesPitch;
		}
		
		delete [] buffer;
		return true;
		
    case RGB_555 :
		
		// fac o paleta de referinta, pentru viteza
		for(i = 0; i < 256; i++)
		{
			refPal[i] =
				(unsigned __int16)
				(((pallete[i].red >> 3) << 10)  |
				((pallete[i].green >> 3) << 5) |
				(pallete[i].blue >> 3));
		}
		
		// pentru optimizare; avem 2 octeti/pixel in formatul 555
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
		//startPtr = data + (width << 1) * (height - 1);
		
		buffer = new unsigned char[width];
		if (!buffer) 
		{
			return false;
		}
		
		// conversiile intre suprafete sint mai lente
		for(i = 0; i < height; i++)
		{
			result = read(buffer, width);
			if (result == -1)
			{
				delete [] buffer;
				return false;
			}
			if (result == 0) break;
			
			// conversie 256 culori -> 555
			for(j = 0; j < width ; j++)
			{
				*((unsigned __int16*)startPtr + j) = refPal[buffer[j]];
			}
			
			// aliniere la 32-bit boundary
			if (!skip(skipSize))
			{
				delete [] buffer;
				return false;
			}
			
			// modific scan line start
			startPtr -= bytesPitch;
		}
		
		delete [] buffer;
		return true;
		
    case RGB_24 :
		// aliniere la un numar de octeti
		// pentru optimizare
		if (bytesPitch == -1)
		{
			startPtr   = data + (width * 3) * (height - 1);
			bytesPitch = (width * 3);
		}
		else
		{
			if (bytesPitch / 3 < width) 
			{
				return false;
			}
			startPtr = data + bytesPitch * (height - 1);
		}
		//startPtr = data + width * 3 * (height - 1);
		
		buffer = new unsigned char[width];
		if (!buffer) 
		{
			return false;
		}
		
		// conversiile intre suprafete sint mai lente
		for(i = 0; i < height; i++)
		{
			result = read(buffer, width);
			if (result == -1)
			{
				delete [] buffer;
				return false;
			}
			if (result == 0) 
			{
				break;
			}
			
			// conversie 256 culori -> 24
			for(j = 0; j < width ; j++)
			{
				memcpy(startPtr + j * 3, (char*)pallete + sizeof(TRgb) * buffer[j], 3);
			}
			
			// aliniere la 32-bit boundary
			if (!skip(skipSize))
			{
				delete [] buffer;
				return false;
			}
			
			// modific scan line start
			startPtr -= bytesPitch;
		}
		
		delete[]  buffer;
		return true;
		
    case RGB_32 :
		// pentru optimizare
		// aliniere la un numar de octeti
		if (bytesPitch == -1)
		{
			startPtr   = data + (width * 4) * (height - 1);
			bytesPitch = (width * 4);
		}
		else
		{
			if (bytesPitch / 4 < width) 
			{
				return false;
			}
			startPtr = data + bytesPitch * (height - 1);
		}
		//startPtr = data + (width << 2) * (height - 1);
		
		buffer = new unsigned char[width];
		if (!buffer) 
		{
			return false;
		}
		
		// conversiile intre suprafete sint mai lente
		for(i = 0; i < height; i++)
		{
			result = read(buffer, width);
			if (result == -1)
			{
				delete [] buffer;
				return false;
			}
			if (result == 0) 
			{
				break;
			}
			
			// conversie 256 culori -> 32
			for(j = 0; j < width ; j++)
			{
				memcpy(startPtr + (j << 2), (char*)(pallete + buffer[j]), sizeof(TRgb));
			}
			
			// aliniere la 32-bit boundary
			if (!skip(skipSize))
			{
				delete [] buffer;
				return false;
			}
			
			// modific scan line start
			startPtr -= bytesPitch;
		}
		
		delete [] buffer;
		return true;
		
    default :
		return false;
	}
}
//---------------------------------------------------------------------------


// Descriere   : incarca un fisier BMP 16bit color intr'o suprafata cu formatul dat de dataBpp
// Param       : data - unde se incarca, pointer ce trebuie alocat de user;
//               width, height - dimensiunile suprafetei;
//               dataBpp - formatul de culoare al suprafetei
// Rezultat    : true/false
// Comentarii  : inca trebuie testata
// History     : 28-Jan-2000 [Kafka] added explicit conversion to eliminate warnings
bool GBmpLoad::Load16Bits (unsigned char* data, int width, int height, int bytesPitch, TColorFormat dataBpp)
{
	int              result, j, i; // [Kafka]
	int              scanLineSize = width * 2;
	int              skipSize = (scanLineSize%4 == 0) ? 0 : 4 - scanLineSize%4;
	unsigned char*   buffer = NULL, *startPtr = NULL;
	unsigned __int32 redMask, greenMask, blueMask;
	unsigned __int8  redOffset, greenOffset, blueOffset;
	unsigned __int8  redMax, greenMax, blueMax;
	unsigned __int8  red, green, blue;
	
	if (!data) 
	{
		return false;
	}
	
	switch((TBmpShrink)infoHeader.compressionType)
	{
        case SHRINK_BITFIELDS :
		// citesc 3 DWORD, adica mastile celor 3 componente de culoare
		if (read(&redMask,   sizeof(__int32)) == -1) return false;
		if (read(&greenMask, sizeof(__int32)) == -1) return false;
		if (read(&blueMask,  sizeof(__int32)) == -1) return false;
		break;
		
	case SHRINK_NONE :
		redMask   = 0x7C00;
		greenMask = 0x03E0;
		blueMask  = 0x001F;
		break;
		
	default :
		return false;
	}
	
	// merg la inceputul bitmap'ului
	if (!seek(fileHeader.bmpFileOffset)) return false;
	
	// completez valorile necesare conversiilor intre formate de culoare
	redMax   = CountBits32(redMask);
	greenMax = CountBits32(greenMask);
	blueMax  = CountBits32(blueMask);
	
	redMax   = (unsigned __int8)(1 << redMax); // [Kafka]
	greenMax = (unsigned __int8)(1 << greenMax); // [Kafka]
	blueMax  = (unsigned __int8)(1 << blueMax);  // [Kafka]
	
	redOffset   = MaskOffset32(redMask);
	greenOffset = MaskOffset32(greenMask);
	blueOffset  = MaskOffset32(blueMask);
	
	// nu se poate incarca in suprafete (char*) mai putin de 16 bpp
	// pare in plus verificarea, pentru ca functia e interna si eu stiu
	// sa o apelez, dar...
	switch(dataBpp)
	{
    case RGB_4 :
    case RGB_8 :
		return false;
		
    case RGB_565 :
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
		//startPtr = data + (width << 1) * (height - 1);
		
		buffer = new unsigned char[scanLineSize];
		if (!buffer) 
		{
			return false;
		}
		
		// conversiile intre suprafete sint mai lente
		for(i = 0; i < height; i++)
		{
			result = read(buffer, scanLineSize);
			if (result == -1)
			{
				delete [] buffer;
				return false;
			}
			if (result == 0) 
			{
				break;
			}
			
			// conversie generic 16 bit -> 565
			for(j = 0; j < width ; j++)
			{
				red   = (unsigned __int8)((*((__int16*)buffer + j) & redMask) >> redOffset);  // [Kafka]
				green = (unsigned __int8)((*((__int16*)buffer + j) & greenMask) >> greenOffset); // [Kafka]
				blue  = (unsigned __int8)((*((__int16*)buffer + j) & blueMask) >> blueOffset); // [Kafka]
				red   = (unsigned __int8)(32 * red   / redMax);   // [Kafka]
				green = (unsigned __int8)(64 * green / greenMax); // [Kafka]
				blue  = (unsigned __int8)(32 * blue  / blueMax);  // [Kafka]
				*((__int16*)startPtr + j) = (__int16)((red << 11) | (green << 5) | (blue)); // [Kafka]
			}
			
			// aliniere la 32-bit boundary
			if (!skip(skipSize))
			{
				delete [] buffer;
				return false;
			}
			
			// modific scan line start
			startPtr -= bytesPitch;
		}
		
		delete [] buffer;
		return true;
		
    case RGB_555 :
		// pentru optimizare; avem 2 octeti/pixel in formatul 555
		// aliniere la un numar de octeti
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
		//startPtr = data + (width << 1) * (height - 1);
		
		buffer = new unsigned char[scanLineSize];
		if (!buffer) 
		{
			return false;
		}
		
		// conversiile intre suprafete sint mai lente
		for(i = 0; i < height; i++)
		{
			result = read(buffer, scanLineSize);
			if (result == -1)
			{
				delete [] buffer;
				return false;
			}
			if (result == 0) 
			{
				break;
			}
			
			// conversie 16 bit generic -> 555
			for(j = 0; j < width ; j++)
			{
				red   = (unsigned __int8)((*((__int16*)buffer + j) & redMask) >> redOffset);     // [Kafka]
				green = (unsigned __int8)((*((__int16*)buffer + j) & greenMask) >> greenOffset); // [Kafka]
				blue  = (unsigned __int8)((*((__int16*)buffer + j) & blueMask)  >> blueOffset);  // [Kafka]
				red   = (unsigned __int8)(32 * red   / redMax);                                  // [Kafka]
				green = (unsigned __int8)(32 * green / greenMax);                                // [Kafka]
				blue  = (unsigned __int8)(32 * blue  / blueMax);                                 // [Kafka]
				*((__int16*)startPtr + j) = (__int16)((red << 10) | (green << 5) | (blue));      // [Kafka]
			}
			
			// aliniere la 32-bit boundary
			if (!skip(skipSize))
			{
				delete [] buffer;
				return false;
			}
			
			// modific scan line start
			startPtr -= bytesPitch;
		}
		
		delete [] buffer;
		return true;
		
    case RGB_24 :
		// aliniere la un numar de octeti
		// pentru optimizare
		if (bytesPitch == -1)
		{
			startPtr   = data + (width * 3) * (height - 1);
			bytesPitch = (width * 3);
		}
		else
		{
			if (bytesPitch / 3 < width) 
			{
				return false;
			}
			startPtr = data + bytesPitch * (height - 1);
		}
		//startPtr = data + width * 3 * (height - 1);
		
		buffer = new unsigned char[scanLineSize];
		if (!buffer) 
		{
			return false;
		}
		
		// conversiile intre suprafete sint mai lente
		for(i = 0; i < height; i++)
		{
			result = read(buffer, scanLineSize);
			if (result == -1)
			{
				delete [] buffer;
				return false;
			}
			if (result == 0) 
			{
				break;
			}
			
			// conversie 16 bit generic -> 24
			for(j = 0; j < width ; j++)
			{
				red   = (unsigned __int8)((*((__int16*)buffer + j) & redMask)   >> redOffset);  // [Kafka]
				green = (unsigned __int8)((*((__int16*)buffer + j) & greenMask) >> greenOffset);// [Kafka]
				blue  = (unsigned __int8)((*((__int16*)buffer + j) & blueMask)  >> blueOffset); // [Kafka]
				red   = (unsigned __int8)(256 * red   / redMax);                                // [Kafka]
				green = (unsigned __int8)(256 * green / greenMax);                              // [Kafka]
				blue  = (unsigned __int8)(256 * blue  / blueMax);                               // [Kafka]
				*(startPtr + j * 3 + 2) = red;
				*(startPtr + j * 3 + 1) = green;
				*(startPtr + j * 3)     = blue;
			}
			
			// aliniere la 32-bit boundary
			if (!skip(skipSize))
			{
				delete [] buffer;
				return false;
			}
			
			// modific scan line start
			startPtr -= bytesPitch;
		}
		
		delete [] buffer;
		return true;
		
    case RGB_32 :
		// aliniere la un numar de octeti
		// pentru optimizare
		if (bytesPitch == -1)
		{
			startPtr   = data + (width * 4) * (height - 1);
			bytesPitch = (width * 4);
		}
		else
		{
			if (bytesPitch / 4 < width) 
			{
				return false;
			}
			startPtr = data + bytesPitch * (height - 1);
		}
		//startPtr = data + (width << 2) * (height - 1);
		
		buffer = new unsigned char[scanLineSize];
		if (!buffer) 
		{
			return false;
		}
		
		// conversiile intre suprafete sint mai lente
		for(i = 0; i < height; i++)
		{
			result = read(buffer, scanLineSize);
			if (result == -1)
			{
				delete [] buffer;
				return false;
			}
			if (result == 0)
			{
				break;
			}
			
			// conversie 16 bit -> 32
			for(j = 0; j < width ; j++)
			{
				red   = (unsigned __int8)((*((__int16*)buffer + j) & redMask)   >> redOffset);  // [Kafka]
				green = (unsigned __int8)((*((__int16*)buffer + j) & greenMask) >> greenOffset);// [Kafka]
				blue  = (unsigned __int8)((*((__int16*)buffer + j) & blueMask)  >> blueOffset); // [Kafka]
				red   = (unsigned __int8)(256 * red   / redMax);                                // [Kafka]
				green = (unsigned __int8)(256 * green / greenMax);                              // [Kafka]
				blue  = (unsigned __int8)(256 * blue  / blueMax);                               // [Kafka]
				*(startPtr + j * 4 + 3) = 0;
				*(startPtr + j * 4 + 2) = red;
				*(startPtr + j * 4 + 1) = green;
				*(startPtr + j * 4)     = blue;
				//*((unsigned __int32*)startPtr + j) = (unsigned __int32)red << 16 | (unsigned __int32)green<<8 | (unsigned __int32)blue;
			}
			
			// aliniere la 32-bit boundary
			if (!skip(skipSize))
			{
				delete [] buffer;
				return false;
			}
			
			// modific scan line start
			startPtr -= bytesPitch;
		}
		
		delete [] buffer;
		return true;
		
    default :
		return false;
	}
  //  return false;
}
//---------------------------------------------------------------------------


// Descriere   : incarca un fisier BMP 24bit color intr'o suprafata cu formatul dat de dataBpp
// Param       : data - unde se incarca, pointer ce trebuie alocat de user;
//               width, height - dimensiunile suprafetei;
//               dataBpp - formatul de culoare al suprafetei
// Rezultat    : true/false
// Comentarii  : inca trebuie testata
//               scanLineSize - dimensiunea unei linii din imagine, adica width * 3, fiind 24 biti
//               skipSize - se adauga zero la o linie a.i. sa se termine la adresa multiplu de 32
// History     : 28-Jan-2000 [Kafka] added explicit conversion to eliminate warnings
bool GBmpLoad::Load24Bits (unsigned char* data, int width, int height, int bytesPitch, TColorFormat dataBpp)
{
	int            result, j, i;
	int            scanLineSize = width * 3;
	int            skipSize = (scanLineSize%4 == 0) ? 0 : 4 - scanLineSize%4;
	unsigned char* buffer = NULL, *startPtr = NULL;
	
	if (!data) 
	{
		return false;
	}
	
	if (!seek(fileHeader.bmpFileOffset)) return false;
	
	// nu se poate incarca in suprafete (char*) mai putin de 16 bpp
	// pare in plus verificarea, pentru ca functia e interna si eu stiu
	// sa o apelez, dar...
	switch(dataBpp)
	{
    case RGB_4 :
    case RGB_8 :
		return false;
		
    case RGB_565 :
		// pentru optimizare; avem 2 octeti/pixel in formatul 565
		// aliniere la un numar de octeti
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
		
		buffer = new unsigned char[scanLineSize];
		if (!buffer) return false;
		
		// conversiile intre suprafete sint mai lente
		for(i = 0; i < height; i++)
		{
			result = read(buffer, scanLineSize);
			if (result == -1)
			{
				delete [] buffer;
				return false;
			}
			if (result == 0) 
			{
				break;
			}
			
			// conversie 24 -> 565
			for(j = 0; j < width ; j++)
			{
				*((__int16*)startPtr + j) =
					(__int16)((((__int16)buffer[j*3+2] >> 3) << 11) |
					(((__int16)buffer[j*3+1] >> 2) << 5)  |
					(buffer[j*3] >> 3)); // [Kafka]
			}
			
			// aliniere la 32-bit boundary
			if (!skip(skipSize))
			{
				delete [] buffer;
				return false;
			}
			
			// modific scan line start
			startPtr -= bytesPitch;
		}
		
		delete [] buffer;
		return true;
		
    case RGB_555 :
		// pentru optimizare; avem 2 octeti/pixel in formatul 555
		// aliniere la un numar de octeti
		if (bytesPitch == -1)
		{
			startPtr   = data + (width << 1) * (height - 1);
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
		//startPtr = data + (width << 1) * (height - 1);
		
		buffer = new unsigned char[scanLineSize];
		if (!buffer) 
		{
			return false;
		}
		
		// conversiile intre suprafete sint mai lente
		for(i = 0; i < height; i++)
		{
			result = read(buffer, scanLineSize);
			if (result == -1)
			{
				delete [] buffer;
				return false;
			}
			if (result == 0) 
			{
				break;
			}
			
			// conversie 24 -> 555
			for(j = 0; j < width ; j++)
			{
				*((__int16*)startPtr + j) =
					(__int16)((((__int16)buffer[j*3+2] >> 3) << 10) |
					(((__int16)buffer[j*3+1] >> 3) << 5)  |
					(buffer[j*3] >> 3)); // [Kafka] 
			}
			
			// aliniere la 32-bit boundary
			if (!skip(skipSize))
			{
				delete [] buffer;
				return false;
			}
			
			// modific scan line start
			startPtr -= bytesPitch;
		}
		
		delete [] buffer;
		return true;
		
    case RGB_24 :
		// aliniere la un numar de octeti
		// pentru optimizare
		if (bytesPitch == -1)
		{
			startPtr   = data + (width * 3) * (height - 1);
			bytesPitch = (width * 3);
		}
		else
		{
			if (bytesPitch / 3 < width) 
			{
				return false;
			}
			startPtr = data + bytesPitch * (height - 1);
		}
		//startPtr = data + scanLineSize * (height - 1);
		
		// nu imi trebuie buffer
		for(i = 0; i < height; i++)
		{
			result = read(startPtr, scanLineSize);
			if (result == -1) 
			{
				return false;
			}
			if (result == 0) 
			{
				break;
			}
			
			// aliniere la 32-bit boundary
			if (!skip(skipSize)) return false;
			
			// modific scan line start
			startPtr -= bytesPitch;
		}
		
		return true;
		
    case RGB_32 :
		// pentru optimizare
		// aliniere la un numar de octeti
		// pentru optimizare
		if (bytesPitch == -1)
		{
			startPtr   = data + (width * 4) * (height - 1);
			bytesPitch = (width * 4);
		}
		else
		{
			if (bytesPitch / 4 < width) 
			{
				return false;
			}
			startPtr = data + bytesPitch * (height - 1);
		}
		//startPtr = data + (width << 2) * (height - 1);
		
		buffer = new unsigned char[scanLineSize];
		if (!buffer) 
		{
			return false;
		}
		
		// conversiile intre suprafete sint mai lente
		for(i = 0; i < height; i++)
		{
			result = read(buffer, scanLineSize);
			if (result == -1)
			{
				delete [] buffer;
				return false;
			}
			if (result == 0) 
			{
				break;
			}
			
			// conversie 24 -> 32
			for(j = 0; j < width ; j++)
			{
				memcpy(startPtr + (j << 2), buffer + j * 3, 3);
			}
			
			// aliniere la 32-bit boundary
			if (!skip(skipSize))
			{
				delete [] buffer;
				return false;
			}
			
			// modific scan line start
			startPtr -= bytesPitch;
		}
		
		delete [] buffer;
		return true;
		
    default :
		return false;
	}
}
//---------------------------------------------------------------------------

// Descriere   : destructor
// Param       : n/a
// Rezultat    : n/a
// Comentarii  : eventual ceva dezalocari de memorie, daca bufferul va fi pointer
GBmpLoad::~GBmpLoad()
{

	if (Close() != true)
	{
		// do nothing
	}
}
//---------------------------------------------------------------------------

bool GBmpSave::WriteHeaders()
{
  //if (!seek(0, SEEK_SET))
  //{
  //  return false;
  //}

  // din cauza align la word/quad

  // file header
  if (!write(mFileHeader.id, 2*sizeof(char)))
  {
    return false;
  }

  if (!write(&mFileHeader.fileSize, sizeof(__int32)))
  {
    return false;
  }

  if (!write(&mFileHeader.reserved, sizeof(__int32)))
  {
    return false;
  }

  if (!write(&mFileHeader.bmpFileOffset, sizeof(__int32)))
  {
    return false;
  }


  // info header
  if (!write(&mInfoHeader.headerSize, 4))
  {
    return false;
  }

  if (!write(&mInfoHeader.imgWidth,          4))
  {
          return false;
  }

  if (!write(&mInfoHeader.imgHeight,         4))
  {
          return false;
  }

  if (!write(&mInfoHeader.planes,            2))
  {
          return false;
  }

  if (!write(&mInfoHeader.bpp,               2))
  {
          return false;
  }

  if (!write(&mInfoHeader.compressionType,   4))
  {
          return false;
  }

  if (!write(&mInfoHeader.imgSize,           4))
  {
          return false;
  }

  if (!write(&mInfoHeader.xPelsPerMeter,     4))
  {
          return false;
  }

  if (!write(&mInfoHeader.yPelsPerMeter,     4))
  {
          return false;
  }

  if (!write(&mInfoHeader.usedColors,        4))
  {
          return false;
  }

  if (!write(&mInfoHeader.importantColors,   4))
  {
          return false;
  }


  return true;
}
//---------------------------------------------------------------------------

// Description : saves memory data to a file
// Params      : filename - file name, will be overwritten
//               data, width, height, dataBpp - parameters of memory surface to be dumped
// Result      : true/false if succeded
// Comments    :
bool GBmpSave::Save(unsigned char* data, int width, int height, TColorFormat dataBpp, TColorFormat fileBpp)
{
  return SaveAligned(data, width, height, width, dataBpp, fileBpp);
}
//---------------------------------------------------------------------------

// Description : saves memory aligned data to a file
// Params      : filename - file name, will be overwritten
//               data, width, height, dataBpp - parameters of memory surface to be dumped
// Result      : true/false if succeded
// Comments    :
bool GBmpSave::SaveAligned(unsigned char* data, int width, int height, int bytesPitch, TColorFormat dataBpp, TColorFormat fileBpp)
{
  int               x, y;
  int               lScanLineSize = width * 3;
  int               lBufferSize   = lScanLineSize + ((lScanLineSize%4 == 0) ? 0 : (4 - lScanLineSize%4));
  //auto_ptr<__int16> lBuffer(new __int16[lBufferSize]); // bleah?
  __int8*           lBuffer;
  __int16           lColor16;

  //mHandle = open(_fileName, O_CREAT |O_TRUNC | O_BINARY | O_RDWR, S_IREAD | S_IWRITE);

  // fill headers
  strcpy(mFileHeader.id, BMP_ID);
  mFileHeader.reserved = 0;
  mFileHeader.bmpFileOffset = FILE_HEADER_SIZE + INFO_HEADER_SIZE;
  mFileHeader.fileSize = FILE_HEADER_SIZE + INFO_HEADER_SIZE + lBufferSize * height;

  // essential header informations
  mInfoHeader.headerSize      = INFO_HEADER_SIZE;
  mInfoHeader.imgWidth        = width;
  mInfoHeader.imgHeight       = height;
  mInfoHeader.planes          = 0;
  mInfoHeader.compressionType = SHRINK_NONE;
  mInfoHeader.bpp             = 24;

  if (!WriteHeaders())
  {
    return false;
  }

  // get some buffer memory
  try
  {
    lBuffer = new __int8[lBufferSize];
  }
  catch(...)
  {
    VLOG(9) << "GBmpSave::SaveAligned - cannot get buffer memory.";
    return false;
  }

  // write data
  switch(dataBpp)
  {
    case RGB_565:
      //logWriteLn("Writing as 565...");
      //logWrite("Data size: ("); logWriteI(width); logWrite(","); logWriteI(height); logWrite(","); logWriteI(bytesPitch); logWriteLn(")");
      //logWrite("Working with a buffer of size: "); logWriteILn(lBufferSize);
      for(y = height - 1; y > 0; y--)
      {
        for(x = 0; x < width; x++)
        {
          lColor16 = *((__int16*)(data + y * bytesPitch + x * 2));

          lBuffer[x * 3 + 2] = (__int8)(((lColor16 & 0xF800) >> 11) * 8);
          lBuffer[x * 3 + 1] = (__int8)(((lColor16 & 0x07E0) >> 5) * 4);
          lBuffer[x * 3 + 0] = (__int8)((lColor16 & 0x001F) * 8);
        }

        // dump to file
        if (write(lBuffer, lBufferSize) == -1)
        {
		  VLOG(9) << "GBmpSave::SaveAligned - cannot write to .bmp file.";
          delete [] lBuffer;
          return false;
        }
      }

    break;

    case RGB_555:
      for(y = height - 1; y > 0; y--)
      {
        for(x = 0; x < width; x++)
        {
          lColor16 = *((__int16*)(data + y * bytesPitch + x * 2));

          lBuffer[x * 3 + 2] = (__int8)(((lColor16 & 0x7C00) >> 11) * 8);
          lBuffer[x * 3 + 1] = (__int8)(((lColor16 & 0x03E0) >> 5) * 4);
          lBuffer[x * 3 + 0] = (__int8)((lColor16 & 0x001F) * 8);
        }

        // dump to file
        if (write(lBuffer, lBufferSize) == -1)
        {
		  VLOG(9) << "GBmpSave::SaveAligned - cannot write to .bmp file.";
          delete [] lBuffer;
          return false;
        }
      }
      break;

    case RGB_24:
      break;

    default:
      return false;
  }

  delete [] lBuffer;
  return true;
}
//---------------------------------------------------------------------------


