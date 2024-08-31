//---------------------------------------------------------------------------
#include <fstream>
#include <sstream>
#include <SysUtils.hpp>

#include <algorithm>
using std::min;
using std::max;
#include <gdiplus.h>

#pragma hdrstop

#include "Script.h"
#include "XSpriteDesigner.h"
#include "ZFileLib.h"
#include "ZMathLib.h"
#include "ZStringLib.h"
#include "ZPixelLib.h"

using namespace std;

//---------------------------------------------------------------------------

#pragma package(smart_init)

SentryScript::SentryScript(CXSpriteDesigner* sprite) : mCompiler(0), mSprite(sprite)
{
}
//---------------------------------------------------------------------------

SentryScript::~SentryScript()
{
  delete mCompiler;

  mCompiler   = 0;
  mModuleName = "";
  mScript     = "";
  mSprite     = 0;
}
//---------------------------------------------------------------------------

bool SentryScript::LoadScriptFromFile(std::string scriptFile)
{
  mScript = "";

  ifstream file;

  file.open (scriptFile.c_str(), ifstream::in | ifstream::binary);

  if (!file.is_open())
  {
    return false;
  }

  while (!file.eof())
  {
    mScript += (char) file.get();
  }

  mScript.resize(mScript.length() - 1);

  file.close();

  return InitCsl();
}
//---------------------------------------------------------------------------

void SentryScript::addModuleFunc(const char* szMemberName, const ZCsl_callback_t& callback)
{
  if (!mCompiler)
  {
    return;
  }

  mCompiler->addFunc(mModuleName, szMemberName, callback);
}
//---------------------------------------------------------------------------

bool SentryScript::InitCsl()
{
  delete mCompiler;

  mModuleName = "SentryScript";

  try
  {
    mCompiler = new ZCsl();
  }
  catch(...)
  {
    mErrorDesc = "Errors while memory alloc for script compiler.";
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
    mErrorDesc = "Errors initializing scripting interface";
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
    mErrorDesc = "Errors compiling script \n";

    for (int i = 0; i < err.count(); i++)
    {
      mErrorDesc += (char*)err[i];
    }

    return false;
  }

  return true;
}
//---------------------------------------------------------------------------

void SentryScript::InitScriptingLibs()
{
  // load file library
  ZFileLib* lFileLib = new ZFileLib(mCompiler, "File Library");
  lFileLib->InitLibrary();
  mCompiler->loadLibrary(lFileLib);

  // load string library
  ZStringLib* lStringLib = new ZStringLib(mCompiler, "String Library");
  lStringLib->InitLibrary();
  mCompiler->loadLibrary(lStringLib);

  // load pixel library
  ZPixelLib* lPixelLib = new ZPixelLib(mCompiler, "Pixel Library");
  lPixelLib->InitLibrary();
  mCompiler->loadLibrary(lPixelLib);
}
//---------------------------------------------------------------------------

