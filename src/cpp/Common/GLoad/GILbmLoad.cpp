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
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#pragma hdrstop

#include "BitOp.h"
#include "GILbmLoad.h"
#include "Platform.h"
#ifdef PPT_PLATFORM_WIN
#include <io.h>
#endif

//---------------------------------------------------------------------------
#ifdef __BORLANDC__
#pragma package(smart_init)
#endif

GILbmLoad::GILbmLoad()
{
  hasBmhd      = false;
  hasCMap      = false;
  cMapChunkPos = -1;
  bodyChunkPos = -1;

  fileBuffer      = NULL;
  fileBufferIndex = -1;

  memset(&bmpHeader,  0x00, sizeof(TILbm_Bmp_Header));
  memset(&fileHeader, 0x00, sizeof(TILbmChunk));
}
//---------------------------------------------------------------------------

bool GILbmLoad::Load (unsigned char* data, int width, int height, TColorFormat dataBpp)
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
  return InternalLoad(data, width, height, -1, dataBpp);
}
//---------------------------------------------------------------------------

bool GILbmLoad::LoadAligned (unsigned char* data, int width, int height, int bytesPitch, TColorFormat dataBpp)
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
  return InternalLoad(data, width, height, bytesPitch, dataBpp);
}
//---------------------------------------------------------------------------

bool GILbmLoad::InitFileBuffer()
{
  fileBufferIndex = 0;
  if (!read(fileBuffer, ILBM_FILE_BUFFER_SIZE))
  {
    return false;
  }

  return true;
}
//---------------------------------------------------------------------------

// Descriere   : obtine urmatorul byte din bufferul de fisier
// Param       : n/a
// Rezultat    : n/a
// Comentarii  : metoda interna, a nu se apela aiurea
unsigned __int8 GILbmLoad::NextFileBufferByte()
{
  if (fileBufferIndex >= ILBM_FILE_BUFFER_SIZE)
  {
    fileBufferIndex = 0;
    // citesc bufferul din nou
    if (!read(fileBuffer, ILBM_FILE_BUFFER_SIZE))
    {
      return -1;
    }
  }

  fileBufferIndex++;
  return fileBuffer[fileBufferIndex - 1];
}
//---------------------------------------------------------------------------

// Descriere   : decompreseaza RLE un plan de biti
// Param       : bitPlane - locul unde se stocheaza rezultatul; rowSize - ?
// Rezultat    :
// Comentarii  : metoda interna, a nu se apela decit din Load type functions;
//               inca nu face disk caching
bool GILbmLoad::DecodeBitPlane(unsigned __int8 *bitPlane, int rowSize)
{
  int             decodedBytes = 0, k, count32;
  unsigned __int8 count;
  unsigned __int8 byteData;

  switch((TILbmCompression)bmpHeader.compression)
  {
    case ILBM_BYTERUN :
      while (decodedBytes < rowSize)
      {
        // byte run length mother fucking bitplanes (!) unpacking
        count = NextFileBufferByte();
        //read(&count, 1);
        if (count < 128)
        {
          count++;
          count32 = count;

          // test
          if (decodedBytes + count32 > rowSize)
          {
            // count32 = rowSize - decodedBytes;
            return false;
          }

          for(k = 0; k < count32; k++)
          {
            byteData = NextFileBufferByte();
            //read(&byteData, 1);
            bitPlane[decodedBytes + k] = byteData;
          }

          decodedBytes += count;
        }
        else
        {
          if (count > 128)
          {
            byteData = NextFileBufferByte();
            //read(&byteData, 1);
            count32 = (int)(count ^ 0xFF) + 2;

            // test
            if (decodedBytes + count32 > rowSize)
            {
              // count32 = rowSize - decodedBytes;
              return false;
            }

            // memset, de fapt
            memset(bitPlane + decodedBytes, byteData, count32);
            /*
            for(k = 0; k < count32; k++)
            {
              bitPlane[decodedBytes + k] = byteData;
            }
            */

            decodedBytes += (int)(count ^ 0xFF) + 2;
          }
        }
      } // while
      break;

    case ILBM_NOCOMPRESSION :
      read(bitPlane, rowSize);
      return true;
  }

  // e inutila, s'ar fi iesit demult din cod daca decodedBytes ar fi depasit
  if (decodedBytes > rowSize)
  {
    return false;
  }

  return true;
}
//---------------------------------------------------------------------------

