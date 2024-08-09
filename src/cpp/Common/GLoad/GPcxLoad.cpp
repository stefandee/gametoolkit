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
//      (commented unreferenced parameters names (eliminate warnings))
//	  30-Jan-2000 [Kafka]
//		(making right deallocation for buffer [delete [] buffer])
//---------------------------------------------------------------------------
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#pragma hdrstop

#include "Platform.h"
#include "GPcxLoad.h"
#ifdef PPT_PLATFORM_WIN
#include <io.h>
#endif
//---------------------------------------------------------------------------

#ifdef __BORLANDC__
#pragma package(smart_init)
#endif

// Descriere   : constructor
// Param       : n/a
// Rezultat    : n/a
// Comentarii  : initializeaza handle'ul de fisier cu -1
// History     : 30-Jan-2000 [Kafka] initialization of "header" member variables
GPcxLoad::GPcxLoad()
{
	memset(&header, 0x00, sizeof(TPcxFileHeader)); // [Kafka] set member to 0
}

//---------------------------------------------------------------------------

// Descriere   : incarca efectiv fisierul grafic intr'o suprafata (zona de memorie)
//               data de user
// Param       : data - zona de memorie, width - latime, height - inaltime, dataBpp - formatul zonei de memorie
// Rezultat    : true/false
// Comentarii  : metoda poate face conversii de color format
bool GPcxLoad::Load(unsigned char* data, int width, int height, TColorFormat dataBpp)
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

  // aleg metoda de incarcare; exista numai 2 tipuri de PCX'uri
  switch(info.bpp)
  {
    case RGB_8 :
      return Load8Bits(data, width, height, -1, dataBpp);

    case RGB_24 :
      return Load24Bits(data, width, height, -1, dataBpp);

    default :
      return false;
  }
}
//---------------------------------------------------------------------------

// Descriere   : incarca efectiv fisierul grafic intr'o suprafata (zona de memorie)
//               data de user
// Param       : data - zona de memorie, width - latime, height - inaltime, dataBpp - formatul zonei de memorie, bytesPitch - width'ul real (diferit de cel al imaginii, pentru aliniament)
// Rezultat    : true/false
// Comentarii  : metoda poate face conversii de color format
bool GPcxLoad::LoadAligned(unsigned char* data, int width, int height, int bytesPitch, TColorFormat dataBpp)
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

  // aleg metoda de incarcare; exista numai 2 tipuri de PCX'uri
  switch(info.bpp)
  {
    case RGB_8 :
      return Load8Bits(data, width, height, bytesPitch, dataBpp);

    case RGB_24 :
      return Load24Bits(data, width, height, bytesPitch, dataBpp);

    default :
      return false;
  }
}
//---------------------------------------------------------------------------

// Descriere   : incarca headerul fisierului si verifica daca este un fisier valid
// Param       : n/a
// Rezultat    : n/a
// Comentarii  : din cauza alinierii datelor la altceva decit la byte, trebuie
//               explicit citite din fisier cimpurile headerului
// History     : 30-Jan-2000 [Kafka] removed "&" from header.colorMap in function read
bool GPcxLoad::ReadHeader()
{
	//int result;
	
	if (!rewind())
	{
          return false;
	}
	/*
	result = read(fileHandle, &header, sizeof(TPcxFileHeader));
	// headerele nu pot sa termine fisierul, e invalid
	if (result <= 0) return false;
	*/

	// din cauza la dword align
	if (!read(&header.manufacturer, sizeof(__int8))) return false;
	if (!read(&header.version,      sizeof(__int8))) return false;
	if (!read(&header.encoding,     sizeof(__int8))) return false;
	if (!read(&header.bitsPerPixel, sizeof(__int8))) return false;
	if (!read(&header.xMin,         sizeof(__int16))) return false;
	if (!read(&header.yMin,         sizeof(__int16))) return false;
	if (!read(&header.xMax,         sizeof(__int16))) return false;
	if (!read(&header.yMax,         sizeof(__int16))) return false;
	if (!read(&header.hDpi,         sizeof(__int16))) return false;
	if (!read(&header.vDpi,         sizeof(__int16))) return false;
	if (!read(header.colorMap,     48 * sizeof(__int8))) return false; // [Kafka]
	if (!read(&header.reserved,     sizeof(__int8))) return false;
	if (!read(&header.nPlanes,      sizeof(__int8))) return false;
	if (!read(&header.bytesPerLine, sizeof(__int16))) return false;
	if (!read(&header.palleteInfo,  sizeof(__int16))) return false;
	if (!read(&header.hScreenSize,  sizeof(__int16))) return false;
	if (!read(&header.vScreenSize,  sizeof(__int16))) return false;

	// headerul.manufacturer e ok ? (avem un bitmap)
	if (PCX_ID != header.manufacturer)
	{
		return false;
	}

	// versiunea e mai buna de 3.0 ?
	if (header.version < 5)
	{
		return false;
	}

	// bits per pixel e 8 (adica 256 de culori sau 24 bits) ?
	if (header.bitsPerPixel != 8)
	{
		return false;
	}

	// compresia e RLE ?
	if (header.encoding != PCX_ENCODING)
	{
		return false;
	}

	// completez o structura TGInfo
	info.width  = header.xMax - header.xMin + 1;
	info.height = header.yMax - header.yMin + 1;
	switch(header.nPlanes)
	{
    case 1 :
		info.bpp = RGB_8;
		break;
    case 3 :
		info.bpp = RGB_24;
		break;
    default :
		return false;
	}

	return true;
}
//---------------------------------------------------------------------------