void SentryScript::InitScriptingInterface()
{
  // image pixel format consts
  // TODO: add more
  /*
  mCompiler->addVar("IMAGE_PIXEL_FORMAT_8BPP", "0");
  mCompiler->addVar("IMAGE_PIXEL_FORMAT_RGB",  "1");
  mCompiler->addVar("IMAGE_PIXEL_FORMAT_ARGB", "2");

  // fmodule transformation constants
  mCompiler->addVar("FMODULE_BITFLAG_FLIP_X", "1");
  mCompiler->addVar("FMODULE_BITFLAG_FLIP_Y", "2");
  mCompiler->addVar("FMODULE_BITFLAG_ROT90",  "4");
  mCompiler->addVar("FMODULE_BITFLAG_ROT270", "8");

  // aframe transformation (rotation) constants
  mCompiler->addVar("ANIMFRAME_BITFLAG_FLIP_X", "1");
  mCompiler->addVar("ANIMFRAME_BITFLAG_FLIP_Y", "2");
  mCompiler->addVar("ANIMFRAME_BITFLAG_ROT90",  "4");
  mCompiler->addVar("ANIMFRAME_BITFLAG_ROT270", "8");

  // frame logic item type
  mCompiler->addVar("FRAME_LOGIC_ITEM_POINT", "0");
  mCompiler->addVar("FRAME_LOGIC_ITEM_RECT",  "1");
  */

  // add functions

  // misc
  addModuleFunc("GetSpritePath()",                      mHandleGetSpritePath);
  addModuleFunc("ExtractPath(const path)",              mHandleExtractPath);
  addModuleFunc("ExtractFileName(const path)",          mHandleExtractFileName);
  addModuleFunc("ChangeFileExt(const path, const ext)", mHandleChangeFileExt);

  // images
  addModuleFunc("GetImageCount()",                  mHandleGetImageCount);
  addModuleFunc("GetImageFileName(const index)",    mHandleGetImageFileName);
  addModuleFunc("GetImageId(const index)", 	        mHandleGetImageId);
  addModuleFunc("GetImageInfo(const index)", 	    mHandleGetImageInfo);
  addModuleFunc("GetImageTColor(const index)", 	    mHandleGetImageTColor);
  addModuleFunc("GetImageIndexById(const id)", 	    mHandleGetImageIndexById);

  addModuleFunc("GetImagePixelFormat(const index)",                                 mHandleGetImagePixelFormat);
  addModuleFunc("GetImagePixel(const index, const x, const y)", 	                mHandleGetImagePixel);
  addModuleFunc("SetImagePixel(const index, const x, const y, const color)", 	    mHandleSetImagePixel);

  // color maps
  addModuleFunc("GetColorMapsCount(const imageIndex)",                                                     mHandleGetColorMapsCount);
  addModuleFunc("GetColorMapId(const imageIndex, const colorMapIndex)",                                    mHandleGetColorMapId);
  addModuleFunc("GetColorMapItemsCount(const imageIndex, const colorMapIndex)",                            mHandleGetColorMapItemsCount);
  addModuleFunc("GetColorMapItemId(const imageIndex, const colorMapIndex, const colorMapItemIndex)",       mHandleGetColorMapItemId);
  addModuleFunc("GetColorMapItemSrcColor(const imageIndex, const colorMapIndex, const colorMapItemIndex)", mHandleGetColorMapItemSrcColor);
  addModuleFunc("GetColorMapItemDstColor(const imageIndex, const colorMapIndex, const colorMapItemIndex)", mHandleGetColorMapItemDstColor);

  // modules
  addModuleFunc("GetModuleCount()",                 mHandleGetModuleCount);
  addModuleFunc("GetModuleId(const index)",         mHandleGetModuleId);
  addModuleFunc("GetModuleImageId(const index)",    mHandleGetModuleImageId);
  addModuleFunc("GetModuleIndexById(const id)",     mHandleGetModuleIndexById);
  addModuleFunc("GetModuleX(const index)",          mHandleGetModuleX);
  addModuleFunc("GetModuleY(const index)",          mHandleGetModuleY);
  addModuleFunc("GetModuleWidth(const index)",      mHandleGetModuleWidth);
  addModuleFunc("GetModuleHeight(const index)",     mHandleGetModuleHeight);
  addModuleFunc("GetModuleInfo(const index)", 	    mHandleGetModuleInfo);

  // frame modules
  addModuleFunc("GetFModuleModuleId(const frameIndex, const index)", mHandleGetFModuleModuleId);
  addModuleFunc("GetFModuleX(const frameIndex, const index)",        mHandleGetFModuleX);
  addModuleFunc("GetFModuleY(const frameIndex, const index)",        mHandleGetFModuleY);
  addModuleFunc("GetFModuleFlags(const frameIndex, const index)",    mHandleGetFModuleFlags);

  // frame logic item
  addModuleFunc("GetFLogicItemId(const frameIndex, const fLogicIndex, const index)", 	    mHandleGetFLogicItemId);
  addModuleFunc("GetFLogicItemType(const frameIndex, const fLogicIndex, const index)",      mHandleGetFLogicItemType);
  addModuleFunc("GetFLogicItemX(const frameIndex, const fLogicIndex, const index)", 	    mHandleGetFLogicItemX);
  addModuleFunc("GetFLogicItemY(const frameIndex, const fLogicIndex, const index)",         mHandleGetFLogicItemY);
  addModuleFunc("GetFLogicItemWidth(const frameIndex, const fLogicIndex, const index)",     mHandleGetFLogicItemWidth);
  addModuleFunc("GetFLogicItemHeight(const frameIndex, const fLogicIndex, const index)",    mHandleGetFLogicItemHeight);

  // frame logic
  addModuleFunc("GetFLogicItemCount(const frameIndex, const index)",    mHandleGetFLogicItemCount);
  addModuleFunc("GetFLogicId(const frameIndex, const index)",           mHandleGetFLogicId);
  addModuleFunc("GetFLogicInfo(const frameIndex, const index)",         mHandleGetFLogicInfo);

  // frames
  addModuleFunc("GetFrameCount()",                          mHandleGetFrameCount);
  addModuleFunc("GetFModuleCount(const frameIndex)", 	    mHandleGetFModuleCount);
  addModuleFunc("GetFLogicCount(const frameIndex)", 	    mHandleGetFLogicCount);
  addModuleFunc("GetFrameIndexById(const id)",              mHandleGetFrameIndexById);
  addModuleFunc("GetFrameId(const frameIndex)",             mHandleGetFrameId);
  addModuleFunc("GetFrameInfo(const frameIndex)",           mHandleGetFrameInfo);
  addModuleFunc("GetFrameBoundX(const frameIndex)", 	    mHandleGetFrameBoundX);
  addModuleFunc("GetFrameBoundY(const frameIndex)",         mHandleGetFrameBoundY);
  addModuleFunc("GetFrameBoundWidth(const frameIndex)",     mHandleGetFrameBoundWidth);
  addModuleFunc("GetFrameBoundHeight(const frameIndex)",    mHandleGetFrameBoundHeight);

  // anim frames
  addModuleFunc("GetAFrameId(const animIndex, const index)",        mHandleGetAFrameId);
  addModuleFunc("GetAFrameTime(const animIndex, const index)",      mHandleGetAFrameTime);
  addModuleFunc("GetAFrameFrameId(const animIndex, const index)",   mHandleGetAFrameFrameId);
  addModuleFunc("GetAFrameFlags(const animIndex, const index)",     mHandleGetAFrameFlags);
  addModuleFunc("GetAFrameX(const animIndex, const index)", 	    mHandleGetAFrameX);
  addModuleFunc("GetAFrameY(const animIndex, const index)",         mHandleGetAFrameY);

  // anim
  addModuleFunc("GetAnimCount()",                   mHandleGetAnimCount);
  addModuleFunc("GetAFrameCount(const animIndex)",  mHandleGetAFrameCount);
  addModuleFunc("GetAnimId(const animIndex)",       mHandleGetAnimId);
  addModuleFunc("GetAnimInfo(const animIndex)",     mHandleGetAnimInfo);
}
//---------------------------------------------------------------------------

