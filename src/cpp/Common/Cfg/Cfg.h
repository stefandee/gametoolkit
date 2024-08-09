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
//  Unit              : tipuri generale si clasa de baza pentru fisierele de configurare (Cfg)
//
//  Versiune          : 1.0
//
//  Descriere         :
//    * un singur tip, deocamdata
//    * headerul e "share'uit" de CfgBinFile si CfgTxtFile
//
//  Istorie           :
//    [21.02.2000] - [Karg] - a fost creat
//    [23.02.2000] - [Karg] - am scris metodele clasei CCfg
//                          - am eliminat un bug la Open (fisierele erau deschise RDONLY, trebuia RDWR)
//-----------------------------------------------------------------------------

//---------------------------------------------------------------------------
#ifndef CfgH
#define CfgH

#define FILE_EXT         ".pCfg"
#define CFG_RESNAME_SIZE 1024

// tipul resursei
enum TResourceType {RES_NOTHING, RES_CHAR, RES_INT, RES_DOUBLE, RES_TPOINT, RES_TRECT, RES_STRING32, RES_STRING64, RES_STRING256};

class CCfg
{
  private:

  protected:
    int fileHandle;
    virtual bool ReadHeader();

  public:
    CCfg();

    virtual bool Open(const char* fileName);
    virtual bool Create(const char* fileName);

    // resources
    virtual bool GetResource(const char* resName, void* value);
    virtual bool SetResource(const char* resName, void* value);
    virtual bool AddResource(const char* resName, TResourceType resType, void* value);
    virtual bool DeleteResource(const char* resName);

    // stergere
    virtual bool Empty();

    // metode de inchidere
    virtual bool Close();

    ~CCfg();
};

#endif
