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
//  Unit              : Serializable classes (Serializable.h)
//
//  Versiune          : 1.0
//
//  Descriere         :
//    * contine clasa de baza (abstracta) pentru serializare precum si
//      citeva clase uzuale (wrappere pentru int, float, string) ce pot fi
//      serializate
//
//  Istorie           :
//    [02.10.2000] - [Karg] - unitul a fost creat
//    [03.10.2000] - [Karg] - mici modificari (& la parametrul CArchive)
//-----------------------------------------------------------------------------

//---------------------------------------------------------------------------
#ifndef SerializableH
#define SerializableH
//---------------------------------------------------------------------------

//#include "Archive.h"
#include "Stream_ANSI_C.h"
#include "PP_Counter.h"
#include "basic2d.h"
#include "PP_String.h"

// clasa de baza
class CSerializable
{
  private:

  protected:

  public:
    CSerializable() {};

    virtual void Serialize(PP::Stream& _a) = 0;
    virtual unsigned __int32 DataSize() { return 0; };
    virtual void DeSerialize(PP::Stream& _a) = 0;

    ~CSerializable() {};
};

// un wrapper pentry int, serializabil
class CSerInt : public CSerializable
{
  private:
    int mData;

  protected:

  public:
    CSerInt(int _data) {mData = _data;};
    CSerInt() {};

    virtual void Serialize(PP::Stream& _a);
    virtual unsigned __int32 DataSize();
    virtual void DeSerialize(PP::Stream& _a);

    int Get() { return mData; };

    ~CSerInt() {};
};

// un wrapper pentry float, serializabil
class CSerFloat : public CSerializable
{
  private:
    float mData;

  protected:

  public:
    CSerFloat(float _data) {mData = _data;};

    virtual void Serialize(PP::Stream& _a);
    virtual unsigned __int32 DataSize();
    virtual void DeSerialize(PP::Stream& _a);

    float Get() { return mData; };

    ~CSerFloat() {};
};

// un wrapper pentry string, serializabil
class CSerString : public CSerializable
{
  private:
    CPString mData;

  protected:

  public:
    CSerString(CPString _data) {mData = _data;}
    CSerString() { mData = ""; };

    virtual void Serialize(PP::Stream& _a);
    virtual unsigned __int32 DataSize();
    virtual void DeSerialize(PP::Stream& _a);

    CPString Get() { return mData; };

    ~CSerString() {};
};

// un wrapper pentry string, serializabil
class CSerWString : public CSerializable
{
  private:
	std::wstring mData;

  protected:

  public:
	CSerWString(std::wstring _data) {mData = _data;}
	CSerWString() { mData = L""; };

	virtual void Serialize(PP::Stream& _a);
	virtual unsigned __int32 DataSize();
	virtual void DeSerialize(PP::Stream& _a);

	std::wstring Get() { return mData; };

    ~CSerWString() {};
};

class CSerCounter : public CSerializable
{
  private:
    CPCounter mData;

  protected:

  public:
    CSerCounter(CPCounter _data) {mData = _data;};
    CSerCounter() {};

    virtual void Serialize(PP::Stream& _a);
    virtual unsigned __int32 DataSize();
    virtual void DeSerialize(PP::Stream& _a);

    CPCounter Get() { return mData; };

    ~CSerCounter() {};

    CSerCounter& operator=(const CSerCounter&);
};

class CSerPoint : public CSerializable
{
  private:
    CPPoint mData;

  protected:

  public:
    CSerPoint(CPPoint _data) {mData = _data;};
    CSerPoint() {};

    virtual void Serialize(PP::Stream& _a);
    virtual unsigned __int32 DataSize();
    virtual void DeSerialize(PP::Stream& _a);

    CPPoint Get() { return mData; };

    ~CSerPoint() {};

    CSerPoint& operator=(const CSerPoint&);
};

class CSerRect : public CSerializable
{
  private:
    CPRect mData;

  protected:

  public:
    //CSerRect(CPRect _data) { mData = _data; }
    CSerRect(const CPRect& _data) { mData = _data; }
    CSerRect() {};

    virtual void Serialize(PP::Stream& _a);
    virtual unsigned __int32 DataSize();
    virtual void DeSerialize(PP::Stream& _a);

    CPRect Get() { return mData; };

    ~CSerRect() {};

    CSerRect& operator=(const CSerRect&);
};

#endif
