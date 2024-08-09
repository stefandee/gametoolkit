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
//      (i've "castrat" some implicit conversions to explicit ones (warnings))
//      (at "if" instructions i've put a block in place of one instruction)
//		(removed method Open (moved in base class))
//	  29-Jan-2000 [Kafka]
//      (commented unreferenced parameters names (eliminate warnings))
//	  31-Jan-2000 [Kafka]
//		(initialized in constructor "header" member variable)
//---------------------------------------------------------------------------
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#pragma hdrstop

#include "Platform.h"
#include "GPImgLoad.h"
#ifdef PPT_PLATFORM_WIN
#include <io.h>
#endif

//---------------------------------------------------------------------------
#ifdef __BORLANDC__
#pragma package(smart_init)
#endif

GPImgLoad::GPImgLoad()
{
	memset(&header, 0x00, sizeof(TPImgFileHeader));
}

//---------------------------------------------------------------------------
// Descriere   : incarca efectiv fisierul grafic intr'o suprafata (zona de memorie)
//               data de user
// Param       : data - zona de memorie, width - latime, height - inaltime, dataBpp - formatul zonei de memorie
// Rezultat    : true/false
// Comentarii  : metoda poate face conversii de color format
bool GPImgLoad::Load(unsigned char* data, int width, int height, TColorFormat dataBpp)
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
    case RGB_565 :
		return Load16Bits_565(data, width, height, -1, dataBpp);
		
    case RGB_32 :
		return Load32Bits(data, width, height, -1, dataBpp);
		
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
bool GPImgLoad::LoadAligned(unsigned char* data, int width, int height, int bytesPitch, TColorFormat dataBpp)
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
    case RGB_565 :
		return Load16Bits_565(data, width, height, bytesPitch, dataBpp);
		
    case RGB_32 :
		return Load32Bits(data, width, height, bytesPitch, dataBpp);
		
    default :
		return false;
	}
}

// Descriere   : un fel de callback pe care il apeleaza Save in caz ca fisierul exista; metoda e virtual, deci poate fi suprascrisa dupa nevoi si sistem de operare
// Param       : n/a
// Rezultat    : true/false in functie de alegerea userului
// Comentarii  : aici rezultatul e tot timpul true
bool UserConfirm()
{
	return true;
}
//---------------------------------------------------------------------------

// Descriere   : incarca headerul fisierului si verifica daca este un fisier valid
// Param       : n/a
// Rezultat    : n/a
// Comentarii  : din cauza alinierii datelor la altceva decit la byte, trebuie
//               explicit citite din fisier cimpurile headerului
bool GPImgLoad::ReadHeader()
{
	//int result;

        if (!rewind())
        {
          return false;
        }

	// citesc headerul
	// if (read(fileHandle, &header, sizeof(TPImgFileHeader)) <= 0) return false;
	if (!read(&header.id,       4)) return false;
	if (!read(&header.version,  4)) return false;
	if (!read(&header.width,    4)) return false;
	if (!read(&header.height,   4)) return false;
	if (!read(&header.bpp,      4)) return false;
	if (!read(&header.encoding, 4)) return false;
	if (!read(header.fill,      8)) return false;

	// e chiar o imagine .pImg ?
	if (PIMG_ID != header.id)
	{
		return false;
	}

	// versiunea e diferita de 1.0 ?
	if ((header.version >> 16) != PIMG_MAJOR_VERSION &&
		(header.version & 0xFF00) != PIMG_MINOR_VERSION)
	{
		return false;
	}

	// completez o structura TGInfo
	info.width  = header.width;
	info.height = header.height;
	switch(header.bpp)
	{
    case RGB_565 :
		info.bpp = RGB_565;
		break;
		
    case RGB_32 :
		info.bpp = RGB_32;
		break;
		
    default :
		return false;
	}
	
	return true;
}
//---------------------------------------------------------------------------