// Descriere   : incarca o paleta de culori
// Param       :
// Rezultat    :
// Comentarii  :
bool GILbmLoad::LoadCMap(unsigned __int8* pallete, int colorNumber)
{
  if (cMapChunkPos == -1)
  {
    return false;
  }

  if (!seek(cMapChunkPos))
  {
    return false;
  }

  // citesc paleta din fisier - paleta trebuie sa fie alocata
  if (!read(pallete, colorNumber * 3))
  {
    return false;
  }

  return true;
}
//---------------------------------------------------------------------------

// Descriere   : incarca un fisier ILBM care contine informatii de paleta
// Param       : clasici
// Rezultat    : clasici
// Comentarii  : poate incarca de la monochrome(2 culori) pina la 256 de culori;
//               evident conversia in formate RGB_16 sau mai mari are nevoie de paleta
//               bitPlanes tin bitii pentru o SINGURA linie
//               codul este cam incarcat, deoarece incearca sa rezolve si incarcarea 24 bit (fara paleta) si celelate moduri (1-8 biti), care cer paleta
//               pentru data cu RGB_565/RGB_555, am unit codul, diferentele facindu'se doar la compunerea paletei de referinta; poate ar trebui scos acest
//               cod in afara (se mai face inca un switch, tot dupa dataBpp)
bool GILbmLoad::InternalLoad(unsigned char* data, int width, int height, int bytesPitch, TColorFormat dataBpp)
{
  int              decodedBytes, y, encodedRowSize, planeByte, plane, colors = (1 << bmpHeader.nPlanes);
  unsigned __int8  mask, pallete[256][3];
  unsigned __int8  *bitPlanes;
  unsigned __int16 refPal16[256];
  unsigned __int32 refPal32[256], colorIndex;

  // verificari
  if (!data)
  {
    return false;
  }

  if (info.bpp != RGB_24)
  {
    if (!LoadCMap((unsigned __int8*)pallete, colors))
    {
      return false;
    }
  }

  if (!seek(bodyChunkPos))
  {
    return false;
  }

  encodedRowSize = ((info.width + 15) / 16) * 2;

  // aloc memoria pentru bitPlanes
  try
  {
    bitPlanes  = new unsigned __int8[encodedRowSize * (bmpHeader.nPlanes + 1)];
    fileBuffer = new unsigned __int8[ILBM_FILE_BUFFER_SIZE];
  }
  catch(...)
  {
    // nu merge try...catch cu return, dar viteza nu e o problema
    return false;
  }

  if (!InitFileBuffer())
  {
    return false;
  }

  // read din fisier fara buffering, doar de test
  switch(dataBpp)
  {
    case RGB_555 :
    case RGB_565 :
      // setup
      if (bytesPitch == -1)
      {
        bytesPitch = info.width * 2;
      }

      if (info.bpp != RGB_24)
      {
        if (dataBpp == RGB_565)
        {
          // compun paleta de referinta RGB_565
          for(int i = 0; i < colors; i++)
          {
            refPal16[i] = (unsigned __int16)(((pallete[i][0] >> 3) << 11) | ((pallete[i][1] >> 2) << 5) | (pallete[i][2] >> 3));
          }
        }
        else
        {
          // compun paleta de referinta RGB_555
          for(int i = 0; i < colors; i++)
          {
            refPal16[i] = (unsigned __int16)(((pallete[i][0] >> 3) << 10) | ((pallete[i][1] >> 3) << 5) | (pallete[i][2] >> 3));
          }
        }
      }

      for(y = 0; y < info.height; y++)
      {                                                                              
        for(plane = 0; plane < bmpHeader.nPlanes; plane++)
        {
          DecodeBitPlane(bitPlanes + plane * encodedRowSize, encodedRowSize);
        }

        decodedBytes = 0;
        mask         = 128;
        planeByte    = 0;
        while(decodedBytes < info.width)
        {
          //setup
          colorIndex = 0;

          // asamblez indexul de culoare
          for(plane = bmpHeader.nPlanes - 1; plane >= 0; plane--)
          {
            colorIndex = (unsigned __int32)(colorIndex << 1);
            if ((((bitPlanes + plane * encodedRowSize)[planeByte]) & mask) != 0)
            {
              colorIndex = (unsigned __int32)(colorIndex | 1);
            }
          }

          // test mask - refac masca
          mask = (unsigned __int8)(mask >> 1);
          if (mask == 0)
          {
            planeByte++;
            mask = 128;
          }

          // o comparatie la fiecare pixel ? cam lent
          if (info.bpp == RGB_24)
          {
            unsigned __int16 blue  = (unsigned __int16)((colorIndex & 0xFF0000) >> 16),
                             green = (unsigned __int16)((colorIndex & 0x00FF00) >> 8),
                             red   = (unsigned __int16)((colorIndex & 0x0000FF));

            switch(dataBpp)
            {
              case RGB_555 :
                ((unsigned __int16*)data)[decodedBytes] =  (unsigned __int16)(((red >> 3) << 10) |  ((green >> 3) << 5) | (blue >> 3));
                break;

              case RGB_565 :
                ((unsigned __int16*)data)[decodedBytes] =  (unsigned __int16)(((red >> 3) << 11) |  ((green >> 2) << 5) | (blue >> 3));
                break;
            }
          }
          else
          {
            ((unsigned __int16*)data)[decodedBytes] = refPal16[colorIndex];
          }

          decodedBytes++;
        }

        data += bytesPitch;
      }
      break;

    case RGB_24  :
      // setup
      if (bytesPitch == -1)
      {
        bytesPitch = info.width * 3;
      }
      // nu e chiar nevoie, doar maninca timp
      // memset(data, 0, info.width * info.height * 3);

      // no sweat
      //for(y = info.height - 1; y >= 0 ; y--)
      for(y = 0; y < info.height; y++)
      {
        for(plane = 0; plane < bmpHeader.nPlanes; plane++)
        {
          DecodeBitPlane(bitPlanes + plane * encodedRowSize, encodedRowSize);
        }

        decodedBytes = 0;
        mask         = 128;
        planeByte    = 0;
        while(decodedBytes < info.width)
        {
          //setup
          colorIndex = 0;

          // asamblez indexul de culoare
          for(plane = bmpHeader.nPlanes - 1; plane >= 0; plane--)
          {
            colorIndex = (unsigned __int32)(colorIndex << 1);
            if ((((bitPlanes + plane * encodedRowSize)[planeByte]) & mask) != 0)
            {
              colorIndex = (unsigned __int32)(colorIndex | 1);
            }
          }

          // test mask - refac masca
          mask = (unsigned __int8)(mask >> 1);
          if (mask == 0)
          {
            planeByte++;
            mask = 128;
          }

          if (info.bpp == RGB_24)
          {
            data[3 * decodedBytes + 0] = (unsigned __int8)((colorIndex & 0xFF0000) >> 16);
            data[3 * decodedBytes + 1] = (unsigned __int8)((colorIndex & 0x00FF00) >> 8);
            data[3 * decodedBytes + 2] = (unsigned __int8)(colorIndex & 0x0000FF);
          }
          else
          {
            data[3 * decodedBytes + 2] = pallete[colorIndex][0];
            data[3 * decodedBytes + 1] = pallete[colorIndex][1];
            data[3 * decodedBytes + 0] = pallete[colorIndex][2];
          }  

          decodedBytes++;
        }

        data += bytesPitch;
      }
      break;

    case RGB_32  :
      // setup
      if (bytesPitch == -1)
      {
        bytesPitch = info.width * 4;
      }

      // compun paleta de referinta
      if (info.bpp != RGB_24)
      {
        for(int i = 0; i < colors; i++)
        {
          refPal32[i] = (pallete[i][0] << 16) | (pallete[i][1] << 8) | pallete[i][2];
        }
      }

      for(y = 0; y < info.height; y++)
      {
        for(plane = 0; plane < bmpHeader.nPlanes; plane++)
        {
          DecodeBitPlane(bitPlanes + plane * encodedRowSize, encodedRowSize);
        }

        decodedBytes = 0;
        mask         = 128;
        planeByte    = 0;
        while(decodedBytes < info.width)
        {
          //setup
          colorIndex = 0;

          // asamblez indexul de culoare
          for(plane = bmpHeader.nPlanes - 1; plane >= 0; plane--)
          //for(plane = 0; plane < bmpHeader.nPlanes; plane++)
          {
            colorIndex = (unsigned __int32)(colorIndex << 1);
            if ((((bitPlanes + plane * encodedRowSize)[planeByte]) & mask) != 0)
            {
              colorIndex = (unsigned __int32)(colorIndex | 1);
            }
          }

          // test mask - refac masca
          mask = (unsigned __int8)(mask >> 1);
          if (mask == 0)
          {
            planeByte++;
            mask = 128;
          }

          if (info.bpp == RGB_24)
          {
            unsigned __int32 blue  = (colorIndex & 0xFF0000) >> 16,
                             green = (colorIndex & 0x00FF00) >> 8,
                             red   = (colorIndex & 0x0000FF);
                             
            ((unsigned __int32*)data)[decodedBytes] = ((blue) | (green << 8) | (red << 16));
          }
          else
          {
            ((unsigned __int32*)data)[decodedBytes] = refPal32[colorIndex];
          }  

          decodedBytes++;
        }

        data += bytesPitch;
      }
      break;
    
    default :
      return false;
  }

  delete [] bitPlanes;
  delete [] fileBuffer;

  return true;
}
//---------------------------------------------------------------------------

