#include <SysUtils.hpp>
#pragma hdrstop
//---------------------------------------------------------------------------

#include "tinyxml2.h"
#include "XSpriteDesigner.h"
#include "Script.h"

using namespace tinyxml2;
//---------------------------------------------------------------------------

void CImage::SetFileName(std::string filename)
{
  mFileName = filename;

  ImageCache::Instance()->Add(mFileName);
}
//---------------------------------------------------------------------------

CImage::~CImage()
{
  ImageCache::Instance()->Remove(mFileName);
}
//---------------------------------------------------------------------------

void CImage::SetTransparentColor(int v)
{
  mTransparentColor = (0xFF000000 | v);
}
//---------------------------------------------------------------------------

CRect CFrame::GetBoundRect(CManager<CModule> modulesManager)
{
  CRect rect;

  for(int i = 0; i < mFModules.Size(); i++)
  {
    CFrameModule fmodule = mFModules.Get(i);
    CPoint pos = fmodule.GetPos();

    int moduleIndex = modulesManager.GetItemIndexById(fmodule.GetModuleId());

    if (moduleIndex == -1)
    {
      continue;
    }

    CModule module = modulesManager.Get(moduleIndex);

    int width = module.GetWidth();
    int height = module.GetHeight();

    if (rect.mX > pos.mX)
    {
      rect.mWidth += rect.mX - pos.mX;
      rect.mX = pos.mX;
    }
    
    if (rect.mX + rect.mWidth < pos.mX + width)
    {
      rect.mWidth += pos.mX + width - (rect.mX + rect.mWidth);
    }

    if (rect.mY > pos.mY)
    {
      rect.mHeight += rect.mY - pos.mY;
      rect.mY = pos.mY;
    }

    if (rect.mY + rect.mHeight < pos.mY + height)
    {
      rect.mHeight += pos.mY + height - (rect.mY + rect.mHeight);
    }
  }

  return rect;
}
//---------------------------------------------------------------------------

void CXSpriteDesigner::New()
{
  mState = SPR_DOC_STATE_NEW;
  mFileName = "";

  mScript = new SentryScript(this);
}
//---------------------------------------------------------------------------