void SentryScript::InitHandlers()
{
  // misc
  mHandleGetSpritePath      .set(this, &SentryScript::getSpritePath);
  mHandleExtractPath        .set(this, &SentryScript::extractPath);
  mHandleExtractFileName    .set(this, &SentryScript::extractFileName);
  mHandleChangeFileExt      .set(this, &SentryScript::changeFileExt);

  // images
  mHandleGetImageCount      .set(this, &SentryScript::getImageCount);
  mHandleGetImageFileName   .set(this, &SentryScript::getImageFileName);
  mHandleGetImageId         .set(this, &SentryScript::getImageId);         
  mHandleGetImageInfo       .set(this, &SentryScript::getImageInfo);       
  mHandleGetImageTColor     .set(this, &SentryScript::getImageTColor);
  mHandleGetImageIndexById  .set(this, &SentryScript::getImageIndexById);

  mHandleGetImagePixelFormat.set(this, &SentryScript::getImagePixelFormat);
  mHandleGetImagePixel      .set(this, &SentryScript::getImagePixel);
  mHandleSetImagePixel      .set(this, &SentryScript::setImagePixel);

  // color maps
  mHandleGetColorMapsCount       .set(this, &SentryScript::getColorMapsCount);
  mHandleGetColorMapId           .set(this, &SentryScript::getColorMapId);
  mHandleGetColorMapItemsCount   .set(this, &SentryScript::getColorMapItemsCount);
  mHandleGetColorMapItemId       .set(this, &SentryScript::getColorMapItemId);
  mHandleGetColorMapItemSrcColor .set(this, &SentryScript::getColorMapItemSrcColor);
  mHandleGetColorMapItemDstColor .set(this, &SentryScript::getColorMapItemDstColor);

  // modules
  mHandleGetModuleCount     .set(this, &SentryScript::getModuleCount);
  mHandleGetModuleId        .set(this, &SentryScript::getModuleId);
  mHandleGetModuleImageId   .set(this, &SentryScript::getModuleImageId);
  mHandleGetModuleIndexById .set(this, &SentryScript::getModuleIndexById);
  mHandleGetModuleX         .set(this, &SentryScript::getModuleX);
  mHandleGetModuleY         .set(this, &SentryScript::getModuleY);
  mHandleGetModuleWidth     .set(this, &SentryScript::getModuleWidth);
  mHandleGetModuleHeight    .set(this, &SentryScript::getModuleHeight);
  mHandleGetModuleInfo      .set(this, &SentryScript::getModuleInfo);

  // frame modules
  mHandleGetFModuleModuleId .set(this, &SentryScript::getFModuleModuleId);
  mHandleGetFModuleX        .set(this, &SentryScript::getFModuleX);
  mHandleGetFModuleY        .set(this, &SentryScript::getFModuleY);
  mHandleGetFModuleFlags    .set(this, &SentryScript::getFModuleFlags);

  // frame logic items
  mHandleGetFLogicItemId    .set(this, &SentryScript::getFLogicItemId);
  mHandleGetFLogicItemType  .set(this, &SentryScript::getFLogicItemType);
  mHandleGetFLogicItemX     .set(this, &SentryScript::getFLogicItemX);
  mHandleGetFLogicItemY     .set(this, &SentryScript::getFLogicItemY);
  mHandleGetFLogicItemWidth .set(this, &SentryScript::getFLogicItemWidth);
  mHandleGetFLogicItemHeight.set(this, &SentryScript::getFLogicItemHeight);

  // frame logics
  mHandleGetFLogicItemCount .set(this, &SentryScript::getFLogicItemCount);
  mHandleGetFLogicId        .set(this, &SentryScript::getFLogicId);
  mHandleGetFLogicInfo      .set(this, &SentryScript::getFLogicInfo);

  // frames
  mHandleGetFrameCount      .set(this, &SentryScript::getFrameCount);
  mHandleGetFModuleCount    .set(this, &SentryScript::getFModuleCount);
  mHandleGetFLogicCount     .set(this, &SentryScript::getFLogicCount);
  mHandleGetFrameIndexById  .set(this, &SentryScript::getFrameIndexById);
  mHandleGetFrameId         .set(this, &SentryScript::getFrameId);
  mHandleGetFrameInfo       .set(this, &SentryScript::getFrameInfo);
  mHandleGetFrameBoundX     .set(this, &SentryScript::getFrameBoundX);
  mHandleGetFrameBoundY     .set(this, &SentryScript::getFrameBoundY);
  mHandleGetFrameBoundWidth .set(this, &SentryScript::getFrameBoundWidth);
  mHandleGetFrameBoundHeight.set(this, &SentryScript::getFrameBoundHeight);

  // anim frames
  mHandleGetAFrameId        .set(this, &SentryScript::getAFrameId);
  mHandleGetAFrameTime      .set(this, &SentryScript::getAFrameTime);
  mHandleGetAFrameFrameId   .set(this, &SentryScript::getAFrameFrameId);
  mHandleGetAFrameFlags     .set(this, &SentryScript::getAFrameFlags);
  mHandleGetAFrameX         .set(this, &SentryScript::getAFrameX);
  mHandleGetAFrameY         .set(this, &SentryScript::getAFrameY);

  // anims
  mHandleGetAnimCount       .set(this, &SentryScript::getAnimCount);
  mHandleGetAFrameCount     .set(this, &SentryScript::getAFrameCount);
  mHandleGetAnimId          .set(this, &SentryScript::getAnimId);
  mHandleGetAnimInfo        .set(this, &SentryScript::getAnimInfo);
}
//---------------------------------------------------------------------------


std::string SentryScript::GetScriptVar(std::string name)
{
  if (!mCompiler)
  {
    return "";
  }

  if (!mCompiler->varExists(name.c_str()))
  {
    return "";
  }

  return std::string((mCompiler->get(name.c_str())).buffer());
}
//---------------------------------------------------------------------------

void SentryScript::SetScriptVar(std::string name, std::string value)
{
  if (!mCompiler)
  {
    return;
  }

  if (mCompiler->varExists(name.c_str()))
  {
    mCompiler->set(name.c_str(), value.c_str());
  }
}
//---------------------------------------------------------------------------

std::string SentryScript::GetParam(int index, int type)
{
  if (!mCompiler)
  {
    return "";
  }

  char indexStr[128];
  char typeStr[128];

  itoa(index, indexStr, 10);
  itoa(type, typeStr, 10);

  return std::string(mCompiler->call("SentryScript", "GetParam", 2, indexStr, typeStr).buffer());
}
//---------------------------------------------------------------------------

std::string SentryScript::SetParam(int index, int type, const char* value)
{
  if (!mCompiler)
  {
    return "";
  }

  char indexStr[128];
  char typeStr[128];

  itoa(index, indexStr, 10);
  itoa(type, typeStr, 10);

  return std::string(mCompiler->call("SentryScript", "SetParam", 3, indexStr, typeStr, value).buffer());
}
//---------------------------------------------------------------------------

bool SentryScript::RunMethod(const char* method)
{
  if (!mCompiler || !mSprite)
  {
    mErrorDesc = "Invalid script.";
    return false;
  }

  try
  {
    mCompiler->call(mModuleName, method);
  }
  catch(const ZException& err)
  {
    mErrorDesc = "RunMethod() failed with error(s): ";

    for (int i = 0; i < err.count(); i++)
    {
      mErrorDesc += (char*)err[i];
    }

    return false;
  }

  return true;
}
//---------------------------------------------------------------------------

bool SentryScript::Execute()
{
  return RunMethod("execute");
}
//---------------------------------------------------------------------------

bool SentryScript::Init()
{
  return RunMethod("init");
}
//---------------------------------------------------------------------------

std::string SentryScript::GetScript()
{
  return mScript;
}
//---------------------------------------------------------------------------

std::string SentryScript::GetError()
{
  return mErrorDesc;
}
//---------------------------------------------------------------------------