// Descriere   : incarca un fisier PCX 256 culori intr'o suprafata cu formatul dat de dataBpp
// Param       : data - unde se incarca, pointer ce trebuie alocat de user;
//               width, height - dimensiunile suprafetei;
//               dataBpp - formatul de culoare al suprafetei
// Rezultat    : true/false
// Comentarii  : inca trebuie testata
// History     : 28-Jan-2000 [Kafka] added explicit conversion to eliminate warnings
//                                   commented out unused default variable value for bufferIndex
//				 30-Jan-2000 [Kafka] initialize pointers with NULL 
//                                   (so that NULL test after allocation to work)
bool GPcxLoad::Load8Bits(unsigned char* data, int width, int /*height*/, int bytesPitch, TColorFormat dataBpp)
{
	int              i, crtLine, decodedBytes, repeatCount;
	int              fileSize, compressSize, bufferSize, bufferIndex /*= 0*/, compressRead, totalBytes; // [Kafka]
	unsigned char*   buffer=NULL;
	unsigned char*   scanLine=NULL;
	TPcxRgb          pallete[256];
	unsigned __int16 refPal16[256], color16;
	unsigned __int32 refPal32[256], color32;
	unsigned __int8  palleteId, data1, color8;
	
	if (!data) 
	{
          return false;
	}
	
	// caut paleta
	fileSize = length();

	if (!seek(fileSize - (signed)(256 * sizeof(TPcxRgb) + 1)))
	{
          return false;
	}
	
	// vad daca e valida
	if (!read(&palleteId, 1))
	{
		return false;
	}

	if (palleteId != PCX_PALLETE_ID)
	{
		return false;
	}

	// citesc paleta efectiv
	if (!read(pallete, 256 * sizeof(TPcxRgb))) return false;

	// merg la inceputul datelor
	if (!seek(sizeof(TPcxFileHeader))) return false;

	// numarul de octeti pe o linie
	totalBytes = header.nPlanes * header.bytesPerLine;

	// aloc memorie pentru bufferul de citire din fisier
	compressSize = fileSize - 128 - 769;
	if (compressSize > PCX_BUFFER_SIZE)
	{
		bufferSize = PCX_BUFFER_SIZE;
	}
	else
	{
		bufferSize = compressSize;
	}

	buffer = new unsigned char[bufferSize];

	if (!buffer) 
	{
		return false;
	}
	
	// nu se poate incarca in suprafete (char*) mai putin de 16 bpp
	switch(dataBpp)
	{
    case RGB_555 :
    case RGB_565 :
		// pentru optimizare
		// aliniere la un numar de octeti
		if (bytesPitch == -1)
		{
			bytesPitch = (width * 2);
		}
		else
		{
			if (bytesPitch / 2 < width) 
			{
				delete [] buffer;
				return false;
			}
		}
		
		// fac o paleta de referinta, pentru viteza
		// in acest caz (imagine paletizata), cele 2 user surfaces se trateaza la fel din punct de
		// vedere al decodarii
		if (dataBpp == RGB_565)
		{
			for(i = 0; i < 256; i++)
			{
				refPal16[i] =
					(unsigned __int16)
					(((pallete[i].red >> 3) << 11)  |
					((pallete[i].green >> 2) << 5) |
					(pallete[i].blue >> 3)); //[Kafka]
			}
		}
		else
		{
			for(i = 0; i < 256; i++)
			{
				refPal16[i] =
					(unsigned __int16)
					(((pallete[i].red >> 3) << 10)  |
					((pallete[i].green >> 3) << 5) |
					(pallete[i].blue >> 3)); // [Kafka]
			}
		}
		// aloc scanline'ul
		scanLine = new unsigned char[2 * totalBytes];
		if (!scanLine)
		{
			delete [] buffer;
			return false;
		}
		
		// citesc in buffer
		bufferIndex = -1;
		compressRead = bufferSize;
		if (!read(buffer, bufferSize))
		{
			delete [] buffer;
			delete [] scanLine;
			return false;
		}
		
		// nu e optim, se fac 2 scrieri in memorie (una in scanLine, care se transfera apoi in data)
		// in plus, se poate face si mai elegant, scriind for/while o singura data si rescriind doar functiile de conversie
		for(crtLine = 0; crtLine < info.height; crtLine++)
		{
			decodedBytes = 0;
			while (decodedBytes < totalBytes)
			{
				bufferIndex++;
				// overflow in buffer ?
				if (bufferIndex >= bufferSize)
				{
					bufferIndex = 0;
					if (compressSize - compressRead < bufferSize)
					{
						bufferSize = compressSize - compressRead;
					}

					if (!read(buffer, bufferSize))
					{
						delete [] buffer;
						delete [] scanLine;
						return false;
					}
					compressRead += bufferSize;
				}
				
				data1 = buffer[bufferIndex];
				
				if ((data1 & 0xC0) == 0xC0)
				{
					bufferIndex++;
					// overflow in buffer ?
					if (bufferIndex >= bufferSize)
					{
						bufferIndex = 0;
						if (compressSize - compressRead < bufferSize)
						{
							bufferSize = compressSize - compressRead;
						}

						if (!read(buffer, bufferSize))
						{
							delete [] buffer;
							delete [] scanLine;
							return false;
						}
						compressRead += bufferSize;
					}
					
					repeatCount = (data1 & 0x3F);
					color16 = refPal16[buffer[bufferIndex]];
				}
				else
				{
					repeatCount = 1;
					color16 = refPal16[data1];
				}
				
				for(i = 0; i < repeatCount; i++)
				{
					*(((unsigned __int16*)scanLine + decodedBytes + i)) = color16;
				}
				decodedBytes += repeatCount;
			}
			
			// nu e optim, se fac 2 scrieri in memorie
			//memcpy((unsigned __int16*)data + crtLine * info.width, scanLine, info.width * 2);
			memcpy(data + crtLine * bytesPitch, scanLine, info.width * 2);
		}		
		break;
		
    case RGB_24  :
		// pentru optimizare
		// aliniere la un numar de octeti
		if (bytesPitch == -1)
		{
			bytesPitch = (width * 3);
		}
		else
		{
			if (bytesPitch / 3 < width) 
			{
				delete [] buffer;
				return false;
			}
		}
		
		// aloc scanline'ul
		scanLine = new unsigned char[3 * totalBytes];
		if (!scanLine)
		{
			delete [] buffer;
			return false;
		}
		
		// citesc in buffer
		bufferIndex = -1;
		compressRead = bufferSize;
		if (!read(buffer, bufferSize))
		{
			delete [] buffer;
			delete [] scanLine;
			return false;
		}
		
		// nu e optim, se fac 2 scrieri in memorie (una in scanLine, care se transfera apoi in data)
		// in plus, se poate face si mai elegant, scriind for/while o singura data si rescriind doar functiile de conversie
		for(crtLine = 0; crtLine < info.height; crtLine++)
		{
			decodedBytes = 0;
			while (decodedBytes < totalBytes)
			{
				bufferIndex++;
				// overflow in buffer ?
				if (bufferIndex >= bufferSize)
				{
					bufferIndex = 0;
					if (compressSize - compressRead < bufferSize) 
					{
						bufferSize = compressSize - compressRead;
					}
					if (!read(buffer, bufferSize))
					{
						delete [] buffer;
						delete [] scanLine;
						return false;
					}
					compressRead += bufferSize;
				}
				
				data1 = buffer[bufferIndex];
				
				if ((data1 & 0xC0) == 0xC0)
				{
					bufferIndex++;
					// overflow in buffer ?
					if (bufferIndex >= bufferSize)
					{
						bufferIndex = 0;
						if (compressSize - compressRead < bufferSize) 
						{
							bufferSize = compressSize - compressRead;
						}
						if (!read(buffer, bufferSize))
						{
							delete [] buffer;
							delete [] scanLine;
							return false;
						}
						compressRead += bufferSize;
					}
					
					repeatCount = (data1 & 0x3F);
					color8 = buffer[bufferIndex];
				}
				else
				{
					repeatCount = 1;
					color8 = data1;
				}
				
				for(i = 0; i < repeatCount; i++)
				{
					*(scanLine + (decodedBytes + i) * 3)     = pallete[color8].blue;
					*(scanLine + (decodedBytes + i) * 3 + 1) = pallete[color8].green;
					*(scanLine + (decodedBytes + i) * 3 + 2) = pallete[color8].red;
				}
				decodedBytes += repeatCount;
			}
			
			// nu e optim, se fac 2 scrieri in memorie
			// memcpy(data + crtLine * info.width * 3, scanLine, info.width * 3);
			memcpy(data + crtLine * bytesPitch, scanLine, info.width * 3);
		}
		break;
		
    case RGB_32  :
		// pentru optimizare
		// aliniere la un numar de octeti
		if (bytesPitch == -1)
		{
			bytesPitch = (width * 4);
		}
		else
		{
			if (bytesPitch / 4 < width) 
			{
				delete [] buffer;
				return false;
			}
		}
		
		// clasica paleta de referinta
		for(i = 0; i < 256; i++)
		{
			refPal32[i] = (((pallete[i].red << 8) | pallete[i].green) << 8) | pallete[i].blue;
		}
		
		// aloc scanline'ul
		scanLine = new unsigned char[4 * totalBytes];
		if (!scanLine)
		{
			delete [] buffer;
			return false;
		}
		
		// citesc in buffer
		bufferIndex = -1;
		compressRead = bufferSize;
		if (!read(buffer, bufferSize))
		{
			delete [] buffer;
			delete [] scanLine;
			return false;
		}
		
		// nu e optim, se fac 2 scrieri in memorie (una in scanLine, care se transfera apoi in data)
		// in plus, se poate face si mai elegant, scriind for/while o singura data si rescriind doar functiile de conversie
		for(crtLine = 0; crtLine < info.height; crtLine++)
		{
			decodedBytes = 0;
			while (decodedBytes < totalBytes)
			{
				bufferIndex++;
				// overflow in buffer ?
				if (bufferIndex >= bufferSize)
				{
					bufferIndex = 0;
					if (compressSize - compressRead < bufferSize) 
					{
						bufferSize = compressSize - compressRead;
					}
					if (!read(buffer, bufferSize))
					{
						delete [] buffer;
						delete [] scanLine;
						return false;
					}
					compressRead += bufferSize;
				}
				
				data1 = buffer[bufferIndex];
				
				if ((data1 & 0xC0) == 0xC0)
				{
					bufferIndex++;
					// overflow in buffer ?
					if (bufferIndex >= bufferSize)
					{
						bufferIndex = 0;
						if (compressSize - compressRead < bufferSize) 
						{
							bufferSize = compressSize - compressRead;
						}
						if (!read(buffer, bufferSize))
						{
							delete [] buffer;
							delete [] scanLine;
							return false;
						}
						compressRead += bufferSize;
					}					
					repeatCount = (data1 & 0x3F);
					color32 = refPal32[buffer[bufferIndex]];
				}
				else
				{
					repeatCount = 1;
					color32 = refPal32[data1];
				}
				
				for(i = 0; i < repeatCount; i++)
				{
					*((unsigned __int32*)scanLine + decodedBytes + i) = color32;
				}
				decodedBytes += repeatCount;
			}
			
			// nu e optim, se fac 2 scrieri in memorie
			//memcpy(data + crtLine * info.width * 4, scanLine, info.width * 4);
			memcpy(data + crtLine * bytesPitch, scanLine, info.width * 4);
		}
		break;
		
    default :
		delete [] buffer;
		return false;
	}
  
	delete [] buffer;
	delete [] scanLine;
	return true;
}
//---------------------------------------------------------------------------

