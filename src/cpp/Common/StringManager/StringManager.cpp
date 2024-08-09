//---------------------------------------------------------------------------

#pragma hdrstop

#include "StringManager.h"
#include "PP_StdLib.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

TiXmlNode* StringManager::GetPackage(int index)
{
  TiXmlNode* node = 0;
  node = mDocument.FirstChild( "package" );
  int count = 0;

  if (!node)
  {
    // no pac
	return 0;
  }

  do
  {
    TiXmlElement* elem = node->ToElement();

    const char* name = elem->Attribute( "name" );

	if (name)
    {
      if (count == index)
      {
        return node;
      }

      count++;
    }

    node = node->NextSibling( "package" );
  }
  while(node);

  // not found
  return 0;
}
//---------------------------------------------------------------------------

bool StringManager::LoadFile(std::wstring fileName)
{
  mErrors = L"";

  // TODO tinyxml can't handle wstring, need to convert to string
  mDocument = TiXmlDocument( WStringToString(fileName).c_str() );

  bool result = mDocument.LoadFile();

  if (!result)
  {
    // TODO not quite correct, but until tinyxml is converted to handle wchar_t...
	mErrors = StringToWString(mDocument.ErrorDesc());
  }

  return result;
}
//---------------------------------------------------------------------------

std::wstring StringManager::GetErrors()
{
  return mErrors;
}
//---------------------------------------------------------------------------

int         StringManager::GetPackageCount()
{
  TiXmlNode* node = 0;
  node = mDocument.FirstChild( "package" );
  int count = 0;

  if (!node)
  {
    return -1;
  }

  do
  {
    TiXmlElement* elem = node->ToElement();

    const char* name = elem->Attribute( "name" );

    if (name)
    {
      count++;
    }

    node = node->NextSibling( "package" );
  }
  while(node);

  return count;
}
//---------------------------------------------------------------------------

std::string StringManager::GetPackageName(int packageIndex)
{
  TiXmlNode* node = 0;
  node = mDocument.FirstChild( "package" );
  int count = 0;

  if (!node)
  {
    // no pac
    return "";
  }

  do
  {
    TiXmlElement* elem = node->ToElement();

    if (!elem)
    {
      continue;
    }

    const char* name = elem->Attribute( "name" );

    if (name)
    {
      if (count == packageIndex)
      {
        return name;
      }

      count++;
    }

    node = node->NextSibling( "package" );
  }
  while(node);

  // not found
  return "";
}
//---------------------------------------------------------------------------

std::string StringManager::GetPackageLang(int packageIndex)
{
  TiXmlNode* node = 0;
  node = mDocument.FirstChild( "package" );
  int count = 0;

  if (!node)
  {
    // no pac
    return "";
  }

  do
  {
    TiXmlElement* elem = node->ToElement();

    if (!elem)
    {
      continue;
    }

    const char* name = elem->Attribute( "lang" );

    if (name)
    {
      if (count == packageIndex)
      {
        return name;
      }

      count++;
    }

    node = node->NextSibling( "package" );
  }
  while(node);

  // not found
  return "";
}
//---------------------------------------------------------------------------

int         StringManager::GetPackageStringCount(int packageIndex)
{
  TiXmlNode* pkg = GetPackage(packageIndex);

  if (!pkg)
  {
    return -1;
  }

  // parse the current packege xmlelement and search for "string" elements with id and value attributes
  int stringCount = -1;

  TiXmlNode* node = 0;
  node = pkg->FirstChild( "string" );

  if (!node)
  {
    return -1;
  }

  do
  {
    TiXmlElement* elem = node->ToElement();

    if (!elem)
    {
      continue;
    }

    const char* id = elem->Attribute( "id" );
    const char* value = elem->Attribute( "value" );

    if (id && value)
    {
      stringCount++;
    }

    node = node->NextSibling( "string" );
  }
  while(node);

  return (stringCount + 1);
}
//---------------------------------------------------------------------------

std::string StringManager::GetPackageStringId(int packageIndex, int stringIndex)
{
  TiXmlNode* pkg = GetPackage(packageIndex);

  if (!pkg)
  {
    return "";
  }

  // parse the current packege xmlelement and search for "string" elements with id and value attributes
  int stringCount = -1;

  TiXmlNode* node = 0;
  node = pkg->FirstChild( "string" );

  if (!node)
  {
    return "";
  }

  do
  {
    TiXmlElement* elem = node->ToElement();

    if (!elem)
    {
      continue;
    }

    const char* id = elem->Attribute( "id" );
    const char* value = elem->Attribute( "value" );

    if (id && value)
    {
      stringCount++;

      if (stringCount == stringIndex)
      {
        return id;
      }
    }

    node = node->NextSibling( "string" );
  }
  while(node);

  return "";
}
//---------------------------------------------------------------------------

std::string StringManager::GetPackageStringValue(int packageIndex, int stringIndex)
{
  TiXmlNode* pkg = GetPackage(packageIndex);

  if (!pkg)
  {
    return "";
  }

  // parse the current packege xmlelement and search for "string" elements with id and value attributes
  int stringCount = -1;

  TiXmlNode* node = 0;
  node = pkg->FirstChild( "string" );

  if (!node)
  {
    return "";
  }

  do
  {
    TiXmlElement* elem = node->ToElement();

    if (!elem)
    {
      continue;
    }

    const char* id = elem->Attribute( "id" );
    const char* value = elem->Attribute( "value" );

    if (id && value)
    {
      stringCount++;

      if (stringCount == stringIndex)
      {
        return value;
      }
    }

    node = node->NextSibling( "string" );
  }
  while(node);

  return "";
}
//---------------------------------------------------------------------------


