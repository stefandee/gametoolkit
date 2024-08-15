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
//-----------------------------------------------------------------------------
//  Unit              : CfgFile.cpp
//
//  Versiune          : 1.0
//
//  Descriere         :
//    * clasa pentru manipulat un fisier de configurare
//    * fisierul de configurare este binar
//  Istorie           :
//    [07.12.1999] - [Karg] - a fost creat si fisierul si clasa
//    [23.02.2000] - [Karg] - am rescris citeva metode si am integrat clasa in clasa CCfg
//-----------------------------------------------------------------------------

#ifndef CfgBinH
#define CfgBinH
//---------------------------------------------------------------------------

#include "Cfg.h"

// defines
#define CFG_FILEID           "PIRON BIN CFG"
#define CFG_FILEID_SIZE      13
#define CFG_DESCRIPTION_SIZE 32
#define CFG_VERSION_MAJOR    0
#define CFG_VERSION_MINOR    40
#define CFG_BUFFER_SIZE      1024
//---------------------------------------------------------------------------

// formatul fisierului de configurare

// headerul
typedef struct
{
  char fileId[CFG_FILEID_SIZE];
  char description[CFG_DESCRIPTION_SIZE];
  int  versionMajor, versionMinor;
} TCfgFileHeader;

// o linie de configurare
typedef struct
{
  char           resName[CFG_RESNAME_SIZE];
  TResourceType  resType;
} TCfgLine;
//---------------------------------------------------------------------------

// clasa
class CCfgBin : public CCfg
{
  private:
	char  buffer[CFG_BUFFER_SIZE];

  protected:
    virtual bool ReadHeader();

  public:
    CCfgBin();

    // header
    bool Verify();
    bool GetInfo(char *description, int *versionMajor, int *versionMinor);
    bool SetInfo(char *description);

    // resources
    virtual bool GetResource(const char *resName, void *value);
    virtual bool SetResource(const char *resName, void *value);
    virtual bool AddResource(const char *resName, TResourceType resType, void *value);
    virtual bool DeleteResource(const char *resName);

    // stergere
    virtual bool Empty();

    ~CCfgBin();

};


#endif