// misc
ZString SentryScript::getSpritePath(ZCsl* csl)
{
  if (mSprite->GetDocState() == SPR_DOC_STATE_NEW ||
      mSprite->GetDocState() == SPR_DOC_STATE_CLOSED)
  {
    return "sprite not saved";
  }

  return mSprite->GetFileName().c_str();
}
//---------------------------------------------------------------------------

ZString SentryScript::extractPath(ZCsl* csl)
{
  ZString path = csl->get("path");

  return UTF8Encode(ExtractFileDir(path.buffer())).c_str();
}
//---------------------------------------------------------------------------

ZString SentryScript::extractFileName(ZCsl* csl)
{
  ZString path = csl->get("path");

  return UTF8Encode(ExtractFileName(path.buffer())).c_str();
}
//---------------------------------------------------------------------------

ZString SentryScript::changeFileExt(ZCsl* csl)
{
  ZString path = csl->get("path");
  ZString ext = csl->get("ext");

  return UTF8Encode(ChangeFileExt(path.buffer(), ext.buffer())).c_str();
}
//---------------------------------------------------------------------------

// images
ZString SentryScript::getImageCount(ZCsl* csl)
{
  return mSprite->mImagesManager.Size();
}
//---------------------------------------------------------------------------

ZString SentryScript::getImageFileName(ZCsl* csl)
{
  int index = csl->get("index").asInt();

  if (index < 0 || index >= mSprite->mImagesManager.Size())
  {
    return "";
  }

  return mSprite->mImagesManager.Get(index).GetFileName().c_str();
}
//---------------------------------------------------------------------------

ZString SentryScript::getImageId(ZCsl* csl)
{
  int index = csl->get("index").asInt();

  if (index < 0 || index >= mSprite->mImagesManager.Size())
  {
    return "";
  }

  return mSprite->mImagesManager.Get(index).GetId();
}
//---------------------------------------------------------------------------

ZString SentryScript::getImageInfo(ZCsl* csl)
{
  int index = csl->get("index").asInt();

  if (index < 0 || index >= mSprite->mImagesManager.Size())
  {
    return "";
  }

  return mSprite->mImagesManager.Get(index).GetInfo().c_str();
}
//---------------------------------------------------------------------------

ZString SentryScript::getImageTColor(ZCsl* csl)
{
  int index = csl->get("index").asInt();

  if (index < 0 || index >= mSprite->mImagesManager.Size())
  {
    return "";
  }

  int color = mSprite->mImagesManager.Get(index).GetTransparentColor();

  return color;
}
//---------------------------------------------------------------------------

ZString SentryScript::getImageIndexById(ZCsl* csl)
{
  int id = csl->get("id").asInt();

  for(int i = 0; i < mSprite->mImagesManager.Size(); i++)
  {
    if (mSprite->mImagesManager.Get(i).GetId() == id)
    {
      return i;
    }
  }

  return -1;
}
//---------------------------------------------------------------------------

ZString SentryScript::getImagePixelFormat(ZCsl* csl)
{
  // TODO: not used at the moment, all images are RGB
  return "1";
}
//---------------------------------------------------------------------------

ZString SentryScript::getImagePixel(ZCsl* csl)
{
  int index = csl->get("index").asInt();
  int x = csl->get("x").asInt();
  int y = csl->get("y").asInt();

  if (index < 0 || index >= mSprite->mImagesManager.Size())
  {
    return "-1";
  }

  // return (int)(ImageCache::Instance()->GetBitmap(mSprite->mImagesManager.Get(index).GetFileName())->Canvas->Pixels[x][y]);

  // implemented using GDI+
  Gdiplus::Bitmap* tmpBmp = ImageCache::Instance()->GetBitmap(mSprite->mImagesManager.Get(index).GetFileName());

  Gdiplus::Color color;

  tmpBmp->GetPixel(x, y, &color);

  int value = color.GetValue();

  return color.GetValue();
}
//---------------------------------------------------------------------------

ZString SentryScript::setImagePixel(ZCsl* csl)
{
  int index = csl->get("index").asInt();
  int x     = csl->get("x").asInt();
  int y     = csl->get("y").asInt();
  int color = csl->get("color").asInt();

  if (index < 0 || index >= mSprite->mImagesManager.Size())
  {
    return "0";
  }

  //ImageCache::Instance()->GetBitmap(mSprite->mImagesManager.Get(index).GetFileName())->Canvas->Pixels[x][y] = (TColor)color;

  // implemented using GDI+
  Gdiplus::Bitmap* tmpBmp = ImageCache::Instance()->GetBitmap(mSprite->mImagesManager.Get(index).GetFileName());
  tmpBmp->SetPixel(x, y, Gdiplus::Color(color));

  return "1";
}
//---------------------------------------------------------------------------


// modules
ZString SentryScript::getModuleCount(ZCsl* csl)
{
  return mSprite->mModulesManager.Size();
}
//---------------------------------------------------------------------------

ZString SentryScript::getModuleId(ZCsl* csl)
{
  int index = csl->get("index").asInt();

  if (index < 0 || index >= mSprite->mModulesManager.Size())
  {
    return "-1";
  }

  return mSprite->mModulesManager.Get(index).GetId();
}
//---------------------------------------------------------------------------

ZString SentryScript::getModuleImageId(ZCsl* csl)
{
  int index = csl->get("index").asInt();

  if (index < 0 || index >= mSprite->mModulesManager.Size())
  {
    return "-1";
  }

  return mSprite->mModulesManager.Get(index).GetImageId();
}
//---------------------------------------------------------------------------

ZString SentryScript::getModuleIndexById(ZCsl* csl)
{
  int id = csl->get("id").asInt();

  for(int i = 0; i < mSprite->mModulesManager.Size(); i++)
  {
    if (mSprite->mModulesManager.Get(i).GetId() == id)
    {
      return i;
    }
  }

  return -1;
}
//---------------------------------------------------------------------------

ZString SentryScript::getModuleX(ZCsl* csl)
{
  int index = csl->get("index").asInt();

  if (index < 0 || index >= mSprite->mModulesManager.Size())
  {
    return "-1";
  }

  return mSprite->mModulesManager.Get(index).GetX();
}
//---------------------------------------------------------------------------

ZString SentryScript::getModuleY(ZCsl* csl)
{
  int index = csl->get("index").asInt();

  if (index < 0 || index >= mSprite->mModulesManager.Size())
  {
    return "-1";
  }

  return mSprite->mModulesManager.Get(index).GetY();
}
//---------------------------------------------------------------------------