bool CXSpriteDesigner::Save(std::string fileName)
{
  mState = SPR_DOC_STATE_SAVED;

  tinyxml2::XMLDocument doc;

  doc.Parse("<?xml version=\"1.0\" encoding=\"UTF-8\" ?>");

  auto root = doc.NewElement("xsprite");
  root->SetAttribute("version", "1.0");

  //
  // Write images tag
  //

  auto images = doc.NewElement("images");

  for(int i = 0; i < mImagesManager.Size(); i++)
  {
    CImage image = mImagesManager.Get(i);

    auto imageElem = doc.NewElement("image");
    imageElem->SetAttribute("id", image.GetId());
    imageElem->SetAttribute("file", image.GetFileName().c_str());
    imageElem->SetAttribute("transparent", image.GetTransparentColor());
    imageElem->SetAttribute("info", image.GetInfo().c_str());

    //
    // build colormaps tag for this image
    //
    auto colorMapsElem = doc.NewElement("colormaps");

    for(int i = 0; i < image.mColorMaps.Size(); i++)
    {
      CColorMap colorMap = image.mColorMaps.Get(i);

      auto colorMapElem = doc.NewElement("colormap");

      colorMapElem->SetAttribute("id",       colorMap.GetId());
      //colorMapElem.SetAttribute("mImageId", colorMap.GetImageId());
      colorMapElem->SetAttribute("info",     colorMap.GetInfo().c_str());

      for(int j = 0; j < colorMap.mItems.Size(); j++)
      {
        CColorMapItem colorMapItem = colorMap.mItems.Get(j);

        auto colorMapItemElem = doc.NewElement("colormapitem");

        colorMapItemElem->SetAttribute("id",  colorMapItem.GetId());
        colorMapItemElem->SetAttribute("src", colorMapItem.GetSrcColor());
        colorMapItemElem->SetAttribute("dst", colorMapItem.GetDstColor());

        colorMapElem->InsertEndChild(colorMapItemElem);
      }

      colorMapsElem->InsertEndChild(colorMapElem);
    }

    root->InsertEndChild(colorMapsElem);

    images->InsertEndChild(imageElem);
  }

  root->InsertEndChild(images);

  //
  // Write modules tag
  //
  auto modules = doc.NewElement("modules");

  for(int i = 0; i < mModulesManager.Size(); i++)
  {
    CModule item = mModulesManager.Get(i);

    auto module = doc.NewElement("module");
    module->SetAttribute("id", item.GetId());
    module->SetAttribute("imageid", item.GetImageId());
    module->SetAttribute("x", item.GetX());
    module->SetAttribute("y", item.GetY());
    module->SetAttribute("width", item.GetWidth());
    module->SetAttribute("height", item.GetHeight());
    module->SetAttribute("info", item.GetInfo().c_str());

    modules->InsertEndChild(module);
  }

  root->InsertEndChild(modules);

  //
  // Write frames tag
  //
  auto frames = doc.NewElement("frames");

  for(int i = 0; i < mFramesManager.Size(); i++)
  {
    CFrame item = mFramesManager.Get(i);

    auto frame = doc.NewElement("frame");

    frame->SetAttribute("id", item.GetId());
    frame->SetAttribute("info", item.GetInfo().c_str());

    // build the frame modules tag
    XMLElement* fmodules = doc.NewElement("fmodules");

    for(int j = 0; j < item.mFModules.Size(); j++)
    {
      CFrameModule fmoduleItem = item.mFModules.Get(j);

      auto fmodule = doc.NewElement("fmodule");

      fmodule->SetAttribute("moduleid", fmoduleItem.GetModuleId());
      fmodule->SetAttribute("x", fmoduleItem.GetPos().mX);
      fmodule->SetAttribute("y", fmoduleItem.GetPos().mY);
      fmodule->SetAttribute("flags", fmoduleItem.GetFlags());

      fmodules->InsertEndChild(fmodule);
    }

    frame->InsertEndChild(fmodules);

    // build the frame logic tag
    XMLElement* flogics = doc.NewElement("flogics");

    for(int j = 0; j < item.mFLogic.Size(); j++)
    {
      CFrameLogic flogicA = item.mFLogic.Get(j);

      XMLElement* flogic = doc.NewElement("flogic");

      flogic->SetAttribute("id", flogicA.GetId());
      flogic->SetAttribute("info", flogicA.GetInfo().c_str());

      for(int k = 0; k < flogicA.mItems.Size(); k++)
      {
        CFrameLogicItem flogicItem = flogicA.mItems.Get(k);

        auto xmlElemFLogicItem = doc.NewElement("flogicitem");

        xmlElemFLogicItem->SetAttribute("id", flogicItem.GetId());
        xmlElemFLogicItem->SetAttribute("type", flogicItem.GetType());

        switch(flogicItem.GetType())
        {
          case FRAME_LOGIC_ITEM_POINT:
            xmlElemFLogicItem->SetAttribute("x", flogicItem.GetX());
            xmlElemFLogicItem->SetAttribute("y", flogicItem.GetY());
            break;

          case FRAME_LOGIC_ITEM_RECT:
            xmlElemFLogicItem->SetAttribute("x", flogicItem.GetX());
            xmlElemFLogicItem->SetAttribute("y", flogicItem.GetY());
            xmlElemFLogicItem->SetAttribute("w", flogicItem.GetWidth());
            xmlElemFLogicItem->SetAttribute("h", flogicItem.GetHeight());
            break;
        }

        flogic->InsertEndChild(xmlElemFLogicItem);
      }

      flogics->InsertEndChild(flogic);
    }

    frame->InsertEndChild(flogics);

    // frame is complete, insert it
    frames->InsertEndChild(frame);
  }

  root->InsertEndChild(frames);

  //
  // Write anims tag
  //
  auto anims = doc.NewElement("anims");

  for(int i = 0; i < mAnimsManager.Size(); i++)
  {
    CAnim item = mAnimsManager.Get(i);

    auto anim = doc.NewElement("anim");

    anim->SetAttribute("id", item.GetId());
    anim->SetAttribute("info", item.GetInfo().c_str());

    for(int j = 0; j < item.mAFrames.Size(); j++)
    {
      CAnimFrame aFrameItem = item.mAFrames.Get(j);

      auto aFrame = doc.NewElement("aframe");

      aFrame->SetAttribute("frameid", aFrameItem.GetFrameId());
      aFrame->SetAttribute("time", aFrameItem.GetTime());
      aFrame->SetAttribute("x", aFrameItem.GetPos().mX);
      aFrame->SetAttribute("y", aFrameItem.GetPos().mY);
      aFrame->SetAttribute("flags", aFrameItem.GetFlags());

      anim->InsertEndChild(aFrame);
    }

    anims->InsertEndChild(anim);
  }

  root->InsertEndChild(anims);

  doc.InsertEndChild(root);
  
  doc.SaveFile(fileName.c_str());

  // TODO: if savefile returns true
  mState = SPR_DOC_STATE_SAVED;
  mFileName = fileName;

  return true;
}
//---------------------------------------------------------------------------

