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
/*---------------------------------------------------------------------------
 application: Pure Power

 description: Class Piron Component Interface Object
 last modify: 17 04 00
        by grabX
---------------------------------------------------------------------------*/
//---------------------------------------------------------------------------
#ifndef PP_CObjectH
#define PP_CObjectH

#include "PP_String.h"
#include "PP_GuiDef.h"
#include <functional>

class CPIObject
{
   public:
      virtual CPString ClassName () {return PICN_OBJECT;}
      virtual bool IsDerivedFrom(CPString className);
      virtual void Update ();

  protected:
    //## Constructors
      CPIObject();
  public:
    //## Virtual Destructor
      virtual ~CPIObject();

  public:
    //## Get and Set Operations for Class Attributes (generated)
      int GetTag () ;
      void SetTag (int value);

      void * GetVoidTag () ;
      void SetVoidTag (void * value);

      CPString GetName () ;
      void SetName (CPString value);

  private: //## implementation
      int mTag;                  //integer tag - u never know when u might need it
      void *mVoidTag;            //void * tag  - it might prove useful
      CPString mName;            //this is not class name
};

/////////////////////////////////////////////////////////////////////////////
//GET-SET INLINE SECTION
/////////////////////////////////////////////////////////////////////////////

// Class CPIObject
inline int CPIObject::GetTag ()
{
  return mTag;
}

inline void CPIObject::SetTag (int value)
{
  mTag = value;
}

inline void * CPIObject::GetVoidTag ()
{
  return mVoidTag;
}

inline void CPIObject::SetVoidTag (void * value)
{
  mVoidTag = value;
}

inline CPString CPIObject::GetName ()
{
  return mName;
}

inline void CPIObject::SetName(CPString value)
{
  mName = value;
}
//---------------------------------------------------------------------------
#endif

