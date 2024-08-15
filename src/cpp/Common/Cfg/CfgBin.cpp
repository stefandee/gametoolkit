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
#include <cstring>
#include <cstdlib>
#include "Platform.h"

#ifdef PPT_PLATFORM_WIN
#include <io.h>
#endif

//#if defined(__SUNPRO_CC) || defined(__GNUG__) 
#if defined(PPT_PLATFORM_LINUX) || defined(PPT_PLATFORM_SOLARIS)
#include <unistd.h>
#endif

#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>
#include <stdio.h>
#pragma hdrstop

#include "config.h"
#include "basic2d.h"
#include "CfgBin.h"

//---------------------------------------------------------------------------
#ifdef __BORLANDC__
#pragma package(smart_init)
#endif

// Descriere   : constructor
// Param       : n/a
// Rezultat    : n/a
// Comentarii  : n/a
CCfgBin::CCfgBin()
{
}
//---------------------------------------------------------------------------

// Descriere   : verifica daca fisierul de configurare este corect (are id'ul la inceput)
// Param       : fileName - fisierul
// Rezultat    : true/false - fisierul e valid sau nu
// Comentarii  : n/a
bool CCfgBin::ReadHeader()
{
  if (fileHandle > 0)
  {
    // merg la inceputul fisierului
    if (lseek(fileHandle, 0, SEEK_SET) == -1) return false;

    TCfgFileHeader header;

    // citesc headerul
    int size = read(fileHandle, &header, sizeof(header));

    // verific eroare de citire
    if (size < 0) return false;

    // headerul se potriveste ?
    if ( (strncmp(header.fileId, CFG_FILEID, CFG_FILEID_SIZE) == 0) &&
         (header.versionMajor == CFG_VERSION_MAJOR) &&
         (header.versionMinor == CFG_VERSION_MINOR)) return true;
  }

  return false;
}
//---------------------------------------------------------------------------

// Descriere   : extrage din fisierul de configurare descrierea si versiunea
// Param       : description - descrierea, versionMajor/Minor - versiunea
// Rezultat    : true/false
// Comentarii  : n/a
bool CCfgBin::GetInfo(char* description, int* versionMajor, int* versionMinor)
{
  if (fileHandle > 0)
  {
	// merg la inceputul fisierului
    if (lseek(fileHandle, 0, SEEK_SET) == -1) return false;

    TCfgFileHeader header;

    // citesc headerul
    int size = read(fileHandle, &header, sizeof(header));

    if (size >= 0)
    {
      // citirea headerului a fost succes, returnez informatiile
      strncpy(description, header.description, CFG_DESCRIPTION_SIZE);
      *versionMajor = header.versionMajor;
      *versionMinor = header.versionMinor;
      
      return true;
    }
  }

  return false;
}
//---------------------------------------------------------------------------

// Descriere   : seteaza informatiile din header
// Param       : description - descrierea fisierului de configurare
// Rezultat    :
// Comentarii  :
bool CCfgBin::SetInfo(char* description)
{
  if (fileHandle > 0)
  {
    // merg la inceputul fisierului
    if (lseek(fileHandle, 0, SEEK_SET) == -1) return false;

    // completez headerul
    TCfgFileHeader header;

    strcpy(header.fileId, CFG_FILEID);

    // ca sa nu apara gunoaie in fisier
    memset(header.description, '\0', CFG_DESCRIPTION_SIZE);
    
    int descLength = strlen(description);

    if (descLength > CFG_DESCRIPTION_SIZE - 1) descLength = CFG_DESCRIPTION_SIZE - 1;
    strncpy(header.description, description, descLength + 1);

    // set final, in cazul in care descrierea depaseste CFG_DESCRIPTION_SIZE caractere
    header.description[descLength] = '\0';

    header.versionMajor = CFG_VERSION_MAJOR;
    header.versionMinor = CFG_VERSION_MINOR;

    // scriu headerul
    if (write(fileHandle, &header, sizeof(header)) != -1) return true;
  }

  return false;
}
//---------------------------------------------------------------------------