ZString SentryScript::getModuleWidth(ZCsl* csl)
{
  int index = csl->get("index").asInt();

  if (index < 0 || index >= mSprite->mModulesManager.Size())
  {
    return "-1";
  }

  return mSprite->mModulesManager.Get(index).GetWidth();
}
//---------------------------------------------------------------------------

ZString SentryScript::getModuleHeight(ZCsl* csl)
{
  int index = csl->get("index").asInt();

  if (index < 0 || index >= mSprite->mModulesManager.Size())
  {
    return "-1";
  }

  return mSprite->mModulesManager.Get(index).GetHeight();
}
//---------------------------------------------------------------------------

ZString SentryScript::getModuleInfo(ZCsl* csl)
{
  int index = csl->get("index").asInt();

  if (index < 0 || index >= mSprite->mModulesManager.Size())
  {
    return "index out of range";
  }

  return mSprite->mModulesManager.Get(index).GetInfo().c_str();
}
//---------------------------------------------------------------------------


// frame modules
ZString SentryScript::getFModuleModuleId(ZCsl* csl)
{
  int frameIndex  = csl->get("frameIndex").asInt();
  int index       = csl->get("index").asInt();

  if (frameIndex < 0 || frameIndex >= mSprite->mFramesManager.Size())
  {
    return "-1";
  }

  if (index < 0 || index >= mSprite->mFramesManager.Get(frameIndex).mFModules.Size())
  {
    return -1;
  }

  return mSprite->mFramesManager.Get(frameIndex).mFModules.Get(index).GetModuleId();
}
//---------------------------------------------------------------------------

ZString SentryScript::getFModuleX(ZCsl* csl)
{
  int frameIndex  = csl->get("frameIndex").asInt();
  int index       = csl->get("index").asInt();

  if (frameIndex < 0 || frameIndex >= mSprite->mFramesManager.Size())
  {
    return "-1";
  }

  if (index < 0 || index >= mSprite->mFramesManager.Get(frameIndex).mFModules.Size())
  {
    return -1;
  }

  return mSprite->mFramesManager.Get(frameIndex).mFModules.Get(index).GetPos().mX;
}
//---------------------------------------------------------------------------

ZString SentryScript::getFModuleY(ZCsl* csl)
{
  int frameIndex  = csl->get("frameIndex").asInt();
  int index       = csl->get("index").asInt();

  if (frameIndex < 0 || frameIndex >= mSprite->mFramesManager.Size())
  {
    return "-1";
  }

  if (index < 0 || index >= mSprite->mFramesManager.Get(frameIndex).mFModules.Size())
  {
    return -1;
  }

  return mSprite->mFramesManager.Get(frameIndex).mFModules.Get(index).GetPos().mY;
}
//---------------------------------------------------------------------------

ZString SentryScript::getFModuleFlags(ZCsl* csl)
{
  int frameIndex  = csl->get("frameIndex").asInt();
  int index       = csl->get("index").asInt();

  if (frameIndex < 0 || frameIndex >= mSprite->mFramesManager.Size())
  {
    return "0";
  }

  if (index < 0 || index >= mSprite->mFramesManager.Get(frameIndex).mFModules.Size())
  {
    return "0";
  }

  switch(mSprite->mFramesManager.Get(frameIndex).mFModules.Get(index).GetFlags())
  {
    case FMODULE_BITFLAG_FLIP_X:
      return "1";

    case FMODULE_BITFLAG_FLIP_Y:
      return "2";

    case FMODULE_BITFLAG_ROT90:
      return "4";

    case FMODULE_BITFLAG_ROT270:
      return "8";
  }

  return "0";
}
//---------------------------------------------------------------------------


// frame logic items
ZString SentryScript::getFLogicItemId(ZCsl* csl)
{
  int frameIndex  = csl->get("frameIndex").asInt();
  int fLogicIndex = csl->get("fLogicIndex").asInt();
  int index       = csl->get("index").asInt();

  if (frameIndex < 0 || frameIndex >= mSprite->mFramesManager.Size())
  {
    return "-1";
  }

  if (fLogicIndex < 0 || fLogicIndex >= mSprite->mFramesManager.Get(frameIndex).mFLogic.Size())
  {
    return "-1";
  }

  // suffice to say: it works :)
  if (index < 0 || index >= mSprite->mFramesManager.Get(frameIndex).mFLogic.Get(fLogicIndex).mItems.Size())
  {
    return "-1";
  }

  return mSprite->mFramesManager.Get(frameIndex).mFLogic.Get(fLogicIndex).GetId();
}
//---------------------------------------------------------------------------

ZString SentryScript::getFLogicItemType(ZCsl* csl)
{
  int frameIndex  = csl->get("frameIndex").asInt();
  int fLogicIndex = csl->get("fLogicIndex").asInt();
  int index       = csl->get("index").asInt();

  if (frameIndex < 0 || frameIndex >= mSprite->mFramesManager.Size())
  {
    return "-1";
  }

  if (fLogicIndex < 0 || fLogicIndex >= mSprite->mFramesManager.Get(frameIndex).mFLogic.Size())
  {
    return "-1";
  }

  // suffice to say: it works :)
  if (index < 0 || index >= mSprite->mFramesManager.Get(frameIndex).mFLogic.Get(fLogicIndex).mItems.Size())
  {
    return "-1";
  }

  switch(mSprite->mFramesManager.Get(frameIndex).mFLogic.Get(fLogicIndex).mItems.Get(index).GetType())
  {
    case FRAME_LOGIC_ITEM_POINT:
      return "0";

    case FRAME_LOGIC_ITEM_RECT:
      return "1";
  }

  return 0;
}
//---------------------------------------------------------------------------

ZString SentryScript::getFLogicItemX(ZCsl* csl)
{
  int frameIndex  = csl->get("frameIndex").asInt();
  int fLogicIndex = csl->get("fLogicIndex").asInt();
  int index       = csl->get("index").asInt();

  if (frameIndex < 0 || frameIndex >= mSprite->mFramesManager.Size())
  {
    return "-1";
  }

  if (fLogicIndex < 0 || fLogicIndex >= mSprite->mFramesManager.Get(frameIndex).mFLogic.Size())
  {
    return "-1";
  }

  // suffice to say: it works :)
  if (index < 0 || index >= mSprite->mFramesManager.Get(frameIndex).mFLogic.Get(fLogicIndex).mItems.Size())
  {
    return "-1";
  }

  return mSprite->mFramesManager.Get(frameIndex).mFLogic.Get(fLogicIndex).mItems.Get(index).GetX();
}
//---------------------------------------------------------------------------

