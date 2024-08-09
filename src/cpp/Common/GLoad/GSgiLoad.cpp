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
#pragma hdrstop

#include "Platform.h"
#include "GSgiLoad.h"
#include "BitOp.h"
#ifdef PPT_PLATFORM_WIN
#include <io.h>
#endif

//---------------------------------------------------------------------------
#ifdef __BORLANDC__
#pragma package(smart_init)
#endif

#ifdef _MSC_VER	// Standard workaround for Microsoft compiler bug
#define for if(0) {} else for
#endif

GSgiLoad::GSgiLoad()
{
  memset(&fileHeader, 0x0, sizeof(TSgiFileHeader));
}
//---------------------------------------------------------------------------

// Descriere   : incarca efectiv fisierul grafic intr'o suprafata (zona de memorie)
//               data de user
// Param       : data - zona de memorie, width - latime, height - inaltime, dataBpp - formatul zonei de memorie
// Rezultat    : true/false
// Comentarii  : metoda poate face conversii de color format
bool GSgiLoad::Load(unsigned char* data, int width, int height, TColorFormat dataBpp)
{
  // verificari
  if (!data)
  {
   return false;
  }

  // nu se poate incarca in suprafete (char*) mai putin de 16 bpp
  switch(dataBpp)
  {
    case BW     :
    case RGB_4  :
    case RGB_8  :
    case RGB_16 :
      return false;
  }

  // aleg metoda de incarcare
  switch(fileHeader.storage)
  {
    case SGI_RLE :
      return LoadRle(data, width, height, -1, dataBpp);

    case SGI_VERBATIM :
      return LoadVerbatim(data, width, height, -1, dataBpp);

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
bool GSgiLoad::LoadAligned(unsigned char* data, int width, int height, int bytesPitch, TColorFormat dataBpp)
{
  // verificari
  if (!data)
  {
    return false;
  }

  // nu se poate incarca in suprafete (char*) mai putin de 16 bpp
  switch(dataBpp)
  {
    case BW     :
    case RGB_4  :
    case RGB_8  :
    case RGB_16 :
      return false;
  }

  // aleg metoda de incarcare
  switch(fileHeader.storage)
  {
    case SGI_RLE :
      return LoadRle(data, width, height, bytesPitch, dataBpp);

    case SGI_VERBATIM :
      return LoadVerbatim(data, width, height, bytesPitch, dataBpp);

    default :
      return false;
  }
}
//---------------------------------------------------------------------------

// Descriere   : citeste headerul unui fisier SGI
// Param       : n/a
// Rezultat    : true/false
// Comentarii  : formatul SGI e cam prost specificat, de aceea, pina la teste mai ca lumea, metoda ramine in plop
bool GSgiLoad::ReadHeader()
{
  //int result;

  if (!rewind())
  {
    return false;
  }

  // cifrele trebuie inlocuite cu sizeof
  if (!read(&fileHeader.fileId,     2)) return false;
  if (!read(&fileHeader.storage,    1)) return false;
  if (!read(&fileHeader.bpc,        1)) return false;
  if (!read(&fileHeader.dimension,  2)) return false;
  if (!read(&fileHeader.xSize,      2)) return false;
  if (!read(&fileHeader.ySize,      2)) return false;
  if (!read(&fileHeader.zSize,      2)) return false;
  if (!read(&fileHeader.pixMin,     4)) return false;
  if (!read(&fileHeader.pixMax,     4)) return false;
  if (!read(&fileHeader.dummy,      4)) return false;
  if (!read(&fileHeader.imageName,  80)) return false;
  if (!read(&fileHeader.colorMapId, 4)) return false;

  fileHeader.fileId     = Swap16(fileHeader.fileId);
  fileHeader.dimension  = Swap16(fileHeader.dimension);
  fileHeader.xSize      = Swap16(fileHeader.xSize);
  fileHeader.ySize      = Swap16(fileHeader.ySize);
  fileHeader.zSize      = Swap16(fileHeader.zSize);
  fileHeader.pixMin     = Swap32(fileHeader.pixMin);
  fileHeader.pixMax     = Swap32(fileHeader.pixMax);
  fileHeader.colorMapId = Swap32(fileHeader.colorMapId);

  if (fileHeader.fileId != SGI_FILEID)
  {
    return false;
  }

  if ((fileHeader.bpc < 1) || (fileHeader.bpc > 2))
  {
    return false;
  }

  if ((fileHeader.dimension < 1) || (fileHeader.dimension > 3))
  {
    return false;
  }

  if (fileHeader.colorMapId != SGI_NORMAL)
  {
    return false;
  }

  if ((fileHeader.storage != (int)SGI_RLE) && (fileHeader.storage != (int)SGI_VERBATIM))
  {
    return false;
  }

  // completez o structura TGInfo
  info.width           = fileHeader.xSize;
  if (fileHeader.dimension < 2) info.height  = 1;
  else info.height = fileHeader.ySize;
  if (fileHeader.dimension < 3) fileHeader.zSize = 1;

  info.bytesPerChannel = fileHeader.bpc;
  switch(fileHeader.zSize)
  {
    case 1 :
      info.bpp = BW;
      break;

    case 3 :
      info.bpp = RGB_24;
      break;

    case 4 :
      info.bpp = RGB_32;
      break;
  }

  return true;
}
//---------------------------------------------------------------------------

// Descriere   : incarca cele doua tabele (de inceput de rind si de lungime de rind)
// Param       : tablesLength - dimensiunea tabelelor, startTable - pointer la tabela de inceput de linie, lengthTable - pointer la tabela de lungimi pentru linii
// Rezultat    : true/false
// Comentarii  : n/a
bool GSgiLoad::LoadScanTable(unsigned tablesLength, unsigned __int32 *startTable, unsigned __int32 *lengthTable)
{
  // merg la inceputul tabelelor
  if (!seek(sizeof(TSgiFileHeader)))
  {
   return false;
  }

  // citesc tabelele
  if (read(startTable,  4 * tablesLength) == -1)
  {
    return false;
  }

  if (read(lengthTable, 4 * tablesLength) == -1)
  {
    return false;
  }

  unsigned int fileLength = length();

  // swapbytes si verificare de rutina
  for(unsigned int i = 0; i < tablesLength; i++)
  {
    startTable[i]  = Swap32(startTable[i]);
    lengthTable[i] = Swap32(lengthTable[i]);
    if ((fileLength < startTable[i]) || (startTable[i] == 0))
    {
      return false;
    }
  }

  return true;
}
//---------------------------------------------------------------------------

// Descriere   : metoda interna de incarcare a SGI'urilor compresate cu RLE
// Param       : clasici
// Rezultat    : true/false
// Comentarii  : nu suporta bpc == 2
bool GSgiLoad::LoadRle (unsigned char* data, int width, int height, int bytesPitch, TColorFormat dataBpp)
{
  unsigned __int32 *startTable, *lengthTable;
  unsigned __int8  *buffer;
  unsigned __int8  *scanLine, color;
  int              bufferIndex, scanLineIndex, count, bufferSize;

  if (!data)
  {
   return false;
  }

  // deocamdata, fara suport pentru bpc = 2
  if (fileHeader.bpc == 2)
  {
    return false;
  }

  int tablesLength = info.height * fileHeader.zSize;

  // aloc memorie
  try
  {
    startTable  = new unsigned __int32[tablesLength];
    lengthTable = new unsigned __int32[tablesLength];
  }
  catch(...)
  {
    // nu e chiar corect
    return false;
  }

  // citesc tabelele de lungime, si de inceput
  if (!LoadScanTable(tablesLength, startTable, lengthTable))
  {
    return false;
  }

  // caut dimensiunea maxima de alocat pentru buffer
  bufferSize = 0;
  for(int i = 0; i < tablesLength; i++)
  {
    if (lengthTable[i] > (unsigned int)bufferSize)
    {
       bufferSize = lengthTable[i];
    }
  }

  try
  {
    buffer = new unsigned __int8 [info.width * fileHeader.bpc];
  }
  catch(...)
  {
    delete [] startTable;
    delete [] lengthTable;
    return false;
  }

  // nu se poate incarca in suprafete (char*) mai putin de 16 bpp
  // pare in plus verificarea, pentru ca functia e interna si eu stiu
  // sa o apelez, dar...
  switch(dataBpp)
  {
    case RGB_565 :
      // setup
      if (bytesPitch == -1)
      {
        bytesPitch = info.width * 2;
      }
      memset(data, 0, bytesPitch * info.height);
      data += bytesPitch * (info.height - 1);

      try
      {
        scanLine = new unsigned __int8 [bytesPitch * fileHeader.zSize];
      }
      catch(...)
      {
        delete [] startTable;
        delete [] lengthTable;
        delete [] buffer;
        return false;
      }

      for(int i = 0; i < info.height; i++)
      {
        // citire din fisier si decompresie
        for(int j = 0; j < fileHeader.zSize; j++)
        {
          // caut inceputul channelului in fisier
          if (!seek(startTable[j * info.height + i]))
          {
            delete [] startTable;
            delete [] lengthTable;
            delete [] scanLine;
            delete [] buffer;
            return false;
          }

          // citesc channelul in buffer
          if (!read(buffer, lengthTable[j * info.height + i]))
          {
            delete [] startTable;
            delete [] lengthTable;
            delete [] scanLine;
            delete [] buffer;
            return false;
          }

          // decompresez RLE canalul j
          bufferIndex   = 0;
          count         = buffer[bufferIndex] & 127;
          scanLineIndex = j * bytesPitch;

          while(count != 0)
          {
            if ((buffer[bufferIndex] & 128) == 128)
            {
              // copiez din sursa in destinatie
              memcpy(scanLine + scanLineIndex, buffer + bufferIndex + 1, count);
              bufferIndex += count + 1;
            }
            else
            {
              color = buffer[bufferIndex + 1];
              for(int k = 0; k < count; k++) scanLine[scanLineIndex + k] = color;
              bufferIndex += 2;
            }

            scanLineIndex += count;
            if (scanLineIndex - j * bytesPitch > info.width)
            {
              return false;
            }

            count = buffer[bufferIndex] & 127;
          }
        }

        // conversie
        switch(info.bpp)
        {
          case BW     :
            // trebuie testata
            for(int j = 0; j < info.width; j++)
            {
              ((unsigned __int16*)data)[j] = (unsigned __int16)(((scanLine[j] >> 3) << 11)  |
					                                     ((scanLine[j] >> 2) << 5) |
					                                     (scanLine[j]  >> 3));
            }
            data -= bytesPitch;
            break;

          case RGB_24 :
          case RGB_32 :
            for(int j = 0; j < info.width; j++)
            {
              ((unsigned __int16*)data)[j] = (unsigned __int16)(((scanLine[j] >> 3) << 11)  |
					                                     ((scanLine[j + bytesPitch] >> 2) << 5) |
					                                     (scanLine[j + (bytesPitch << 1)] >> 3));
            }
            data -= bytesPitch;
            break;
        }
      }
      break;

    case RGB_555 :
      // setup
      if (bytesPitch == -1)
      {
        bytesPitch = info.width * 2;
      }
      memset(data, 0, bytesPitch * info.height);
      data += bytesPitch * (info.height - 1);

      try
      {
        scanLine = new unsigned __int8 [bytesPitch * fileHeader.zSize];
      }
      catch(...)
      {
        delete [] startTable;
        delete [] lengthTable;
        delete [] buffer;
        return false;
      }

      for(int i = 0; i < info.height; i++)
      {
        // citire din fisier si decompresie
        for(int j = 0; j < fileHeader.zSize; j++)
        {
          // caut inceputul channelului in fisier
          if (!seek(startTable[j * info.height + i]))
          {
            delete [] startTable;
            delete [] lengthTable;
            delete [] scanLine;
            delete [] buffer;
            return false;
          }

          // citesc channelul in buffer
          if (!read(buffer, lengthTable[j * info.height + i]))
          {
            delete [] startTable;
            delete [] lengthTable;
            delete [] scanLine;
            delete [] buffer;
            return false;
          }

          // decompresez RLE canalul j
          bufferIndex   = 0;
          count         = buffer[bufferIndex] & 127;
          scanLineIndex = j * bytesPitch;

          while(count != 0)
          {
            if ((buffer[bufferIndex] & 128) == 128)
            {
              // copiez din sursa in destinatie
              memcpy(scanLine + scanLineIndex, buffer + bufferIndex + 1, count);
              bufferIndex += count + 1;
            }
            else
            {
              color = buffer[bufferIndex + 1];
              for(int k = 0; k < count; k++) scanLine[scanLineIndex + k] = color;
              bufferIndex += 2;
            }

            scanLineIndex += count;
            if (scanLineIndex - j * bytesPitch > info.width)
            {
              return false;
            }

            count = buffer[bufferIndex] & 127;
          }
        }

        // conversie
        switch(info.bpp)
        {
          case BW     :
            // trebuie testata
            for(int j = 0; j < info.width; j++)
            {
              ((unsigned __int16*)data)[j] = (unsigned __int16)(((scanLine[j] >> 3) << 10)  |
					                                                       ((scanLine[j] >> 3) << 5) |
                                                                 (scanLine[j]  >> 3));
            }
            data -= bytesPitch;
            break;

          case RGB_24 :
          case RGB_32 :
            for(int j = 0; j < info.width; j++)
            {
              ((unsigned __int16*)data)[j] = (unsigned __int16)(((scanLine[j] >> 3) << 10)  |
                                                                ((scanLine[j + bytesPitch] >> 3) << 5) |
                                                                 (scanLine[j + (bytesPitch << 1)] >> 3));
            }
            data -= bytesPitch;
            break;
        }
      }
      break;

    // datele sint pe 24 de biti
    case RGB_24 :

      // setup
      if (bytesPitch == -1)
      {
        bytesPitch = info.width * 3;
      }
      memset(data, 0, bytesPitch * info.height);
      data += bytesPitch * (info.height - 1);

      try
      {
        scanLine = new unsigned __int8 [bytesPitch * fileHeader.zSize];
      }
      catch(...)
      {
        delete [] startTable;
        delete [] lengthTable;
        delete [] buffer;
        return false;
      }

      for(int i = 0; i < info.height; i++)
      {
        // citire din fisier si decompresie
        for(int j = 0; j < fileHeader.zSize; j++)
        {
          // caut inceputul channelului in fisier
          if (!seek(startTable[j * info.height + i]))
          {
            delete [] startTable;
            delete [] lengthTable;
            delete [] scanLine;
            delete [] buffer;
            return false;
          }

          // citesc channelul in buffer
          if (!read(buffer, lengthTable[j * info.height + i]))
          {
            delete [] startTable;
            delete [] lengthTable;
            delete [] scanLine;
            delete [] buffer;
            return false;
          }

          // decompresez RLE canalul j
          bufferIndex   = 0;
          count         = buffer[bufferIndex] & 127;
          scanLineIndex = j * bytesPitch;

          while(count != 0)
          {
            if ((buffer[bufferIndex] & 128) == 128)
            {
              // copiez din sursa in destinatie
              memcpy(scanLine + scanLineIndex, buffer + bufferIndex + 1, count);
              bufferIndex += count + 1;
            }
            else
            {
              color = buffer[bufferIndex + 1];
              for(int k = 0; k < count; k++) scanLine[scanLineIndex + k] = color;
              bufferIndex += 2;
            }

            scanLineIndex += count;
            if (scanLineIndex - j * bytesPitch > info.width)
            {
              return false;
            }

            count = buffer[bufferIndex] & 127;
          }
        }

        // conversie
        switch(info.bpp)
        {
          case BW     :
            // trebuie testata
            for(int j = 0; j < info.width; j++)
            {
              data[(j * 3) + 2] = scanLine[j];
              data[(j * 3) + 1] = scanLine[j];
              data[(j * 3) + 0] = scanLine[j];
            }
            data -= bytesPitch;
            break;

          case RGB_24 :
          case RGB_32 :
            for(int j = 0; j < info.width; j++)
            {
              data[(j * 3) + 2] = scanLine[j];
              data[(j * 3) + 1] = scanLine[j + bytesPitch];
              data[(j * 3) + 0] = scanLine[j + (bytesPitch << 1)];
            }
            data -= bytesPitch;
            break;
        }
      }
      break;

    // datele sint pe 32 de biti
    case RGB_32 :

      // setup
      if (bytesPitch == -1)
      {
        bytesPitch = info.width * 4;
      }
      memset(data, 0, bytesPitch * info.height);
      data += bytesPitch * (info.height - 1);

      try
      {
        scanLine = new unsigned __int8 [bytesPitch * fileHeader.zSize];
      }
      catch(...)
      {
        delete [] startTable;
        delete [] lengthTable;
        delete [] buffer;
        return false;
      }

      for(int i = 0; i < info.height; i++)
      {
        // citire din fisier si decompresie
        for(int j = 0; j < fileHeader.zSize; j++)
        {
          // caut inceputul channelului in fisier
          if (!seek(startTable[j * info.height + i]))
          {
            delete [] startTable;
            delete [] lengthTable;
            delete [] scanLine;
            delete [] buffer;
            return false;
          }

          // citesc channelul in buffer
          if (!read(buffer, lengthTable[j * info.height + i]))
          {
            delete [] startTable;
            delete [] lengthTable;
            delete [] scanLine;
            delete [] buffer;
            return false;
          }

          // decompresez RLE canalul j
          bufferIndex   = 0;
          count         = buffer[bufferIndex] & 127;
          scanLineIndex = j * bytesPitch;

          while(count != 0)
          {
            if ((buffer[bufferIndex] & 128) == 128)
            {
              // copiez din sursa in destinatie
              memcpy(scanLine + scanLineIndex, buffer + bufferIndex + 1, count);
              bufferIndex += count + 1;
            }
            else
            {
              color = buffer[bufferIndex + 1];
              for(int k = 0; k < count; k++) scanLine[scanLineIndex + k] = color;
              bufferIndex += 2;
            }

            scanLineIndex += count;
            if (scanLineIndex - j * bytesPitch > info.width)
            {
              return false;
            }

            count = buffer[bufferIndex] & 127;
          }
        }

        // conversie
        switch(info.bpp)
        {
          case RGB_24 :
            for(int j = 0; j < info.width; j++)
            {
              data[(j << 2) + 2] = scanLine[j];
              data[(j << 2) + 1] = scanLine[j + bytesPitch];
              data[(j << 2) + 0] = scanLine[j + (bytesPitch << 1)];
            }
            data -= bytesPitch;
            break;

          case BW     :
            // trebuie testata
            for(int j = 0; j < info.width; j++)
            {
              data[(j << 2) + 2] = scanLine[j];
              data[(j << 2) + 1] = scanLine[j];
              data[(j << 2) + 0] = scanLine[j];
            }
            data -= bytesPitch;
            break;

          case RGB_32 :
            // trebuie testata
            for(int j = 0; j < info.width; j++)
            {
              data[(j << 2) + 2] = scanLine[j];
              data[(j << 2) + 1] = scanLine[j + bytesPitch];
              data[(j << 2) + 0] = scanLine[j + (bytesPitch << 1)];
              data[(j << 2) + 3] = scanLine[j + bytesPitch * 3];
            }
            data -= bytesPitch;
            break;
        }
      }
      break;

    default :
      return false;
  }

  delete [] startTable;
  delete [] lengthTable;
  delete [] scanLine;
  delete [] buffer;
  return true;
}
//---------------------------------------------------------------------------

// Descriere   : cel mai clasic mod de incarcare al imaginilor, necompresat
// Param       : data - zona de memorie in care se incarca,
//               width, height - ar trebui folositi pentru a se incarca doar o bucata de imagine; deocamdata, insa, trebuie sa reprezinte w/h ale imaginii
//               bytesPitch    - aliniamentul zonei de memorie in care se incarca
//               dataBpp       - formatul de culoare al zonei in care se incarca
// Rezultat    : true/false
// Comentarii  : documentatia la SGI e foarte vaga si nici nu am avut imagine de test, deci nu se stie daca functioneaza corect
bool GSgiLoad::LoadVerbatim (unsigned char* data, int width, int height, int bytesPitch, TColorFormat dataBpp)
{
  unsigned __int8 *scanLineBuffer, shiftTable[3], ditherTable[3];

  if (!data)
  {
   return false;
  }

  // merg la inceputul datelor
  if (!seek(sizeof(TSgiFileHeader)))
  {
   return false;
  }

  // aloc memorie
  try
  {
    scanLineBuffer = new unsigned __int8[info.width * fileHeader.bpc];
  }
  catch(...)
  {
    return false;
  }

  memset(shiftTable,  0, 3);
  memset(ditherTable, 0, 3);

  // nu se poate incarca in suprafete (char*) mai putin de 16 bpp
  // pare in plus verificarea, pentru ca functia e interna si eu stiu
  // sa o apelez, dar...
  switch(dataBpp)
  {
    case RGB_565 :
      // setup
      if (bytesPitch == -1)
      {
        bytesPitch = info.width * 2;
      }

      memset(data, 0, bytesPitch * info.height);

      switch(info.bpp)
      {
        case RGB_24 :
        case RGB_32 :
          shiftTable[0] = 11;
          shiftTable[1] = 5;
          shiftTable[2] = 0;

          ditherTable[0] = 3;
          ditherTable[1] = 2;
          ditherTable[2] = 3;
          break;
      }

      // nu conteaza info.bpp
      for(int k = 0; k < fileHeader.zSize; k++)
      {
        data += bytesPitch * (info.height - 1);

        // citesc tot canalul i in data
        for(int i = 0; i < info.height; i++)
        {
          // citesc canalul
          if (!read(scanLineBuffer, info.width * fileHeader.bpc))
          {
            delete [] scanLineBuffer;
            return false;
          }

          //...o integrez in data pointer
          switch(fileHeader.bpc)
          {
            case SGI_BPC1 :
              for(int j = 0; j < info.width; j++) data[j * 2] = (unsigned __int8)((((unsigned __int16)scanLineBuffer[j]) >> ditherTable[k]) << shiftTable[k]) | data[j * 3 + k];
              break;

            case SGI_BPC2 :
              for(int j = 0; j < info.width; j++) data[j * 2] = (unsigned __int8)((((unsigned __int16*)scanLineBuffer)[j] >> (8 + ditherTable[k])) << shiftTable[k]) | data[j * 3 + k]; // dithering
              break;
          }

          data -= bytesPitch;
        }
      }
      break;

    case RGB_555 :
      // setup
      if (bytesPitch == -1)
      {
        bytesPitch = info.width * 2;
      }

      memset(data, 0, bytesPitch * info.height);

      switch(info.bpp)
      {
        case RGB_24 :
        case RGB_32 :
          shiftTable[0] = 10;
          shiftTable[1] = 5;
          shiftTable[2] = 0;

          ditherTable[0] = 3;
          ditherTable[1] = 3;
          ditherTable[2] = 3;
          break;
      }

      // nu conteaza info.bpp
      for(int k = 0; k < fileHeader.zSize; k++)
      {
        data += bytesPitch * (info.height - 1);

        // citesc tot canalul i in data
        for(int i = 0; i < info.height; i++)
        {
          // citesc canalul
          if (!read(scanLineBuffer, info.width * fileHeader.bpc))
          {
            delete [] scanLineBuffer;
            return false;
          }

          //...o integrez in data pointer
          switch(fileHeader.bpc)
          {
            case SGI_BPC1 :
              for(int j = 0; j < info.width; j++) data[j * 2] = (unsigned __int8)((((unsigned __int16)scanLineBuffer[j]) >> 3) << shiftTable[k]) | data[j * 3 + k];
              break;

            case SGI_BPC2 :
              for(int j = 0; j < info.width; j++) data[j * 2] = (unsigned __int8)((((unsigned __int16*)scanLineBuffer)[j] >> 11) << shiftTable[k]) | data[j * 3 + k];; // dithering
              break;
          }

          data -= bytesPitch;
        }
      }
      break;

    case RGB_24 :
      // setup
      if (bytesPitch == -1)
      {
        bytesPitch = info.width * 3;
      }

      memset(data, 0, bytesPitch * info.height);

      // nu conteaza info.bpp

      for(int k = 0; k < fileHeader.zSize; k++)
      {
        data += bytesPitch * (info.height - 1);

        // citesc tot canalul i in data
        for(int i = 0; i < info.height; i++)
        {
          // citesc canalul
          if (!read(scanLineBuffer, info.width * fileHeader.bpc))
          {
            delete [] scanLineBuffer;
            return false;
          }

          //...o integrez in data pointer
          switch(fileHeader.bpc)
          {
            case SGI_BPC1 :
              for(int j = 0; j < info.width; j++) data[j * 3 + k] = scanLineBuffer[j];
              break;

            case SGI_BPC2 :
              for(int j = 0; j < info.width; j++) data[j * 3 + k] = (unsigned __int8)(((unsigned __int16*)scanLineBuffer)[j] / 256); // dithering
              break;
          }

          data -= bytesPitch;
        }
      }
      break;

    case RGB_32 :
      // setup
      if (bytesPitch == -1)
      {
        bytesPitch = info.width * 4;
      }

      memset(data, 0, bytesPitch * info.height);

      // nu conteaza info.bpp

      for(int k = 0; k < fileHeader.zSize; k++)
      {
        data += bytesPitch * (info.height - 1);

        // citesc tot canalul i in data
        for(int i = 0; i < info.height; i++)
        {
          // citesc canalul
          if (!read(scanLineBuffer, info.width * fileHeader.bpc))
          {
            delete [] scanLineBuffer;
            return false;
          }

          //...o integrez in data pointer
          switch(fileHeader.bpc)
          {
            case SGI_BPC1 :
              for(int j = 0; j < info.width; j++) data[(j << 2) + k] = scanLineBuffer[j];
              break;

            case SGI_BPC2 :
              for(int j = 0; j < info.width; j++) data[(j << 2) + k] = (unsigned __int8)(((unsigned __int16*)scanLineBuffer)[j] / 256); // dithering
              break;
          }

          data -= bytesPitch;
        }
      }
      break;

    default :
      return false;
  }

  delete [] scanLineBuffer;

  return true;
}
//---------------------------------------------------------------------------

GSgiLoad::~GSgiLoad()
{
  Close();
}
//---------------------------------------------------------------------------