bool CXSpriteDesigner::Load(std::string fileName)
{
  Close();

  tinyxml2::XMLDocument doc;
  auto loadResult = doc.LoadFile(fileName.c_str());

  if (loadResult != XML_SUCCESS)
  {
    mFileMessages.push_back("Fatal: error reading document.");
    return false;
  }

  auto nodeXSprite = doc.FirstChildElement( "xsprite" );

  if (!nodeXSprite)
  {
    mFileMessages.push_back("Fatal: invalid xsprite document - missing <xsprite> tag.");
    return false;
  }

  //
  // Read images
  //
  auto nodeImages = nodeXSprite->FirstChildElement( "images" );

  if (nodeImages)
  {
    do
    {
      auto nodeImageItem = nodeImages->FirstChildElement( "image" );

      if (nodeImageItem)
      {
        do
        {
          auto elem = nodeImageItem->ToElement();

          if (!elem)
          {
            continue;
          }

          const char* id = elem->Attribute( "id" );
          const char* file = elem->Attribute( "file" );
          const char* transparent = elem->Attribute( "transparent" );
          const char* info = elem->Attribute( "info" );

          if (id && file && transparent && info)
          {
            CImage image;

            image.SetId(atoi(id));

            std::string imageFile(file);

            // if the image file has no path set, try to load the image from the same
            // folder as the sprite definition file
            if (ExtractFileDir(imageFile.c_str()) == "")
            {
              imageFile = std::string(UTF8Encode(ExtractFileDir(fileName.c_str())).c_str()) + "\\" + imageFile;
            }

            image.SetFileName(imageFile);

            image.SetTransparentColor(atoi(transparent));
            image.SetInfo(info);


            // TODO: read the color maps for this image
            //
            // Read palette maps
            //
            auto nodeColorMaps = nodeImageItem->FirstChildElement( "colormaps" );

            if (nodeColorMaps)
            {
              do
              {
                auto nodeColorMap = nodeColorMaps->FirstChildElement( "colormap" );

                if (nodeColorMap)
                {
                  do
                  {
                    auto elem = nodeColorMap->ToElement();

                    if (!elem)
                    {
                      continue;
                    }

                    const char* id      = elem->Attribute( "id" );
                    const char* imageId = elem->Attribute( "imageid" );
                    const char* info    = elem->Attribute( "info" );

                    if (id && info && imageId)
                    {
                      CColorMap colorMap;

                      colorMap.SetId(atoi(id));
                      colorMap.SetId(atoi(imageId));
                      colorMap.SetInfo(info);

                      //
                      // Read items (color mappings) for this colormap
                      //
                      auto nodeColorMapItem = nodeColorMap->FirstChildElement( "cmitem" );

                      if (nodeColorMapItem)
                      {
                        do
                        {
                          auto elem = nodeColorMapItem->ToElement();

                          if (!elem)
                          {
                            continue;
                          }

                          const char* id       = elem->Attribute( "id" );
                          const char* srcColor = elem->Attribute( "src" );
                          const char* dstColor = elem->Attribute( "dst" );

                          if (id && srcColor && dstColor)
                          {
                            CColorMapItem colorMapItem;

                            colorMapItem.SetId(atoi(id));
                            colorMapItem.SetSrcColor(atoi(srcColor));
                            colorMapItem.SetDstColor(atoi(dstColor));

                            int index = colorMap.mItems.New();

                            colorMap.mItems.Set(index, colorMapItem);
                          }
                          else
                          {
                            mFileMessages.push_back("Warning: <colormapitem> ignored - missing attributes.");
                          }
                        }
                        while((nodeColorMapItem = nodeColorMapItem->NextSiblingElement( "cmitem" )) != 0);
                      }

                      int index = image.mColorMaps.New();

                      image.mColorMaps.Set(index, colorMap);
                    }
                    else
                    {
                       mFileMessages.push_back("Warning: <colormap> ignored - missing attributes.");
                    }
                  }
                  while((nodeColorMap = nodeColorMap->NextSiblingElement( "colormap" )) != 0);
                }
                else
                {
                   mFileMessages.push_back("Warning: no <colormap> tags found.");
                }
              }
              while((nodeColorMaps = nodeColorMaps->NextSiblingElement( "colormaps" )) != 0);
            }
            else
            {
              mFileMessages.push_back("Warning: <colormaps> tag missing.");
            }

            // add the image
            int index = mImagesManager.New();

            mImagesManager.Set(index, image);
          }
        }
        while((nodeImageItem = nodeImageItem->NextSiblingElement( "image" )) != 0);
      }
      else
      {
         mFileMessages.push_back("Warning: no <images> tags found.");
      }
    }
    while((nodeImages = nodeImages->NextSiblingElement( "images" )) != 0);
  }
  else
  {
    mFileMessages.push_back("Warning: <images> tag missing.");
  }

  //
  // Read modules
  //
  auto nodeModules = nodeXSprite->FirstChildElement( "modules" );

  if (nodeModules)
  {
    do
    {
      auto nodeModuleItem = nodeModules->FirstChildElement( "module" );

      if (nodeModuleItem)
      {
        do
        {
          auto elem = nodeModuleItem->ToElement();

          if (!elem)
          {
            continue;
          }

          const char* id = elem->Attribute( "id" );
          const char* imageid = elem->Attribute( "imageid" );
          const char* x = elem->Attribute( "x" );
          const char* y = elem->Attribute( "y" );
          const char* w = elem->Attribute( "width" );
          const char* h = elem->Attribute( "height" );
          const char* info = elem->Attribute( "info" );

          if (id && imageid && x && y && w && h && info)
          {
            CModule module;

            module.SetId(atoi(id));
            module.SetImageId(atoi(imageid));
            module.SetX(atoi(x));
            module.SetY(atoi(y));
            module.SetWidth(atoi(w));
            module.SetHeight(atoi(h));
            module.SetInfo(info);

            int index = mModulesManager.New();

            mModulesManager.Set(index, module);
          }
          else
          {
             mFileMessages.push_back("Warning: <module> ignored - missing attributes.");
          }
        }
        while((nodeModuleItem = nodeModuleItem->NextSiblingElement( "module" )) != 0);
      }
      else
      {
         mFileMessages.push_back("Warning: no <module> tags found.");
      }
    }
    while((nodeModules = nodeModules->NextSiblingElement( "modules" )) != 0);
  }
  else
  {
    mFileMessages.push_back("Warning: <modules> tag missing.");
  }

  //
  // Read frames
  //
  auto nodeFrames = nodeXSprite->FirstChildElement( "frames" );

  if (nodeFrames)
  {
    do
    {
      auto nodeFrameItem = nodeFrames->FirstChildElement( "frame" );

      if (nodeFrameItem)
      {
        do
        {
          auto elem = nodeFrameItem->ToElement();

          if (!elem)
          {
            continue;
          }

          const char* id = elem->Attribute( "id" );
          const char* info = elem->Attribute( "info" );

          if (id && info)
          {
            CFrame frame;

            frame.SetId(atoi(id));
            frame.SetInfo(info);

            auto nodeFModules = nodeFrameItem->FirstChildElement( "fmodules" );

            if (nodeFModules)
            {
              //
              // Read fmodules for this frame
              //
              auto nodeFModuleItem = nodeFModules->FirstChildElement( "fmodule" );

              if (nodeFModuleItem)
              {
                do
                {
                  auto elem = nodeFModuleItem->ToElement();

                  if (!elem)
                  {
                    continue;
                  }

                  const char* moduleId = elem->Attribute( "moduleid" );
                  const char* x = elem->Attribute( "x" );
                  const char* y = elem->Attribute( "y" );
                  const char* flags = elem->Attribute( "flags" );

                  if (moduleId && x && y && flags)
                  {
                    CFrameModule fmodule;

                    fmodule.SetPos(CPoint(atoi(x), atoi(y)));
                    fmodule.SetModuleId(atoi(moduleId));
                    fmodule.SetFlags(atoi(flags));

                    int index = frame.mFModules.New();

                    frame.mFModules.Set(index, fmodule);
                  }
                  else
                  {
                    mFileMessages.push_back("Warning: <fmodule> ignored - missing attributes.");
                  }
                }
                while((nodeFModuleItem = nodeFModuleItem->NextSiblingElement( "fmodule" )) != 0);
              }
            }
            else
            {
              mFileMessages.push_back("Warning: no <fmodules> tag found for frame.");
            }

            //
            // TODO: Read the frame logics for this frame
            //
            auto nodeFLogics = nodeFrameItem->FirstChildElement( "flogics" );

            if (nodeFLogics)
            {
              //
              // Read flogics for this frame
              //
              auto nodeFLogicItem = nodeFLogics->FirstChildElement( "flogic" );

              if (nodeFLogicItem)
              {
                do
                {
                  auto elem = nodeFLogicItem->ToElement();

                  if (!elem)
                  {
                    continue;
                  }

                  const char* flogicId   = elem->Attribute( "id" );
                  const char* flogicInfo = elem->Attribute( "info" );

                  if (flogicId && flogicInfo)
                  {
                    CFrameLogic fLogic;

                    fLogic.SetId(atoi(flogicId));
                    fLogic.SetInfo(flogicInfo);

                    // get the items for this flogic
                    auto nodeFLogicItems = nodeFLogicItem->FirstChildElement( "flogicitem" );

                    if (nodeFLogicItems)
                    {
                      do
                      {
                        auto elem = nodeFLogicItems->ToElement();

                        if (!elem)
                        {
                          continue;
                        }

                        // TODO: get the flogic item attributes
                        const char* flogicItemId   = elem->Attribute( "id" );
                        const char* flogicItemType = elem->Attribute( "type" );

                        if (flogicItemId && flogicItemType)
                        {
                          switch(atoi(flogicItemType))
                          {
                            case FRAME_LOGIC_ITEM_POINT:
                              {
                                const char* x = elem->Attribute( "x" );
                                const char* y = elem->Attribute( "y" );

                                if (x && y)
                                {
                                  CFrameLogicItem fLogicItem;

                                  fLogicItem.SetId(atoi(flogicItemId));
                                  fLogicItem.SetType((TFrameLogicItem)atoi(flogicItemType));
                                  fLogicItem.SetX(atoi(x));
                                  fLogicItem.SetY(atoi(y));

                                  int index = fLogic.mItems.New();

                                  fLogic.mItems.Set(index, fLogicItem);
                                }
                              }
                              break;

                            case FRAME_LOGIC_ITEM_RECT:
                              {
                                const char* x = elem->Attribute( "x" );
                                const char* y = elem->Attribute( "y" );
                                const char* w = elem->Attribute( "w" );
                                const char* h = elem->Attribute( "h" );

                                if (x && y && w && h)
                                {
                                  CFrameLogicItem fLogicItem;

                                  fLogicItem.SetId(atoi(flogicItemId));
                                  fLogicItem.SetType((TFrameLogicItem)atoi(flogicItemType));
                                  fLogicItem.SetX(atoi(x));
                                  fLogicItem.SetY(atoi(y));
                                  fLogicItem.SetWidth(atoi(w));
                                  fLogicItem.SetHeight(atoi(h));

                                  int index = fLogic.mItems.New();

                                  fLogic.mItems.Set(index, fLogicItem);
                                }
                              }
                              break;
                          }
                        }
                      }
                      while((nodeFLogicItems = nodeFLogicItems->NextSiblingElement("flogicitem")) != 0);
                    }

                    int index = frame.mFLogic.New();

                    frame.mFLogic.Set(index, fLogic);
                  }
                  else
                  {
                    mFileMessages.push_back("Warning: <fmodule> ignored - missing attributes.");
                  }
                }
                while((nodeFLogicItem = nodeFLogicItem->NextSiblingElement( "flogic" )) != 0);
              }
            }
            else
            {
              mFileMessages.push_back("Warning: no <flogics> tag found for frame.");
            }

            int index = mFramesManager.New();

            mFramesManager.Set(index, frame);
          }
          else
          {
             mFileMessages.push_back("Warning: <Frame> ignored - missing attributes.");
          }
        }
        while((nodeFrameItem = nodeFrameItem->NextSiblingElement( "frame" )) != 0);
      }
      else
      {
         mFileMessages.push_back("Warning: no <Frame> tags found.");
      }
    }
    while((nodeFrames = nodeFrames->NextSiblingElement( "frames" )) != 0);
  }
  else
  {
    mFileMessages.push_back("Warning: <Frames> tag missing.");
  }

  //
  // Read anims
  //
  auto nodeAnims = nodeXSprite->FirstChildElement( "anims" );

  if (nodeAnims)
  {
    do
    {
      auto nodeAnimItem = nodeAnims->FirstChildElement( "anim" );

      if (nodeAnimItem)
      {
        do
        {
          auto elem = nodeAnimItem->ToElement();

          if (!elem)
          {
            continue;
          }

          const char* id = elem->Attribute( "id" );
          const char* info = elem->Attribute( "info" );

          if (id && info)
          {
            CAnim anim;

            anim.SetId(atoi(id));
            anim.SetInfo(info);

            //
            // Read aframes for this anim
            //
            auto nodeAFrameItem = nodeAnimItem->FirstChildElement( "aframe" );

            if (nodeAFrameItem)
            {
              do
              {
                auto elem = nodeAFrameItem->ToElement();

                if (!elem)
                {
                  continue;
                }

                const char* frameId = elem->Attribute( "frameid" );
                const char* time = elem->Attribute( "time" );
                const char* x = elem->Attribute( "x" );
                const char* y = elem->Attribute( "y" );
                const char* flags = elem->Attribute( "flags" );

                if (frameId && x && y && flags && time)
                {
                  CAnimFrame aFrame;

                  aFrame.SetPos(CPoint(atoi(x), atoi(y)));
                  aFrame.SetFrameId(atoi(frameId));
                  aFrame.SetTime(atoi(time));
                  aFrame.SetFlags(atoi(flags));

                  int index = anim.mAFrames.New();

                  anim.mAFrames.Set(index, aFrame);
                }
                else
                {
                  mFileMessages.push_back("Warning: <aframe> ignored - missing attributes.");
                }
              }
              while((nodeAFrameItem = nodeAFrameItem->NextSiblingElement( "aframe" )) != 0);
            }

            int index = mAnimsManager.New();

            mAnimsManager.Set(index, anim);
          }
          else
          {
             mFileMessages.push_back("Warning: <AFrame> ignored - missing attributes.");
          }
        }
        while((nodeAnimItem = nodeAnimItem->NextSiblingElement( "anim" )) != 0);
      }
      else
      {
         mFileMessages.push_back("Warning: no <Anim> tags found.");
      }
    }
    while((nodeAnims = nodeAnims->NextSiblingElement( "anims" )) != 0);
  }
  else
  {
    mFileMessages.push_back("Warning: <Anims> tag missing.");
  }

  mState = SPR_DOC_STATE_SAVED;
  mFileName = fileName;
  mScript = new SentryScript(this);

  return true;
}
//---------------------------------------------------------------------------