// Descriere   : incarca un fisier PCX 24 bit intr'o suprafata cu formatul dat de dataBpp
// Param       : data - unde se incarca, pointer ce trebuie alocat de user;
//               width, height - dimensiunile suprafetei;
//               dataBpp - formatul de culoare al suprafetei
// Rezultat    : true/false
// Comentarii  : inca trebuie testata
// History     : 28-Jan-2000 [Kafka] added explicit conversion to eliminate warnings
//                                   commented out unused default variable value for bufferIndex
bool GPcxLoad::Load24Bits(unsigned char* data, int width, int height, int bytesPitch, TColorFormat dataBpp)
{
	int              i, j, crtLine, decodedBytes, repeatCount;
	int              fileSize, compressSize, bufferSize, bufferIndex/* = 0*/, compressRead; // [Kafka]
	unsigned char*   buffer = NULL, *startPtr = NULL;
	unsigned __int8  data1, color;
	
	if (!data) 
	{
		return false;
	}
	
	// merg la inceputul datelor
	fileSize = length();

	if (!seek(sizeof(TPcxFileHeader))) return false;
	
	// aloc memorie pentru bufferul de citire din fisier
	compressSize = fileSize - sizeof(TPcxFileHeader);
	if (compressSize > PCX_BUFFER_SIZE) 
	{
		bufferSize = PCX_BUFFER_SIZE;
	}
	else 
	{
		bufferSize = compressSize;
	}

	buffer = new unsigned char[bufferSize];

	if (!buffer) 
	{
		return false;
	}
	
	switch(dataBpp)
	{
    case RGB_565 :
		// pentru optimizare
		// aliniere la un numar de octeti
		if (bytesPitch == -1)
		{
			bytesPitch = (width * 2);
		}
		else
		{
			if (bytesPitch / 2 < width) 
			{
				delete [] buffer;
				return false;
			}
		}
		startPtr = data;
		
		// citesc in buffer
		bufferIndex  = -1;
		compressRead = bufferSize;
		if (!read(buffer, bufferSize))
		{
			delete [] buffer;
			return false;
		}

		for(crtLine = 0; crtLine < info.height; crtLine++)
		{
			for(j = 0; j < 3; j++)
			{
				decodedBytes = 0;
				while (decodedBytes < header.bytesPerLine)
				{
					// overflow in buffer ?
					bufferIndex++;
					if (bufferIndex >= bufferSize)
					{
						bufferIndex = 0;
						if (compressSize - compressRead < bufferSize) bufferSize = compressSize - compressRead;
						if (!read(buffer, bufferSize))
						{
							delete [] buffer;
							return false;
						}
						compressRead += bufferSize;
					}
					
					data1 = buffer[bufferIndex];
					if ((data1 & 0xC0) == 0xC0)
					{
						bufferIndex++;
						// overflow in buffer ?
						if (bufferIndex >= bufferSize)
						{
							bufferIndex = 0;
							if (compressSize - compressRead < bufferSize) 
							{
								bufferSize = compressSize - compressRead;
							}
							if (!read(buffer, bufferSize))
							{
								delete [] buffer;
								return false;
							}
							compressRead += bufferSize;
						}						
						repeatCount = (data1 & 0x3F);
						color       = buffer[bufferIndex];
					}
					else
					{
						repeatCount = 1;
						color       = data1;
					}
					
					for(i = 0; i < ((decodedBytes + repeatCount > info.width)?info.width - decodedBytes:repeatCount); i++)
					{
						//if (i + decodedBytes < info.width)
						//{
						// memorie vs. viteza; if'urile in for sint total neoptime dpv al vitezei
						switch(j)
						{
						case 0 :
							*((unsigned __int16*)startPtr + i + decodedBytes)  =
								(unsigned __int16)((unsigned __int16)(color >> 3) << 11); // [Kafka]
							break;
						case 1 :
							*((unsigned __int16*)startPtr + i + decodedBytes) |=
								(unsigned __int16)((unsigned __int16)(color >> 2) << 5);  // [Kafka]
							break;
						case 2 :
							*((unsigned __int16*)startPtr + i + decodedBytes) |=
								(unsigned __int16)(color >> 3);
							break;
						}
						//}
					}
					
					decodedBytes += repeatCount;
				}
			}
			startPtr += bytesPitch;
		}
		break;
		
    case RGB_555 :
		// pentru optimizare
		// aliniere la un numar de octeti
		if (bytesPitch == -1)
		{
			bytesPitch = (width * 2);
		}
		else
		{
			if (bytesPitch / 2 < width) 
			{
				delete [] buffer;
				return false;
			}
		}
		startPtr = data;
		
		// citesc in buffer
		bufferIndex  = -1;
		compressRead = bufferSize;
		if (!read(buffer, bufferSize))
		{
			delete [] buffer;
			return false;
		}
		
		for(crtLine = 0; crtLine < info.height; crtLine++)
		{
			for(j = 0; j < 3; j++)
			{
				decodedBytes = 0;
				while (decodedBytes < header.bytesPerLine)
				{
					// overflow in buffer ?
					bufferIndex++;
					if (bufferIndex >= bufferSize)
					{
						bufferIndex = 0;
						if (compressSize - compressRead < bufferSize) 
						{
							bufferSize = compressSize - compressRead;
						}
						if (!read(buffer, bufferSize))
						{
							delete [] buffer;
							return false;
						}
						compressRead += bufferSize;
					}
					
					data1 = buffer[bufferIndex];
					if ((data1 & 0xC0) == 0xC0)
					{
						bufferIndex++;
						// overflow in buffer ?
						if (bufferIndex >= bufferSize)
						{
							bufferIndex = 0;
							if (compressSize - compressRead < bufferSize) 
							{
								bufferSize = compressSize - compressRead;
							}
							if (!read(buffer, bufferSize))
							{
								delete [] buffer;
								return false;
							}
							compressRead += bufferSize;
						}
						repeatCount = (data1 & 0x3F);
						color       = buffer[bufferIndex];
					}
					else
					{
						repeatCount = 1;
						color       = data1;
					}
					
					for(i = 0; i < ((decodedBytes + repeatCount > info.width)?info.width - decodedBytes:repeatCount); i++)
					{
						//if (i + decodedBytes < info.width)
						//{
						// memorie vs. viteza; if'urile in for sint total neoptime dpv al vitezei
						switch(j)
						{
						case 0 :
							*((unsigned __int16*)startPtr + i + decodedBytes)  =
								(unsigned __int16)((unsigned __int16)(color >> 3) << 10); //[Kafka]
							break;
						case 1 :
							*((unsigned __int16*)startPtr + i + decodedBytes) |=
								(unsigned __int16)((unsigned __int16)(color >> 3) << 5);  //[Kafka]
							break;
						case 2 :
							*((unsigned __int16*)startPtr + i + decodedBytes) |= (unsigned __int16)(color >> 3);
							break;
						}
						//}
					}					
					decodedBytes += repeatCount;
				}
			}
			startPtr += bytesPitch;
		}
		break;
		
    case RGB_24  :
		// pentru optimizare
		// aliniere la un numar de octeti
		if (bytesPitch == -1)
		{
			bytesPitch = (width * 3);
		}
		else
		{
			if (bytesPitch / 3 < width) 
			{
				delete [] buffer;
				return false;
			}
		}
		startPtr = data;
		
		// citesc in buffer
		bufferIndex  = -1;
		compressRead = bufferSize;
		if (!read(buffer, bufferSize))
		{
			delete [] buffer;
			return false;
		}
		
		for(crtLine = 0; crtLine < info.height; crtLine++)
		{
			for(j = 0; j < 3; j++)
			{
				decodedBytes = 0;
				while (decodedBytes < header.bytesPerLine)
				{
					// overflow in buffer ?
					bufferIndex++;
					if (bufferIndex >= bufferSize)
					{
						bufferIndex = 0;
						if (compressSize - compressRead < bufferSize) 
						{
							bufferSize = compressSize - compressRead;
						}
						if (!read(buffer, bufferSize))
						{
							delete [] buffer;
							return false;
						}
						compressRead += bufferSize;
					}
					
					data1 = buffer[bufferIndex];
					if ((data1 & 0xC0) == 0xC0)
					{
						bufferIndex++;
						// overflow in buffer ?
						if (bufferIndex >= bufferSize)
						{
							bufferIndex = 0;
							if (compressSize - compressRead < bufferSize) 
							{
								bufferSize = compressSize - compressRead;
							}
							if (!read(buffer, bufferSize))
							{
								delete [] buffer;
								return false;
							}
							compressRead += bufferSize;
						}

						repeatCount = (data1 & 0x3F);
						color       = buffer[bufferIndex];
					}
					else
					{
						repeatCount = 1;
						color       = data1;
					}
					
					for(i = 0; i < ((decodedBytes + repeatCount > info.width)?info.width - decodedBytes:repeatCount); i++)
					{
						//if (i + decodedBytes < info.width)
						//{
						*(startPtr + ((i + decodedBytes) * 3 + 2 - j)) = color;
						//}
					}					
					decodedBytes += repeatCount;
				}
			}
			startPtr += bytesPitch;
		}
		break;
		
    case RGB_32  :
		// pentru optimizare
		// aliniere la un numar de octeti
		if (bytesPitch == -1)
		{
			bytesPitch = (width * 4);
		}
		else
		{
			if (bytesPitch / 4 < width) 
			{
				delete [] buffer;
				return false;
			}
		}
		startPtr = data;
		
		// citesc in buffer
		bufferIndex  = -1;
		compressRead = bufferSize;
		if (!read(buffer, bufferSize))
		{
			delete [] buffer;
			return false;
		}
		
		for(crtLine = 0; crtLine < info.height; crtLine++)
		{
			for(j = 0; j < 3; j++)
			{
				decodedBytes = 0;
				while (decodedBytes < header.bytesPerLine)
				{
					// overflow in buffer ?
					bufferIndex++;
					if (bufferIndex >= bufferSize)
					{
						bufferIndex = 0;
						if (compressSize - compressRead < bufferSize) 
						{
							bufferSize = compressSize - compressRead;
						}

						if (!read(buffer, bufferSize))
						{
							delete [] buffer;
							return false;
						}
						compressRead += bufferSize;
					}
					
					data1 = buffer[bufferIndex];
					if ((data1 & 0xC0) == 0xC0)
					{
						bufferIndex++;
						// overflow in buffer ?
						if (bufferIndex >= bufferSize)
						{
							bufferIndex = 0;
							if (compressSize - compressRead < bufferSize) 
							{
								bufferSize = compressSize - compressRead;
							}
							if (!read(buffer, bufferSize))
							{
								delete [] buffer;
								return false;
							}
							compressRead += bufferSize;
						}
						
						repeatCount = (data1 & 0x3F);
						color       = buffer[bufferIndex];
					}
					else
					{
						repeatCount = 1;
						color       = data1;
					}
					
					for(i = 0; i < ((decodedBytes + repeatCount > info.width)?info.width - decodedBytes:repeatCount); i++)
					{
						//if (i + decodedBytes < info.width)
						//{
						*(startPtr + ((i + decodedBytes) * 4 + 2 - j)) = color;
						//}
					}
					
					decodedBytes += repeatCount;
				}
			}
			startPtr += bytesPitch;
		}
		break;
		
    default :
		delete [] buffer;
		return false;
	}
  
	delete [] buffer;
	return true;
}
//---------------------------------------------------------------------------

// Descriere   : destructor
// Param       : n/a
// Rezultat    : n/a
// Comentarii  : eventual ceva dezalocari de memorie, daca bufferul va fi pointer
GPcxLoad::~GPcxLoad()
{
	if (Close() != true)
	{
		// do nothing
	}
}
//---------------------------------------------------------------------------

