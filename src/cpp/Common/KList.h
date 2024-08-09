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
//  Unit              : Template List (list.h)
//
//  Versiune          : 1.0
//
//  Descriere         :
//    * contine o clasa template pentru lucrul cu liste
//    * sint destul de multe obiecte in joc care trebuie sa fie grupate in liste
//      si, de aceea, e nevoie de o astfel de clasa
//    * s'ar putea sa apara si un unit cu Iteratori
//    * din motive necunoscute mie inca, codul template'ului tre' sa fie in acelasi
//      modul cu declaratia - nu e frumos, dar macar compileaza :)
//
//  Istorie           :
//    [Karg] - [9.04.2000] - a fost creat
//-----------------------------------------------------------------------------

//---------------------------------------------------------------------------
#ifndef KListH
#define KListH
//---------------------------------------------------------------------------

#include <stddef.h>
#include "easylogging++.h"

#ifdef _MSC_VER	// Standard workaround for Microsoft compiler bug
#define for if(0) {} else for
#endif

// template pentru lista normala, alocata static
template <class T> class CList
{
  private :

  protected :
    T*  mData;
    int mSizeMax;
    int mItems;

  public :
    CList(int _capacity);

    int Count();
    T*  Get(int _index);
    int  Add(T _item);
    void Delete(int _index);
    void Clear();

    ~CList();
};

template <class T> CList<T>::CList(int _capacity)
{
  mData     = NULL;
  mData     = new T[_capacity];
  mSizeMax  = _capacity;
  mItems    = 0;
}
//---------------------------------------------------------------------------

template <class T>int CList<T>::Count()
{
  return mItems;
}
//---------------------------------------------------------------------------

template <class T>T* CList<T>::Get(int _index)
{
  return &mData[_index];
}
//---------------------------------------------------------------------------

template <class T>int CList<T>::Add(T _item)
{
  if (mItems + 1 > mSizeMax)
  {
    return -1;
  }

  mData[mItems] = _item;

  mItems++;
  return mItems - 1; // indicele nou inclus
}
//---------------------------------------------------------------------------

template <class T> void CList<T>::Delete(int _index)
{
  if ((mItems == 0) || (mItems <= _index) || (_index < 0))
  {
    return;
  }

  mData[_index] = mData[mItems - 1];

  mItems--;
}
//---------------------------------------------------------------------------

template <class T>void CList<T>::Clear()
{
  delete [] mData;
  mItems = 0;
  mData  = new T[mSizeMax];
}
//---------------------------------------------------------------------------

template <class T> CList<T>::~CList()
{
  delete [] mData;
}
//---------------------------------------------------------------------------

// template pentru lista normala, alocata static
template <class T> class CPList
{
  private :

  protected :
    T**  mData;
    int  mSizeMax;
    int  mItems;

  public :
    CPList(int _capacity);

    int Count();
    T*  Get(int _index);
    int  Add(T* _item);
    void Delete(int _index);
    void Remove(int _index);
    bool Contain(T* _item);
    void Insert(T* _item, int _pos);
    void Clear();
    //void AddListUnique(CPList<T>* aList);

    ~CPList();
};

template <class T> CPList<T>::CPList(int _capacity)
{
  mData    = new T*[_capacity];
  mItems   = 0;
  mSizeMax = _capacity;

  for(int i = 0; i < mSizeMax; i++)
  {
    mData[i] = NULL;
  }
}
//---------------------------------------------------------------------------

template <class T>int CPList<T>::Count()
{
  return mItems;
}
//---------------------------------------------------------------------------

template <class T>T* CPList<T>::Get(int _index)
{
  //logWriteLn("CPList::Get - called");

  if ((_index < 0) || (_index >= mItems))
  {
    return NULL;
  }

  return mData[_index];
}
//---------------------------------------------------------------------------

template <class T>int CPList<T>::Add(T* _item)
{
  //logWriteLn("CPList::Add - called");

  if (mItems + 1 >= mSizeMax)
  {
    // time to reallocate
    //logWriteLn("Reallocating ");
    //logWriteILn(mSizeMax);
    T** mData2 = new T*[mSizeMax + 16];

    for(int i = 0; i < mSizeMax + 16; i++)
    {
      mData2[i] = NULL;
    }

    for(int i = 0; i < mItems; i++)
    {
      mData2[i] = mData[i];
    }

    delete [] mData;

    mData = mData2;
    mSizeMax += 16;
  }

  mData[mItems] = _item;

  mItems++;

  return mItems - 1;
}
//---------------------------------------------------------------------------

template <class T>void CPList<T>::Delete(int _index)
{
  //logWriteLn("CPList::Delete - called");

  if ((mItems == 0) || (mItems <= _index) || (_index < 0))
  {
    return;
  }

  if (!mData[_index])
  {
    LOG(ERROR) << "Trying to delete a null pointer...";
    return;
  }

  delete mData[_index];

  for (int i = _index; i < Count() - 1; i++)
  {
    mData[i] = mData[i+1];
  }

  mData[mItems - 1] = NULL;

  /*
  mData[_index] = mData[mItems - 1];
  mData[mItems - 1] = NULL;
  */

  mItems--;
}
//---------------------------------------------------------------------------

template <class T>void CPList<T>::Remove(int _index)
{
  //logWriteLn("CPList::Remove - called");

  if ((mItems == 0) || (mItems <= _index) || (_index < 0))
  {
    return;
  }

  for (int i = _index; i < Count() - 1; i++)
  {
    mData[i] = mData[i+1];
  }

  mData[mItems - 1] = NULL;
  /*
  mData[_index] = mData[mItems - 1];
  mData[mItems - 1] = NULL;
  */

  mItems--;
}
//---------------------------------------------------------------------------

template<class T> bool CPList<T>::Contain(T* _item)
{
  //logWriteLn("CPList::Contain - called");

  if (_item == NULL)
  {
    return false;
  }

  for (int i = 0, listCount = Count(); i < listCount; i++)
  {
    if (_item == mData[i])
    {
      return true;
    }
  }

  return false;
}
//---------------------------------------------------------------------------

template <class T>void CPList<T>::Clear()
{
  //logWriteLn("CPList::Clear - called");

  for(int i = 0; i < mItems; i++)
  {
    //delete mData[i];
    mData[i] = NULL;
  }

  mItems = 0;
}
//---------------------------------------------------------------------------

template <class T>void CPList<T>::Insert(T* _item, int _pos)
{
  //logWriteLn("CPList::Insert - called");

  if (mItems + 1 >= mSizeMax)
  {
    return;
  }

  if (_pos >= mItems)
  {
    return;
  }

  mItems++;

  for (int i = Count() - 1; i > _pos; i--)
  {
    mData[i] = mData[i - 1];
  }

  mData[_pos] = _item;
}
//---------------------------------------------------------------------------

template <class T> CPList<T>::~CPList()
{
  //Clear();
  //logWriteLn("CPList::Insert - called");
  try
  {
    delete [] mData;
  }
  catch(...)
  {
    LOG(ERROR) << "CPList::~CPList - failed to dealocate";
  }
}
//---------------------------------------------------------------------------

/*
template <class T>void CPList<T>::AddListUnique(CPList<T>* aList)
{
  for (int i = 0, listCount = aList->Count(); i < listCount; i++)
  {
      if (!Contain(aList[i]))
      {
          Add(aList[i]);
      }
  }
}
//---------------------------------------------------------------------------
*/

#endif
