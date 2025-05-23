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
#include "string.h"
#pragma hdrstop

#include "SAuLoad.h"
#include "BitOp.h"

//---------------------------------------------------------------------------
#ifdef __BORLANDC__
#pragma package(smart_init)
#endif

SAuLoad::SAuLoad()
{
  memset(&fileHeader, 0, sizeof(TAuFileHeader)); // de la Kafka citire 
}
//---------------------------------------------------------------------------

bool SAuLoad::Load(unsigned char* data1, int dataSize1, unsigned char* data2, int dataSize2)
{
  __int8 temp;
  __int32 bytesRead = 0;
  __int32 toRead;

  // seek la inceputul datelor
  if (!seek(fileHeader.headerSize))
  {
   return false;
  }

  // citirea efectiva a datelor
  // prima bucata
  if (dataSize1 > 0)
  {
    if (!data1)
    {
      return false;
    }

    // sau cu try..catch ?
    while(bytesRead < dataSize1)
    {
      if (bytesRead + AU_LOADER_BUFFER_SIZE < dataSize1) toRead = AU_LOADER_BUFFER_SIZE;
      else toRead = dataSize1 - bytesRead;

      if (!read(buffer, toRead))
      {
        return false;
      }

      // byte reversal, daca avem 16 biti
      if (info.bitsPerSample == 16)
      {
        if (toRead%2 == 1)
        {
          // Houston, we HAVE a problem 
        }

        for(int i = 0; i < toRead / 2; i++)
        {
          temp              = buffer[i * 2 + 1];
          buffer[i * 2 + 1] = buffer[i * 2];
          buffer[i * 2]     = temp;
        }
      }

      // transfer in data1
      memcpy(data1 + bytesRead, buffer, toRead);

      bytesRead += toRead;
    }
  }

  // a doua bucata, daca e necesar
  if (dataSize2 > 0)
  {
    if (!data2)
    {
      return false;
    }

    bytesRead = 0;
    while(bytesRead < dataSize1)
    {
      if (bytesRead + AU_LOADER_BUFFER_SIZE < dataSize2) toRead = AU_LOADER_BUFFER_SIZE;
      else toRead = dataSize2 - bytesRead + 1;

      if (!read(buffer, toRead))
      {
        return false;
      }

      // byte reversal, daca avem 16 biti
      if (info.bitsPerSample == 16)
      {
        for(int i = 0; i < toRead / 2; i++)
        {
          temp              = buffer[i * 2 + 1];
          buffer[i * 2 + 1] = buffer[i * 2];
          buffer[i * 2]     = temp;
        }
      }

      // transfer in data1
      memcpy(data2 + bytesRead, buffer, toRead);

      bytesRead += toRead;
    }
  }

  return true;
}
//---------------------------------------------------------------------------

bool SAuLoad::ReadHeader()
{
  if (!rewind())
  {
    return false;
  }

  // citirea cimpurilor headerului; din cauza ca structurile pot fi aliniate dqord/qword, trebuie facuta citirea
  // fiecarui cimp individual
  if (!read(&fileHeader.id,         sizeof(__int32)))
  {
    return false;
  }

  if (!read(&fileHeader.headerSize, sizeof(__int32)))
  {
    return false;
  }

  if (!read(&fileHeader.dataSize,   sizeof(__int32)))
  {
    return false;
  }

  if (!read(&fileHeader.encoding,   sizeof(__int32)))
  {
    return false;
  }

  if (!read(&fileHeader.sampleRate, sizeof(__int32)))
  {
    return false;
  }

  if (!read(&fileHeader.channels,   sizeof(__int32)))
  {
    return false;
  }

  // headerul e in big-endian; deci trebuie inversati octetii
  fileHeader.id         = Swap32(fileHeader.id);
  fileHeader.headerSize = Swap32(fileHeader.headerSize);
  fileHeader.dataSize   = Swap32(fileHeader.dataSize);
  fileHeader.encoding   = Swap32(fileHeader.encoding);
  fileHeader.sampleRate = Swap32(fileHeader.sampleRate);
  fileHeader.channels   = Swap32(fileHeader.channels);

  // validare
  if (fileHeader.id != AU_FILE_ID)
  {
    return false;
  }

  // completez structura TSndInfo
  info.channels      = fileHeader.channels;
  info.sampleRate    = fileHeader.sampleRate;
  if (fileHeader.sampleRate == 0)
  {
    return false;
  }

  switch(fileHeader.encoding)
  {
    case AU_FILE_ENCODING_LINEAR_8     :
      info.bitsPerSample = 8;
      break;

    case AU_FILE_ENCODING_LINEAR_16    :
      info.bitsPerSample = 16;
      break;

    case AU_FILE_ENCODING_LINEAR_24    :
    case AU_FILE_ENCODING_LINEAR_32    :
    case AU_FILE_ENCODING_FLOAT        :
    case AU_FILE_ENCODING_DOUBLE       :
    case AU_FILE_ENCODING_ADPCM_G721   :
    case AU_FILE_ENCODING_ADPCM_G722   :
    case AU_FILE_ENCODING_ADPCM_G723_3 :
    case AU_FILE_ENCODING_MULAW_8      :
    case AU_FILE_ENCODING_ALAW_8       :
    case AU_FILE_ENCODING_ADPCM_G723_5 :
      return false;

    default :
      return false;
  }

  info.blockAlign    = info.channels * (info.bitsPerSample / 8);
  info.length        = length() - fileHeader.headerSize;

  if (fileHeader.channels > 0)
  {
    info.time = (float)((info.length / (info.channels * (info.bitsPerSample / 8))) * (1.0 / (float)info.sampleRate)); // complicat, nu ? :)
  }  
  else info.time = 0.0;

  return true;
}
//---------------------------------------------------------------------------


