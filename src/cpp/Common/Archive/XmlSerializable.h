//-----------------------------------------------------------------------------
// This file is part of PPTactical Engine.                                   //
//                                                                           //
// PPTactical Engine - engine for tactical/strategy games                    //
// Copyright (C) 2005-2017 Stefan Dicu                                       //
//                                                                           //
// PPTactical Engine is free software; you can redistribute it               //
// and/or modify it under the terms of the GNU Lesser General Public License //
// as published by the Free Software Foundation; either version 2.1 of the   //
// License, or (at your option) any later version.                           //
//                                                                           //
// PPTactical Engine is distributed in the hope that it will be useful, but  //
// WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY//
// or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public   //
// License for more details.                                                 //
//                                                                           //
// You should have received a copy of the GNU Lesser General Public License  //
// along with PPTactical Engine; if not, write to the Free Software          //
// Foundation Inc. 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA   //
//-----------------------------------------------------------------------------

#ifndef XMLSERIALIZABLE_H
#define XMLSERIALIZABLE_H

#include <string>
#include "tinyxml.h"
#include "PP_String.h"

class BaseDeSerializer
{
    public:
        virtual ~BaseDeSerializer() {}

        virtual void readAttribute(const char* name, bool& value ) = 0;
        virtual void readAttribute(const char* name, int& value) = 0;
        virtual void readAttribute(const char* name, CPString& value) = 0;
        virtual void readAttribute(const char* name, std::string& value) = 0;

        virtual void readAttribute(const char* name, bool& value, bool defaultValue ) = 0;
        virtual void readAttribute(const char* name, int& value, int defaultValue) = 0;
        virtual void readAttribute(const char* name, CPString& value, const char* defaultValue) = 0;
        virtual void readAttribute(const char* name, std::string& value, const char* defaultValue) = 0;

        //template<typename T> void readElementAttribute(const char* elementName, const char* name, T& value, T defaultValue) {}

        virtual void beginElement(const char* name) = 0;
        virtual void endElement() = 0;
        virtual bool hasElement(const char* name) = 0;
        virtual int readElementCount() = 0;
};

class BaseSerializer
{
    public:
        virtual ~BaseSerializer() {}

        virtual void createAttribute(const char* name, int value) = 0;
        virtual void createAttribute(const char* name, bool value) = 0;
        virtual void createAttribute(const char* name, const char* value) = 0;

        virtual void beginElement(const char* name) = 0;
        virtual void endElement() = 0;

        virtual void rawWrite(int value) = 0; /**< necessary for writing pure int data, e.g. element count (binary serialization) */
};

class BinDeSerializer : public BaseDeSerializer
{

};

class BinSerializer : public BaseSerializer
{

};

class XmlDeSerializer : public BaseDeSerializer
{
    public:
        XmlDeSerializer(TiXmlDocument doc);
        virtual ~XmlDeSerializer();

    public:
        void readAttribute(const char* name, bool& value );
        void readAttribute(const char* name, int& value);
        void readAttribute(const char* name, CPString& value);
        void readAttribute(const char* name, std::string& value);

        void readAttribute(const char* name, bool& value, bool defaultValue );
        void readAttribute(const char* name, int& value, int defaultValue );
        void readAttribute(const char* name, CPString& value, const char* defaultValue );
        void readAttribute(const char* name, std::string& value, const char* defaultValue);

        //template<typename T> void readElementAttribute(const char* elementName, const char* name, T& value, T defaultValue);

        void beginElement(const char* name);
        void endElement();
        int readElementCount();
        bool hasElement(const char* name);

    private:
        TiXmlDocument mDoc;
        TiXmlElement* mCurrentElement;
};

class XmlSerializer : public BaseSerializer
{
    public:
        XmlSerializer();
        virtual ~XmlSerializer();

    public:
        void createAttribute(const char* name, int value);
        void createAttribute(const char* name, bool value);
        void createAttribute(const char* name, const char* value);

        void beginElement(const char* name);
        void endElement();

        void rawWrite(int value); /**< writes an xml comment */

        TiXmlDocument& GetDoc() { return mDoc; }

    private:
        TiXmlDocument mDoc;
        TiXmlElement* mCurrentElement;
};

class Serializable2
{
    public:
        Serializable2() {}
        virtual ~Serializable2() {}

    public:
        virtual void Serialize(BaseSerializer& out) = 0;
        virtual void DeSerialize(BaseDeSerializer& in) = 0;

    protected:

    private:
};

#endif // XMLSERIALIZABLE_H