void CXSpriteDesigner::Close()
{
  mFileMessages.clear();

  mState = SPR_DOC_STATE_CLOSED;

  mImagesManager.Clear();
  mModulesManager.Clear();
  mFramesManager.Clear();
  mAnimsManager.Clear();
  //mColorMapManager.Clear();

  mFileName = "";

  delete mScript;
  mScript = 0;
}
//---------------------------------------------------------------------------

Gdiplus::Bitmap* CXSpriteDesigner::GetModuleBitmap(CModule module)
{
  for(int i = 0; i < mImagesManager.Size(); i++)
  {
    if (mImagesManager.Get(i).GetId() == module.GetImageId())
    {
      return ImageCache::Instance()->GetBitmap(mImagesManager.Get(i).GetFileName());
    }
  }

  return 0;
}
//---------------------------------------------------------------------------

bool CXSpriteDesigner::LoadScriptFromFile(std::string fileName)
{
  if (mScript)
  {
    mScript->LoadScriptFromFile(fileName);

    return true;
  }

  return false;
}
//---------------------------------------------------------------------------

std::string CXSpriteDesigner::GetScriptVar(std::string name)
{
  if (mScript)
  {
    return mScript->GetScriptVar(name);
  }

  return "";
}
//---------------------------------------------------------------------------

