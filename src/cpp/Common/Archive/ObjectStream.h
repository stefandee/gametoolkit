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
//  Unit              : Object Stream
//
//  Version           : 1.0
//
//  Description       :
//    * template class used to manage a stream of related objects (related = part
//      of a class hierarchy)
//    * it's a typed stream
//    * this is useful when planning to save/load in a file a number of related objects
//      (perhaps with an editor); concrete examples: managing a curve database file
//      (there are several class curves that may be written); managing a unit
//      database file (as units are childs of CIGBaseObject); managing a particle
//      effects database file
//    * the classes that are saved/loaded must have Serialize/Deserialize/ClassName methods
//      and must provide a factory-style class
//
//  History           :
//    [20.11.2004] - [Karg] - unit created
//-----------------------------------------------------------------------------

#ifndef ObjectStreamH
#define ObjectStreamH
//---------------------------------------------------------------------------

#include "Stream.h"
#include "Serializable.h"

template <class Base, class Factory> class ObjectStream
{
  public:
    ObjectStream(PP::Stream& _stream) : mStream(_stream) { mStreamStart = _stream.tell(); BuildStreamPos(); }
    ~ObjectStream() { delete mStreamPos; }

  public:
    virtual int   Count();
    virtual Base* Get(int _index);
    virtual void  Set(Base* _obj);

  protected:
    ObjectStream();

  private:
    void BuildStreamPos();

    PP::Stream& mStream;
    int         mCount;
    int         mStreamStart;
    int*        mStreamPos;
};

template <class Base, class Factory> int ObjectStream<Base, Factory>::Count()
{
  return mCount;
}
//---------------------------------------------------------------------------

template <class Base, class Factory> Base* ObjectStream<Base, Factory>::Get(int _index)
{
  if (_index >= mCount)
  {
    return 0;
  }

  mStream.seek(mStreamPos[_index]);

  CSerString lClassName;

  lClassName.DeSerialize(mStream);

  Base* lObj = Factory().Get(lClassName.Get());

  if (lObj)
  {
    lObj->DeSerialize(mStream);
  }

  return lObj;
}
//---------------------------------------------------------------------------

template <class Base, class Factory> void ObjectStream<Base, Factory>::BuildStreamPos()
{
  Factory lFactory;
  int     lStreamPosIndex = 0;

  mStreamPos = 0;
  mCount     = 0;

  // the stream is empty
  if (!mStream.rewind())
  {
    return;
  }

  mStream.readInt(&mCount);

  delete mStreamPos;
  mStreamPos = new int[mCount];

  for(int i = 0; i < mCount; i++)
  {
    mStreamPos[lStreamPosIndex++] = mStream.tell();

    CSerString lClassName;

    lClassName.DeSerialize(mStream);

    Base* lObj = lFactory.Get(lClassName.Get());

    if (lObj)
    {
      lObj->DeSerialize(mStream);
    }

    delete lObj;
  }
}
//---------------------------------------------------------------------------

template <class Base, class Factory> void ObjectStream<Base, Factory>::Set(Base* _obj)
{
  if (!_obj)
  {
    return;
  }

  mStream.rewind();

  mCount++;

  mStream.writeInt(&mCount);

  // append at the end of the file, move the position
  mStream.seek(mStream.length() - 1);

  // write classname so that we know what we have written
  CSerString(_obj->ClassName()).Serialize(mStream);

  _obj->Serialize(mStream);
}
//---------------------------------------------------------------------------

#endif