ZString SentryScript::getFLogicItemY(ZCsl* csl)
{
  int frameIndex  = csl->get("frameIndex").asInt();
  int fLogicIndex = csl->get("fLogicIndex").asInt();
  int index       = csl->get("index").asInt();

  if (frameIndex < 0 || frameIndex >= mSprite->mFramesManager.Size())
  {
    return "-1";
  }

  if (fLogicIndex < 0 || fLogicIndex >= mSprite->mFramesManager.Get(frameIndex).mFLogic.Size())
  {
    return "-1";
  }

  // suffice to say: it works :)
  if (index < 0 || index >= mSprite->mFramesManager.Get(frameIndex).mFLogic.Get(fLogicIndex).mItems.Size())
  {
    return "-1";
  }

  return mSprite->mFramesManager.Get(frameIndex).mFLogic.Get(fLogicIndex).mItems.Get(index).GetY();
}
//---------------------------------------------------------------------------

ZString SentryScript::getFLogicItemWidth(ZCsl* csl)
{
  int frameIndex  = csl->get("frameIndex").asInt();
  int fLogicIndex = csl->get("fLogicIndex").asInt();
  int index       = csl->get("index").asInt();

  if (frameIndex < 0 || frameIndex >= mSprite->mFramesManager.Size())
  {
    return "-1";
  }

  if (fLogicIndex < 0 || fLogicIndex >= mSprite->mFramesManager.Get(frameIndex).mFLogic.Size())
  {
    return "-1";
  }

  // suffice to say: it works :)
  if (index < 0 || index >= mSprite->mFramesManager.Get(frameIndex).mFLogic.Get(fLogicIndex).mItems.Size())
  {
    return "-1";
  }

  return mSprite->mFramesManager.Get(frameIndex).mFLogic.Get(fLogicIndex).mItems.Get(index).GetWidth();
}
//---------------------------------------------------------------------------

ZString SentryScript::getFLogicItemHeight(ZCsl* csl)
{
  int frameIndex  = csl->get("frameIndex").asInt();
  int fLogicIndex = csl->get("fLogicIndex").asInt();
  int index       = csl->get("index").asInt();

  if (frameIndex < 0 || frameIndex >= mSprite->mFramesManager.Size())
  {
    return "-1";
  }

  if (fLogicIndex < 0 || fLogicIndex >= mSprite->mFramesManager.Get(frameIndex).mFLogic.Size())
  {
    return "-1";
  }

  // suffice to say: it works :)
  if (index < 0 || index >= mSprite->mFramesManager.Get(frameIndex).mFLogic.Get(fLogicIndex).mItems.Size())
  {
    return "-1";
  }

  return mSprite->mFramesManager.Get(frameIndex).mFLogic.Get(fLogicIndex).mItems.Get(index).GetHeight();
}
//---------------------------------------------------------------------------


// frame logics
ZString SentryScript::getFLogicItemCount(ZCsl* csl)
{
  int frameIndex  = csl->get("frameIndex").asInt();
  int index       = csl->get("index").asInt();

  if (frameIndex < 0 || frameIndex >= mSprite->mFramesManager.Size())
  {
    return "-1";
  }

  if (index < 0 || index >= mSprite->mFramesManager.Get(frameIndex).mFLogic.Size())
  {
    return "-1";
  }

  return mSprite->mFramesManager.Get(frameIndex).mFLogic.Get(index).mItems.Size();
}
//---------------------------------------------------------------------------

ZString SentryScript::getFLogicId(ZCsl* csl)
{
  int frameIndex  = csl->get("frameIndex").asInt();
  int index       = csl->get("index").asInt();

  if (frameIndex < 0 || frameIndex >= mSprite->mFramesManager.Size())
  {
    return "-1";
  }

  if (index < 0 || index >= mSprite->mFramesManager.Get(frameIndex).mFLogic.Size())
  {
    return "-1";
  }

  return mSprite->mFramesManager.Get(frameIndex).mFLogic.Get(index).GetId();
}
//---------------------------------------------------------------------------

ZString SentryScript::getFLogicInfo(ZCsl* csl)
{
  int frameIndex  = csl->get("frameIndex").asInt();
  int index       = csl->get("index").asInt();

  if (frameIndex < 0 || frameIndex >= mSprite->mFramesManager.Size())
  {
    return "frame index out of range!";
  }

  if (index < 0 || index >= mSprite->mFramesManager.Get(frameIndex).mFLogic.Size())
  {
    return "frame logic index out of range!";
  }

  return mSprite->mFramesManager.Get(frameIndex).mFLogic.Get(index).GetInfo().c_str();
}
//---------------------------------------------------------------------------


// frames
ZString SentryScript::getFrameCount(ZCsl* csl)
{
  return mSprite->mFramesManager.Size();
}
//---------------------------------------------------------------------------

ZString SentryScript::getFModuleCount(ZCsl* csl)
{
  int frameIndex  = csl->get("frameIndex").asInt();

  if (frameIndex < 0 || frameIndex >= mSprite->mFramesManager.Size())
  {
    return "-1";
  }

  return mSprite->mFramesManager.Get(frameIndex).mFModules.Size();
}
//---------------------------------------------------------------------------

ZString SentryScript::getFLogicCount(ZCsl* csl)
{
  int frameIndex  = csl->get("frameIndex").asInt();

  if (frameIndex < 0 || frameIndex >= mSprite->mFramesManager.Size())
  {
    return "-1";
  }

  return mSprite->mFramesManager.Get(frameIndex).mFLogic.Size();
}
//---------------------------------------------------------------------------

ZString SentryScript::getFrameIndexById(ZCsl* csl)
{
  int id = csl->get("id").asInt();

  for(int i = 0; i < mSprite->mFramesManager.Size(); i++)
  {
    if (mSprite->mFramesManager.Get(i).GetId() == id)
    {
      return i;
    }
  }

  return -1;
}
//---------------------------------------------------------------------------

