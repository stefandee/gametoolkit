#if defined(__BORLANDC__) || defined(__SUNPRO_CC) || defined (__GNUG__)
  #include <strstream>
#else
  #include "strstrea.h"
#endif

#include<iostream>
#include<fstream>
using namespace std;

#include "TGenScriptAlg.h"
#include "ZMathLib.h"

TGenScriptAlg::TGenScriptAlg()
{
  // init scripting related variables
  mCompiler   = 0;
  mModuleName = "Scripted Terrain Generation Compiler";
  mScript     = "";

  // init internals of csl
  InitHandlers();
  InitCsl();
}
//---------------------------------------------------------------------------

TGenScriptAlg::~TGenScriptAlg()
{
  delete mCompiler;

  CleanMaps();
}
//---------------------------------------------------------------------------

HeightMap* TGenScriptAlg::Generate()
{
  if (!mCompiler)
  {
    return 0;
  }

  mOutputMap = new HeightMap(GetWidth(), GetHeight());
  mOutputMap->Reset(0.);

  try
  {
    // compile
    mCompiler->call(mModuleName, "main");
  }
  catch(const ZException& err)
  {
    for (int i = 0; i < err.count(); i++)
    {
      //printf("%s\n", ((char*)err[i]));
    }

    delete mCompiler;
    mCompiler = 0;
  }

  return mOutputMap;
}
//---------------------------------------------------------------------------

void TGenScriptAlg::addModuleFunc(const char* szMemberName, const ZCsl_callback_t& callback)
{
  mCompiler->addFunc(mModuleName, szMemberName, callback);
}
//---------------------------------------------------------------------------

void TGenScriptAlg::InitCsl()
{
  delete mCompiler;

  try
  {
    mCompiler = new ZCsl();
  }
  catch(...)
  {
    return;
  }

  try
  {
    // load math library
    ZMathLib* lMathLib = new ZMathLib(mCompiler, "Math Library");

    lMathLib->InitLibrary();
    mCompiler->loadLibrary(lMathLib);

    // add functions
    addModuleFunc("GetWidth(const mapid)", mHandleGetWidth);
    addModuleFunc("GetHeight(const mapid)", mHandleGetHeight);
    addModuleFunc("Set(const mapid, const x, const y, const value)", mHandleSet);
    addModuleFunc("Get(const mapid, const x, const y)", mHandleGet);
    addModuleFunc("HasInputMap()", mHandleHasInputMap);
    addModuleFunc("CreateMap(const width, const height)", mHandleCreateMap);

    addModuleFunc("Normalize(const mapid)", mHandleNormalize);
    addModuleFunc("Scale(const mapid, const min, const max)", mHandleScale);
    addModuleFunc("Reset(const mapid, const value)", mHandleReset);
    addModuleFunc("Smooth(const mapid, const value)", mHandleSmooth);
  }
  catch(...)
  {
    delete mCompiler;
    mCompiler = 0;
    return;
  }
}
//---------------------------------------------------------------------------

void TGenScriptAlg::InitHandlers()
{
  mHandleGetWidth.set(this, &TGenScriptAlg::mcGetWidth);
  mHandleGetHeight.set(this, &TGenScriptAlg::mcGetHeight);
  mHandleSet.set(this, &TGenScriptAlg::mcSet);
  mHandleGet.set(this, &TGenScriptAlg::mcGet);
  mHandleHasInputMap.set(this, &TGenScriptAlg::mcHasInputMap);
  mHandleCreateMap.set(this, &TGenScriptAlg::mcCreateMap);
  mHandleNormalize.set(this, &TGenScriptAlg::mcNormalize);
  mHandleScale.set(this, &TGenScriptAlg::mcScale);
  mHandleReset.set(this, &TGenScriptAlg::mcReset);
  mHandleSmooth.set(this, &TGenScriptAlg::mcSmooth);
}
//---------------------------------------------------------------------------

int TGenScriptAlg::CreateMap(int _width, int _height)
{
  mMaps.push_back(new HeightMap(_width, _height));

  return mMaps.size() - 1;
}
//---------------------------------------------------------------------------

void TGenScriptAlg::CleanMaps()
{
  for(unsigned int i = 0; i < mMaps.size(); i++)
  {
    delete mMaps[i];
  }
}
//---------------------------------------------------------------------------

HeightMap* TGenScriptAlg::GetMap(unsigned int _mapId)
{
  switch(_mapId)
  {
    case TGEN_MAP_OUT:
      return mOutputMap;

    case TGEN_MAP_IN :
      return GetInputMap();

    default:
      if (_mapId - 2 < mMaps.size())
      {
        return mMaps[_mapId - 2];
      }
  }

  return 0;
}
//---------------------------------------------------------------------------