// Descriere   : returneaza o valoare citita din fisier
// Param       : name - numele resursei; value - pointer la valoare
// Rezultat    : true/false - value e valida
// Comentarii  : pare un pic cam dubioasa organizarea in while (face niste citiri in plus din fisier)
bool CCfgBin::GetResource(const char* resName, void* value)
{
  if (fileHandle > 0 && (value != NULL))
  {
    // setez pointerul de fisier dupa header
    if (lseek(fileHandle, sizeof(TCfgFileHeader), SEEK_SET) == -1) return false;

    TCfgLine cfgLine;
    int size;

    // parcurg resursele secvential, pentru a gasi resursa cautata
    while(1)
    {
      // citesc headerul liniei
      size = read(fileHandle, &cfgLine, sizeof(TCfgLine));

      // eroare de citire / sfirsit de fisier
      if (size <= 0) return false;

      switch(cfgLine.resType)
      {
		case RES_CHAR :
          read(fileHandle, (char*)value, sizeof(char));
          break;
        case RES_INT  :
          read(fileHandle, (int*)value, sizeof(int));
          break;
        case RES_DOUBLE :
          read(fileHandle, (double*)value, sizeof(double));
          break;
        case RES_STRING32 :
          read(fileHandle, (char*)value, 32);
          break;
        case RES_STRING256 :
		  read(fileHandle, (char*)value, 256);
		  break;
		case RES_TPOINT :
		  read(fileHandle, (CPPoint*)value, sizeof(CPPoint));
		  break;
		case RES_TRECT :
		  read(fileHandle, (CPRect*)value, sizeof(CPRect));
		  break;
		default :
          return false;
      }

      if (strcmp(cfgLine.resName, resName) == 0) return true;
    }
  }

  return false;
}
//---------------------------------------------------------------------------

// Descriere   : seteaza valoarea unei resurse
// Param       : name - numele resursei, value - noua valoare
// Rezultat    : true/false
// Comentarii  : n/a
bool CCfgBin::SetResource(const char* resName, void* value)
{
  if (fileHandle > 0 && (value != NULL))
  {
    // setez pointerul de fisier dupa header
    if (lseek(fileHandle, sizeof(TCfgFileHeader), SEEK_SET) == -1) return false;

    TCfgLine cfgLine;
    int size;

    // parcurg resursele secvential, pentru a gasi resursa cautata
    while(1)
    {
      // citesc headerul liniei
      size = read(fileHandle, &cfgLine, sizeof(TCfgLine));

      // eroare de citire / sfirsit de fisier
      if (size <= 0) return false;

      if (strcmp(resName, cfgLine.resName) == 0)
      {
		switch(cfgLine.resType)
        {
          case RES_CHAR :
            if (write(fileHandle, (char*)value, sizeof(char)) == -1) return false;
            break;
		  case RES_INT  :
            if (write(fileHandle, (int*)value, sizeof(int)) == -1) return false;
            break;
          case RES_DOUBLE :
			if (write(fileHandle, (double*)value, sizeof(double)) == -1) return false;
            break;
          case RES_STRING32 :
			strcpy_s(buffer, CFG_BUFFER_SIZE, (char*)value);
			if (write(fileHandle, buffer, 32) == -1) return false;
			break;
		  case RES_STRING256 :
			strcpy_s(buffer, CFG_BUFFER_SIZE, (char*)value);
			if (write(fileHandle, buffer, 256) == -1) return false;
			break;
		  case RES_TPOINT :
			if (write(fileHandle, (CPPoint*)value, sizeof(CPPoint)) == -1) return false;
			break;
		  case RES_TRECT :
			if (write(fileHandle, (CPRect*)value, sizeof(CPRect)) == -1) return false;
			break;
		  default :
            return false;
        }

        return true;
      }

	  // sar peste un numar de octeti
      switch(cfgLine.resType)
      {
        case RES_CHAR :
          if (lseek(fileHandle,sizeof(char), SEEK_CUR) == -1) return false;
          break;
        case RES_INT  :
          if (lseek(fileHandle, sizeof(int), SEEK_CUR) == -1) return false;
          break;
        case RES_DOUBLE :
		  if (lseek(fileHandle, sizeof(double), SEEK_CUR) == -1) return false;
		  break;
        case RES_STRING32 :
		  if (lseek(fileHandle, 32 * sizeof(char), SEEK_CUR) == -1) return false;
		  break;
		case RES_STRING256 :
		  if (lseek(fileHandle, 256 * sizeof(char), SEEK_CUR) == -1) return false;
          break;
        case RES_TPOINT :
          if (lseek(fileHandle, sizeof(CPPoint), SEEK_CUR) == -1) return false;
          break;
        case RES_TRECT :
          if (lseek(fileHandle, sizeof(CPRect), SEEK_CUR) == -1) return false;
          break;
        default :
          return false;
      }
	}
  }

  return false;
}
//---------------------------------------------------------------------------