ZString SentryScript::getFrameId(ZCsl* csl)
{
  int frameIndex  = csl->get("frameIndex").asInt();

  if (frameIndex < 0 || frameIndex >= mSprite->mFramesManager.Size())
  {
    return "-1";
  }

  return mSprite->mFramesManager.Get(frameIndex).GetId();
}
//---------------------------------------------------------------------------

ZString SentryScript::getFrameInfo(ZCsl* csl)
{
  int frameIndex  = csl->get("frameIndex").asInt();

  if (frameIndex < 0 || frameIndex >= mSprite->mFramesManager.Size())
  {
    return "frame index out of range!";
  }

  return mSprite->mFramesManager.Get(frameIndex).GetInfo().c_str();
}
//---------------------------------------------------------------------------

ZString SentryScript::getFrameBoundX(ZCsl* csl)
{
  int frameIndex  = csl->get("frameIndex").asInt();

  if (frameIndex < 0 || frameIndex >= mSprite->mFramesManager.Size())
  {
    return "-1";
  }

  return mSprite->mFramesManager.Get(frameIndex).GetBoundRect(mSprite->mModulesManager).mX;
}
//---------------------------------------------------------------------------

ZString SentryScript::getFrameBoundY(ZCsl* csl)
{
  int frameIndex  = csl->get("frameIndex").asInt();

  if (frameIndex < 0 || frameIndex >= mSprite->mFramesManager.Size())
  {
    return "-1";
  }

  return mSprite->mFramesManager.Get(frameIndex).GetBoundRect(mSprite->mModulesManager).mY;
}
//---------------------------------------------------------------------------

ZString SentryScript::getFrameBoundWidth(ZCsl* csl)
{
  int frameIndex  = csl->get("frameIndex").asInt();

  if (frameIndex < 0 || frameIndex >= mSprite->mFramesManager.Size())
  {
    return "-1";
  }

  return mSprite->mFramesManager.Get(frameIndex).GetBoundRect(mSprite->mModulesManager).mWidth;
}
//---------------------------------------------------------------------------

ZString SentryScript::getFrameBoundHeight(ZCsl* csl)
{
  int frameIndex  = csl->get("frameIndex").asInt();

  if (frameIndex < 0 || frameIndex >= mSprite->mFramesManager.Size())
  {
    return "-1";
  }

  return mSprite->mFramesManager.Get(frameIndex).GetBoundRect(mSprite->mModulesManager).mHeight;
}
//---------------------------------------------------------------------------


// anim frames
ZString SentryScript::getAFrameId(ZCsl* csl)
{
  int animIndex  = csl->get("animIndex").asInt();
  int index      = csl->get("index").asInt();

  if (animIndex < 0 || animIndex >= mSprite->mAnimsManager.Size())
  {
    return "-1";
  }

  if (index < 0 || index >= mSprite->mAnimsManager.Get(animIndex).mAFrames.Size())
  {
    return "-1";
  }

  return mSprite->mAnimsManager.Get(animIndex).mAFrames.Get(index).GetId();
}
//---------------------------------------------------------------------------

ZString SentryScript::getAFrameTime(ZCsl* csl)
{
  int animIndex  = csl->get("animIndex").asInt();
  int index      = csl->get("index").asInt();

  if (animIndex < 0 || animIndex >= mSprite->mAnimsManager.Size())
  {
    return "-1";
  }

  if (index < 0 || index >= mSprite->mAnimsManager.Get(animIndex).mAFrames.Size())
  {
    return "-1";
  }

  return mSprite->mAnimsManager.Get(animIndex).mAFrames.Get(index).GetTime();
}
//---------------------------------------------------------------------------

ZString SentryScript::getAFrameFrameId(ZCsl* csl)
{
  int animIndex  = csl->get("animIndex").asInt();
  int index      = csl->get("index").asInt();

  if (animIndex < 0 || animIndex >= mSprite->mAnimsManager.Size())
  {
    return "-1";
  }

  if (index < 0 || index >= mSprite->mAnimsManager.Get(animIndex).mAFrames.Size())
  {
    return "-1";
  }

  return mSprite->mAnimsManager.Get(animIndex).mAFrames.Get(index).GetFrameId();
}
//---------------------------------------------------------------------------

ZString SentryScript::getAFrameFlags(ZCsl* csl)
{
  int animIndex  = csl->get("animIndex").asInt();
  int index      = csl->get("index").asInt();

  if (animIndex < 0 || animIndex >= mSprite->mAnimsManager.Size())
  {
    return "0";
  }

  if (index < 0 || index >= mSprite->mAnimsManager.Get(animIndex).mAFrames.Size())
  {
    return "0";
  }

  switch(mSprite->mAnimsManager.Get(animIndex).mAFrames.Get(index).GetFlags())
  {
    case ANIMFRAME_BITFLAG_FLIP_X:
      return "1";

    case ANIMFRAME_BITFLAG_FLIP_Y:
      return "2";

    case ANIMFRAME_BITFLAG_ROT90:
      return "4";

    case ANIMFRAME_BITFLAG_ROT270:
      return "8";
  }

  return 0;
}
//---------------------------------------------------------------------------

ZString SentryScript::getAFrameX(ZCsl* csl)
{
  int animIndex  = csl->get("animIndex").asInt();
  int index      = csl->get("index").asInt();

  if (animIndex < 0 || animIndex >= mSprite->mAnimsManager.Size())
  {
    return "-1";
  }

  if (index < 0 || index >= mSprite->mAnimsManager.Get(animIndex).mAFrames.Size())
  {
    return "-1";
  }

  return mSprite->mAnimsManager.Get(animIndex).mAFrames.Get(index).GetPos().mX;
}
//---------------------------------------------------------------------------

ZString SentryScript::getAFrameY(ZCsl* csl)
{
  int animIndex  = csl->get("animIndex").asInt();
  int index      = csl->get("index").asInt();

  if (animIndex < 0 || animIndex >= mSprite->mAnimsManager.Size())
  {
    return "-1";
  }

  if (index < 0 || index >= mSprite->mAnimsManager.Get(animIndex).mAFrames.Size())
  {
    return "-1";
  }

  return mSprite->mAnimsManager.Get(animIndex).mAFrames.Get(index).GetPos().mY;
}
//---------------------------------------------------------------------------


// anims
ZString SentryScript::getAnimCount(ZCsl* csl)
{
  return mSprite->mAnimsManager.Size();
}
//---------------------------------------------------------------------------

