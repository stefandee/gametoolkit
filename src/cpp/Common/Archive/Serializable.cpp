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
#include <string.h>
#pragma hdrstop

#include "Serializable.h"

//---------------------------------------------------------------------------
#ifdef __BORLANDC__
#pragma package(smart_init)
#endif

// int thingies
void CSerInt::Serialize(PP::Stream& _a)
{
  _a.writeInt(&mData);
}
//---------------------------------------------------------------------------

unsigned __int32 CSerInt::DataSize()
{
  return sizeof(mData);
}
//---------------------------------------------------------------------------

void CSerInt::DeSerialize(PP::Stream& _a)
{
  _a.readInt(&mData);
}
//---------------------------------------------------------------------------

// float thingies
void CSerFloat::Serialize(PP::Stream& _a)
{
  _a.writeFloat(&mData);
}
//---------------------------------------------------------------------------

unsigned __int32 CSerFloat::DataSize()
{
  return sizeof(mData);
}
//---------------------------------------------------------------------------

void CSerFloat::DeSerialize(PP::Stream& _a)
{
  _a.readFloat(&mData);
}
//---------------------------------------------------------------------------

// string thingies
void CSerString::Serialize(PP::Stream& _a)
{
  if (mData.Length() <= 0)
  {
    CSerInt(0).Serialize(_a);
    return;
  }

  CSerInt(mData.Length()).Serialize(_a);

  char* lTemp = new char[mData.Length() + 1];

  strcpy(lTemp, mData.c_str());

  _a.write(lTemp, mData.Length() + 1);

  delete [] lTemp;
}
//---------------------------------------------------------------------------

unsigned __int32 CSerString::DataSize()
{
  return mData.Length() + sizeof(int);
}
//---------------------------------------------------------------------------

void CSerString::DeSerialize(PP::Stream& _a)
{
  char*    lTempData;
  CSerInt  lTempInt(0);
  //CPString lTempString;

  lTempInt.DeSerialize(_a);

  if (lTempInt.Get() == 0)
  {
    return;
  }

  try
  {
    lTempData = new char[lTempInt.Get() + 1];
  }
  catch(...)
  {
    //delete [] lTempData;
    return;
  }

  if (lTempInt.Get() != 0)
  {
    _a.read(lTempData, lTempInt.Get() + 1);
    mData = CPString(lTempData);
  }
  else
  {
    mData = "";
  }

  delete [] lTempData;
}
//---------------------------------------------------------------------------

// wstring thingies
void CSerWString::Serialize(PP::Stream& _a)
{
  int dataLength = mData.length();

  if (dataLength <= 0)
  {
	CSerInt(0).Serialize(_a);
	return;
  }

  CSerInt(dataLength).Serialize(_a);

  _a.write((void*)mData.c_str(), sizeof(wchar_t) * (dataLength + 1));
}
//---------------------------------------------------------------------------

unsigned __int32 CSerWString::DataSize()
{
  return (mData.length() + 1) * sizeof(wchar_t) + sizeof(int);
}
//---------------------------------------------------------------------------

void CSerWString::DeSerialize(PP::Stream& _a)
{
  wchar_t*    lTempData;
  CSerInt  lTempInt(0);
  //CPString lTempString;

  lTempInt.DeSerialize(_a);

  if (lTempInt.Get() == 0)
  {
	return;
  }

  try
  {
	lTempData = new wchar_t[lTempInt.Get() + 1];
  }
  catch(...)
  {
	//delete [] lTempData;
	return;
  }

  if (lTempInt.Get() != 0)
  {
	_a.read(lTempData, lTempInt.Get() + 1);
	mData = std::wstring(lTempData);
  }
  else
  {
	mData = L"";
  }

  delete [] lTempData;
}
//---------------------------------------------------------------------------

void CSerCounter::Serialize(PP::Stream& _a)
{
  int   lCounter, lMinValue, lMaxValue;
  bool  lCircular, lMaxReached, lMinReached, lInfinite;

  lCounter    = mData.GetCounter();
  lMinValue   = mData.GetMinValue();
  lMaxValue   = mData.GetMaxValue();
  lCircular   = mData.GetCircular();
  lMinReached = mData.GetMinReached();
  lMaxReached = mData.GetMaxReached();
  lInfinite   = mData.GetInfinite();

  _a.writeInt(&lCounter);
  _a.writeInt(&lMinValue);
  _a.writeInt(&lMaxValue);
  _a.writeBool(&lCircular);
  _a.writeBool(&lMaxReached);
  _a.writeBool(&lMinReached);
  _a.writeBool(&lInfinite);
}
//---------------------------------------------------------------------------

unsigned __int32 CSerCounter::DataSize()
{
  return 3*sizeof(__int32) + 4*sizeof(__int8);
}
//---------------------------------------------------------------------------

void CSerCounter::DeSerialize(PP::Stream& _a)
{
  int   lCounter, lMinValue, lMaxValue;
  bool  lCircular, lMaxReached, lMinReached, lInfinite;

  _a.readInt(&lCounter);
  _a.readInt(&lMinValue);
  _a.readInt(&lMaxValue);
  _a.readBool(&lCircular);
  _a.readBool(&lMaxReached);
  _a.readBool(&lMinReached);
  _a.readBool(&lInfinite);

  mData.SetMaxValue(lMaxValue);
  mData.SetMinValue(lMinValue);
  mData.SetCounter(lCounter);
  mData.SetCircular(lCircular);
  mData.SetMinReached(lMinReached);
  mData.SetMaxReached(lMaxReached);
  mData.SetInfinite(lInfinite);
}
//---------------------------------------------------------------------------

CSerCounter& CSerCounter::operator=(const CSerCounter& _data)
{
  if (&_data == this)
  {
    return *this;
  }

  mData = _data.mData;
  return *this;
}
//---------------------------------------------------------------------------

void CSerPoint::Serialize(PP::Stream& _a)
{
  int lx = mData.x, ly = mData.y;

  _a.writeInt(&lx);
  _a.writeInt(&ly);
}
//---------------------------------------------------------------------------

unsigned __int32 CSerPoint::DataSize()
{
  return 2 * sizeof(__int32);
}
//---------------------------------------------------------------------------

void CSerPoint::DeSerialize(PP::Stream& _a)
{
  int lx, ly;

  _a.readInt(&lx);
  _a.readInt(&ly);

  mData = CPPoint(lx, ly);
}
//---------------------------------------------------------------------------

CSerPoint& CSerPoint::operator=(const CSerPoint& _data)
{
  if (&_data == this)
  {
    return *this;
  }

  mData = _data.mData;
  return *this;
}
//---------------------------------------------------------------------------

void CSerRect::Serialize(PP::Stream& _a)
{
  _a.writeInt(&mData.left);
  _a.writeInt(&mData.right);
  _a.writeInt(&mData.top);
  _a.writeInt(&mData.bottom);
}
//---------------------------------------------------------------------------

unsigned __int32 CSerRect::DataSize()
{
  return 4 * sizeof(__int32);
}
//---------------------------------------------------------------------------

void CSerRect::DeSerialize(PP::Stream& _a)
{
  _a.readInt(&mData.left);
  _a.readInt(&mData.right);
  _a.readInt(&mData.top);
  _a.readInt(&mData.bottom);
}
//---------------------------------------------------------------------------

CSerRect& CSerRect::operator=(const CSerRect& _data)
{
  if (&_data == this)
  {
    return *this;
  }

  mData = _data.mData;
  return *this;
}
//---------------------------------------------------------------------------