// Descriere   : adauga o resursa la sfirsitul fisierului de configurare
// Param       : name - numele resursei, resType - tipul, value - valoarea
// Rezultat    : true/false
// Comentarii  : n/a
bool CCfgBin::AddResource(const char* resName, TResourceType resType, void* value)
{
  if (fileHandle > 0 && (value != NULL))
  {
    // setez pointerul de fisier la sfirsitul fisierului
    if (lseek(fileHandle, 0, SEEK_END) == -1) return false;

    TCfgLine cfgLine;

    // set initial, ca sa nu apara gunoaie in fisier
    memset(cfgLine.resName, '\0', CFG_RESNAME_SIZE);

    int resNameLength = strlen(resName);
    if (resNameLength > CFG_RESNAME_SIZE - 1) resNameLength = CFG_RESNAME_SIZE - 1;
    strncpy(cfgLine.resName, resName, resNameLength + 1);

    // set final, in cazul in care resName depaseste CFG_RESNAME_SIZE caractere
    cfgLine.resName[resNameLength] = '\0';

    cfgLine.resType = resType;

    // scriu headerul
    int size = write(fileHandle, &cfgLine, sizeof(TCfgLine));

    // eroare de scriere ?
    if (size < 0) return false;

    // acum scriu valoarea efectiva
    switch(cfgLine.resType)
    {
      case RES_CHAR :
        if (write(fileHandle, (char*)value, sizeof(char)) == -1) return false;
		break;
      case RES_INT  :
		if (write(fileHandle, (int*)value, sizeof(int)) == -1) return false;
        break;
	  case RES_DOUBLE :
		if (write(fileHandle, (double*)value, sizeof(double)) == -1) return false;
        break;
	  case RES_STRING32 :
		strcpy_s(buffer, CFG_BUFFER_SIZE, (char*)value);
		if (write(fileHandle, buffer, 32) == -1) return false;
		break;
	  case RES_STRING256 :
		strcpy_s(buffer, CFG_BUFFER_SIZE, (char*)value);
        if (write(fileHandle, buffer, 256) == -1) return false;
        break;
      case RES_TPOINT :
        if (write(fileHandle, (CPPoint*)value, sizeof(CPPoint)) == -1) return false;
        break;
      case RES_TRECT :
        if (write(fileHandle, (CPRect*)value, sizeof(CPRect)) == -1) return false;
        break;
      default :
        return false;
    }

    return true;
  }

  return false;
}
//---------------------------------------------------------------------------

// Descriere   : sterge o resursa (o linie) din fisierul de configurare
// Param       : name - resursa pentru sters
// Rezultat    : true/false
// Comentarii  : neimplementata; numai un schelet de baza 
bool CCfgBin::DeleteResource(const char* resName)
{
  if (fileHandle < 0)
  {
    return false;
  }

  return false;
}
//---------------------------------------------------------------------------

// Descriere   : ramine numai headerul, resursele se duc
// Param       : n/a
// Rezultat    : true/false
// Comentarii  : n/a
bool CCfgBin::Empty()
{
  if (fileHandle > 0)
  {
    if (chsize(fileHandle, sizeof(TCfgFileHeader)) == 0) return true;
  }

  return false;
}
//---------------------------------------------------------------------------

// Descriere   : destructor, inchide fiserul deschis
// Param       : n/a
// Rezultat    : n/a
// Comentarii  : n/a
CCfgBin::~CCfgBin()
{
  Close();
}
//---------------------------------------------------------------------------