bool TGenScriptAlg::SetScriptFromFile(const char* _fileName)
{
  ifstream lFile(_fileName, ios_base::in);

  if (!lFile.is_open())
  {
    return false;
  }

  lFile.seekg(0);

  char lBuf[512];

  ZString lScript = "";

  while(lFile.getline(lBuf, 512))
  {
    lScript = lScript + ZString(lBuf) + "\n";
  }

  //printf("%s\n", (char*)mScript);
  SetScript(lScript);

  return true;
}
//---------------------------------------------------------------------------

void TGenScriptAlg::SetScript(ZString _script)
{
  mScript = _script;

  try
  {
    // The const_cast is a workaround for a Microsoft interface bug.
    std::istrstream str((char*)mScript);

    // compile
    mCompiler->loadScript(mModuleName, &str);
  }
  catch(const ZException& err)
  {
    for (int i = 0; i < err.count(); i++)
    {
      printf("%s\n", ((char*)err[i]));
    }

    delete mCompiler;
    mCompiler = 0;
  }
}
//---------------------------------------------------------------------------

ZString TGenScriptAlg::mcGetWidth(ZCsl* csl)
{
  unsigned int lMapId = csl->get("mapid").asInt();

  HeightMap* lMap = GetMap(lMapId);
  if (lMap)
  {
    return lMap->GetWidth();
  }

  return -1;
}
//---------------------------------------------------------------------------

ZString TGenScriptAlg::mcGetHeight(ZCsl* csl)
{
  unsigned int lMapId = csl->get("mapid").asInt();

  HeightMap* lMap = GetMap(lMapId);
  if (lMap)
  {
    return lMap->GetHeight();
  }

  return -1;
}
//---------------------------------------------------------------------------

ZString TGenScriptAlg::mcSet(ZCsl* csl)
{
  unsigned int lMapId = csl->get("mapid").asInt();
  int lX    = csl->get("x").asInt();
  int lY    = csl->get("y").asInt();
  double lValue = csl->get("value").asDouble();

  HeightMap* lMap = GetMap(lMapId);
  if (lMap)
  {
    lMap->Set(lX, lY, lValue);
    return 1;
  }

  return 0;
}
//---------------------------------------------------------------------------

ZString TGenScriptAlg::mcGet(ZCsl* csl)
{
  unsigned int lMapId = csl->get("mapid").asInt();
  int lX    = csl->get("x").asInt();
  int lY    = csl->get("y").asInt();

  HeightMap* lMap = GetMap(lMapId);
  if (lMap)
  {
    return ZString(lMap->Get(lX, lY));
  }

  return 0;
}
//---------------------------------------------------------------------------

ZString TGenScriptAlg::mcHasInputMap(ZCsl* csl)
{
  return ZString((char)(GetInputMap() != 0));
}
//---------------------------------------------------------------------------

ZString TGenScriptAlg::mcCreateMap(ZCsl* csl)
{
  int lWidth  = csl->get("width").asInt();
  int lHeight = csl->get("height").asInt();

  // first two ids are output and inputmaps
  return ZString(CreateMap(lWidth, lHeight) + 2);
}
//---------------------------------------------------------------------------

ZString TGenScriptAlg::mcNormalize(ZCsl* csl)
{
  unsigned int lMapId = csl->get("mapid").asInt();

  HeightMap* lMap = GetMap(lMapId);
  if (lMap)
  {
    lMap->Normalize();
    return 1;
  }

  return 0;
}
//---------------------------------------------------------------------------

ZString TGenScriptAlg::mcScale(ZCsl* csl)
{
  unsigned int lMapId = csl->get("mapid").asInt();
  double lMin         = csl->get("min").asDouble();
  double lMax         = csl->get("max").asDouble();

  HeightMap* lMap = GetMap(lMapId);
  if (lMap)
  {
    lMap->Scale(lMin, lMax);
    return 1;
  }

  return 0;
}
//---------------------------------------------------------------------------

ZString TGenScriptAlg::mcReset(ZCsl* csl)
{
  unsigned int lMapId = csl->get("mapid").asInt();
  double lValue       = csl->get("value").asDouble();

  HeightMap* lMap = GetMap(lMapId);
  if (lMap)
  {
    lMap->Reset(lValue);
    return 1;
  }

  return 0;
}
//---------------------------------------------------------------------------

ZString TGenScriptAlg::mcSmooth(ZCsl* csl)
{
  unsigned int lMapId = csl->get("mapid").asInt();
  double lValue       = csl->get("value").asDouble();

  HeightMap* lMap = GetMap(lMapId);
  if (lMap)
  {
    lMap->Smooth(lValue);
    return 1;
  }

  return 0;
}
//---------------------------------------------------------------------------


