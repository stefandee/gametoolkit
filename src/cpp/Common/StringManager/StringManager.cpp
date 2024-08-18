//---------------------------------------------------------------------------

#pragma hdrstop

#include "StringManager.h"
#include "PP_StdLib.h"
#include "tinyxml2.h"

using namespace tinyxml2;
//---------------------------------------------------------------------------
#pragma package(smart_init)

XMLNode* StringManager::GetPackage(int index)
{
  XMLNode* node = 0;
  node = mDocument.FirstChildElement( "package" );
  int count = 0;

  if (!node)
  {
    // no pac
	return 0;
  }

  do
  {
	XMLElement* elem = node->ToElement();

    const char* name = elem->Attribute( "name" );

	if (name)
	{
      if (count == index)
      {
        return node;
      }

      count++;
    }

	node = node->NextSibling();
  }
  while(node);

  // not found
  return 0;
}
//---------------------------------------------------------------------------

bool StringManager::LoadFile(std::string fileName)
{
  mErrors = "";

  XMLError result = mDocument.LoadFile(fileName.c_str());

  if (result != XML_SUCCESS)
  {
	mErrors = std::string(mDocument.ErrorStr());
  }

  return result;
}
//---------------------------------------------------------------------------

std::string StringManager::GetErrors()
{
  return mErrors;
}
//---------------------------------------------------------------------------

int         StringManager::GetPackageCount()
{
  XMLNode* node = 0;
  node = mDocument.FirstChildElement( "package" );
  int count = 0;

  if (!node)
  {
    return -1;
  }

  do
  {
	XMLElement* elem = node->ToElement();

    const char* name = elem->Attribute( "name" );

    if (name)
    {
      count++;
    }

    node = node->NextSibling();
  }
  while(node);

  return count;
}
//---------------------------------------------------------------------------

std::string StringManager::GetPackageName(int packageIndex)
{
  XMLNode* node = 0;
  node = mDocument.FirstChildElement( "package" );
  int count = 0;

  if (!node)
  {
    // no pac
    return "";
  }

  do
  {
	XMLElement* elem = node->ToElement();

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

    node = node->NextSibling();
  }
  while(node);

  // not found
  return "";
}
//---------------------------------------------------------------------------

std::string StringManager::GetPackageLang(int packageIndex)
{
  XMLNode* node = 0;
  node = mDocument.FirstChildElement( "package" );
  int count = 0;

  if (!node)
  {
    // no pac
    return "";
  }

  do
  {
	XMLElement* elem = node->ToElement();

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

    node = node->NextSibling();
  }
  while(node);

  // not found
  return "";
}
//---------------------------------------------------------------------------

int         StringManager::GetPackageStringCount(int packageIndex)
{
  XMLNode* pkg = GetPackage(packageIndex);

  if (!pkg)
  {
    return -1;
  }

  // parse the current packege xmlelement and search for "string" elements with id and value attributes
  int stringCount = -1;

  XMLNode* node = 0;
  node = pkg->FirstChildElement( "string" );

  if (!node)
  {
    return -1;
  }

  do
  {
	XMLElement* elem = node->ToElement();

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

    node = node->NextSibling();
  }
  while(node);

  return (stringCount + 1);
}
//---------------------------------------------------------------------------

std::string StringManager::GetPackageStringId(int packageIndex, int stringIndex)
{
  XMLNode* pkg = GetPackage(packageIndex);

  if (!pkg)
  {
    return "";
  }

  // parse the current packege xmlelement and search for "string" elements with id and value attributes
  int stringCount = -1;

  XMLNode* node = 0;
  node = pkg->FirstChildElement( "string" );

  if (!node)
  {
    return "";
  }

  do
  {
	XMLElement* elem = node->ToElement();

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

    node = node->NextSibling();
  }
  while(node);

  return "";
}
//---------------------------------------------------------------------------

std::string StringManager::GetPackageStringValue(int packageIndex, int stringIndex)
{
  XMLNode* pkg = GetPackage(packageIndex);

  if (!pkg)
  {
	return "";
  }

  // parse the current packege xmlelement and search for "string" elements with id and value attributes
  int stringCount = -1;

  XMLNode* node = 0;
  node = pkg->FirstChildElement( "string" );

  if (!node)
  {
    return "";
  }

  do
  {
    XMLElement* elem = node->ToElement();

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

    node = node->NextSibling();
  }
  while(node);

  return "";
}
//---------------------------------------------------------------------------