// Descriere   : incarca un fisier .pImage 16 biti intr'o suprafata cu formatul dat de dataBpp
// Param       : data - unde se incarca, pointer ce trebuie alocat de user;
//               width, height - dimensiunile suprafetei;
//               dataBpp - formatul de culoare al suprafetei
// Rezultat    : true/false
// Comentarii  : n/a
// History     : 28-Jan-2000 [Kafka] added explicit conversion to eliminate warnings
bool GPImgLoad::Load16Bits_565 (unsigned char* data, int width, int height, int bytesPitch, TColorFormat dataBpp)
{
	int              totalBytes, readBytes = 0, toRead, i, skipSize, bytesCounter = 1;
	unsigned char*   buffer = NULL;
	unsigned char*   startPtr = NULL;
	unsigned __int16 color;
	unsigned __int8  red, green, blue;
	
	if (!data) 
	{
		return false;
	}
	
	// merg la inceputul datelor
	if (!seek(sizeof(TPImgFileHeader))) 
	{
		return false;
	}
	
	// nu se poate incarca in suprafete (char*) mai putin de 16 bpp
	// pare in plus verificarea, pentru ca functia e interna si eu stiu
	// sa o apelez, dar...
	switch(dataBpp)
	{
    case RGB_4 :
    case RGB_8 :
		return false;
		
    case RGB_555 :
		// din cauza alinierii datelor la bytesPitch
		if (bytesPitch == -1)
		{
			skipSize   = 2;
		}
		else
		{
			if (bytesPitch / 2 < width)
			{
				return false;
			}
			skipSize = bytesPitch - width * 2 + 2;
		}
		startPtr   = data;

		buffer     = new unsigned char[PIMG_BUFFER_SIZE];
		totalBytes = info.width * info.height * 2;

		while(readBytes < totalBytes)
		{
			toRead = (totalBytes - readBytes < PIMG_BUFFER_SIZE) ? (totalBytes - readBytes): PIMG_BUFFER_SIZE;
			if (!read(buffer, toRead))
			{
				delete [] buffer;
				return false;
			}
			
			readBytes += toRead;
			
			// trebuie facuta o conversie 565 -> 555; am citit readBytes, deci readBytes/2 pixeli
			for(i = 0; i < toRead / 2; i++)
			{
				color = *((unsigned __int16*)buffer + i);
				red   = (unsigned __int8)((color & 0xF800) >> 11); // [Kafka]
				green = (unsigned __int8)(((color & 0x07E0) >> 5)  >> 1); // [Kafka]
				blue  = (unsigned __int8)(color & 0x001F); // [Kafka]
				*((unsigned __int16*)startPtr) = (unsigned __int16)((red << 10) | (green << 5) | blue); // [Kafka]
				
				if (bytesCounter == info.width)
				{
					startPtr += skipSize;
					bytesCounter = 0;
				}
				else
				{
					startPtr += 2;
				}
				bytesCounter++;
			}
			
			//startPtr  += toRead;
		}
		
		delete [] buffer;
		break;
		
    case RGB_565 :
		// citirea se face direct, nu mai e nevoie de conversie
		totalBytes = info.width * info.height * 2;
		
		// din cauza alinierii datelor la bytesPitch
		if (bytesPitch == -1)
		{
			bytesPitch = width * 2;
		}
		else
		{
			if (bytesPitch / 2 < width) 
			{
				return false;
			}
		}
		startPtr   = data;
		
		while(readBytes < totalBytes)
		{
        /*
        toRead = (totalBytes - readBytes < PIMG_BUFFER_SIZE) ? (totalBytes - readBytes): PIMG_BUFFER_SIZE;
        if (read(fileHandle, startPtr, toRead) < 0)
		return false;
		
		  readBytes += toRead;
		  startPtr  += toRead;
			*/
			if (!read(startPtr, width * 2))
			{
				return false;
			}
			
			readBytes += width * 2;
			startPtr  += bytesPitch;
		}
		
		break;
		
    case RGB_24 :
		// din cauza alinierii datelor la bytesPitch
		if (bytesPitch == -1)
		{
			skipSize   = 3;
		}
		else
		{
			if (bytesPitch / 3 < width) 
			{
				return false;
			}
			skipSize = bytesPitch - width * 3 + 3;
		}
		startPtr   = data;
		
		buffer     = new unsigned char[PIMG_BUFFER_SIZE];
		totalBytes = info.width * info.height * 2;
		
		while(readBytes < totalBytes)
		{
			toRead = (totalBytes - readBytes < PIMG_BUFFER_SIZE) ? (totalBytes - readBytes): PIMG_BUFFER_SIZE;
			if (!read(buffer, toRead))
			{
				delete [] buffer;
				return false;
			}
			
			// trebuie facuta o conversie 565 -> 24; am citit readBytes, deci readBytes/2 pixeli
			for(i = 0; i < toRead / 2; i++)
			{
				color = *((unsigned __int16*)buffer + i);
				*(startPtr + 2) = (unsigned char)(((color & 0xF800) >> 11) * 8); // [Kafka]
				*(startPtr + 1) = (unsigned char)(((color & 0x07E0) >> 5)  * 4); // [Kafka]
				*startPtr       = (unsigned char)((color & 0x001F) * 8);         // [Kafka]
				if (bytesCounter == info.width)
				{
					startPtr += skipSize;
					bytesCounter = 0;
				}  
				else
				{
					startPtr += 3;
				}
				bytesCounter++;
			}
			readBytes += toRead;
			//startPtr  += toRead;
		}
		
		delete [] buffer;
		break;
		
    case RGB_32 :
		// din cauza alinierii datelor la bytesPitch
		if (bytesPitch == -1)
		{
			skipSize   = 4;
		}
		else
		{
			if (bytesPitch / 4 < width) 
			{
				return false;
			}
			skipSize = bytesPitch - width * 4 + 4;
		}
		startPtr   = data;
		
		buffer     = new unsigned char[PIMG_BUFFER_SIZE];
		totalBytes = info.width * info.height * 2;
		
		while(readBytes < totalBytes)
		{
			toRead = (totalBytes - readBytes < PIMG_BUFFER_SIZE) ? (totalBytes - readBytes): PIMG_BUFFER_SIZE;
			if (!read(buffer, toRead))
			{
				delete [] buffer;
				return false;
			}
			
			readBytes += toRead;
			
			// trebuie facuta o conversie 565 -> 32; seamana cu conversia la 24; am citit readBytes, deci readBytes/2 pixeli
			for(i = 0; i < toRead / 2; i++)
			{
				color = *((unsigned __int16*)buffer + i);
				*(startPtr + 2) = (unsigned char)(((color & 0xF800) >> 11) * 8); // [Kafka]
				*(startPtr + 1) = (unsigned char)(((color & 0x07E0) >> 5)  * 4); // [Kafka]
				*startPtr       = (unsigned char)((color & 0x001F) * 8);         // [Kafka]
				if (bytesCounter == info.width)
				{
					startPtr += skipSize;
					bytesCounter = 0;
				}  
				else 
				{
					startPtr += 4;
				}
				bytesCounter++;
			}
			
			//startPtr  += toRead;
		}
		
		delete [] buffer;
		break;
		
    default :
		return false;
	}
  
	return true;
}
//---------------------------------------------------------------------------