void CXSpriteDesigner::SetScriptVar(std::string name, std::string value)
{
  if (mScript)
  {
    mScript->SetScriptVar(name, value);
  }
}
//---------------------------------------------------------------------------

std::string CXSpriteDesigner::GetScript()
{
  if (mScript)
  {
    return mScript->GetScript();
  }

  return "";
}
//---------------------------------------------------------------------------

std::string CXSpriteDesigner::GetScriptError()
{
  if (mScript)
  {
    return mScript->GetError();
  }

  return "";
}
//---------------------------------------------------------------------------

std::string CXSpriteDesigner::GetScriptParam(int index, int type)
{
  if (mScript)
  {
    return mScript->GetParam(index, type);
  }

  return "";
}
//---------------------------------------------------------------------------

std::string CXSpriteDesigner::SetScriptParam(int index, int type, const char* value)
{
  if (mScript)
  {
    return mScript->SetParam(index, type, value);
  }

  return "";
}
//---------------------------------------------------------------------------

bool CXSpriteDesigner::InitScript()
{
  if (mScript)
  {
    return mScript->Init();
  }

  return false;
}
//---------------------------------------------------------------------------

bool CXSpriteDesigner::ExecuteScript()
{
  if (mScript)
  {
    return mScript->Execute();
  }

  return false;
}
//---------------------------------------------------------------------------

#pragma package(smart_init)

