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
//  Unit              : text configuration files manager (CfgTxtFile)
//
//  Versiune          : 1.0
//
//  Descriere         :
//    * structuri de date si clase pentru manipularea unui fisier de configurare
//      de tip text
//    * clasa CTxtCfg contine un mini compilator, de fapt; in plus, va completa
//      o tabela de simboluri, care va fi accesata prin functiile de acces la date
//    * exista urmatoarele elemente de baza :
//      header : PIRON TXT CFG FILE
//      group  : [GROUP ID]
//      resource : RESNAME = VALUE
//      comentariu : <;>COMENTARIU
//    * poate fi scrisa si o gramatica (cu mici exceptii corecta dpv al conventiei BNF) :
//        CfgFile  ::= Group*
//        Group    ::= GroupHdr Resource*
//        GroupHdr ::= <[> <IDENT> <]>
//        Resource ::= <IDENT> <=> StaticValue
//        StaticValue ::= Const_Numerica | Const_Sir | Point | Rect
//        Const_Numerica ::= Const_Int | Const_Real
//        Const_Sir ::= <"> IDENT <">
//        Point ::= <POINT> Const_Int Const_Int                           
//        Rect ::= <RECT> Const_Int Const_Int Const_Int Const_Int
//    * BUG la citire din fisier : daca nu exista inca un caracter dupa ultimul
//      atom, nu citeste corect resursa 
//
//  Istorie           :
//    [21.02.2000] - [Karg] - a fost creat fisierul
//    [14.04.2000] - [Karg] - am corectat bugul cu citirea unei valori - era din
//                            cauza BackChar();
//    [11.06.2005] - Karg - fixed 2 bugs related to RECT and POINT reading
//-----------------------------------------------------------------------------

//---------------------------------------------------------------------------
#ifndef CfgTxtH
#define CfgTxtH
//---------------------------------------------------------------------------

#include "Cfg.h"

#define CFG_SYMBOLS 1024

// valorile
// enum TCfgVal {CFGV_INT, CFGV_REAL, CFGV_STRING};

// codurile atomilor folositi
#define CFGA_NOTHING       99
#define CFGA_BRACKET_LEFT  100
#define CFGA_BRACKET_RIGHT 101
#define CFGA_EQUAL         102
#define CFGA_COMMENT       103
#define CFGA_KEYWORD       104
#define CFGA_IDENT         105
//#define CFGA_GHILIMEA      106
//#define CFGA_DOT           107
#define CFGA_STRING        108
#define CFGA_INT           109
#define CFGA_REAL          110
#define CFGA_EOF           98

// atom-valoare
typedef struct
{
  __int32 lexCode;
  void*   value;
} TCfgAtomValue;

// tabela de simboluri - resursa = valoare
typedef struct
{
  char          resName[CFG_RESNAME_SIZE + 1];
  TResourceType resType;
  void*         value;
} TResources;

class CCfgTxt : public CCfg
{
  private:
    TCfgAtomValue av;

  protected:
    int  keywordsCount;
    char cfgKeywords[64][1024];
    
    int        symbols;
    TResources symbolTable[CFG_SYMBOLS];

    // analiza sintactica cu descendenti recursivi
    bool                  PointDecl (const char*);
    bool                  RectDecl  (const char*);
    bool                  Resources ();
    bool                  Groups    ();
    bool                  Group     ();
    virtual bool          CfgASin   ();

    // analiza lexicala (alex manual)
    bool                  IsKeyWord (char* name);
    virtual int           NextChar  ();
    virtual int           BackChar  ();
    virtual bool          CfgALex   ();

    // tabela de simboluri
    bool AddDouble (char* resName, char* value);
    bool AddInt    (char* resName, char* value);
    bool AddPoint  (char* resName, __int32 x, __int32 y);
    bool AddRect   (char* resName, __int32 left, __int32 top, __int32 right, __int32 bottom);
    bool AddString (char* resName, char *value);

    virtual bool          ReadHeader();

  public:
    CCfgTxt();

    virtual bool GetResource(const char* resName, void* value);
    virtual TResourceType GetResourceType(const char* resName);
    virtual bool SetResource(const char* resName, void* value);
    virtual bool AddResource(const char* resName, TResourceType resType, void* value);
    virtual bool DeleteResource(const char* resName);

    ~CCfgTxt();
};


#endif