// Descriere   : incarca un fisier .pImage 32 biti intr'o suprafata cu formatul dat de dataBpp
// Param       : data - unde se incarca, pointer ce trebuie alocat de user;
//               width, height - dimensiunile suprafetei;
//               dataBpp - formatul de culoare al suprafetei
// Rezultat    : true/false
// Comentarii  : n/a
bool GPImgLoad::Load32Bits (unsigned char* data, int width, int height, int bytesPitch, TColorFormat dataBpp)
{
	int              totalBytes, readBytes = 0, toRead, i, bytesCounter = 1, skipSize;
	unsigned char*   buffer = NULL;
	unsigned char*   startPtr = NULL;
	//unsigned __int16 color;
	//unsigned __int8  red, green, blue;
	
	if (!data) 
	{
		return false;
	}
	
	// merg la inceputul datelor
        if (!seek(sizeof(TPImgFileHeader)))
        {
          return false;
        }

	// nu se poate incarca in suprafete (char*) mai putin de 16 bpp
	// pare in plus verificarea, pentru ca functia e interna si eu stiu
	// sa o apelez, dar...
	switch(dataBpp)
	{
    case RGB_4 :
    case RGB_8 :
		return false;
		
    case RGB_555 :
		// din cauza alinierii datelor la bytesPitch
		if (bytesPitch == -1)
		{
			skipSize   = 2;
		}
		else
		{
			if (bytesPitch / 2 < width) 
			{
				return false;
			}
			skipSize = bytesPitch - width * 2 + 2;
		}
		startPtr   = data;
		
		buffer     = new unsigned char[PIMG_BUFFER_SIZE];
		if (!buffer) // [Kafka]
		{
			return false;
		}

		totalBytes = info.width * info.height * 4;
		
		while(readBytes < totalBytes)
		{
			toRead = (totalBytes - readBytes < PIMG_BUFFER_SIZE) ? (totalBytes - readBytes): PIMG_BUFFER_SIZE;
			if (!read(buffer, toRead))
			{
				delete [] buffer;
				return false;
			}
			
			// trebuie facuta o conversie 32 -> 555; am citit toRead, deci toRead/4 pixeli (4 = 32bits)
			for(i = 0; i < toRead / 4; i++)
			{
				*((unsigned __int16*)startPtr) =
					(unsigned __int16)
					(((*(buffer + i * 4 + 2) >> 3) << 10) |
					((*(buffer + i * 4 + 1) >> 3) << 5)  |
					(*(buffer + i * 4) >> 3)); // [Kafka]
				if (bytesCounter == info.width)
				{
					startPtr += skipSize;
					bytesCounter = 0;
				}  
				else
				{
					startPtr += 2;
				}
				bytesCounter++;
			}
			
			readBytes += toRead;
			//startPtr  += toRead;
		}
		
		delete [] buffer;
		break;
		
    case RGB_565 :
		// din cauza alinierii datelor la bytesPitch
		if (bytesPitch == -1)
		{
			skipSize   = 2;
		}
		else
		{
			if (bytesPitch / 2 < width)
			{
				return false;
			}
			skipSize = bytesPitch - width * 2 + 2;
		}
		
		startPtr   = data;
		
		buffer     = new unsigned char[PIMG_BUFFER_SIZE];
		totalBytes = info.width * info.height * 4;
		
		while(readBytes < totalBytes)
		{
			toRead = (totalBytes - readBytes < PIMG_BUFFER_SIZE) ? (totalBytes - readBytes): PIMG_BUFFER_SIZE;
			if (!read(buffer, toRead))
			{
				delete [] buffer;
				return false;
			}
			
			// trebuie facuta o conversie 32 -> 565; am citit toRead, deci toRead/4 pixeli (4 = 32bits)
			for(i = 0; i < toRead / 4; i++)
			{
				*((unsigned __int16*)startPtr) =
					(unsigned __int16)
					(((*(buffer + i * 4 + 2) >> 3) << 11) |
					((*(buffer + i * 4 + 1) >> 2) << 5)  |
					(*(buffer + i * 4) >> 3)); // [Kafka]

				if (bytesCounter == info.width)
				{
					startPtr += skipSize;
					bytesCounter = 0;
				}  
				else 
				{
					startPtr += 2;
				}
				bytesCounter++;
			}
			
			readBytes += toRead;
			//startPtr  += toRead;
		}
		
		delete [] buffer;
		break;
		
    case RGB_24 :
		// din cauza alinierii datelor la bytesPitch
		if (bytesPitch == -1)
		{
			skipSize   = 3;
		}
		else
		{
			if (bytesPitch / 3 < width) 
			{
				return false;
			}
			skipSize = bytesPitch - width * 3 + 3;
		}
		startPtr   = data;
		
		buffer     = new unsigned char[PIMG_BUFFER_SIZE];
		if (!buffer)
		{
			return false;
		}
		totalBytes = info.width * info.height * 4;
		
		while(readBytes < totalBytes)
		{
			toRead = (totalBytes - readBytes < PIMG_BUFFER_SIZE) ? (totalBytes - readBytes): PIMG_BUFFER_SIZE;
			if (!read(buffer, toRead))
			{
				delete [] buffer;
				return false;
			}
			
			// trebuie facuta o conversie 32 -> 24; am citit toRead, deci toRead/4 pixeli (4 = 32bits)
			for(i = 0; i < toRead / 4; i++)
			{
				*(startPtr + 2) = *(buffer + i * 4 + 2);
				*(startPtr + 1) = *(buffer + i * 4 + 1);
				*startPtr       = *(buffer + i * 4);
				if (bytesCounter == info.width)
				{
					startPtr += skipSize;
					bytesCounter = 0;
				}  
				else 
				{
					startPtr += 3;
				}
				bytesCounter++;
			}
			
			readBytes += toRead;
			//startPtr  += toRead;
		}
		
		delete [] buffer;
		break;
		
    case RGB_32 :
		// din cauza alinierii datelor la bytesPitch
		if (bytesPitch == -1)
		{
			bytesPitch   = info.width;
		}
		else
		{
			if (bytesPitch / 4 < width) 
			{
				return false;
			}
		}
		startPtr   = data;
		
		// citirea se face direct, nu mai e nevoie de conversie
		totalBytes = info.width * info.height * 4;
		
		while(readBytes < totalBytes)
		{
			/*
			toRead = (totalBytes - readBytes < PIMG_BUFFER_SIZE) ? (totalBytes - readBytes): PIMG_BUFFER_SIZE;
			if (read(fileHandle, startPtr, toRead) < 0)
			return false;
		
			readBytes += toRead;
			startPtr  += toRead;
			*/
			if (!read(startPtr, width * 4))
			{
				return false;
			}
			readBytes += width * 4;
			startPtr  += bytesPitch;
		}
		
		break;
		
	default :
		return false; 
	}
  
	return true;
}
//---------------------------------------------------------------------------

