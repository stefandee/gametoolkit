//---------------------------------------------------------------------------

#pragma hdrstop

#include <sstream>
#include <fstream>
#include "XmlToCsl.h"
#include "PP_StdLib.h"
#include "ZFileLib.h"
#include "ZStringLib.h"
#include "tinyxml2.h"

using std::wstring;
using std::ifstream;
//---------------------------------------------------------------------------
#pragma package(smart_init)

CXmlToCsl::CXmlToCsl() : mCompiler(0)
{
}
//---------------------------------------------------------------------------

CXmlToCsl::~CXmlToCsl()
{
  delete mCompiler;
  mCompiler = 0;
}
//---------------------------------------------------------------------------

bool CXmlToCsl::Run(std::wstring inputFile, std::wstring outputDir, std::wstring scriptFile)
{
  bool loadOk = mStringManager.LoadFile(WStringToString(inputFile)) == tinyxml2::XML_SUCCESS;

  if ( !loadOk )
  {
    mErrorDesc = StringToWString(mStringManager.GetErrors());

	return false;
  }

  mOutputDir = outputDir;

  if (mOutputDir.length() != 0)
  {
    // check the last character for path separator \; add it if it misses
    if (mOutputDir[mOutputDir.length() - 1] != '\\')
    {
      mOutputDir += L"\\";
    }
  }

  if (!InitCsl(scriptFile))
  {
    return false;
  }

  try
  {
    mCompiler->call(mModuleName, "main");
  }
  catch(const ZException& err)
  {
    mErrorDesc = L"Fail to call main:";

    for (int i = 0; i < err.count(); i++)
    {
	  mErrorDesc += StringToWString(std::string(err[i]));
    }

    return false;
  }

  return true;
}
//---------------------------------------------------------------------------

bool CXmlToCsl::ReadScript(std::wstring scriptFile)
{
  mScript = "";

  ifstream file;

  file.open (scriptFile.c_str(), ifstream::in | ifstream::binary);

  if (!file.good()) {
       return false;
  }

  while (!file.eof())
  {
    mScript += (char) file.get();
  }

  mScript.resize(mScript.length() - 1);

  file.close();
}
//---------------------------------------------------------------------------

void CXmlToCsl::addModuleFunc(const char* szMemberName, const ZCsl_callback_t& callback)
{
  if (!mCompiler)
  {
    return;
  }

  mCompiler->addFunc(mModuleName, szMemberName, callback);
}
//---------------------------------------------------------------------------

bool CXmlToCsl::InitCsl(wstring scriptFile)
{
  if (!ReadScript(scriptFile))
  {
	mErrorDesc = L"Could not open the script file.";
	return false;
  }

  delete mCompiler;

  mModuleName = "StringTool";

  try
  {
	mCompiler = new ZCsl();
  }
  catch(...)
  {
	mErrorDesc = L"Errors while memory alloc for script compiler.";
	return false;
  }

  try
  {
	InitHandlers();
	InitScriptingLibs();
	InitScriptingInterface();
  }
  catch(...)
  {
	mErrorDesc = L"Errors initializing scripting interface";
    return false;
  }

  try
  {
	std::istringstream str(const_cast<char*>(mScript.c_str()));

    // compile
    mCompiler->loadScript(mModuleName, &str);
  }
  catch(const ZException& err)
  {
	mErrorDesc = L"Errors compiling script " + scriptFile + L"\n";

    for (int i = 0; i < err.count(); i++)
    {
	  mErrorDesc += StringToWString(std::string(err[i]));
    }

    return false;
  }

  return true;
}
//---------------------------------------------------------------------------

void CXmlToCsl::InitScriptingLibs()
{
  // load file library
  ZFileLib* lFileLib = new ZFileLib(mCompiler, "File Library");
  lFileLib->InitLibrary();
  mCompiler->loadLibrary(lFileLib);

  // load string library
  ZStringLib* lStringLib = new ZStringLib(mCompiler, "String Library");
  lStringLib->InitLibrary();
  mCompiler->loadLibrary(lStringLib);
}
//---------------------------------------------------------------------------

void CXmlToCsl::InitScriptingInterface()
{
  // add functions
  addModuleFunc("GetPackageCount()", mHandleGetPackageCount);
  addModuleFunc("GetPackageName(const index)", mHandleGetPackageName);
  addModuleFunc("GetPackageLang(const index)", mHandleGetPackageLang);
  addModuleFunc("GetPackageStringCount(const index)", mHandleGetPackageStringCount);
  addModuleFunc("GetPackageStringId(const packageIndex, const stringIndex)", mHandleGetPackageStringId);
  addModuleFunc("GetPackageStringValue(const packageIndex, const stringIndex)", mHandleGetPackageStringValue);
  addModuleFunc("GetOutputDir()", mHandleGetOutputDir);
}
//---------------------------------------------------------------------------

void CXmlToCsl::InitHandlers()
{
  mHandleGetPackageCount      .set(this, &CXmlToCsl::getPackageCount);
  mHandleGetPackageName       .set(this, &CXmlToCsl::getPackageName);
  mHandleGetPackageLang       .set(this, &CXmlToCsl::getPackageLang);
  mHandleGetPackageStringCount.set(this, &CXmlToCsl::getPackageStringCount);
  mHandleGetPackageStringId   .set(this, &CXmlToCsl::getPackageStringId);
  mHandleGetPackageStringValue.set(this, &CXmlToCsl::getPackageStringValue);
  mHandleGetOutputDir         .set(this, &CXmlToCsl::getOutputDir);
}
//---------------------------------------------------------------------------

ZString CXmlToCsl::getPackageCount(ZCsl* csl)
{
  return mStringManager.GetPackageCount();
}
//---------------------------------------------------------------------------

ZString CXmlToCsl::getPackageName(ZCsl* csl)
{
  int index = csl->get("index").asInt();

  return mStringManager.GetPackageName(index).c_str();
}
//---------------------------------------------------------------------------

ZString CXmlToCsl::getPackageLang(ZCsl* csl)
{
  int index = csl->get("index").asInt();

  return mStringManager.GetPackageLang(index).c_str();
}
//---------------------------------------------------------------------------

ZString CXmlToCsl::getPackageStringCount(ZCsl* csl)
{
  int pkgIndex = csl->get("index").asInt();

  return mStringManager.GetPackageStringCount(pkgIndex);
}
//---------------------------------------------------------------------------

ZString CXmlToCsl::getPackageStringId(ZCsl* csl)
{
  int pkgIndex = csl->get("packageIndex").asInt();
  int stringIndex = csl->get("stringIndex").asInt();

  return mStringManager.GetPackageStringId(pkgIndex, stringIndex).c_str();
}
//---------------------------------------------------------------------------

ZString CXmlToCsl::getPackageStringValue(ZCsl* csl)
{
  int pkgIndex = csl->get("packageIndex").asInt();
  int stringIndex = csl->get("stringIndex").asInt();

  return mStringManager.GetPackageStringValue(pkgIndex, stringIndex).c_str();
}
//---------------------------------------------------------------------------

ZString CXmlToCsl::getOutputDir(ZCsl* csl)
{
  return ZString(WStringToString(mOutputDir).c_str());
}
//---------------------------------------------------------------------------