ZString SentryScript::getAFrameCount(ZCsl* csl)
{
  int animIndex  = csl->get("animIndex").asInt();

  if (animIndex < 0 || animIndex >= mSprite->mAnimsManager.Size())
  {
    return "-1";
  }

  return mSprite->mAnimsManager.Get(animIndex).mAFrames.Size();
}
//---------------------------------------------------------------------------

ZString SentryScript::getAnimId(ZCsl* csl)
{
  int animIndex  = csl->get("animIndex").asInt();

  if (animIndex < 0 || animIndex >= mSprite->mAnimsManager.Size())
  {
    return "-1";
  }

  return mSprite->mAnimsManager.Get(animIndex).GetId();
}
//---------------------------------------------------------------------------

ZString SentryScript::getAnimInfo(ZCsl* csl)
{
  int animIndex  = csl->get("animIndex").asInt();

  if (animIndex < 0 || animIndex >= mSprite->mAnimsManager.Size())
  {
    return "-1";
  }

  return mSprite->mAnimsManager.Get(animIndex).GetInfo().c_str();
}
//---------------------------------------------------------------------------

/*
  addModuleFunc("GetColorMapsCount(const imageIndex)",                            mHandleGetColorMapsCount);
  addModuleFunc("GetColorMapId(const imageIndex, const index)",                   mHandleGetColorMapId);
  addModuleFunc("GetColorMapItemsCount(const imageIndex, const colorMapIndex)",   mHandleGetColorMapItemsCount);
  addModuleFunc("GetColorMapItemId(const imageIndex, const colorMapIndex)",       mHandleGetColorMapItemId);
  addModuleFunc("GetColorMapItemSrcColor(const imageIndex, const colorMapIndex)", mHandleGetColorMapItemSrcColor);
  addModuleFunc("GetColorMapItemSrcColor(const imageIndex, const colorMapIndex)", mHandleGetColorMapItemDstColor);
*/

ZString SentryScript::getColorMapsCount(ZCsl* csl)
{
  int imageIndex = csl->get("imageIndex").asInt();

  if (imageIndex < 0 || imageIndex >= mSprite->mImagesManager.Size())
  {
    return "-1";
  }

  return mSprite->mImagesManager.Get(imageIndex).mColorMaps.Size();
}
//---------------------------------------------------------------------------

ZString SentryScript::getColorMapId(ZCsl* csl)
{
  int imageIndex    = csl->get("imageIndex").asInt();
  int colorMapIndex = csl->get("colorMapIndex").asInt();

  if (imageIndex < 0 || imageIndex >= mSprite->mImagesManager.Size())
  {
    return "-1";
  }

  CImage image = mSprite->mImagesManager.Get(imageIndex);

  if (colorMapIndex < 0 || colorMapIndex >= image.mColorMaps.Size())
  {
    return "-1";
  }

  return image.mColorMaps.Get(colorMapIndex).GetId();
}
//---------------------------------------------------------------------------

ZString SentryScript::getColorMapItemsCount(ZCsl* csl)
{
  int imageIndex = csl->get("imageIndex").asInt();
  int colorMapIndex = csl->get("colorMapIndex").asInt();

  if (imageIndex < 0 || imageIndex >= mSprite->mImagesManager.Size())
  {
    return "-1";
  }

  CImage image = mSprite->mImagesManager.Get(imageIndex);

  if (colorMapIndex < 0 || colorMapIndex >= image.mColorMaps.Size())
  {
    return "-1";
  }

  return image.mColorMaps.Get(colorMapIndex).mItems.Size();
}
//---------------------------------------------------------------------------

ZString SentryScript::getColorMapItemId(ZCsl* csl)
{
  int imageIndex = csl->get("imageIndex").asInt();
  int colorMapIndex = csl->get("colorMapIndex").asInt();
  int colorMapItemIndex = csl->get("colorMapItemIndex").asInt();

  if (imageIndex < 0 || imageIndex >= mSprite->mImagesManager.Size())
  {
    return "-1";
  }

  CImage image = mSprite->mImagesManager.Get(imageIndex);

  if (colorMapIndex < 0 || colorMapIndex >= image.mColorMaps.Size())
  {
    return "-1";
  }

  CColorMap colorMap = image.mColorMaps.Get(colorMapIndex);

  if (colorMapItemIndex < 0 || colorMapItemIndex >= colorMap.mItems.Size())
  {
    return "-1";
  }

  return colorMap.mItems.Get(colorMapItemIndex).GetId();
}
//---------------------------------------------------------------------------

ZString SentryScript::getColorMapItemSrcColor(ZCsl* csl)
{
  int imageIndex = csl->get("imageIndex").asInt();
  int colorMapIndex = csl->get("colorMapIndex").asInt();
  int colorMapItemIndex = csl->get("colorMapItemIndex").asInt();

  if (imageIndex < 0 || imageIndex >= mSprite->mImagesManager.Size())
  {
    return "-1";
  }

  CImage image = mSprite->mImagesManager.Get(imageIndex);

  if (colorMapIndex < 0 || colorMapIndex >= image.mColorMaps.Size())
  {
    return "-1";
  }

  CColorMap colorMap = image.mColorMaps.Get(colorMapIndex);

  if (colorMapItemIndex < 0 || colorMapItemIndex >= colorMap.mItems.Size())
  {
    return "-1";
  }

  return colorMap.mItems.Get(colorMapItemIndex).GetSrcColor();
}
//---------------------------------------------------------------------------

ZString SentryScript::getColorMapItemDstColor(ZCsl* csl)
{
  int imageIndex = csl->get("imageIndex").asInt();
  int colorMapIndex = csl->get("colorMapIndex").asInt();
  int colorMapItemIndex = csl->get("colorMapItemIndex").asInt();

  if (imageIndex < 0 || imageIndex >= mSprite->mImagesManager.Size())
  {
    return "-1";
  }

  CImage image = mSprite->mImagesManager.Get(imageIndex);

  if (colorMapIndex < 0 || colorMapIndex >= image.mColorMaps.Size())
  {
    return "-1";
  }

  CColorMap colorMap = image.mColorMaps.Get(colorMapIndex);

  if (colorMapItemIndex < 0 || colorMapItemIndex >= colorMap.mItems.Size())
  {
    return "-1";
  }

  return colorMap.mItems.Get(colorMapItemIndex).GetDstColor();
}
//---------------------------------------------------------------------------