// Descriere   : destructor
// Param       : n/a
// Rezultat    : n/a
// Comentarii  : eventual ceva dezalocari de memorie, daca bufferul va fi pointer
GPImgLoad::~GPImgLoad()
{
	if (Close() != true)
	{
		// do nothing
	}
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Descriere   : salveaza in fisier o suprafata
// Param       : data - zona de memorie, width - latime, height - inaltime, dataBpp - formatul zonei de memorie, fileBpp - formatul imaginii pe disc
// Rezultat    : true/false
// Comentarii  : metoda poate face conversii de color format; a fost scrisa prima, pentru a putea scoate fisiere pentru testarea metodei de load
// History     : 28-Jan-2000 [Kafka] added explicit conversion to eliminate warnings
//               31-Jan-2000 [Kafka] removed "&" from hdr.fill (no need, is pointer)
bool GPImgSave::Save(unsigned char* data, int width, int height, TColorFormat dataBpp, TColorFormat fileBpp)
{
  return SaveAligned(data, width, height, width, dataBpp, fileBpp);
}
//---------------------------------------------------------------------------

// Descriere:
// Param    :
// Comments : This method only works for bytesPitch == width!!!
// TODO: remake this method to work properly
bool GPImgSave::SaveAligned(unsigned char* data, int width, int height, int bytesPitch, TColorFormat dataBpp, TColorFormat fileBpp)
{
  TPImgFileHeader  hdr;
  unsigned char*   buffer = NULL;
  int              bytesTotal, bytesWrote, pixelsInBuffer, bytesPerPixel, i;
  unsigned __int16 color16;
  unsigned __int8  red, green, blue;
  //int              fileHandle;

  // datele sint valide ?
  if (!data)
  {
    return false;
  }

  // completez header'ul
  hdr.id       = PIMG_ID;
  hdr.version  = ((long)PIMG_MAJOR_VERSION << 16) | PIMG_MINOR_VERSION;
  hdr.width    = width;
  hdr.height   = height;
  hdr.encoding = PIMG_ENCODING_NONE;
  
  switch(fileBpp)
  {
    case RGB_565  :
      bytesPerPixel = 2;
      hdr.bpp = fileBpp;
      break;

    case RGB_32   :
      bytesPerPixel = 4;
      hdr.bpp = fileBpp;
      break;

    default :
      return false;
  }

  // scriu bufferul in fisier; din cauza align'ului, nu scriu tot hdr dintr'o data
  if (
       (!write(&hdr.id,       sizeof(unsigned __int32))) ||
       (!write(&hdr.version , sizeof(unsigned __int32))) ||
       (!write(&hdr.width,    sizeof(unsigned __int32))) ||
       (!write(&hdr.height,   sizeof(unsigned __int32))) ||
       (!write(&hdr.bpp,      sizeof(unsigned __int32))) ||
       (!write(&hdr.encoding, sizeof(unsigned __int32))) ||
       (!write(hdr.fill,     2 * sizeof(unsigned __int32)))
     )
  {
    return false;
  }
	
  // in functie de formatul datelor, salvez
  switch(dataBpp)
  {
    case RGB_555 :
      buffer = new unsigned char[PIMG_BUFFER_SIZE];
      if (!buffer)
      {
              return false;
      }
		
      bytesTotal = width * height * 2;
      bytesWrote = 0;
		
      // bucla de conversie - scriere
      while (bytesWrote < bytesTotal)
      {
              if (bytesTotal - bytesWrote > PIMG_BUFFER_SIZE)
              {
                      pixelsInBuffer = PIMG_BUFFER_SIZE / bytesPerPixel;
              }
              else
              {
                      pixelsInBuffer = (bytesTotal - bytesWrote) / 2;
              }

              //fac conversie din *data in *buffer
              switch(fileBpp)
              {
              case RGB_32  :
                      for(i = 0; i < pixelsInBuffer; i++)
                      {
                              color16 = *((unsigned __int16*)data);
                              *(buffer + i * bytesPerPixel)     = (unsigned char)((color16  & 0x001F) * 8); //[Kafka]
                              *(buffer + i * bytesPerPixel + 1) = (unsigned char)(((color16 & 0x03E0) >> 5) * 8); //[Kafka]
                              *(buffer + i * bytesPerPixel + 2) = (unsigned char)(((color16 & 0x7C00) >> 10) * 8); //[Kafka]
                              data += 2;
                      }
                      break;
				
              case RGB_565 :
                      for(i = 0; i < pixelsInBuffer; i++)
                      {
                              color16 = *((unsigned __int16*)data);
                              blue    = (unsigned __int8)(color16  & 0x001F); //[Kafka]
                              green   = (unsigned __int8)(((color16 & 0x03E0) >> 5) << 1); //[Kafka]
                              red     = (unsigned __int8)((color16 & 0x7C00) >> 10); //[Kafka]
                              *((unsigned __int16*)buffer + i)  = (unsigned __int16)((red << 11) | (green << 5) | (blue)); //[Kafka]
                              data += 2;
                      }
                      break;
              }
			
              // bufferul e completat, il scriu in fisier
              if (!write(buffer, pixelsInBuffer * bytesPerPixel))
              {
                      delete [] buffer;
                      return false;
              }

              bytesWrote += pixelsInBuffer * 2;
      }

      delete [] buffer;
      break;

    case RGB_565 :
              buffer = new unsigned char[PIMG_BUFFER_SIZE];
              if (!buffer)
              {
                      return false;
              }

              bytesTotal = width * height * 2;
              bytesWrote = 0;

              // bucla de conversie - scriere
              while (bytesWrote < bytesTotal)
              {
                      if (bytesTotal - bytesWrote > PIMG_BUFFER_SIZE)
                      {
                              pixelsInBuffer = PIMG_BUFFER_SIZE / bytesPerPixel;
                      }
                      else
                      {
                              pixelsInBuffer = (bytesTotal - bytesWrote) / 2;
                      }

                      //fac conversie din *data in *buffer
                      switch(fileBpp)
                      {
                      case RGB_32  :
                              for(i = 0; i < pixelsInBuffer; i++)
                              {
                                      color16 = *((unsigned __int16*)data);
                                      *(buffer + i * bytesPerPixel)     = (unsigned char)((color16 & 0x001F) * 8); // [Kafka]
                                      *(buffer + i * bytesPerPixel + 1) = (unsigned char)(((color16 & 0x07E0) >> 5) * 4); // [Kafka]
                                      *(buffer + i * bytesPerPixel + 2) = (unsigned char)(((color16 & 0xF800) >> 11) * 8); // [Kafka]
                                      data += 2;
                              }
                              break;

                      case RGB_565 :
                              for(i = 0; i < pixelsInBuffer; i++)
                              {
                                      *((unsigned __int16*)buffer + i)  = *((unsigned __int16*)data);
                                      data += 2;
                              }
                              break;
                      }

                      // bufferul e completat, il scriu in fisier
                      if (!write(buffer, pixelsInBuffer * bytesPerPixel))
                      {
                              delete [] buffer;
                              return false;
                      }

                      bytesWrote += pixelsInBuffer * 2;
              }

              delete [] buffer;
              break;

    case RGB_24  :
              buffer = new unsigned char[PIMG_BUFFER_SIZE];
              if (!buffer) 
              {
                      return false;
              }
		
              bytesTotal = width * height * 3;
              bytesWrote = 0;
		
              // bucla de conversie - scriere
              while (bytesWrote < bytesTotal)
              {
                      if (bytesTotal - bytesWrote > PIMG_BUFFER_SIZE)
                      {
                              pixelsInBuffer = PIMG_BUFFER_SIZE / bytesPerPixel;
                      }
                      else 
                      {
                              pixelsInBuffer = (bytesTotal - bytesWrote) / 3;
                      }
			
                      //fac conversie din *data in *buffer
                      switch(fileBpp)
                      {
                      case RGB_32  :
                              for(i = 0; i < pixelsInBuffer; i++)
                              {
                                      *(buffer + i * bytesPerPixel)     = *data;
                                      *(buffer + i * bytesPerPixel + 1) = *(data + 1);
                                      *(buffer + i * bytesPerPixel + 2) = *(data + 2);
                                      data += 3;
                              }
                              break;
				
                      case RGB_565 :
                              for(i = 0; i < pixelsInBuffer; i++)
                              {
                                      *((unsigned __int16*)buffer + i)  = (unsigned __int16)((((*(data + 2)) >> 3) << 11) | (((*(data + 1)) >> 2) << 5) | (*data >> 3));;
                                      data += 3;
                              }
                              break;
                      }
			
                      // bufferul e completat, il scriu in fisier
                      if (!write(buffer, pixelsInBuffer * bytesPerPixel))
                      {
                              delete [] buffer;
                              return false;
                      }
			
                      bytesWrote += pixelsInBuffer * 3;
              }
		
              delete [] buffer;
              break;
		
    case RGB_32  :
              buffer = new unsigned char[PIMG_BUFFER_SIZE];
              if (!buffer) 
              {
                      return false;
              }
		
              bytesTotal = width * height * 4;
              bytesWrote = 0;
		
              // bucla de conversie - scriere
              while (bytesWrote < bytesTotal)
              {
                      if (bytesTotal - bytesWrote > PIMG_BUFFER_SIZE) 
                      {
                              pixelsInBuffer = PIMG_BUFFER_SIZE / bytesPerPixel;
                      }
                      else 
                      {
                              pixelsInBuffer = (bytesTotal - bytesWrote) / 4;
                      }
			
                      //fac conversie din *data in *buffer
                      switch(fileBpp)
                      {
                      case RGB_32  :
                              for(i = 0; i < pixelsInBuffer; i++)
                              {
                                      *((unsigned __int32*)buffer + i) = *((unsigned __int32*)data); 
                                      data += 4;
                              }
                              break;

                      case RGB_565 :
                              for(i = 0; i < pixelsInBuffer; i++)
                              {
                                      *((unsigned __int16*)buffer + i)  = (unsigned __int16)((((*(data + 2)) >> 3) << 11) | (((*(data + 1)) >> 2) << 5) | (*data >> 3)); // [Kafka]
                                      data += 4;
                              }
                              break;
                      }
			
                      // bufferul e completat, il scriu in fisier
                      if (!write(buffer, pixelsInBuffer * bytesPerPixel))
                      {
                              delete [] buffer;
                              return false;
                      }
			
                      bytesWrote += pixelsInBuffer * 4;
              }
		
              delete [] buffer;
              break;
		
    default :
      return false;
  }

  return true;
}
//---------------------------------------------------------------------------

