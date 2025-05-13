//-----------------------------------------------------------------------------
// This file is part of PPTactical Engine.                                   //
//                                                                           //
// PPTactical Engine - engine for tactical/strategy games                    //
// Copyright (C) 1998-2004 Stefan Dicu & Tudor Girba                         //
//                                                                           //
// PPTactical Engine is free software; you can redistribute it               //
// and/or modify it under the terms of the GNU Lesser General Public License //
// as published by the Free Software Foundation; either version 2.1 of the   //
// License, or (at your option) any later version.                           //
//                                                                           //
// PPTactical Engine is distributed in the hope that it will be useful, but  //
// WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY//
// or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public    //
// License for more details.                                                 //
//                                                                           //
// You should have received a copy of the GNU Lesser General Public License  //
// along with PPTactical Engine; if not, write to the Free Software          //
// Foundation Inc. 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA   //
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//  Unit              : XML-based config file
//
//  Version           : 1.0
//
//  Description       :
//    * offers interface to custom config in XML format
//    * it's not completed yet (missing rectangles/points/doubles)
//
//  History           :
//    [12.10.2005] - [Karg] - created
//-----------------------------------------------------------------------------

#ifndef CfgXmlH
#define CfgXmlH
//---------------------------------------------------------------------------

#include "tinyxml2.h"
#include "Cfg.h"

class CCfgXml : public CCfg
{
  public:
    CCfgXml();
    ~CCfgXml();

  public:
    virtual bool GetResource(const char* resName, void* value);
    virtual TResourceType GetResourceType(const char* resName);
    virtual bool SetResource(const char* resName, void* value);
    virtual bool AddResource(const char* resName, TResourceType resType, void* value);
    virtual bool DeleteResource(const char* resName);

  public:
    virtual bool Open(const char* fileName);
    virtual bool Create(const char* fileName);
    virtual bool Empty();
    virtual bool Close();

  private:
    tinyxml2::XMLDocument mXmlDoc;
};

#endif