// Descriere   : citeste headerul dintr'un fisier ILBM si verifica daca e corect
// Param       : n/a
// Rezultat    : true/false - headerul e ok, fisierul este un ILBM
// Comentarii  : n/a
bool GILbmLoad::ReadHeader()
{
  TChunkId ilbmId;
  TILbmChunk chunk;
  unsigned int skipSize;

  if (!rewind())
  {
    return false;
  }

  // din cauza align la word/quad
  // file header
  if (!read(fileHeader.chunkId,   ILBMC_SIZE * sizeof(__int8 ))) return false;
  if (!read(&fileHeader.chunkSize,             sizeof(__int32))) return false;
  if (!read(ilbmId,               ILBMC_SIZE * sizeof(__int8 ))) return false;

  // verific daca e un fisier valid IFF
  if (strncmp((char*)fileHeader.chunkId, ILBMC_FILEID, ILBMC_SIZE) != 0)
  {
    return false;
  }

  if ((strncmp((char*)ilbmId, ILBMC_DATAID1, ILBMC_SIZE) != 0) &&
      (strncmp((char*)ilbmId, ILBMC_DATAID2, ILBMC_SIZE) != 0))
  {
    return false;
  }

  fileHeader.chunkSize = Swap32(fileHeader.chunkSize);

  hasCMap      = false;
  hasBmhd      = false;
  cMapChunkPos = -1;
  bodyChunkPos = -1;

  // dimensiunea e buna ?
  /*
  if ((filelength(fileHandle) - ILBMC_SIZE - sizeof(__int32)) != (int)fileHeader.chunkSize)
  {
    return false;
  }
  */

  // citesc chunk'urile din ILBM chunk
  while(1)
  {
    if (!read(chunk.chunkId,   ILBMC_SIZE * sizeof(__int8))) return false;

    if (!read(&chunk.chunkSize, sizeof(__int32))) return false;

    chunk.chunkSize = Swap32(chunk.chunkSize);

    skipSize = chunk.chunkSize;

    // test de chunk'uri
    if (strncmp((char*)chunk.chunkId, ILBMC_BODY, ILBMC_SIZE) == 0)
    {
      break;
    }

    // BMHD chunk ?
    if (strncmp((char*)chunk.chunkId, ILBMC_BMHD, ILBMC_SIZE) == 0)
    {
      // citesc in bmpHeader
      if (!read(&bmpHeader.width,       sizeof(__int16))) return false;
      if (!read(&bmpHeader.height,      sizeof(__int16))) return false;
      if (!read(&bmpHeader.xPos,        sizeof(__int16))) return false;
      if (!read(&bmpHeader.yPos,        sizeof(__int16))) return false;
      if (!read(&bmpHeader.nPlanes,     sizeof(__int8))) return false;
      if (!read(&bmpHeader.masking,     sizeof(__int8))) return false;
      if (!read(&bmpHeader.compression, sizeof(__int8))) return false;
      if (!read(&bmpHeader.pad,         sizeof(__int8))) return false;
      if (!read(&bmpHeader.transparentColor, sizeof(__int16))) return false;
      if (!read(&bmpHeader.xAspect,     sizeof(__int8))) return false;
      if (!read(&bmpHeader.yAspect,     sizeof(__int8))) return false;
      if (!read(&bmpHeader.pageWidth,   sizeof(__int16))) return false;
      if (!read(&bmpHeader.pageHeight,  sizeof(__int16))) return false;

      // corectiile de rigoare - Motorola order
      skipSize = 0;
      hasBmhd  = true; 
    }

    // CMAP chunk ?
    if (strncmp((char*)chunk.chunkId, ILBMC_CMAP, ILBMC_SIZE) == 0)
    {
      // marchez CMAP
      cMapChunkPos = tell();
      hasCMap = true;
    }

    // skip chunkSize octeti
    if (!seek(skipSize))
    {
      return false;
    }

    //if (eof(fileHandle))
    //{
      // chunk'urile sint intr'o ordine cel putin ciudata, sau e un fisier invalid
    //  return false;
    //}
  }

  // verificari
  if (hasCMap && bmpHeader.nPlanes > 8)
  {
    // paleta de culori si nPlanes > 8 (mai mult de 256 de culori) e aberatie
    return false;
  }

  if (bmpHeader.nPlanes == 0)
  {
    // imaginea nu are date, e un color map
    return false;
  }

  if (bmpHeader.compression > 1)
  {
    // metoda de compresie necunoscuta
    return false;
  }

  if (bmpHeader.masking > 3)
  {
    // metoda de masking necunoscuta
    return false;
  }

  // marchez inceputul chunk'ului BODY
  if ((bodyChunkPos = tell()) == -1)
  {
    return false;
  }

  info.width  = Swap16(bmpHeader.width);
  info.height = Swap16(bmpHeader.height);
  info.bytesPerChannel = 1;

  // cred ca se poate optimiza dar imi e lene
  if (bmpHeader.nPlanes > 8)
  {
    if (bmpHeader.nPlanes == 24) info.bpp = RGB_24;
    else
    {
      return false;
    }
  }  
  else
  {
    if (bmpHeader.nPlanes == 4)
    {
      info.bpp = RGB_4;
    }
    else
    {
      if (bmpHeader.nPlanes == 8)
      {
        info.bpp = RGB_8;
      }
    }
  }

  return true;
}
//---------------------------------------------------------------------------

GILbmLoad::~GILbmLoad()
{
  // Close();
}
//---------------------------------------------------------------------------



