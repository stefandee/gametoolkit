//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FMain.h"
#include "FAbout.h"
#include "tinyxml2.h"
#include "Unit1.cpp"
#include "FOptions.h"
#include "Script.h"
#include "FScript.h"
#include <System.hpp>

using namespace tinyxml2;
//---------------------------------------------------------------------------

#pragma package(smart_init)
#pragma link "StringGridX"
#pragma link "CSPIN"
#pragma link "cspin"
#pragma link "StringGridX"
#pragma resource "*.dfm"
TFormMain *FormMain;
//---------------------------------------------------------------------------

__fastcall TFormMain::TFormMain(TComponent* Owner)
    : TForm(Owner)
{
  appPath = ExtractFileDir(ParamStr(0));
  
  // Initialize GDI+.
  GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
  //graphicsModules = Gdiplus::Graphics::FromHDC(paintModules->Canvas->Handle);
  
  ImagesToGrid();
  gridImages->Row = 1;

  ModulesToGrid();
  gridModules->Row = 1;

  FramesToGrid();
  gridFrames->Row = 1;

  FModulesToGrid();
  gridFModules->Row = 1;

  AnimsToGrid();
  gridAnims->Row = 1;

  AFramesToGrid();
  gridAFrames->Row = 1;

  FramesForLogicToGrid();
  gridFramesForLogic->Row = 1;

  FrameLogicToGrid();
  gridFrameLogic->Row = 1;

  FrameLogicItemsToGrid();
  gridFrameLogicItems->Row = 1;

  ImagesForColorMapToGrid();
  gridImagesForColorMap->Row = 1;

  ColorMapToGrid();
  gridColorMaps->Row = 1;

  ColorMapItemsToGrid();
  gridColorMapItems->Row = 1;

  // add an extra menu item (the color picker)
  TMenuItem* colorPicker = new TMenuItem(gridColorMapItems->PopupMenu);
  gridColorMapItems->PopupMenu->Items->Add(colorPicker);

  colorPicker->Caption = "Color Picker...";
  colorPicker->OnClick = ColorPickerClick;


  modulesZoom = 100;
  modulesPan.x = modulesPan.y = 0;

  framesZoom = 100;
  framesPan.x = framesPan.y = 0;

  animsZoom = 100;
  animsPan.x = animsPan.y = 0;

  fLogicZoom = 100;
  fLogicPan.x = fLogicPan.y = 0;

  colorMapZoom = 100;
  colorMapPan.x = colorMapPan.y = 0;

  // seems that is has no effect?
  //GroupBoxCanvas->DoubleBuffered = true;

  moduleAvailableMouseMode = moduleMouseMode = MMM_DEFINE;

  frameMouseMode = frameAvailableMouseMode = MMM_NONE;

  animMouseMode = animAvailableMouseMode = MMM_NONE;

  fLogicAvailableMouseMode = fLogicMouseMode = MMM_DEFINE;

  colorMapMouseMode = colorMapAvailableMouseMode = MMM_PAN;

  RefreshAllScripts();

  ArrangeFramesPageItems();
  ArrangeModulesPageItems();
  ArrangeAnimsPageItems();
  ArrangeFrameLogicPageItems();
  ArrangeColorMapPageItems();

  PageControl1->ActivePage = TabSheetModules;

  LoadConfig();
  
  GUIDefaultConfig();
  GUIFromDocState(mSprite.GetDocState());

  Application->ShowHint = true;
  Application->OnHint = DisplayHint;

  //GroupBoxAnimsCanvas->DoubleBuffered = true;
}
//---------------------------------------------------------------------------

__fastcall TFormMain::~TFormMain()
{
  //shutdown GDI+
  //delete graphicsModules;

  Gdiplus::GdiplusShutdown(gdiplusToken);
}
//---------------------------------------------------------------------------

void TFormMain::GUIDefaultConfig()
{
  cbModulesAxis->Checked  = true;
  cbModulesGrid->Checked  = false;
  cbModulesTrans->Checked = true;
  cbModulesAll->Checked   = false;

  cbFramesAxis->Checked   = true;
  cbFramesGrid->Checked   = false;
  cbFramesTrans->Checked  = true;
  cbFramesOffset->Checked = true;

  cbAnimsAxis->Checked     = true;
  cbAnimsGrid->Checked     = false;
  cbAnimsTrans->Checked    = true;
  cbAnimsFMOffset->Checked = true;
  cbAnimsAFOffset->Checked = true;
  cbAnimsLoop->Checked     = true;
  cbAnimsReverse->Checked  = false;

  cbFrameLogicAxis->Checked   = true;
  cbFrameLogicGrid->Checked   = false;

  cbColorMapAxis->Checked = false;
  cbColorMapGrid->Checked = false;
  cbColorMapTrans->Checked = false;

  TimerAnim->Interval = 1000 / cbFPS->Value;
}
//---------------------------------------------------------------------------

void TFormMain::LoadConfig()
{
  mAppConfig.Load((appPath + "\\Sentry.xml").c_str());

  MakeHistory();
}
//---------------------------------------------------------------------------

void TFormMain::SaveConfig()
{
  mAppConfig.Save((appPath + "\\Sentry.xml").c_str());
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::DisplayHint(TObject* Sender)
{
  // for some reason, GetShortHint and GetLongHint dont work as expected
  int sepIndex = Application->Hint.Pos("$");

  // TODO: dynamic hint (multiple parts)
  if (sepIndex == 0)
  {
    StatusBar->Panels->Items[0]->Text = Application->Hint;
    StatusBar->Panels->Items[1]->Text = "";
  }
  else
  {
    StatusBar->Panels->Items[0]->Text = Application->Hint.SubString(1, sepIndex - 1);
    StatusBar->Panels->Items[1]->Text = Application->Hint.SubString(sepIndex + 1, Application->Hint.Length() - sepIndex + 1);
  }
}
//---------------------------------------------------------------------------

void TFormMain::GUIFromDocState(TSpriteDocState state)
{
  switch(state)
  {
    case SPR_DOC_STATE_NEW:
      //PageControl1->Visible = true;
      Save1->Enabled   = true;
      Saveas1->Enabled = true;
      Close1->Enabled  = true;
      Export1->Enabled = true;
      Import1->Enabled = false;
      break;

    case SPR_DOC_STATE_SAVED:
      Save1->Enabled   = true;
      Saveas1->Enabled = true;
      Close1->Enabled  = true;
      Export1->Enabled = true;
      Import1->Enabled = false;
      break;

    case SPR_DOC_STATE_CLOSED:
      //PageControl1->Visible = false;
      Save1->Enabled   = false;
      Saveas1->Enabled = false;
      Close1->Enabled  = false;
      Export1->Enabled = false;
      Import1->Enabled = false;
      break;
  }
}
//---------------------------------------------------------------------------

void TFormMain::MakeHistory()
{
  OpenRecent1->Clear();

  // we dont have any recent opened items
  if (mAppConfig.mFileHistory.size() == 0)
  {
    OpenRecent1->Enabled = false;
    return;
  }

  for(int i = 0; i < (int)mAppConfig.mFileHistory.size(); i++)
  {
    TMenuItem* menuItem = new TMenuItem(OpenRecent1);

    menuItem->Caption = UnicodeString(i + 1) + " " + UTF8ToString(mAppConfig.mFileHistory[i].c_str());

    menuItem->OnClick = HistoryClick;

    OpenRecent1->Add(menuItem);
  }
}
//---------------------------------------------------------------------------

void TFormMain::LoadDocument(std::string fileName)
{
  if (mSprite.Load(fileName))
  {
    ImagesToGrid();
    ModulesToGrid();

    FramesToGrid();
    FModulesToGrid();
    scrollQuickModules->Position = 0;

    AnimsToGrid();
    AFramesToGrid();
    scrollQuickFrames->Position = 0;

    FramesForLogicToGrid();
    FrameLogicToGrid();
    FrameLogicItemsToGrid();

    ImagesForColorMapToGrid();
    ColorMapToGrid();
    ColorMapItemsToGrid();

    modulesPan = TPoint(0, 0);
    modulesZoom = 100;
    trackModulesZoom->Position = modulesZoom;

    framesPan = TPoint(0, 0);
    framesZoom = 100;

    animsPan = TPoint(0, 0);
    animsZoom = 100;

    colorMapPan = TPoint(0, 0);
    colorMapZoom = 100;
    trackColorMapZoom->Position = colorMapZoom;

    GUIFromDocState(mSprite.GetDocState());

    paintModulesPaint(this);
    paintFramesPaint(this);
    paintAnimsPaint(this);
    paintColorMapPaint(this);

    SetQuickModulesScroll();
    SetQuickFramesScroll();
    
    paintQuickModulesPaint(this);
    paintQuickFramesPaint(this);

    // TODO: eventually refactor
    mAppConfig.AddFileToHistory(fileName);
    MakeHistory();
  }

  MemoMsg->Lines->Clear();

  std::vector<std::string> messages = mSprite.GetFileMessages();

  for(int i = 0; i < (int)messages.size(); i++)
  {
    MemoMsg->Lines->Add(messages[i].c_str());
  }
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
//
//
//                      APPLICATION GENERIC
//
//
//---------------------------------------------------------------------------

void __fastcall TFormMain::HistoryClick(TObject *Sender)
{
  // TODO: get the file name from history
  auto lFile = ((TMenuItem*)Sender)->Caption;

  // first 3 chars are eye candy :)
  lFile.Delete(1, 3);

  LoadDocument(UTF8Encode(lFile).c_str());
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::About1Click(TObject *Sender)
{
  FormAbout->ShowModal();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::Exit1Click(TObject *Sender)
{
  Application->Terminate();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::Saveas1Click(TObject *Sender)
{
  if (SaveDialogXSprite->Execute())
  {
    auto fileNameUTF8 = UTF8Encode(SaveDialogXSprite->FileName).c_str();

    if (mSprite.Save(fileNameUTF8))
    {
      mAppConfig.AddFileToHistory(fileNameUTF8);
    }
  }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::Save1Click(TObject *Sender)
{
  switch (mSprite.GetDocState())
  {
    case SPR_DOC_STATE_NEW:
      if (SaveDialogXSprite->Execute())
      {
        auto fileNameUTF8 = UTF8Encode(SaveDialogXSprite->FileName).c_str();

        if (mSprite.Save(fileNameUTF8))
        {
          mAppConfig.AddFileToHistory(fileNameUTF8);
        }
      }
      break;

    case SPR_DOC_STATE_SAVED:
        if (mSprite.Save(mSprite.GetFileName()))
        {
          mAppConfig.AddFileToHistory(mSprite.GetFileName());
        }
      break;

    case SPR_DOC_STATE_CLOSED:
      break;
  }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::Open1Click(TObject *Sender)
{
  if (mAppConfig.mFileHistory.size() > 0)
  {
    OpenDialogXSprite->InitialDir = UTF8ToString(mAppConfig.mFileHistory[0].c_str());
  }

  if (OpenDialogXSprite->Execute())
  {
    LoadDocument(UTF8Encode(OpenDialogXSprite->FileName).c_str());

    //mAppConfig.AddFileToHistory(std::string(OpenDialogXSprite->FileName.c_str()));
    //MakeHistory();
  }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::Close1Click(TObject *Sender)
{
  mSprite.Close();

  ImagesToGrid();
  gridImages->Row = 1;

  ModulesToGrid();
  gridModules->Row = 1;

  FramesToGrid();
  gridFrames->Row = 1;

  FModulesToGrid();
  gridFModules->Row = 1;

  AnimsToGrid();
  gridAnims->Row = 1;

  AFramesToGrid();
  gridAFrames->Row = 1;

  FramesForLogicToGrid();
  gridFramesForLogic->Row = 1;

  FrameLogicToGrid();
  gridFrameLogic->Row = 1;

  FrameLogicItemsToGrid();
  gridFrameLogicItems->Row = 1;

  ImagesForColorMapToGrid();
  gridImagesForColorMap->Row = 1;

  ColorMapToGrid();
  gridColorMaps->Row = 1;

  ColorMapItemsToGrid();
  gridColorMapItems->Row = 1;

  GUIFromDocState(mSprite.GetDocState());
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::Options1Click(TObject *Sender)
{
  FormOptions->SetAppConfig(mAppConfig);

  if (FormOptions->ShowModal() == mrOk)
  {
    paintModulesPaint(this);
    paintFramesPaint(this);
    paintAnimsPaint(this);
    paintColorMapPaint(this);

    paintQuickModulesPaint(this);
    paintQuickFramesPaint(this);
  }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::FormResize(TObject *Sender)
{
  SetQuickModulesScroll();
  SetQuickFramesScroll();

  ArrangeModulesPageItems();
  ArrangeFramesPageItems();
  ArrangeAnimsPageItems();
  ArrangeFrameLogicPageItems();
  ArrangeColorMapPageItems();
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
//
//
//                      IMAGES & MODULES EDITING
//
//
//---------------------------------------------------------------------------

void __fastcall TFormMain::Splitter2Moved(TObject *Sender)
{
  ArrangeModulesPageItems();
}
//---------------------------------------------------------------------------


void __fastcall TFormMain::Splitter4Moved(TObject *Sender)
{
  ArrangeModulesPageItems();
}
//---------------------------------------------------------------------------

void TFormMain::ArrangeModulesPageItems()
{
  edModules->Top = GroupBoxModules->ClientHeight - edModules->Height - 4;
  gridModules->Height = edModules->Top - gridModules->Top - 4;

  edImages->Top = GroupBoxImages->ClientHeight - edImages->Height - 4;
  gridImages->Height = edImages->Top - gridImages->Top - 4;

  //edPals->Top = GroupBoxPal->ClientHeight - edPals->Height - 4;
  //gridPals->Height = edPals->Top - gridPals->Top - 4;
}
//---------------------------------------------------------------------------

void TFormMain::ImagesToGrid()
{
  gridImages->RowCount = 2;
  gridImages->Cells[0][0] = "Index";
  gridImages->Cells[1][0] = "Id";
  gridImages->Cells[2][0] = "File Name";
  gridImages->Cells[3][0] = "Transparent";
  gridImages->Cells[4][0] = "Info";

  for(int i = 0; i < mSprite.mImagesManager.Size(); i++)
  {
    gridImages->Cells[0][i + 1] = AnsiString(i);
    gridImages->Cells[1][i + 1] = "0x" + AnsiString::IntToHex(mSprite.mImagesManager.Get(i).GetId(), 32);
    gridImages->Cells[2][i + 1] = mSprite.mImagesManager.Get(i).GetFileName().c_str();
    gridImages->Cells[3][i + 1] = "0x" + AnsiString::IntToHex(mSprite.mImagesManager.Get(i).GetTransparentColor(), 32);
    gridImages->Cells[4][i + 1] = mSprite.mImagesManager.Get(i).GetInfo().c_str();
    gridImages->RowCount++;
  }

  // fill the last line in the grid
  gridImages->Cells[0][gridImages->RowCount - 1] = "new...";

  for(int i = 1; i < 5; i++)
  {
    gridImages->Cells[i][gridImages->RowCount - 1] = "";
  }
}
//---------------------------------------------------------------------------

void TFormMain::ModulesToGrid()
{
  gridModules->RowCount = 2;
  gridModules->Cells[0][0] = "Index";
  gridModules->Cells[1][0] = "Id";
  gridModules->Cells[2][0] = "Image Id";
  gridModules->Cells[3][0] = "X";
  gridModules->Cells[4][0] = "Y";
  gridModules->Cells[5][0] = "Width";
  gridModules->Cells[6][0] = "Height";
  gridModules->Cells[7][0] = "Info";

  for(int i = 0; i < mSprite.mModulesManager.Size(); i++)
  {
    gridModules->Cells[0][i + 1] = AnsiString(i);
    gridModules->Cells[1][i + 1] = "0x" + AnsiString::IntToHex(mSprite.mModulesManager.Get(i).GetId(), 32);
    gridModules->Cells[2][i + 1] = "0x" + AnsiString::IntToHex(mSprite.mModulesManager.Get(i).GetImageId(), 32);
    gridModules->Cells[3][i + 1] = AnsiString(mSprite.mModulesManager.Get(i).GetX());
    gridModules->Cells[4][i + 1] = AnsiString(mSprite.mModulesManager.Get(i).GetY());
    gridModules->Cells[5][i + 1] = AnsiString(mSprite.mModulesManager.Get(i).GetWidth());
    gridModules->Cells[6][i + 1] = AnsiString(mSprite.mModulesManager.Get(i).GetHeight());
    gridModules->Cells[7][i + 1] = AnsiString(mSprite.mModulesManager.Get(i).GetInfo().c_str());

    gridModules->RowCount++;
  }

  // fill the last line in the grid
  gridModules->Cells[0][gridModules->RowCount - 1] = "new...";

  for(int i = 1; i < 7; i++)
  {
    gridModules->Cells[i][gridModules->RowCount - 1] = "";
  }
}
//---------------------------------------------------------------------------

void TFormMain::PalsToGrid()
{
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::gridImagesPopNew(TObject *Sender)
{
  mSprite.mImagesManager.New();
  ImagesToGrid();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::gridImagesDblClick(TObject *Sender)
{
  if (gridImages->Row == gridImages->RowCount - 1)
  {
    mSprite.mImagesManager.New();
    ImagesToGrid();
  }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::gridModulesPopNew(TObject *Sender)
{
  int index = mSprite.mModulesManager.New();

  if (gridImages->Row < gridImages->RowCount - 1)
  {
    CModule module = mSprite.mModulesManager.Get(index);

    CImage image = mSprite.mImagesManager.Get(gridImages->Row - 1);

    module.SetImageId(image.GetId());

    mSprite.mModulesManager.Set(index, module);
  }

  ModulesToGrid();
  gridModules->Row = index + 1;
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::gridModulesDblClick(TObject *Sender)
{
  if (gridModules->Row == gridModules->RowCount - 1)
  {
    gridModulesPopNew(Sender);
  }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::gridModulesPopDelete(TObject *Sender)
{
  if (gridModules->Row < gridModules->RowCount - 1)
  {
    int result = mSprite.mModulesManager.Delete(gridModules->Row - 1);

    ModulesToGrid();

    if (result != -1)
    {
      gridModules->Row = result + 1;
    }

    gridModulesClick(this);
  }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::gridModulesPopInsert(TObject *Sender)
{
  if (gridModules->Row < gridModules->RowCount - 1)
  {
    CModule module;

    module.SetId(mSprite.mModulesManager.GenerateUniqueId());

    int row = mSprite.mModulesManager.Insert(gridModules->Row - 1, module);

    ModulesToGrid();

    gridModules->Row = row + 1;
  }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::gridImagesPopDelete(TObject *Sender)
{
  if (gridImages->Row < gridImages->RowCount - 1)
  {
    int result = mSprite.mImagesManager.Delete(gridImages->Row - 1);

    ImagesToGrid();

    if (result != -1)
    {
      gridImages->Row = result + 1;
    }

    gridImagesClick(this);
  }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::gridImagesPopInsert(TObject *Sender)
{
  if (gridImages->Row < gridImages->RowCount - 1)
  {
    CImage image;

    image.SetId(mSprite.mImagesManager.GenerateUniqueId());

    int row = mSprite.mImagesManager.Insert(gridImages->Row - 1, image);

    ImagesToGrid();

    gridImages->Row = row + 1;
  }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::gridModulesPopClone(TObject *Sender)
{
  if (gridModules->Row < gridModules->RowCount - 1)
  {
    int row = mSprite.mModulesManager.Clone(gridModules->Row - 1);

    ModulesToGrid();

    gridModules->Row = row + 1;
  }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::gridImagesPopMoveDown(TObject *Sender)
{
  if (gridImages->Row < gridImages->RowCount - 1)
  {
    int result = mSprite.mImagesManager.MoveDown(gridImages->Row - 1);

    ImagesToGrid();

    if (result != -1)
    {
      gridImages->Row = result + 1;
    }
  }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::gridImagesPopMoveUp(TObject *Sender)
{
  if (gridImages->Row < gridImages->RowCount - 1)
  {
    int result = mSprite.mImagesManager.MoveUp(gridImages->Row - 1);

    ImagesToGrid();

    if (result != -1)
    {
      gridImages->Row = result + 1;
    }
  }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::gridImagesPopClone(TObject *Sender)
{
  if (gridImages->Row < gridImages->RowCount - 1)
  {
    int row = mSprite.mImagesManager.Clone(gridImages->Row - 1);

    ImagesToGrid();

    gridImages->Row = row + 1;
  }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::gridModulesPopMoveUp(TObject *Sender)
{
  if (gridModules->Row < gridModules->RowCount - 1)
  {
    int result = mSprite.mModulesManager.MoveUp(gridModules->Row - 1);

    ModulesToGrid();

    if (result != -1)
    {
      gridModules->Row = result + 1;
    }
  }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::gridModulesPopMoveDown(TObject *Sender)
{
  if (gridModules->Row < gridModules->RowCount - 1)
  {
    int result = mSprite.mModulesManager.MoveDown(gridModules->Row - 1);

    ModulesToGrid();

    if (result != -1)
    {
      gridModules->Row = result + 1;
    }
  }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::gridModulesClick(TObject *Sender)
{
  if (gridModules->Row == gridModules->RowCount - 1)
  {
    edModules->Text = "";
    return;
  }

  CModule module = mSprite.mModulesManager.Get(gridModules->Row - 1);

  edModules->Text =
    AnsiString("id=\"") + "0x" + AnsiString::IntToHex(module.GetId(), 32) + "\" " +
    AnsiString("imageid=\"") + "0x" + AnsiString::IntToHex(module.GetImageId(), 32) + "\" " +
    AnsiString("x=") + module.GetX() + " " +
    AnsiString("y=") + module.GetY() + " " +
    AnsiString("width=") + module.GetWidth() + " " +
    AnsiString("height=") + module.GetHeight() + " " +
    AnsiString("info=\"") + module.GetInfo().c_str() + "\""
    ;

  paintModulesPaint(this);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::gridImagesClick(TObject *Sender)
{
  if (gridImages->Row == gridImages->RowCount - 1)
  {
    edImages->Text = "";
    return;
  }

  CImage image = mSprite.mImagesManager.Get(gridImages->Row - 1);

  edImages->Text =
    AnsiString("id=\"") + "0x" + IntToHex(image.GetId(), 32) + "\" " +
    AnsiString("filename=\"") + image.GetFileName().c_str() + "\" " +
    AnsiString("color=\"") + "0x" + IntToHex(image.GetTransparentColor(), 32) + "\" " +
    AnsiString("info=\"") + image.GetInfo().c_str() + "\""
    ;

  paintModulesPaint(this); 
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::edModulesExit(TObject *Sender)
{
  if (edModules->Text == "")
  {
    return;
  }

  // if one attribute is wrong, is ignored, all the others that are correct,
  // will be set

  // get the entry that is edited
  CModule module = mSprite.mModulesManager.Get(gridModules->Row - 1);

  // parse the new attiributes through tinyxml
  tinyxml2::XMLDocument doc;

  doc.Parse( UTF8Encode("<element " + edModules->Text + "/>").c_str());

  auto elem = doc.FirstChildElement("element");

  int result;

  //
  // Parse integer attributes
  //
  int x, y, width, height;

  result = elem->QueryIntAttribute("x", &x);

  if (result == XML_SUCCESS)
  {
    module.SetX(x);
  }

  result = elem->QueryIntAttribute("y", &y);

  if (result == XML_SUCCESS)
  {
    module.SetY(y);
  }

  result = elem->QueryIntAttribute("width", &width);

  if (result == XML_SUCCESS)
  {
    module.SetWidth(width);
  }

  result = elem->QueryIntAttribute("height", &height);

  if (result == XML_SUCCESS)
  {
    module.SetHeight(height);
  }

  //
  // Parse hex attributes
  //
  const char* id = elem->Attribute("id");

  if (id)
  {
    int value;

    try
    {
      value = StrToInt(id);
      module.SetId(value);
    }
    catch(...)
    {
    }
  }

  const char* imageid = elem->Attribute("imageid");

  if (imageid)
  {
    int value;

    try
    {
      value = StrToInt(imageid);
      module.SetImageId(value);
    }
    catch(...)
    {
    }
  }

  //
  // Parse string attributes
  //
  const char* info = elem->Attribute("info");

  if (info)
  {
    module.SetInfo(info);
  }

  int oldRow = gridModules->Row;

  mSprite.mModulesManager.Set(gridModules->Row - 1, module);

  edModules->Text =
    UnicodeString("id=\"") + "0x" + UnicodeString::IntToHex(module.GetId(), 32) + "\" " +
    UnicodeString("imageid=\"") + "0x" + UnicodeString::IntToHex(module.GetImageId(), 32) + "\" " +
    UnicodeString("x=") + module.GetX() + " " +
    UnicodeString("y=") + module.GetY() + " " +
    UnicodeString("width=") + module.GetWidth() + " " +
    UnicodeString("height=") + module.GetHeight() + " " +
    UnicodeString("info=\"") + UTF8ToString(module.GetInfo().c_str()) + "\""
    ;

  ModulesToGrid();

  gridModules->Row = oldRow;
  
  paintModulesPaint(this);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::edModulesKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
  if (Key == VK_RETURN)
  {
    edModulesExit(Sender);
  }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::edImagesExit(TObject *Sender)
{
  //TODO!!!
  if (edImages->Text == "")
  {
    return;
  }

  // if one attribute is wrong, is ignored, all the others that are correct,
  // will be set

  // get the entry that is edited
  CImage image = mSprite.mImagesManager.Get(gridImages->Row - 1);

  // parse the new attiributes through tinyxml
  tinyxml2::XMLDocument doc;

  doc.Parse( UTF8Encode("<element " + edImages->Text + "/>").c_str());

  auto elem = doc.FirstChildElement("element");

  int result;

  //
  // Parse hex attributes
  //
  const char* id = elem->Attribute("id");

  if (id)
  {
    int value;

    try
    {
      value = StrToInt(id);
      image.SetId(value);
    }
    catch(...)
    {
    }
  }

  const char* color = elem->Attribute("color");

  if (color)
  {
    int value;

    try
    {
      value = StrToInt(color);
      image.SetTransparentColor(value);
    }
    catch(...)
    {
    }
  }

  //
  // Parse string attributes
  //
  const char* info = elem->Attribute("info");

  if (info)
  {
    image.SetInfo(info);
  }

  const char* fileName = elem->Attribute("filename");

  if (fileName)
  {
    image.SetFileName(fileName);
  }

  mSprite.mImagesManager.Set(gridImages->Row - 1, image);

  edImages->Text =
    UnicodeString("id=\"") + "0x" + IntToHex(image.GetId(), 32) + "\" " +
    UnicodeString("filename=\"") + UTF8ToString(image.GetFileName().c_str()) + "\" " +
    UnicodeString("color=\"") + "0x" + IntToHex(image.GetTransparentColor(), 32) + "\" " +
    UnicodeString("info=\"") + image.GetInfo().c_str() + "\""
    ;

  ImagesToGrid();

  paintModulesPaint(this);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::edImagesKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
  if (Key == VK_RETURN)
  {
    edImagesExit(Sender);
  }
}
//---------------------------------------------------------------------------

void TFormMain::DrawModulesOverlay()
{
  if (cbModulesAxis->Checked && !cbModulesGrid->Checked)
  {
    paintModules->Canvas->Pen->Style = psSolid;
    paintModules->Canvas->Pen->Color = (TColor)mAppConfig.mModulesAxisColor;

    paintModules->Canvas->MoveTo(paintModules->Width / 2 - modulesPan.x, 0);
    paintModules->Canvas->LineTo(paintModules->Width / 2 - modulesPan.x, paintModules->Height);

    paintModules->Canvas->MoveTo(0, paintModules->Height / 2 - modulesPan.y);
    paintModules->Canvas->LineTo(paintModules->Width, paintModules->Height / 2 - modulesPan.y);
  }

  if (cbModulesGrid->Checked)
  {
    paintModules->Canvas->Pen->Style = psSolid;
    paintModules->Canvas->Pen->Color = (TColor)mAppConfig.mModulesGridColor;

    for(int x = modulesPan.x % mAppConfig.mModulesGridXSpacing; x < paintModules->Width; x += mAppConfig.mModulesGridXSpacing * modulesZoom / 100)
    {
      paintModules->Canvas->MoveTo(x, 0);
      paintModules->Canvas->LineTo(x, paintModules->Height);
    }

    for(int y = modulesPan.y % mAppConfig.mModulesGridYSpacing; y < paintModules->Height; y += mAppConfig.mModulesGridYSpacing * modulesZoom / 100)
    {
      paintModules->Canvas->MoveTo(0, y);
      paintModules->Canvas->LineTo(paintModules->Width, y);
    }
  }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::paintModulesPaint(TObject *Sender)
{
  int imageIndex  = gridImages->Row - 1;
  //int moduleIndex = gridModules->Row - 1;

  // make clean
  paintModules->Canvas->Brush->Style = bsClear;
  paintModules->Canvas->Brush->Color = (TColor)mAppConfig.mModulesBackgroundColor;
  paintModules->Canvas->FillRect(paintModules->ClientRect);

  // paint the selected image
  if (imageIndex < 0 || imageIndex >= mSprite.mImagesManager.Size())
  {
    DrawModulesOverlay();
    return;
  }

  Gdiplus::Bitmap* bitmap = ImageCache::Instance()->GetBitmap(mSprite.mImagesManager.Get(gridImages->Row - 1).GetFileName());

  if (!bitmap)
  {
    paintModules->Canvas->TextOut(0, 0, "INVALID IMAGE");
    DrawModulesOverlay();
    return;
  }

  // draw the image with zoom and pan
  DrawImage(paintModules->Canvas, gridImages->Row - 1, paintModules->Width / 2 - modulesPan.x, paintModules->Height / 2 - modulesPan.y, modulesZoom, cbModulesTrans->Checked);

  // draw the canvas embelishments (axis, grid and so on)
  DrawModulesOverlay();

  // paint the modules the are defined on this image

  // set the line style
  paintModules->Canvas->Pen->Style = psDash;
  paintModules->Canvas->Brush->Style = bsClear;

  int bitmapWidth  = bitmap->GetWidth()  * modulesZoom / 100;
  int bitmapHeight = bitmap->GetHeight() * modulesZoom / 100;
  
  for(int i = 0; i < mSprite.mModulesManager.Size(); i++)
  {
    CModule m = mSprite.mModulesManager.Get(i);

    int imageX = paintModules->Width / 2 - bitmapWidth / 2 - modulesPan.x;
    int imageY = paintModules->Height / 2 - bitmapHeight / 2 - modulesPan.y;

    bool paintModule = false;

    if (m.GetImageId() == mSprite.mImagesManager.Get(gridImages->Row - 1).GetId() && i == gridModules->Row - 1)
    {
      paintModules->Canvas->Pen->Color = (TColor)mAppConfig.mModulesSelectedColor;
      paintModule = true;
    }
    else
    {
      if (cbModulesAll->Checked)
      {
        paintModules->Canvas->Pen->Color = (TColor)mAppConfig.mModulesNotSelectedColor;
        paintModule = true;
      }  
    }

    if (paintModule)
    {
      int scaledModuleWidth;
      int scaledModuleHeight;
      int scaledModuleX;
      int scaledModuleY;

      if (moduleMouseMode == MMM_DO_DEFINE)
      {
        int translatedX = -paintModules->Width / 2 + (int)std::min(modulesDefine.x, modulesCurrentDefine.x) + bitmapWidth / 2 + modulesPan.x;
        int translatedY = -paintModules->Height / 2 + (int)std::min(modulesDefine.y, modulesCurrentDefine.y) + bitmapHeight / 2 + modulesPan.y;

        scaledModuleWidth  = abs(modulesDefine.x - modulesCurrentDefine.x);
        scaledModuleHeight = abs(modulesDefine.y - modulesCurrentDefine.y);
        scaledModuleX      = translatedX;
        scaledModuleY      = translatedY;
      }
      else if (moduleMouseMode == MMM_MOVE)
      {
        scaledModuleX      = m.GetX() * modulesZoom / 100 + (-modulesDefine.x + modulesCurrentDefine.x);
        scaledModuleY      = m.GetY() * modulesZoom / 100 + (-modulesDefine.y + modulesCurrentDefine.y);
        scaledModuleWidth  = m.GetWidth()  * modulesZoom / 100;
        scaledModuleHeight = m.GetHeight() * modulesZoom / 100;
      }
      else if (moduleMouseMode == MMM_RESIZE_RIGHT_BOTTOM)
      {
        scaledModuleX      = m.GetX() * modulesZoom / 100;
        scaledModuleY      = m.GetY() * modulesZoom / 100;
        int scaledModuleNewX   = (m.GetX() + m.GetWidth()) * modulesZoom / 100 + (-modulesDefine.x + modulesCurrentDefine.x);
        int scaledModuleNewY   = (m.GetY() + m.GetHeight()) * modulesZoom / 100 + (-modulesDefine.y + modulesCurrentDefine.y);

        scaledModuleWidth  = abs(scaledModuleNewX - scaledModuleX);
        scaledModuleHeight = abs(scaledModuleNewY - scaledModuleY);
        scaledModuleX      = (int)std::min(scaledModuleX, scaledModuleNewX);
        scaledModuleY      = (int)std::min(scaledModuleY, scaledModuleNewY);
      }
      else if (moduleMouseMode == MMM_RESIZE_LEFT_TOP)
      {
        scaledModuleX      = (m.GetX() + m.GetWidth()) * modulesZoom / 100;
        scaledModuleY      = (m.GetY() + m.GetHeight()) * modulesZoom / 100;
        int scaledModuleNewX   = m.GetX() * modulesZoom / 100 + (-modulesDefine.x + modulesCurrentDefine.x);
        int scaledModuleNewY   = m.GetY() * modulesZoom / 100 + (-modulesDefine.y + modulesCurrentDefine.y);

        scaledModuleWidth  = abs(scaledModuleNewX - scaledModuleX);
        scaledModuleHeight = abs(scaledModuleNewY - scaledModuleY);
        scaledModuleX      = (int)std::min(scaledModuleX, scaledModuleNewX);
        scaledModuleY      = (int)std::min(scaledModuleY, scaledModuleNewY);
      }
      else if (moduleMouseMode == MMM_RESIZE_RIGHT_TOP)
      {
        scaledModuleX      = m.GetX() * modulesZoom / 100;
        scaledModuleY      = (m.GetY() + m.GetHeight()) * modulesZoom / 100;
        int scaledModuleNewX   = (m.GetX() + m.GetWidth()) * modulesZoom / 100 + (-modulesDefine.x + modulesCurrentDefine.x);
        int scaledModuleNewY   = m.GetY() * modulesZoom / 100 + (-modulesDefine.y + modulesCurrentDefine.y);

        scaledModuleWidth  = abs(scaledModuleNewX - scaledModuleX);
        scaledModuleHeight = abs(scaledModuleNewY - scaledModuleY);
        scaledModuleX      = (int)std::min(scaledModuleX, scaledModuleNewX);
        scaledModuleY      = (int)std::min(scaledModuleY, scaledModuleNewY);
      }
      else if (moduleMouseMode == MMM_RESIZE_LEFT_BOTTOM)
      {
        scaledModuleX      = (m.GetX() + m.GetWidth()) * modulesZoom / 100;
        scaledModuleY      = m.GetY() * modulesZoom / 100;
        int scaledModuleNewX   = m.GetX() * modulesZoom / 100 + (-modulesDefine.x + modulesCurrentDefine.x);
        int scaledModuleNewY   = (m.GetY() + m.GetHeight()) * modulesZoom / 100 + (-modulesDefine.y + modulesCurrentDefine.y);

        scaledModuleWidth  = abs(scaledModuleNewX - scaledModuleX);
        scaledModuleHeight = abs(scaledModuleNewY - scaledModuleY);
        scaledModuleX      = (int)std::min(scaledModuleX, scaledModuleNewX);
        scaledModuleY      = (int)std::min(scaledModuleY, scaledModuleNewY);
      }
      else if (moduleMouseMode == MMM_RESIZE_RIGHT)
      {
        scaledModuleX      = m.GetX() * modulesZoom / 100;
        int scaledModuleNewX   = (m.GetX() + m.GetWidth()) * modulesZoom / 100 + (-modulesDefine.x + modulesCurrentDefine.x);

        scaledModuleWidth  = abs(scaledModuleNewX - scaledModuleX);
        scaledModuleHeight = m.GetHeight() * modulesZoom / 100;
        scaledModuleX      = (int)std::min(scaledModuleX, scaledModuleNewX);
        scaledModuleY      = m.GetY() * modulesZoom / 100;
      }
      else if (moduleMouseMode == MMM_RESIZE_LEFT)
      {
        scaledModuleX        = m.GetX() * modulesZoom / 100 + (-modulesDefine.x + modulesCurrentDefine.x);
        int scaledModuleNewX = (m.GetX() + m.GetWidth()) * modulesZoom / 100;

        scaledModuleWidth  = abs(scaledModuleNewX - scaledModuleX);
        scaledModuleHeight = m.GetHeight() * modulesZoom / 100;
        scaledModuleX      = (int)std::min(scaledModuleX, scaledModuleNewX);
        scaledModuleY      = m.GetY() * modulesZoom / 100;
      }
      else if (moduleMouseMode == MMM_RESIZE_BOTTOM)
      {
        scaledModuleY        = m.GetY() * modulesZoom / 100;
        int scaledModuleNewY = (m.GetY() + m.GetHeight()) * modulesZoom / 100 + (-modulesDefine.y + modulesCurrentDefine.y);

        scaledModuleWidth  = m.GetWidth()  * modulesZoom / 100;
        scaledModuleHeight = abs(scaledModuleNewY - scaledModuleY);
        scaledModuleX      = m.GetX() * modulesZoom / 100;
        scaledModuleY      = (int)std::min(scaledModuleY, scaledModuleNewY);
      }
      else if (moduleMouseMode == MMM_RESIZE_TOP)
      {
        scaledModuleY      = (m.GetY() + m.GetHeight()) * modulesZoom / 100;
        int scaledModuleNewY   = m.GetY() * modulesZoom / 100 + (-modulesDefine.y + modulesCurrentDefine.y);

        scaledModuleWidth  = m.GetWidth()  * modulesZoom / 100;
        scaledModuleHeight = abs(scaledModuleNewY - scaledModuleY);
        scaledModuleX      = m.GetX() * modulesZoom / 100;
        scaledModuleY      = (int)std::min(scaledModuleY, scaledModuleNewY);
      }
      else
      {
        scaledModuleWidth  = m.GetWidth()  * modulesZoom / 100;
        scaledModuleHeight = m.GetHeight() * modulesZoom / 100;
        scaledModuleX      = m.GetX() * modulesZoom / 100;
        scaledModuleY      = m.GetY() * modulesZoom / 100;
      }

      paintModules->Canvas->MoveTo(imageX + scaledModuleX, imageY + scaledModuleY);
      paintModules->Canvas->LineTo(imageX + scaledModuleX + scaledModuleWidth, imageY + scaledModuleY);
      paintModules->Canvas->LineTo(imageX + scaledModuleX + scaledModuleWidth, imageY + scaledModuleY + scaledModuleHeight);
      paintModules->Canvas->LineTo(imageX + scaledModuleX, imageY + scaledModuleY + scaledModuleHeight);
      paintModules->Canvas->LineTo(imageX + scaledModuleX, imageY + scaledModuleY);
    }
  }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::paintModulesMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
  ActiveControl = GroupBoxCanvas;

  if (Button == mbLeft && moduleMouseMode == MMM_PAN)
  {
    // start to pan
    moduleMouseMode = MMM_DO_PAN;
    oldModulesPan.x = X;
    oldModulesPan.y = Y;
  }

  if (Button == mbLeft && moduleAvailableMouseMode == MMM_MOVE)
  {
      moduleMouseMode = MMM_MOVE;
      modulesDefine.x = X;
      modulesDefine.y = Y;

      return;
  }

  if (Button == mbLeft && moduleAvailableMouseMode == MMM_RESIZE_RIGHT_BOTTOM)
  {
      moduleMouseMode = MMM_RESIZE_RIGHT_BOTTOM;
      modulesDefine.x = X;
      modulesDefine.y = Y;

      return;
  }

  if (Button == mbLeft && moduleAvailableMouseMode == MMM_RESIZE_RIGHT_TOP)
  {
      moduleMouseMode = MMM_RESIZE_RIGHT_TOP;
      modulesDefine.x = X;
      modulesDefine.y = Y;

      return;
  }

  if (Button == mbLeft && moduleAvailableMouseMode == MMM_RESIZE_LEFT_BOTTOM)
  {
      moduleMouseMode = MMM_RESIZE_LEFT_BOTTOM;
      modulesDefine.x = X;
      modulesDefine.y = Y;

      return;
  }

  if (Button == mbLeft && moduleAvailableMouseMode == MMM_RESIZE_LEFT_TOP)
  {
      moduleMouseMode = MMM_RESIZE_LEFT_TOP;
      modulesDefine.x = X;
      modulesDefine.y = Y;

      return;
  }

  if (Button == mbLeft && moduleAvailableMouseMode == MMM_RESIZE_LEFT)
  {
      moduleMouseMode = MMM_RESIZE_LEFT;
      modulesDefine.x = X;

      return;
  }

  if (Button == mbLeft && moduleAvailableMouseMode == MMM_RESIZE_RIGHT)
  {
      moduleMouseMode = MMM_RESIZE_RIGHT;
      modulesDefine.x = X;

      return;
  }

  if (Button == mbLeft && moduleAvailableMouseMode == MMM_RESIZE_TOP)
  {
      moduleMouseMode = MMM_RESIZE_TOP;
      modulesDefine.y = Y;

      return;
  }

  if (Button == mbLeft && moduleAvailableMouseMode == MMM_RESIZE_BOTTOM)
  {
      moduleMouseMode = MMM_RESIZE_BOTTOM;
      modulesDefine.y = Y;

      return;
  }

  if (Button == mbLeft && moduleMouseMode == MMM_DEFINE)
  {
    if (gridModules->Row - 1 < mSprite.mModulesManager.Size() && mSprite.mModulesManager.Size() > 0)
    {
      // start to define the module
      moduleMouseMode = MMM_DO_DEFINE;
      modulesDefine.x = X - X % (modulesZoom / 100) - (modulesPan.x % (modulesZoom / 100));
      modulesDefine.y = Y - Y % (modulesZoom / 100) - (modulesPan.y % (modulesZoom / 100));
    }

    return;
  }

  if (Button == mbRight)
  {
    moduleMouseMode = (moduleMouseMode == MMM_PAN) ? MMM_DEFINE : MMM_PAN;
    paintModules->Cursor = GetModuleCursor(moduleMouseMode);
  }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::paintModulesMouseMove(TObject *Sender,
      TShiftState Shift, int X, int Y)
{
  //ActiveControl = GroupBoxCanvas;

  if (moduleMouseMode == MMM_DO_PAN)
  {
    modulesPan.x += -X + oldModulesPan.x;
    modulesPan.y += -Y + oldModulesPan.y;

    oldModulesPan.x = X;
    oldModulesPan.y = Y;

    paintModulesPaint(this);

    return;
  }

  if (moduleMouseMode == MMM_DO_DEFINE)
  {
    modulesCurrentDefine.x = X - X % (modulesZoom / 100) - (modulesPan.x % (modulesZoom / 100)) + modulesZoom / 100;
    modulesCurrentDefine.y = Y - Y % (modulesZoom / 100) - (modulesPan.y % (modulesZoom / 100)) + modulesZoom / 100;

    paintModulesPaint(this);

    return;
  }

  if (moduleMouseMode == MMM_MOVE ||
      moduleMouseMode == MMM_RESIZE_RIGHT_BOTTOM ||
      moduleMouseMode == MMM_RESIZE_LEFT_TOP ||
      moduleMouseMode == MMM_RESIZE_LEFT_BOTTOM ||
      moduleMouseMode == MMM_RESIZE_RIGHT_TOP)
  {
    modulesCurrentDefine.x = X;
    modulesCurrentDefine.y = Y;

    paintModulesPaint(this);

    return;
  }

  if (moduleMouseMode == MMM_RESIZE_RIGHT ||
      moduleMouseMode == MMM_RESIZE_LEFT)
  {
    modulesCurrentDefine.x = X;
    
    paintModulesPaint(this);

    return;
  }

  if (moduleMouseMode == MMM_RESIZE_TOP ||
      moduleMouseMode == MMM_RESIZE_BOTTOM)
  {
    modulesCurrentDefine.y = Y;
    
    paintModulesPaint(this);

    return;
  }

  moduleAvailableMouseMode = GetModuleMouseMode(TPoint(X, Y));

  if (moduleAvailableMouseMode != MMM_NONE)
  {
    paintModules->Cursor = GetModuleCursor(moduleAvailableMouseMode);
  }
  else
  {
    paintModules->Cursor = GetModuleCursor(moduleMouseMode);
  }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::paintModulesMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
  ActiveControl = GroupBoxCanvas;

  if (Button == mbLeft && moduleMouseMode == MMM_DO_PAN)
  {
    moduleMouseMode = MMM_PAN;
  }

  if (Button == mbLeft && moduleMouseMode == MMM_DO_DEFINE)
  {
    // inhibit false defining
    if (abs(X - modulesDefine.x) != 0 && abs(Y - modulesDefine.y) != 0)
    {
      CModule module = mSprite.mModulesManager.Get(gridModules->Row - 1);

      //Graphics::TBitmap* bitmap = mSprite.GetModuleBitmap(module);
      Gdiplus::Bitmap* bitmap = mSprite.GetModuleBitmap(module);

      if (bitmap)
      {
        X = X - X % (modulesZoom / 100) + modulesZoom / 100;
        Y = Y - Y % (modulesZoom / 100) + modulesZoom / 100;

        module.SetX((-paintModules->Width / 2 + (int)std::min((long)X, modulesDefine.x) + (bitmap->GetWidth() / 2) * modulesZoom / 100 + modulesPan.x) * 100 / modulesZoom );
        module.SetY((-paintModules->Height / 2 + (int)std::min((long)Y, modulesDefine.y) + (bitmap->GetHeight() / 2) * modulesZoom / 100 + modulesPan.y) * 100 / modulesZoom );
        module.SetWidth(abs(X - modulesDefine.x) * 100 / modulesZoom );
        module.SetHeight(abs(Y - modulesDefine.y) * 100 / modulesZoom );

        mSprite.mModulesManager.Set(gridModules->Row - 1, module);

        int oldRow = gridModules->Row;

        ModulesToGrid();

        gridModules->Row = oldRow;
      }
    }
    else
    {
      int rowModules = gridModules->Row;

      // select a module
      for(int i = mSprite.mModulesManager.Size() - 1; i >= 0 ; i--)
      {
        CModule m = mSprite.mModulesManager.Get(i);

        Gdiplus::Bitmap* bitmap = mSprite.GetModuleBitmap(m);

        if (bitmap)
        {
          int bitmapWidth  = bitmap->GetWidth()  * modulesZoom / 100;
          int bitmapHeight = bitmap->GetHeight() * modulesZoom / 100;
          int imageX = paintModules->Width / 2 - bitmapWidth / 2 - modulesPan.x;
          int imageY = paintModules->Height / 2 - bitmapHeight / 2 - modulesPan.y;
          int screenModuleWidth  = m.GetWidth()  * modulesZoom / 100;
          int screenModuleHeight = m.GetHeight() * modulesZoom / 100;
          int screenModuleX      = imageX + m.GetX() * modulesZoom / 100;
          int screenModuleY      = imageY + m.GetY() * modulesZoom / 100;

          // check where the cursor is placed and modify the cursor type accordingly
          if (PointInRect(TPoint(X, Y), TRect(screenModuleX, screenModuleY, screenModuleX + screenModuleWidth, screenModuleY + screenModuleHeight)))
          {
            rowModules = i + 1;
            break;
          }
        } 
      }

      gridModules->Row = rowModules;
    }

    moduleMouseMode = MMM_DEFINE;
    paintModulesPaint(this);
  }

  if (Button == mbLeft && moduleMouseMode == MMM_MOVE)
  {
    CModule module = mSprite.mModulesManager.Get(gridModules->Row - 1);
    
    module.SetX(module.GetX() + ((X - modulesDefine.x)) * 100 / modulesZoom );
    module.SetY(module.GetY() + ((Y - modulesDefine.y)) * 100 / modulesZoom );
    mSprite.mModulesManager.Set(gridModules->Row - 1, module);

    int oldRow = gridModules->Row;
    ModulesToGrid();
    gridModules->Row = oldRow;

    moduleMouseMode = MMM_DEFINE;
    paintModulesPaint(this);
  }

  if (Button == mbLeft && moduleMouseMode == MMM_RESIZE_RIGHT_BOTTOM)
  {
    CModule module = mSprite.mModulesManager.Get(gridModules->Row - 1);

    int newX = ((X - modulesDefine.x)) * 100 / modulesZoom + module.GetX() + module.GetWidth();
    int newY = ((Y - modulesDefine.y)) * 100 / modulesZoom + module.GetY() + module.GetHeight();
    int w = abs(newX - module.GetX());
    int h = abs(newY - module.GetY());

    module.SetX((int)std::min(module.GetX(), newX));
    module.SetY((int)std::min(module.GetY(), newY));
    module.SetWidth(w);
    module.SetHeight(h);

    mSprite.mModulesManager.Set(gridModules->Row - 1, module);

    int oldRow = gridModules->Row;
    ModulesToGrid();
    gridModules->Row = oldRow;

    moduleMouseMode = MMM_DEFINE;
    paintModulesPaint(this);
  }

  if (Button == mbLeft && moduleMouseMode == MMM_RESIZE_LEFT_TOP)
  {
    CModule module = mSprite.mModulesManager.Get(gridModules->Row - 1);

    int newX = ((X - modulesDefine.x)) * 100 / modulesZoom + module.GetX();
    int newY = ((Y - modulesDefine.y)) * 100 / modulesZoom + module.GetY();
    int w = abs(newX - module.GetX() - module.GetWidth());
    int h = abs(newY - module.GetY() - module.GetHeight());

    module.SetX((int)std::min(module.GetX() + module.GetWidth(), newX));
    module.SetY((int)std::min(module.GetY() + module.GetHeight(), newY));
    module.SetWidth(w);
    module.SetHeight(h);

    mSprite.mModulesManager.Set(gridModules->Row - 1, module);

    int oldRow = gridModules->Row;
    ModulesToGrid();
    gridModules->Row = oldRow;

    moduleMouseMode = MMM_DEFINE;
    paintModulesPaint(this);
  }

  if (Button == mbLeft && moduleMouseMode == MMM_RESIZE_LEFT_BOTTOM)
  {
    CModule module = mSprite.mModulesManager.Get(gridModules->Row - 1);

    int newX = ((X - modulesDefine.x)) * 100 / modulesZoom + module.GetX();
    int newY = ((Y - modulesDefine.y)) * 100 / modulesZoom + module.GetY() + module.GetHeight();
    int w = abs(newX - module.GetX() - module.GetWidth());
    int h = abs(newY - module.GetY());

    module.SetX((int)std::min(module.GetX() + module.GetWidth(), newX));
    module.SetY((int)std::min(module.GetY(), newY));
    module.SetWidth(w);
    module.SetHeight(h);

    mSprite.mModulesManager.Set(gridModules->Row - 1, module);

    int oldRow = gridModules->Row;
    ModulesToGrid();
    gridModules->Row = oldRow;

    moduleMouseMode = MMM_DEFINE;
    paintModulesPaint(this);
  }

  if (Button == mbLeft && moduleMouseMode == MMM_RESIZE_RIGHT_TOP)
  {
    CModule module = mSprite.mModulesManager.Get(gridModules->Row - 1);

    int newX = ((X - modulesDefine.x)) * 100 / modulesZoom + module.GetX() + module.GetWidth();
    int newY = ((Y - modulesDefine.y)) * 100 / modulesZoom + module.GetY();
    int w = abs(newX - module.GetX());
    int h = abs(newY - module.GetY() - module.GetHeight());

    module.SetX((int)std::min(module.GetX(), newX));
    module.SetY((int)std::min(module.GetY() + module.GetHeight(), newY));
    module.SetWidth(w);
    module.SetHeight(h);

    mSprite.mModulesManager.Set(gridModules->Row - 1, module);

    int oldRow = gridModules->Row;
    ModulesToGrid();
    gridModules->Row = oldRow;

    moduleMouseMode = MMM_DEFINE;
    paintModulesPaint(this);
  }
  
  if (Button == mbLeft && moduleMouseMode == MMM_RESIZE_RIGHT)
  {
    CModule module = mSprite.mModulesManager.Get(gridModules->Row - 1);

    int newX = ((X - modulesDefine.x)) * 100 / modulesZoom + module.GetX() + module.GetWidth();
    int w = abs(newX - module.GetX());

    module.SetX((int)std::min(module.GetX(), newX));
    module.SetWidth(w);

    mSprite.mModulesManager.Set(gridModules->Row - 1, module);

    int oldRow = gridModules->Row;
    ModulesToGrid();
    gridModules->Row = oldRow;

    moduleMouseMode = MMM_DEFINE;
    paintModulesPaint(this);
  }

  if (Button == mbLeft && moduleMouseMode == MMM_RESIZE_LEFT)
  {
    CModule module = mSprite.mModulesManager.Get(gridModules->Row - 1);

    int newX = ((X - modulesDefine.x)) * 100 / modulesZoom + module.GetX();
    int w = abs(newX - module.GetX() - module.GetWidth());

    module.SetX((int)std::min(module.GetX() + module.GetWidth(), newX));
    module.SetWidth(w);

    mSprite.mModulesManager.Set(gridModules->Row - 1, module);

    int oldRow = gridModules->Row;
    ModulesToGrid();
    gridModules->Row = oldRow;

    moduleMouseMode = MMM_DEFINE;
    paintModulesPaint(this);
  }

  if (Button == mbLeft && moduleMouseMode == MMM_RESIZE_TOP)
  {
    CModule module = mSprite.mModulesManager.Get(gridModules->Row - 1);

    int newY = ((Y - modulesDefine.y)) * 100 / modulesZoom + module.GetY();
    int h = abs(newY - module.GetY() - module.GetHeight());

    module.SetY((int)std::min(module.GetY() + module.GetHeight(), newY));
    module.SetHeight(h);

    mSprite.mModulesManager.Set(gridModules->Row - 1, module);

    int oldRow = gridModules->Row;
    ModulesToGrid();
    gridModules->Row = oldRow;

    moduleMouseMode = MMM_DEFINE;
    paintModulesPaint(this);
  }

  if (Button == mbLeft && moduleMouseMode == MMM_RESIZE_BOTTOM)
  {
    CModule module = mSprite.mModulesManager.Get(gridModules->Row - 1);

    int newY = ((Y - modulesDefine.y)) * 100 / modulesZoom + module.GetY() + module.GetHeight();
    int h = abs(newY - module.GetY());

    module.SetY((int)std::min(module.GetY(), newY));
    module.SetHeight(h);

    mSprite.mModulesManager.Set(gridModules->Row - 1, module);

    int oldRow = gridModules->Row;
    ModulesToGrid();
    gridModules->Row = oldRow;

    moduleMouseMode = MMM_DEFINE;
    paintModulesPaint(this);
  }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::FormMouseWheelUp(TObject *Sender,
      TShiftState Shift, TPoint &MousePos, bool &Handled)
{
  if (PageControl1->ActivePage == TabSheetModules)
  {
    TRect rect = paintModules->ClientRect;
    TPoint tmp = paintModules->ScreenToClient(MousePos);

    if (rect.left < tmp.x && tmp.x < rect.right &&
        rect.top < tmp.y && tmp.y < rect.bottom)
    {
      modulesZoom += 10;

      if (modulesZoom > 1600)
      {
        modulesZoom = 1600;
      }

      trackModulesZoom->Position = modulesZoom;

      Handled = true;

      paintModulesPaint(this);
    }
  }

  if (PageControl1->ActivePage == TabSheetFrames)
  {
    TRect rect = paintFrames->ClientRect;
    TPoint tmp = paintFrames->ScreenToClient(MousePos);

    if (rect.left < tmp.x && tmp.x < rect.right &&
        rect.top < tmp.y && tmp.y < rect.bottom)
    {
      framesZoom += 10;

      if (framesZoom > 1600)
      {
        framesZoom = 1600;
      }

      trackFramesZoom->Position = framesZoom;
      Handled = true;
      paintFramesPaint(this);
    }
  }

  if (PageControl1->ActivePage == TabSheetAnim)
  {
    TRect rect = paintAnims->ClientRect;
    TPoint tmp = paintAnims->ScreenToClient(MousePos);

    if (rect.left < tmp.x && tmp.x < rect.right &&
        rect.top < tmp.y && tmp.y < rect.bottom)
    {
      animsZoom += 10;

      if (animsZoom > 1600)
      {
        animsZoom = 1600;
      }

      trackAnimsZoom->Position = animsZoom;
      Handled = true;
      paintAnimsPaint(this);
    }
  }

  if (PageControl1->ActivePage == TabSheetFrameLogic)
  {
    TRect rect = paintFrameLogic->ClientRect;
    TPoint tmp = paintFrameLogic->ScreenToClient(MousePos);

    if (rect.left < tmp.x && tmp.x < rect.right &&
        rect.top < tmp.y && tmp.y < rect.bottom)
    {
      fLogicZoom += 10;

      if (fLogicZoom > 1600)
      {
        fLogicZoom = 1600;
      }

      trackFLogicZoom->Position = fLogicZoom;
      Handled = true;
      paintFrameLogicPaint(this);
    }
  }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::FormMouseWheelDown(TObject *Sender,
      TShiftState Shift, TPoint &MousePos, bool &Handled)
{
  if (PageControl1->ActivePage == TabSheetModules)
  {
    TRect rect = paintModules->ClientRect;
    TPoint tmp = paintModules->ScreenToClient(MousePos);

    if (rect.left < tmp.x && tmp.x < rect.right &&
        rect.top < tmp.y && tmp.y < rect.bottom)
    {
      modulesZoom -= 10;

      if (modulesZoom < 20)
      {
        modulesZoom = 20;
      }

      trackModulesZoom->Position = modulesZoom;
      
      Handled = true;

      paintModulesPaint(this);
    }
  }

  if (PageControl1->ActivePage == TabSheetFrames)
  {
    TRect rect = paintFrames->ClientRect;
    TPoint tmp = paintFrames->ScreenToClient(MousePos);

    if (rect.left < tmp.x && tmp.x < rect.right &&
        rect.top < tmp.y && tmp.y < rect.bottom)
    {
      framesZoom -= 10;

      if (framesZoom < 20)
      {
        framesZoom = 20;
      }

      trackFramesZoom->Position = framesZoom;
      Handled = true;
      paintFramesPaint(this);
    }
  }

  if (PageControl1->ActivePage == TabSheetAnim)
  {
    TRect rect = paintAnims->ClientRect;
    TPoint tmp = paintAnims->ScreenToClient(MousePos);

    if (rect.left < tmp.x && tmp.x < rect.right &&
        rect.top < tmp.y && tmp.y < rect.bottom)
    {
      animsZoom -= 10;

      if (animsZoom < 20)
      {
        animsZoom = 20;
      }

      trackAnimsZoom->Position = animsZoom;
      Handled = true;
      paintAnimsPaint(this);
    }
  }

  if (PageControl1->ActivePage == TabSheetFrameLogic)
  {
    TRect rect = paintFrameLogic->ClientRect;
    TPoint tmp = paintFrameLogic->ScreenToClient(MousePos);

    if (rect.left < tmp.x && tmp.x < rect.right &&
        rect.top < tmp.y && tmp.y < rect.bottom)
    {
      fLogicZoom -= 10;

      if (fLogicZoom < 20)
      {
        fLogicZoom = 20;
      }

      trackFLogicZoom->Position = fLogicZoom;
      Handled = true;
      paintFrameLogicPaint(this);
    }
  }
}
//---------------------------------------------------------------------------

bool TFormMain::PointInRect(TPoint p, TRect r)
{
  return (r.left <= p.x && p.x < r.right && r.top  <= p.y && p.y < r.bottom);
}
//---------------------------------------------------------------------------

TMouseMode TFormMain::GetModuleMouseMode(TPoint p)
{
  TMouseMode mode = MMM_NONE;

  int imageIndex  = gridImages->Row - 1;

  // paint the selected image
  if (imageIndex < 0 || imageIndex >= mSprite.mImagesManager.Size())
  {
    return mode;
  }

  // wow :)
  //Graphics::TBitmap* bitmap = ImageCache::Instance()->GetBitmap(mSprite.mImagesManager.Get(gridImages->Row - 1).GetFileName());
  Gdiplus::Bitmap* bitmap = ImageCache::Instance()->GetBitmap(mSprite.mImagesManager.Get(gridImages->Row - 1).GetFileName());

  if (!bitmap)
  {
    return mode;
  }

  int bitmapWidth  = bitmap->GetWidth()  * modulesZoom / 100;
  int bitmapHeight = bitmap->GetHeight() * modulesZoom / 100;

  for(int i = 0; i < mSprite.mModulesManager.Size(); i++)
  {
    CModule m = mSprite.mModulesManager.Get(i);

    int imageX = paintModules->Width / 2 - bitmapWidth / 2 - modulesPan.x;
    int imageY = paintModules->Height / 2 - bitmapHeight / 2 - modulesPan.y;

    if (m.GetImageId() == mSprite.mImagesManager.Get(gridImages->Row - 1).GetId() && i == gridModules->Row - 1)
    {
      int screenModuleWidth  = m.GetWidth()  * modulesZoom / 100;
      int screenModuleHeight = m.GetHeight() * modulesZoom / 100;
      int screenModuleX      = imageX + m.GetX() * modulesZoom / 100;
      int screenModuleY      = imageY + m.GetY() * modulesZoom / 100;

      // check where the cursor is placed and modify the cursor type accordingly
      if (PointInRect(p, TRect(screenModuleX, screenModuleY, screenModuleX + screenModuleWidth, screenModuleY + screenModuleHeight)))
      {
        mode = MMM_MOVE;
      }
      else
      {
        int edge = 5;

        if (PointInRect(p, TRect(screenModuleX - edge, screenModuleY - edge, screenModuleX, screenModuleY)))
        {
          mode = MMM_RESIZE_LEFT_TOP;
        }

        if (PointInRect(p, TRect(screenModuleX + screenModuleWidth, screenModuleY + screenModuleHeight, screenModuleX + screenModuleWidth + edge, screenModuleY + screenModuleHeight + edge)))
        {
          mode = MMM_RESIZE_RIGHT_BOTTOM;
        }

        if (PointInRect(p, TRect(screenModuleX - edge, screenModuleY + screenModuleHeight, screenModuleX, screenModuleY + screenModuleHeight + edge)))
        {
          mode = MMM_RESIZE_LEFT_BOTTOM;
        }

        if (PointInRect(p, TRect(screenModuleX + screenModuleWidth, screenModuleY - edge, screenModuleX + screenModuleWidth + edge, screenModuleY)))
        {
          mode = MMM_RESIZE_RIGHT_TOP;
        }

        if (PointInRect(p, TRect(screenModuleX - edge, screenModuleY, screenModuleX, screenModuleY + screenModuleHeight)))
        {
          mode = MMM_RESIZE_LEFT;
        }

        if (PointInRect(p, TRect(screenModuleX + screenModuleWidth, screenModuleY, screenModuleX + screenModuleWidth + edge, screenModuleY + screenModuleHeight)))
        {
          mode = MMM_RESIZE_RIGHT;
        }

        if (PointInRect(p, TRect(screenModuleX, screenModuleY - edge, screenModuleX + screenModuleWidth, screenModuleY)))
        {
          mode = MMM_RESIZE_TOP;
        }

        if (PointInRect(p, TRect(screenModuleX, screenModuleY + screenModuleHeight, screenModuleX + screenModuleWidth, screenModuleY + screenModuleHeight + edge)))
        {
          mode = MMM_RESIZE_BOTTOM;
        }
      }
    }
  }

  return mode;
}
//---------------------------------------------------------------------------

TCursor TFormMain::GetModuleCursor(TMouseMode mmm)
{
  switch(mmm)
  {
    case MMM_PAN:
    case MMM_DO_PAN:
      return crHandPoint;

    case MMM_DEFINE:
    case MMM_DO_DEFINE:
      return crDefault;

    case MMM_MOVE:
      return crSizeAll;

    case MMM_RESIZE_LEFT_TOP:
      return crSizeNWSE;

    case MMM_RESIZE_RIGHT_TOP:
      return crSizeNESW;

    case MMM_RESIZE_LEFT_BOTTOM:
      return crSizeNESW;

    case MMM_RESIZE_RIGHT_BOTTOM:
      return crSizeNWSE;

    case MMM_RESIZE_LEFT:
      return crSizeWE;

    case MMM_RESIZE_RIGHT:
      return crSizeWE;

    case MMM_RESIZE_TOP:
      return crSizeNS;

    case MMM_RESIZE_BOTTOM:
      return crSizeNS;

    case MMM_PICK_COLOR:
      return crHelp;

    case MMM_NONE:
      return crDefault;
  }

  return crDefault;
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::cbModulesAxisClick(TObject *Sender)
{
  paintModulesPaint(this);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::trackModulesZoomChange(TObject *Sender)
{
  modulesZoom = trackModulesZoom->Position;

  paintModulesPaint(this);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::cbModulesTransClick(TObject *Sender)
{
  paintModulesPaint(this);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::cbModulesAllClick(TObject *Sender)
{
  paintModulesPaint(this);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::cbModulesGridClick(TObject *Sender)
{
  paintModulesPaint(this);
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
//
//
//                      FRAMES & FRAMES MODULES EDITING
//
//
//---------------------------------------------------------------------------
void TFormMain::ArrangeFramesPageItems()
{
  edFrames->Top = GroupBoxFrames->ClientHeight - edFrames->Height - 4;
  gridFrames->Height = edFrames->Top - gridFrames->Top - 4;

  edFModules->Top = GroupBoxFModules->ClientHeight - edFModules->Height - 4;
  gridFModules->Height = edFModules->Top - gridFModules->Top - 4;
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::Splitter6Moved(TObject *Sender)
{
  ArrangeFramesPageItems();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::Splitter5Moved(TObject *Sender)
{
  ArrangeFramesPageItems();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::Splitter7Moved(TObject *Sender)
{
  ArrangeFramesPageItems();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::trackFramesZoomChange(TObject *Sender)
{
  framesZoom = trackFramesZoom->Position;

  paintFramesPaint(this);
}
//---------------------------------------------------------------------------

void TFormMain::FramesToGrid()
{
  gridFrames->RowCount = 2;
  gridFrames->Cells[0][0] = "Index";
  gridFrames->Cells[1][0] = "Id";
  gridFrames->Cells[2][0] = "FModules";
  gridFrames->Cells[3][0] = "Info";

  for(int i = 0; i < mSprite.mFramesManager.Size(); i++)
  {
    gridFrames->Cells[0][i + 1] = AnsiString(i);
    gridFrames->Cells[1][i + 1] = "0x" + AnsiString::IntToHex(mSprite.mFramesManager.Get(i).GetId(), 32);
    gridFrames->Cells[2][i + 1] = mSprite.mFramesManager.Get(i).mFModules.Size();
    gridFrames->Cells[3][i + 1] = mSprite.mFramesManager.Get(i).GetInfo().c_str();
    gridFrames->RowCount++;
  }

  // fill the last line in the grid
  gridFrames->Cells[0][gridFrames->RowCount - 1] = "new...";

  for(int i = 1; i < 4; i++)
  {
    gridFrames->Cells[i][gridFrames->RowCount - 1] = "";
  }
}
//---------------------------------------------------------------------------

void TFormMain::FModulesToGrid()
{
  gridFModules->RowCount = 2;
  gridFModules->Cells[0][0] = "Index";
  gridFModules->Cells[1][0] = "Module Id";
  gridFModules->Cells[2][0] = "X";
  gridFModules->Cells[3][0] = "Y";
  gridFModules->Cells[4][0] = "Flags";


  if (gridFrames->Row < gridFrames->RowCount - 1 && gridFrames->RowCount > 1)
  {
    CFrame frame = mSprite.mFramesManager.Get(gridFrames->Row - 1);

    for(int i = 0; i < frame.mFModules.Size(); i++)
    {
      CFrameModule fModule = frame.mFModules.Get(i);

      std::string moduleInfo = "?";

      if (mSprite.mModulesManager.GetItemIndexById(fModule.GetModuleId()) != -1)
      {
        moduleInfo = " " + mSprite.mModulesManager.GetItemById(fModule.GetModuleId()).GetInfo();
      }

      gridFModules->Cells[0][i + 1] = AnsiString(i);
      gridFModules->Cells[1][i + 1] = "0x" + AnsiString::IntToHex(fModule.GetModuleId(), 32) + moduleInfo.c_str();
      gridFModules->Cells[2][i + 1] = fModule.GetPos().mX;
      gridFModules->Cells[3][i + 1] = fModule.GetPos().mY;
      gridFModules->Cells[4][i + 1] = fModule.GetFlags();
      gridFModules->RowCount++;
    }

    gridFModules->Enabled = true;
    edFModules->Enabled   = true;
    gridFModules->Color   = clWindow;
    edFModules->Color     = clWindow;
  }
  else
  {
    gridFModules->Enabled = false;
    edFModules->Enabled   = false;

    gridFModules->Color   = clScrollBar;
    edFModules->Color     = clScrollBar;
  }

  // fill the last line in the grid
  gridFModules->Cells[0][gridFModules->RowCount - 1] = "new...";

  for(int i = 1; i < 5; i++)
  {
    gridFModules->Cells[i][gridFModules->RowCount - 1] = "";
  }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::gridFramesClick(TObject *Sender)
{
  FModulesToGrid();

  if (gridFrames->Row == gridFrames->RowCount - 1)
  {
    edFrames->Text = "";
    return;
  }

  CFrame frame = mSprite.mFramesManager.Get(gridFrames->Row - 1);

  edFrames->Text =
    AnsiString("id=\"") + "0x" + AnsiString::IntToHex(frame.GetId(), 32) + "\" " +
    AnsiString("info=\"") + frame.GetInfo().c_str() + "\""
    ;

  paintFramesPaint(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::gridFramesDblClick(TObject *Sender)
{
  if (gridFrames->Row == gridFrames->RowCount - 1)
  {
    gridFramesPopNew(Sender);
  }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::gridFramesPopNew(TObject *Sender)
{
  int newIndex = mSprite.mFramesManager.New();

  FramesToGrid();
  gridFrames->Row = newIndex + 1;

  //gridFramesClick(this);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::gridFramesPopClone(TObject *Sender)
{
  if (gridFrames->Row < gridFrames->RowCount - 1)
  {
    int row = mSprite.mFramesManager.Clone(gridFrames->Row - 1);

    FramesToGrid();

    gridFrames->Row = row + 1;
  }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::gridFramesPopDelete(TObject *Sender)
{
  if (gridFrames->Row < gridFrames->RowCount - 1)
  {
    int result = mSprite.mFramesManager.Delete(gridFrames->Row - 1);

    FramesToGrid();

    if (result != -1)
    {
      gridFrames->Row = result + 1;
    }
    
    gridFramesClick(this);
  }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::gridFramesPopInsert(TObject *Sender)
{
  if (gridFrames->Row < gridFrames->RowCount - 1)
  {
    CFrame frame;

    frame.SetId(mSprite.mFramesManager.GenerateUniqueId());

    int row = mSprite.mFramesManager.Insert(gridFrames->Row - 1, frame);

    FramesToGrid();

    gridFrames->Row = row + 1;
  }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::gridFramesPopMoveDown(TObject *Sender)
{
  if (gridFrames->Row < gridFrames->RowCount - 1)
  {
    int result = mSprite.mFramesManager.MoveDown(gridFrames->Row - 1);

    FramesToGrid();

    if (result != -1)
    {
      gridFrames->Row = result + 1;
    }
  }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::gridFramesPopMoveUp(TObject *Sender)
{
  if (gridFrames->Row < gridFrames->RowCount - 1)
  {
    int result = mSprite.mFramesManager.MoveUp(gridFrames->Row - 1);

    FramesToGrid();

    if (result != -1)
    {
      gridFrames->Row = result + 1;
    }
  }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::edFramesKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
  if (Key == VK_RETURN)
  {
    edFramesExit(Sender);
  }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::edFramesExit(TObject *Sender)
{
  if (edFrames->Text == "")
  {
    return;
  }

  // if one attribute is wrong, is ignored, all the others that are correct,
  // will be set

  // get the entry that is edited
  CFrame frame = mSprite.mFramesManager.Get(gridFrames->Row - 1);

  // parse the new attributes through tinyxml
  tinyxml2::XMLDocument doc;

  doc.Parse( (AnsiString("<element ") + edFrames->Text + "/>").c_str());

  auto elem = doc.FirstChildElement("element");

  int result;

  //
  // Parse integer attributes
  //

  //
  // Parse hex attributes
  //
  const char* id = elem->Attribute("id");

  if (id)
  {
    int value;

    try
    {
      value = StrToInt(id);
      frame.SetId(value);
    }
    catch(...)
    {
    }
  }

  //
  // Parse string attributes
  //
  const char* info = elem->Attribute("info");

  if (info)
  {
    frame.SetInfo(info);
  }

  int oldRow = gridFrames->Row;

  mSprite.mFramesManager.Set(gridFrames->Row - 1, frame);

  edFrames->Text =
    UnicodeString("id=\"") + "0x" + UnicodeString::IntToHex(frame.GetId(), 32) + "\" " +
    UnicodeString("info=\"") + UTF8ToString(frame.GetInfo().c_str()) + "\""
    ;

  FramesToGrid();

  gridFrames->Row = oldRow;

  paintFramesPaint(this);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::gridFramesSelectCell(TObject *Sender, int ACol,
      int ARow, bool &CanSelect)
{
  CanSelect = true;
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::gridFModulesClick(TObject *Sender)
{
  if (gridFModules->Row == gridFModules->RowCount - 1)
  {
    edFModules->Text = "";
    return;
  }

  CFrame frame = mSprite.mFramesManager.Get(gridFrames->Row - 1);
  CFrameModule fmodule = frame.mFModules.Get(gridFModules->Row - 1);

  edFModules->Text =
    AnsiString("moduleid=\"") + "0x" + AnsiString::IntToHex(fmodule.GetModuleId(), 32) + "\" " +
    AnsiString("x=") + fmodule.GetPos().mX + " " +
    AnsiString("y=") + fmodule.GetPos().mY + " " +
    AnsiString("flags=\"") + "0x" + AnsiString::IntToHex(fmodule.GetFlags(), 32) + "\""
    ;

  paintFramesPaint(Sender);  
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::paintQuickModulesPaint(TObject *Sender)
{
  // make clean
  paintQuickModules->Canvas->Brush->Color = (TColor)mAppConfig.mQuickModulesBackgroundColor;
  paintQuickModules->Canvas->FillRect(paintQuickModules->ClientRect);

  Gdiplus::Graphics graphics(paintQuickModules->Canvas->Handle);

  graphics.SetSmoothingMode( Gdiplus::SmoothingModeNone );
  graphics.SetPixelOffsetMode( Gdiplus::PixelOffsetModeHalf );
  graphics.SetInterpolationMode( Gdiplus::InterpolationModeNearestNeighbor );

  // draw all modules here
  for(int i = 0; i < mSprite.mModulesManager.Size() - scrollQuickModules->Position; i++)
  {
    CModule module = mSprite.mModulesManager.Get(i + scrollQuickModules->Position);

    Gdiplus::Bitmap* bitmap = mSprite.GetModuleBitmap(module);
    int transparentColor = mSprite.mImagesManager.GetItemById(module.GetImageId()).GetTransparentColor();

    if (bitmap)
    {
      if (module.GetWidth() != -1 && module.GetHeight() != -1)
      {
        Gdiplus::Rect destRect(
          i*mAppConfig.mQuickModuleSize + mAppConfig.mQuickModuleSize / 2 - module.GetWidth() / 2,
          mAppConfig.mQuickModuleSize / 2 - module.GetHeight() / 2,
          module.GetWidth(),
          module.GetHeight()
        );

        if (module.GetWidth() > mAppConfig.mQuickModuleSize || module.GetHeight() > mAppConfig.mQuickModuleSize)
        {
          // keep the scaling factor
          // TODO: prevent modules with zero width and height
          // TODO: test module width against height to chose whether scaling will be horizontally or vertically
          destRect = Gdiplus::Rect(
            i*mAppConfig.mQuickModuleSize,
            0,
            mAppConfig.mQuickModuleSize,
            mAppConfig.mQuickModuleSize * module.GetHeight() / module.GetWidth()
          );
        }

        Gdiplus::ImageAttributes imAtt;

        if (transparentColor != -1)
        {
          imAtt.SetColorKey(
            Gdiplus::Color(GetRValue(transparentColor), GetGValue(transparentColor), GetBValue(transparentColor)),
            Gdiplus::Color(GetRValue(transparentColor), GetGValue(transparentColor), GetBValue(transparentColor)),
            Gdiplus::ColorAdjustTypeDefault);
        }
        
        graphics.DrawImage((Gdiplus::Image*)bitmap, destRect, module.GetX(), module.GetY(), module.GetWidth(), module.GetHeight(), Gdiplus::UnitPixel, &imAtt);


        /*
        Graphics::TBitmap* chunk = new Graphics::TBitmap();
        chunk->Width  = module.GetWidth();
        chunk->Height = module.GetHeight();

        chunk->Canvas->CopyRect(TRect(0, 0, module.GetWidth(), module.GetHeight()), bitmap->Canvas, TRect(module.GetX(), module.GetY(), module.GetX() + module.GetWidth(), module.GetY() + module.GetHeight()));

        TRect rect = TRect(i*mAppConfig.mQuickModuleSize + mAppConfig.mQuickModuleSize / 2 - module.GetWidth() / 2, mAppConfig.mQuickModuleSize / 2 - module.GetHeight() / 2, i*mAppConfig.mQuickModuleSize + mAppConfig.mQuickModuleSize / 2 + module.GetWidth() / 2, mAppConfig.mQuickModuleSize / 2 + module.GetHeight() / 2);

        if (module.GetWidth() > mAppConfig.mQuickModuleSize || module.GetHeight() > mAppConfig.mQuickModuleSize)
        {
          // keep the scaling factor
          // TODO: prevent modules with zero width and height
          // TODO: test module width against height to chose whether scaling will be horizontally or vertically
          rect = TRect(i*mAppConfig.mQuickModuleSize, 0, (i + 1)*mAppConfig.mQuickModuleSize, mAppConfig.mQuickModuleSize * module.GetHeight() / module.GetWidth());
        }

        chunk->Transparent      = true;
        chunk->TransparentMode  = tmFixed;
        chunk->TransparentColor = (TColor)(mSprite.mImagesManager.GetItemById(module.GetImageId()).GetTransparentColor());

        paintQuickModules->Canvas->StretchDraw(rect , chunk);

        delete chunk;
        */

        //paintQuickModules->Canvas->CopyRect(TRect(i*64, 0, i*64 + module.GetWidth(), module.GetHeight()), bitmap->Canvas, TRect(module.GetX(), module.GetY(), module.GetX() + module.GetWidth(), module.GetY() + module.GetHeight()));
      }
    }

    paintQuickModules->Canvas->Font->Color = (TColor)mAppConfig.mQuickModulesIdTextColor;
    //SelectObject(paintQuickModules->Canvas->Handle, paintQuickModules->Canvas->Font->Handle);
    SetBkMode(paintQuickModules->Canvas->Handle, TRANSPARENT);

    AnsiString tmp = "0x" + AnsiString::IntToHex(module.GetId(), 32);
    TSize textSize = paintQuickModules->Canvas->TextExtent(tmp);
    //paintQuickModules->Canvas->TextRect(TRect(i*mAppConfig.mQuickModuleSize, 2, i*mAppConfig.mQuickModuleSize + mAppConfig.mQuickModuleSize, 2 + textSize.cy), i*mAppConfig.mQuickModuleSize + mAppConfig.mQuickModuleSize / 2 - textSize.cx / 2, 2, tmp);
    paintQuickModules->Canvas->TextOut(i*mAppConfig.mQuickModuleSize + textSize.cx / 2, 2, tmp);

    tmp = module.GetInfo().c_str();
    textSize = paintQuickModules->Canvas->TextExtent(tmp);
    //paintQuickModules->Canvas->TextRect(TRect(i*mAppConfig.mQuickModuleSize, mAppConfig.mQuickModuleSize - textSize.cy - 2, i*mAppConfig.mQuickModuleSize + mAppConfig.mQuickModuleSize, mAppConfig.mQuickModuleSize - 2), i*mAppConfig.mQuickModuleSize + mAppConfig.mQuickModuleSize / 2 - textSize.cx / 2, mAppConfig.mQuickModuleSize - 2 - textSize.cy, tmp);
    paintQuickModules->Canvas->TextOut(i*mAppConfig.mQuickModuleSize + textSize.cx / 2, mAppConfig.mQuickModuleSize - textSize.cy - 2, tmp);
  }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::gridFModulesPopNew(TObject *Sender)
{
  CFrame frame = mSprite.mFramesManager.Get(gridFrames->Row - 1);

  int fmoduleIndex = frame.mFModules.New();

  mSprite.mFramesManager.Set(gridFrames->Row - 1, frame);

  int frameRow = gridFrames->Row;

  FModulesToGrid();
  FramesToGrid();

  gridFrames->Row   = frameRow;
  gridFModules->Row = fmoduleIndex + 1;

  gridFModulesClick(this);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::gridFModulesDblClick(TObject *Sender)
{
  if (gridFModules->Row == gridFModules->RowCount - 1)
  {
    gridFModulesPopNew(Sender);
  }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::edFModulesExit(TObject *Sender)
{
  if (edFModules->Text == "")
  {
    return;
  }

  // if one attribute is wrong, is ignored, all the others that are correct,
  // will be set

  // get the entry that is edited
  CFrame frame = mSprite.mFramesManager.Get(gridFrames->Row - 1);
  CFrameModule fmodule = frame.mFModules.Get(gridFModules->Row - 1);

  // parse the new attiributes through tinyxml
  tinyxml2::XMLDocument doc;

  doc.Parse( UTF8Encode("<element " + edFModules->Text + "/>").c_str() );

  auto elem = doc.FirstChildElement("element");

  int result;

  //
  // Parse integer attributes
  //
  int x, y, flags;

  result = elem->QueryIntAttribute("x", &x);

  if (result == XML_SUCCESS)
  {
    fmodule.SetX(x);
  }

  result = elem->QueryIntAttribute("y", &y);

  if (result == XML_SUCCESS)
  {
    fmodule.SetY(y);
  }

  result = elem->QueryIntAttribute("flags", &flags);

  if (result == XML_SUCCESS)
  {
    fmodule.SetFlags(flags);
  }

  //
  // Parse hex attributes
  //
  const char* moduleid = elem->Attribute("moduleid");

  if (moduleid)
  {
    int value;

    try
    {
      value = StrToInt(moduleid);
      fmodule.SetModuleId(value);
    }
    catch(...)
    {
    }
  }

  int oldRow = gridFModules->Row;

  frame.mFModules.Set(gridFModules->Row - 1, fmodule);
  mSprite.mFramesManager.Set(gridFrames->Row - 1, frame);

  edFModules->Text =
    UnicodeString("moduleid=\"") + "0x" + UnicodeString::IntToHex(fmodule.GetModuleId(), 32) + "\" " +
    UnicodeString("x=") + fmodule.GetPos().mX + " " +
    UnicodeString("y=") + fmodule.GetPos().mY + " " +
    UnicodeString("flags=\"") + "0x" + UnicodeString::IntToHex(fmodule.GetFlags(), 32) + "\""
    ;

  FModulesToGrid();

  gridFModules->Row = oldRow;

  paintFramesPaint(this);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::edFModulesKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
  if (Key == VK_RETURN)
  {
    edFModulesExit(this);
  }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::gridFModulesPopClone(TObject *Sender)
{
  if (gridFModules->Row < gridFModules->RowCount - 1)
  {
    CFrame frame = mSprite.mFramesManager.Get(gridFrames->Row - 1);

    int row = frame.mFModules.Clone(gridFModules->Row - 1);

    mSprite.mFramesManager.Set(gridFrames->Row - 1, frame);

    int frameRow = gridFrames->Row;

    FModulesToGrid();
    FramesToGrid();

    gridFrames->Row   = frameRow;
    gridFModules->Row = row + 1;

    gridFModulesClick(this);
  }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::gridFModulesPopDelete(TObject *Sender)
{
  if (gridFModules->Row < gridFModules->RowCount - 1)
  {
    CFrame frame = mSprite.mFramesManager.Get(gridFrames->Row - 1);

    int row = frame.mFModules.Delete(gridFModules->Row - 1);
    mSprite.mFramesManager.Set(gridFrames->Row - 1, frame);

    int frameRow = gridFrames->Row;
    
    FModulesToGrid();
    FramesToGrid();

    if (row != -1)
    {
      gridFModules->Row = row + 1;
    }

    gridFrames->Row   = frameRow;
    
    gridFModulesClick(this);
  }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::gridFModulesPopInsert(TObject *Sender)
{
  if (gridFModules->Row < gridFModules->RowCount - 1)
  {
    CFrame frame = mSprite.mFramesManager.Get(gridFrames->Row - 1);

    int row = frame.mFModules.Insert(gridFModules->Row - 1, CFrameModule());

    mSprite.mFramesManager.Set(gridFrames->Row - 1, frame);

    int frameRow = gridFrames->Row;

    FModulesToGrid();
    FramesToGrid();

    gridFrames->Row   = frameRow;
    gridFModules->Row = row + 1;

    gridFModulesClick(this);
  }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::gridFModulesPopMoveDown(TObject *Sender)
{
  if (gridFModules->Row < gridFModules->RowCount - 1)
  {
    CFrame frame = mSprite.mFramesManager.Get(gridFrames->Row - 1);

    int row = frame.mFModules.MoveDown(gridFModules->Row - 1);
    mSprite.mFramesManager.Set(gridFrames->Row - 1, frame);

    FModulesToGrid();

    if (row != -1)
    {
      gridFModules->Row = row + 1;
      gridFModulesClick(this);
    }
  }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::gridFModulesPopMoveUp(TObject *Sender)
{
  if (gridFModules->Row < gridFModules->RowCount - 1)
  {
    CFrame frame = mSprite.mFramesManager.Get(gridFrames->Row - 1);

    int row = frame.mFModules.MoveUp(gridFModules->Row - 1);
    mSprite.mFramesManager.Set(gridFrames->Row - 1, frame);

    FModulesToGrid();

    if (row != -1)
    {
      gridFModules->Row = row + 1;
      gridFModulesClick(this);
    }
  }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::paintFramesPaint(TObject *Sender)
{
  // make clean
  paintFrames->Canvas->Brush->Color = (TColor)mAppConfig.mFramesBackgroundColor;
  paintFrames->Canvas->FillRect(paintFrames->ClientRect);

  // set the line style
  paintFrames->Canvas->Pen->Color = (TColor)0x0000FF;
  paintFrames->Canvas->Pen->Style = psDash;

  if (gridFrames->Row < gridFrames->RowCount - 1)
  {
    CFrame frame = mSprite.mFramesManager.Get(gridFrames->Row - 1);

    //Graphics::TBitmap* chunk = new Graphics::TBitmap();

    Gdiplus::Graphics graphics(paintFrames->Canvas->Handle);
    
    graphics.SetSmoothingMode( Gdiplus::SmoothingModeNone );
    graphics.SetPixelOffsetMode( Gdiplus::PixelOffsetModeHalf );
    graphics.SetInterpolationMode( Gdiplus::InterpolationModeNearestNeighbor );

    for(int i = 0; i < frame.mFModules.Size(); i++)
    {
      CFrameModule fmodule = frame.mFModules.Get(i);

      CModule module = mSprite.mModulesManager.GetItemById(fmodule.GetModuleId());

      if (module.GetId() == -1)
      {
        continue;
      }

      Gdiplus::Bitmap* bitmap = mSprite.GetModuleBitmap(module);
      int transparentColor = mSprite.mImagesManager.GetItemById(module.GetImageId()).GetTransparentColor();

      if (bitmap)
      {
        if (module.GetWidth() != -1 && module.GetHeight() != -1)
        {
          Gdiplus::Rect destRect;

          CPoint posOffset = (cbFramesOffset->Checked) ? fmodule.GetPos() : CPoint(0, 0);

          if (frameMouseMode == MMM_MOVE && i == gridFModules->Row - 1)
          {
            destRect = Gdiplus::Rect(
              paintFrames->Width / 2 - framesPan.x + posOffset.mX * framesZoom / 100 + (-framesDefine.x + framesCurrentDefine.x),
              paintFrames->Height / 2 - framesPan.y + posOffset.mY * framesZoom / 100 + (-framesDefine.y + framesCurrentDefine.y),
              module.GetWidth() * framesZoom / 100,
              module.GetHeight() * framesZoom / 100
              );
          }
          else
          {
            destRect = Gdiplus::Rect(
              paintFrames->Width / 2 - framesPan.x + posOffset.mX * framesZoom / 100,
              paintFrames->Height / 2 - framesPan.y + posOffset.mY * framesZoom / 100,
              module.GetWidth() * framesZoom / 100,
              module.GetHeight() * framesZoom / 100
              );
          }

          Gdiplus::ImageAttributes imAtt;
          
          if (transparentColor == -1 || !cbFramesTrans->Checked)
          {
          }
          else
          {
            imAtt.SetColorKey(
              Gdiplus::Color(GetRValue(transparentColor), GetGValue(transparentColor), GetBValue(transparentColor)),
              Gdiplus::Color(GetRValue(transparentColor), GetGValue(transparentColor), GetBValue(transparentColor)),
              Gdiplus::ColorAdjustTypeDefault);

          }

          graphics.DrawImage((Gdiplus::Image*)bitmap, destRect, module.GetX(), module.GetY(), module.GetWidth(), module.GetHeight(), Gdiplus::UnitPixel, &imAtt);
          
          // highlight the selected fmodule
          if (i == gridFModules->Row - 1)
          {
            paintFrames->Canvas->MoveTo(destRect.X, destRect.Y);
            paintFrames->Canvas->LineTo(destRect.X + destRect.Width, destRect.Y);
            paintFrames->Canvas->LineTo(destRect.X + destRect.Width, destRect.Y + destRect.Height);
            paintFrames->Canvas->LineTo(destRect.X, destRect.Y + destRect.Height);
            paintFrames->Canvas->LineTo(destRect.X, destRect.Y);
          }
          
          /*
          chunk->Width  = module.GetWidth();
          chunk->Height = module.GetHeight();

          chunk->Canvas->CopyRect(TRect(0, 0, module.GetWidth(), module.GetHeight()), bitmap->Canvas, TRect(module.GetX(), module.GetY(), module.GetX() + module.GetWidth(), module.GetY() + module.GetHeight()));

          TRect rect;

          CPoint posOffset = (cbFramesOffset->Checked) ? fmodule.GetPos() : CPoint(0, 0);

          if (frameMouseMode == MMM_MOVE && i == gridFModules->Row - 1)
          {
            rect = TRect(
              paintFrames->Width / 2 - framesPan.x + posOffset.mX * framesZoom / 100 + (-framesDefine.x + framesCurrentDefine.x),
              paintFrames->Height / 2 - framesPan.y + posOffset.mY * framesZoom / 100 + (-framesDefine.y + framesCurrentDefine.y),
              paintFrames->Width / 2 - framesPan.x + posOffset.mX * framesZoom / 100 + (-framesDefine.x + framesCurrentDefine.x) + module.GetWidth() * framesZoom / 100,
              paintFrames->Height / 2 - framesPan.y + posOffset.mY * framesZoom / 100 + (-framesDefine.y + framesCurrentDefine.y) + module.GetHeight() * framesZoom / 100
              );
          }
          else
          {
            rect = TRect(
              paintFrames->Width / 2 - framesPan.x + posOffset.mX * framesZoom / 100,
              paintFrames->Height / 2 - framesPan.y + posOffset.mY * framesZoom / 100,
              paintFrames->Width / 2 - framesPan.x + (posOffset.mX + module.GetWidth()) * framesZoom / 100,
              paintFrames->Height / 2 - framesPan.y + (posOffset.mY + module.GetHeight()) * framesZoom / 100
              );
          }

          chunk->Transparent      = cbFramesTrans->Checked;
          chunk->TransparentMode  = tmFixed;
          chunk->TransparentColor = (TColor)(mSprite.mImagesManager.GetItemById(module.GetImageId()).GetTransparentColor());

          paintFrames->Canvas->StretchDraw(rect , chunk);

          //paintQuickModules->Canvas->CopyRect(TRect(i*64, 0, i*64 + module.GetWidth(), module.GetHeight()), bitmap->Canvas, TRect(module.GetX(), module.GetY(), module.GetX() + module.GetWidth(), module.GetY() + module.GetHeight()));

          // highlight the selected fmodule
          if (i == gridFModules->Row - 1)
          {
            paintFrames->Canvas->MoveTo(rect.left, rect.top);
            paintFrames->Canvas->LineTo(rect.right, rect.top);
            paintFrames->Canvas->LineTo(rect.right, rect.bottom);
            paintFrames->Canvas->LineTo(rect.left, rect.bottom);
            paintFrames->Canvas->LineTo(rect.left, rect.top);
          }
          */
        }
      }
    }

    //delete chunk;

    if (cbFramesBoundRect->Checked)
    {
      CRect boundRect = frame.GetBoundRect(mSprite.mModulesManager);
      boundRect.mX      *= framesZoom / 100.;
      boundRect.mY      *= framesZoom / 100.;
      boundRect.mWidth  *= framesZoom / 100.;
      boundRect.mHeight *= framesZoom / 100.;

      boundRect.mX += paintFrames->Width / 2 - framesPan.x;
      boundRect.mY += paintFrames->Height / 2 - framesPan.y;

      paintFrames->Canvas->Pen->Style = psDashDotDot;
      paintFrames->Canvas->Pen->Color = (TColor)mAppConfig.mFramesBoundRectColor;

      paintFrames->Canvas->MoveTo(boundRect.mX, boundRect.mY);
      paintFrames->Canvas->LineTo(boundRect.mX + boundRect.mWidth, boundRect.mY);
      paintFrames->Canvas->LineTo(boundRect.mX + boundRect.mWidth, boundRect.mY + boundRect.mHeight);
      paintFrames->Canvas->LineTo(boundRect.mX, boundRect.mY + boundRect.mHeight);
      paintFrames->Canvas->LineTo(boundRect.mX, boundRect.mY);
    }  
  }

  // draw the canvas embelishments (axis, grid and so on)
  if (cbFramesAxis->Checked && !cbFramesGrid->Checked)
  {
    paintFrames->Canvas->Pen->Style = psSolid;
    paintFrames->Canvas->Pen->Color = (TColor)mAppConfig.mFramesAxisColor;

    paintFrames->Canvas->MoveTo(paintFrames->Width / 2 - framesPan.x, 0);
    paintFrames->Canvas->LineTo(paintFrames->Width / 2 - framesPan.x, paintFrames->Height);

    paintFrames->Canvas->MoveTo(0, paintFrames->Height / 2 - framesPan.y);
    paintFrames->Canvas->LineTo(paintFrames->Width, paintFrames->Height / 2 - framesPan.y);
  }

  if (cbFramesGrid->Checked)
  {
    paintFrames->Canvas->Pen->Style = psSolid;
    paintFrames->Canvas->Pen->Color = (TColor)mAppConfig.mFramesGridColor;

    for(int x = framesPan.x % mAppConfig.mFramesGridXSpacing; x < paintFrames->Width; x += mAppConfig.mFramesGridXSpacing * framesZoom / 100)
    {
      paintFrames->Canvas->MoveTo(x, 0);
      paintFrames->Canvas->LineTo(x, paintFrames->Height);
    }

    for(int y = framesPan.y % mAppConfig.mFramesGridYSpacing; y < paintFrames->Height; y += mAppConfig.mFramesGridYSpacing * framesZoom / 100)
    {
      paintFrames->Canvas->MoveTo(0, y);
      paintFrames->Canvas->LineTo(paintFrames->Width, y);
    }
  }
}
//---------------------------------------------------------------------------


void __fastcall TFormMain::paintFramesMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
  ActiveControl = GroupBoxFramesCanvas;

  if (Button == mbLeft && frameMouseMode == MMM_PAN)
  {
    // start to pan
    frameMouseMode = MMM_DO_PAN;
    oldFramesPan.x = X;
    oldFramesPan.y = Y;
  }

  if (Button == mbLeft && frameAvailableMouseMode == MMM_MOVE)
  {
      frameMouseMode = MMM_MOVE;

      framesDefine.x = X - X % (framesZoom / 100);
      framesDefine.y = Y - Y % (framesZoom / 100);

      return;
  }
  
  if (Button == mbLeft && frameMouseMode == MMM_NONE)
  {
     // select the fmodule
     if (gridFrames->Row < gridFrames->RowCount - 1)
     {
       CFrame frame = mSprite.mFramesManager.Get(gridFrames->Row - 1);

       int fmodulesRow = gridFModules->Row;

       for(int i = frame.mFModules.Size() - 1; i >= 0 ; i--)
       {
         CFrameModule fmodule = frame.mFModules.Get(i);

         if (mSprite.mModulesManager.GetItemIndexById(fmodule.GetModuleId()) == -1)
         {
           continue;
         }

         CModule module = mSprite.mModulesManager.GetItemById(fmodule.GetModuleId());

         int left = paintFrames->Width / 2 + fmodule.GetPos().mX * framesZoom / 100 - framesPan.x;
         int top  = paintFrames->Height / 2 + fmodule.GetPos().mY * framesZoom / 100 - framesPan.y;

         if (PointInRect(TPoint(X, Y), TRect(left, top, left + module.GetWidth() * framesZoom / 100, top + module.GetHeight() * framesZoom / 100)))
         {
           fmodulesRow = i + 1;
           break;
         }
       }

       FModulesToGrid();
       gridFModules->Row = fmodulesRow;
       paintFramesPaint(this);

       gridFModulesClick(Sender);

       return;
     }
  }

  if (Button == mbRight)
  {
    frameMouseMode = (frameMouseMode == MMM_PAN) ? MMM_NONE : MMM_PAN;
    paintFrames->Cursor = GetModuleCursor(frameMouseMode);
  }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::paintFramesMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
  ActiveControl = GroupBoxFramesCanvas;

  if (Button == mbLeft && frameMouseMode == MMM_DO_PAN)
  {
    frameMouseMode = MMM_PAN;
  }

  if (Button == mbLeft && frameMouseMode == MMM_MOVE)
  {

    CFrame frame = mSprite.mFramesManager.Get(gridFrames->Row - 1);
    CFrameModule fmodule = frame.mFModules.Get(gridFModules->Row - 1);

    fmodule.SetX(fmodule.GetPos().mX + (X - X % (framesZoom / 100) - framesDefine.x) * 100 / framesZoom);
    fmodule.SetY(fmodule.GetPos().mY + (Y - Y % (framesZoom / 100) - framesDefine.y) * 100 / framesZoom);

    frame.mFModules.Set(gridFModules->Row - 1, fmodule);
    mSprite.mFramesManager.Set(gridFrames->Row - 1, frame);

    framesDefine.x = framesCurrentDefine.x = 0;
    framesDefine.y = framesCurrentDefine.y = 0;

    int oldFrameRow = gridFrames->Row;
    int oldFModuleRow = gridFModules->Row;

    FModulesToGrid();
    FramesToGrid();

    gridFrames->Row = oldFrameRow;
    gridFModules->Row = oldFModuleRow;

    frameMouseMode = MMM_NONE;
    paintFramesPaint(this);

    gridFModulesClick(Sender);
  }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::paintFramesMouseMove(TObject *Sender,
      TShiftState Shift, int X, int Y)
{
  //ActiveControl = GroupBoxFramesCanvas;

  if (frameMouseMode == MMM_DO_PAN)
  {
    framesPan.x += -X + oldFramesPan.x;
    framesPan.y += -Y + oldFramesPan.y;

    oldFramesPan.x = X;
    oldFramesPan.y = Y;

    paintFramesPaint(Sender);

    return;
  }

  if (frameMouseMode == MMM_MOVE)
  {
    framesCurrentDefine.x = X - X % (framesZoom / 100);
    framesCurrentDefine.y = Y - Y % (framesZoom / 100);

    paintFramesPaint(Sender);

    return;
  }

  frameAvailableMouseMode = GetFrameMouseMode(TPoint(X, Y));

  if (frameAvailableMouseMode != MMM_NONE)
  {
    paintFrames->Cursor = GetModuleCursor(frameAvailableMouseMode);
  }
  else
  {
    paintFrames->Cursor = GetModuleCursor(frameMouseMode);
  }
}
//---------------------------------------------------------------------------

TMouseMode TFormMain::GetFrameMouseMode(TPoint p)
{
  TMouseMode mode = MMM_NONE;

  int frameIndex    = gridFrames->Row - 1;

  // avoid out of bounds
  if (frameIndex < 0 || frameIndex >= mSprite.mFramesManager.Size())
  {
    return mode;
  }

  CFrame frame = mSprite.mFramesManager.Get(frameIndex);

  int fmoduleIndex  = gridFModules->Row - 1;

  // avoid out of bounds
  if (fmoduleIndex < 0 || fmoduleIndex >= frame.mFModules.Size())
  {
    return mode;
  }

  CFrameModule fmodule = frame.mFModules.Get(fmoduleIndex);
  CModule module = mSprite.mModulesManager.GetItemById(fmodule.GetModuleId());

  int left = paintFrames->Width / 2 + (cbFramesOffset->Checked ? fmodule.GetPos().mX : 0) * framesZoom / 100 - framesPan.x;
  int top  = paintFrames->Height / 2 + (cbFramesOffset->Checked ? fmodule.GetPos().mY : 0) * framesZoom / 100 - framesPan.y;

  if (PointInRect(p, TRect(left, top, left + module.GetWidth() * framesZoom / 100, top + module.GetHeight() * framesZoom / 100)))
  {
    mode = MMM_MOVE;
  }

  return mode;
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::TabSheetFramesShow(TObject *Sender)
{
  SetQuickModulesScroll();
}
//---------------------------------------------------------------------------

void TFormMain::SetQuickModulesScroll()
{
  if (mSprite.mModulesManager.Size() < paintQuickModules->Width / mAppConfig.mQuickModuleSize)
  {
    //scrollQuickModules->Enabled = false;
    //paintQuickModules->Enabled = false;
  }
  else
  {
    scrollQuickModules->Enabled = true;
    paintQuickModules->Enabled = true;
    scrollQuickModules->Max = mSprite.mModulesManager.Size() - paintQuickModules->Width / mAppConfig.mQuickModuleSize;
  }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::scrollQuickModulesScroll(TObject *Sender,
      TScrollCode ScrollCode, int &ScrollPos)
{
  scrollQuickModules->Position = ScrollPos;

  paintQuickModulesPaint(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::paintQuickModulesDblClick(TObject *Sender)
{
  // double clicking the box is similar to new fmodule
  if (gridFrames->Row >= gridFrames->RowCount - 1 || gridFrames->RowCount <= 1)
  {
    return;
  }

  for(int i = 0; i < mSprite.mModulesManager.Size() - scrollQuickModules->Position; i++)
  {
    CModule module = mSprite.mModulesManager.Get(i + scrollQuickModules->Position);

    Gdiplus::Bitmap* bitmap = mSprite.GetModuleBitmap(module);

    if (bitmap)
    {
      if (module.GetWidth() != -1 && module.GetHeight() != -1)
      {
        TRect rect = TRect(i*mAppConfig.mQuickModuleSize + mAppConfig.mQuickModuleSize / 2 - module.GetWidth() / 2, mAppConfig.mQuickModuleSize / 2 - module.GetHeight() / 2, i*mAppConfig.mQuickModuleSize + mAppConfig.mQuickModuleSize / 2 + module.GetWidth() / 2, mAppConfig.mQuickModuleSize / 2 + module.GetHeight() / 2);

        if (i*mAppConfig.mQuickModuleSize < mQuickModulePos.x && mQuickModulePos.x < (i + 1) *mAppConfig.mQuickModuleSize &&
            2 < mQuickModulePos.y && mQuickModulePos.y < mAppConfig.mQuickModuleSize)
        {
          CFrame frame = mSprite.mFramesManager.Get(gridFrames->Row - 1);

          int fmoduleIndex = frame.mFModules.New();

          CFrameModule fmodule = frame.mFModules.Get(fmoduleIndex);

          fmodule.SetModuleId(module.GetId());

          frame.mFModules.Set(fmoduleIndex, fmodule);

          mSprite.mFramesManager.Set(gridFrames->Row - 1, frame);

          int frameRow = gridFrames->Row;

          FModulesToGrid();
          FramesToGrid();

          gridFrames->Row   = frameRow;
          gridFModules->Row = fmoduleIndex + 1;

          gridFModulesClick(this);

          // only one module fits the conditions
          return;
        }
      }
    }
  }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::paintQuickModulesMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
  mQuickModulePos.x = X;
  mQuickModulePos.y = Y;
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::cbFramesAxisClick(TObject *Sender)
{
  paintFramesPaint(this);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::cbFramesGridClick(TObject *Sender)
{
  paintFramesPaint(this);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::cbFramesTransClick(TObject *Sender)
{
  paintFramesPaint(this);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::cbFramesAllClick(TObject *Sender)
{
  paintFramesPaint(this);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::cbFramesOffsetClick(TObject *Sender)
{
  paintFramesPaint(this);
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
//
//
//                      ANIMS & ANIMS FRAMES EDITING
//
//
//---------------------------------------------------------------------------
void TFormMain::ArrangeAnimsPageItems()
{
  edAnims->Top = GroupBoxAnims->ClientHeight - edAnims->Height - 4;
  gridAnims->Height = edAnims->Top - gridAnims->Top - 4;

  edAFrames->Top = GroupBoxAFrames->ClientHeight - edAFrames->Height - 4;
  gridAFrames->Height = edAFrames->Top - gridAFrames->Top - 4;
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::Splitter8Moved(TObject *Sender)
{
  ArrangeAnimsPageItems();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::Splitter9Moved(TObject *Sender)
{
  ArrangeAnimsPageItems();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::Splitter10Moved(TObject *Sender)
{
  ArrangeAnimsPageItems();
}
//---------------------------------------------------------------------------

void TFormMain::AnimsToGrid()
{
  gridAnims->RowCount = 2;
  gridAnims->Cells[0][0] = "Index";
  gridAnims->Cells[1][0] = "Id";
  gridAnims->Cells[2][0] = "AFrames";
  gridAnims->Cells[3][0] = "Info";

  for(int i = 0; i < mSprite.mAnimsManager.Size(); i++)
  {
    gridAnims->Cells[0][i + 1] = AnsiString(i);
    gridAnims->Cells[1][i + 1] = "0x" + AnsiString::IntToHex(mSprite.mAnimsManager.Get(i).GetId(), 32);
    gridAnims->Cells[2][i + 1] = mSprite.mAnimsManager.Get(i).mAFrames.Size();
    gridAnims->Cells[3][i + 1] = mSprite.mAnimsManager.Get(i).GetInfo().c_str();
    gridAnims->RowCount++;
  }

  // fill the last line in the grid
  gridAnims->Cells[0][gridAnims->RowCount - 1] = "new...";

  for(int i = 1; i < 4; i++)
  {
    gridAnims->Cells[i][gridAnims->RowCount - 1] = "";
  }
}
//---------------------------------------------------------------------------

void TFormMain::AFramesToGrid()
{
  gridAFrames->RowCount = 2;
  gridAFrames->Cells[0][0] = "Index";
  gridAFrames->Cells[1][0] = "Frame Id";
  gridAFrames->Cells[2][0] = "Time";
  gridAFrames->Cells[3][0] = "X";
  gridAFrames->Cells[4][0] = "Y";
  gridAFrames->Cells[5][0] = "Flags";


  if (gridAnims->Row < gridAnims->RowCount - 1 && gridAnims->RowCount > 1)
  {
    CAnim anim = mSprite.mAnimsManager.Get(gridAnims->Row - 1);

    for(int i = 0; i < anim.mAFrames.Size(); i++)
    {
      CAnimFrame aFrame = anim.mAFrames.Get(i);

      std::string frameInfo = "?";

      if (mSprite.mFramesManager.GetItemIndexById(aFrame.GetFrameId()) != -1)
      {
        frameInfo = " " + mSprite.mFramesManager.GetItemById(aFrame.GetFrameId()).GetInfo();
      }

      gridAFrames->Cells[0][i + 1] = AnsiString(i);
      gridAFrames->Cells[1][i + 1] = "0x" + AnsiString::IntToHex(aFrame.GetFrameId(), 32) + frameInfo.c_str();
      gridAFrames->Cells[2][i + 1] = aFrame.GetTime();
      gridAFrames->Cells[3][i + 1] = aFrame.GetPos().mX;
      gridAFrames->Cells[4][i + 1] = aFrame.GetPos().mY;
      gridAFrames->Cells[5][i + 1] = aFrame.GetFlags();
      gridAFrames->RowCount++;
    }

    gridAFrames->Enabled = true;
    edAFrames->Enabled   = true;
    gridAFrames->Color   = clWindow;
    edAFrames->Color     = clWindow;
  }
  else
  {
    gridAFrames->Enabled = false;
    edAFrames->Enabled   = false;

    gridAFrames->Color   = clScrollBar;
    edAFrames->Color     = clScrollBar;
  }

  gridAFrames->Cells[0][gridAFrames->RowCount - 1] = "new...";

  for(int i = 1; i < 6; i++)
  {
    gridAFrames->Cells[i][gridAFrames->RowCount - 1] = "";
  }
}
//---------------------------------------------------------------------------


void __fastcall TFormMain::gridAnimsDblClick(TObject *Sender)
{
  if (gridAnims->Row == gridAnims->RowCount - 1)
  {
    gridAnimsPopNew(Sender);
  }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::gridAnimsPopNew(TObject *Sender)
{
  mSprite.mAnimsManager.New();

  AnimsToGrid();

  gridAnimsClick(this);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::gridAnimsClick(TObject *Sender)
{
  AFramesToGrid();

  gridAFrames->Row = 1;

  if (gridAnims->Row == gridAnims->RowCount - 1)
  {
    edAnims->Text = "";
    return;
  }

  CAnim anim = mSprite.mAnimsManager.Get(gridAnims->Row - 1);

  edAnims->Text =
    AnsiString("id=\"") + "0x" + AnsiString::IntToHex(anim.GetId(), 32) + "\" " +
    AnsiString("info=\"") + anim.GetInfo().c_str() + "\""
    ;

  paintAnimsPaint(Sender);

  TimerAnim->Enabled = true;
  btnPlay->Enabled   = false;
  btnPause->Enabled  = true;
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::gridAnimsPopClone(TObject *Sender)
{
  if (gridAnims->Row < gridAnims->RowCount - 1)
  {
    int row = mSprite.mAnimsManager.Clone(gridAnims->Row - 1);

    AnimsToGrid();

    gridAnims->Row = row + 1;
  }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::gridAnimsPopDelete(TObject *Sender)
{
  if (gridAnims->Row < gridAnims->RowCount - 1)
  {
    int result = mSprite.mAnimsManager.Delete(gridAnims->Row - 1);

    AnimsToGrid();

    if (result != -1)
    {
      gridAnims->Row = result + 1;
    }

    gridAnimsClick(this);
  }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::gridAnimsPopInsert(TObject *Sender)
{
  if (gridAnims->Row < gridAnims->RowCount - 1)
  {
    CAnim anim;

    anim.SetId(mSprite.mAnimsManager.GenerateUniqueId());

    int row = mSprite.mAnimsManager.Insert(gridAnims->Row - 1, anim);

    AnimsToGrid();

    gridAnims->Row = row + 1;
  }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::gridAnimsPopMoveDown(TObject *Sender)
{
  if (gridAnims->Row < gridAnims->RowCount - 1)
  {
    int result = mSprite.mAnimsManager.MoveDown(gridAnims->Row - 1);

    AnimsToGrid();

    if (result != -1)
    {
      gridAnims->Row = result + 1;
    }
  }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::gridAnimsPopMoveUp(TObject *Sender)
{
  if (gridAnims->Row < gridAnims->RowCount - 1)
  {
    int result = mSprite.mAnimsManager.MoveUp(gridAnims->Row - 1);

    AnimsToGrid();

    if (result != -1)
    {
      gridAnims->Row = result + 1;
    }
  }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::gridAnimsSelectCell(TObject *Sender, int ACol,
      int ARow, bool &CanSelect)
{
  CanSelect = true;
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::gridAFramesClick(TObject *Sender)
{
  if (gridAFrames->Row == gridAFrames->RowCount - 1)
  {
    edAFrames->Text = "";
    return;
  }

  CAnim anim = mSprite.mAnimsManager.Get(gridAnims->Row - 1);

  if (anim.mAFrames.Size() > 0)
  {
    CAnimFrame aFrame = anim.mAFrames.Get(gridAFrames->Row - 1);

    edAFrames->Text =
      AnsiString("frameid=\"") + "0x" + AnsiString::IntToHex(aFrame.GetFrameId(), 32) + "\" " +
      AnsiString("time=") + aFrame.GetTime() + " " +
      AnsiString("x=") + aFrame.GetPos().mX + " " +
      AnsiString("y=") + aFrame.GetPos().mY + " " +
      AnsiString("flags=\"") + "0x" + AnsiString::IntToHex(aFrame.GetFlags(), 32) + "\""
    ;
  }
  else
  {
      edAFrames->Text = "";
  }

  paintAnimsPaint(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::gridAFramesDblClick(TObject *Sender)
{
  if (gridAFrames->Row == gridAFrames->RowCount - 1)
  {
    gridAFramesPopNew(Sender);
  }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::gridAFramesPopClone(TObject *Sender)
{
  if (gridAFrames->Row < gridAFrames->RowCount - 1)
  {
    CAnim anim = mSprite.mAnimsManager.Get(gridAnims->Row - 1);

    int row = anim.mAFrames.Clone(gridAFrames->Row - 1);

    mSprite.mAnimsManager.Set(gridAnims->Row - 1, anim);

    int animRow = gridAnims->Row;

    AFramesToGrid();
    AnimsToGrid();

    gridAnims->Row   = animRow;
    gridAFrames->Row = row + 1;

    gridAFramesClick(this);
  }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::gridAFramesPopDelete(TObject *Sender)
{
  if (gridAFrames->Row < gridAFrames->RowCount - 1)
  {
    CAnim anim = mSprite.mAnimsManager.Get(gridAnims->Row - 1);

    int row = anim.mAFrames.Delete(gridAFrames->Row - 1);
    mSprite.mAnimsManager.Set(gridAnims->Row - 1, anim);

    AnimsToGrid();
    AFramesToGrid();

    if (row != -1)
    {
      gridAFrames->Row = row + 1;
    }
    
    gridAFramesClick(this);
  }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::gridAFramesPopInsert(TObject *Sender)
{
  if (gridAFrames->Row < gridAFrames->RowCount - 1)
  {
    CAnim anim = mSprite.mAnimsManager.Get(gridAnims->Row - 1);

    int row = anim.mAFrames.Insert(gridAFrames->Row - 1, CAnimFrame());

    mSprite.mAnimsManager.Set(gridAnims->Row - 1, anim);

    int animRow = gridAnims->Row;

    AFramesToGrid();
    AnimsToGrid();

    gridAnims->Row   = animRow;
    gridAFrames->Row = row + 1;

    gridAFramesClick(this);
  }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::gridAFramesPopMoveDown(TObject *Sender)
{
  if (gridAFrames->Row < gridAFrames->RowCount - 1)
  {
    CAnim anim = mSprite.mAnimsManager.Get(gridAnims->Row - 1);

    int row = anim.mAFrames.MoveDown(gridAFrames->Row - 1);
    mSprite.mAnimsManager.Set(gridAnims->Row - 1, anim);

    AFramesToGrid();

    if (row != -1)
    {
      gridAFrames->Row = row + 1;
      gridAFramesClick(this);
    }
  }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::gridAFramesPopMoveUp(TObject *Sender)
{
  if (gridAFrames->Row < gridAFrames->RowCount - 1)
  {
    CAnim anim = mSprite.mAnimsManager.Get(gridAnims->Row - 1);

    int row = anim.mAFrames.MoveUp(gridAFrames->Row - 1);
    mSprite.mAnimsManager.Set(gridAnims->Row - 1, anim);

    AFramesToGrid();

    if (row != -1)
    {
      gridAFrames->Row = row + 1;
      gridAFramesClick(this);
    }
  }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::gridAFramesPopNew(TObject *Sender)
{
  CAnim anim = mSprite.mAnimsManager.Get(gridAnims->Row - 1);

  int aFrameIndex = anim.mAFrames.New();

  mSprite.mAnimsManager.Set(gridAnims->Row - 1, anim);

  int animRow = gridAnims->Row;

  AFramesToGrid();
  AnimsToGrid();

  gridAnims->Row   = animRow;
  gridAFrames->Row = aFrameIndex + 1;

  gridAFramesClick(this);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::edAnimsExit(TObject *Sender)
{
  if (edAnims->Text == "")
  {
    return;
  }

  // if one attribute is wrong, is ignored, all the others that are correct,
  // will be set

  // get the entry that is edited
  CAnim anim = mSprite.mAnimsManager.Get(gridAnims->Row - 1);

  // parse the new attiributes through tinyxml
  tinyxml2::XMLDocument doc;

  doc.Parse( UTF8Encode("<element " + edAnims->Text + "/>").c_str());

  auto elem = doc.FirstChildElement("element");

  int result;

  //
  // Parse integer attributes
  //

  //
  // Parse hex attributes
  //
  const char* id = elem->Attribute("id");

  if (id)
  {
    int value;

    try
    {
      value = StrToInt(id);
      anim.SetId(value);
    }
    catch(...)
    {
    }
  }

  //
  // Parse string attributes
  //
  const char* info = elem->Attribute("info");

  if (info)
  {
    anim.SetInfo(info);
  }

  int oldRow = gridAnims->Row;

  mSprite.mAnimsManager.Set(gridAnims->Row - 1, anim);

  edAnims->Text =
    UnicodeString("id=\"") + "0x" + UnicodeString::IntToHex(anim.GetId(), 32) + "\" " +
    UnicodeString("info=\"") + UTF8ToString(anim.GetInfo().c_str()) + "\""
    ;

  AnimsToGrid();

  gridAnims->Row = oldRow;

  paintAnimsPaint(this);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::edAnimsKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
  if (Key == VK_RETURN)
  {
    edAnimsExit(Sender);
  }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::edAFramesKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
  if (Key == VK_RETURN)
  {
    edAFramesExit(this);
  }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::edAFramesExit(TObject *Sender)
{
  if (edAFrames->Text == "")
  {
    return;
  }

  // if one attribute is wrong, is ignored, all the others that are correct,
  // will be set

  // get the entry that is edited
  CAnim anim = mSprite.mAnimsManager.Get(gridAnims->Row - 1);
  CAnimFrame aFrame = anim.mAFrames.Get(gridAFrames->Row - 1);

  // parse the new attiributes through tinyxml
  tinyxml2::XMLDocument doc;

  doc.Parse( UTF8Encode("<element " + edAFrames->Text + "/>").c_str() );

  auto elem = doc.FirstChildElement("element");

  int result;

  //
  // Parse integer attributes
  //
  int x, y, time, flags;

  result = elem->QueryIntAttribute("x", &x);

  if (result == XML_SUCCESS)
  {
    aFrame.SetX(x);
  }

  result = elem->QueryIntAttribute("y", &y);

  if (result == XML_SUCCESS)
  {
    aFrame.SetY(y);
  }

  result = elem->QueryIntAttribute("flags", &flags);

  if (result == XML_SUCCESS)
  {
    aFrame.SetFlags(flags);
  }

  result = elem->QueryIntAttribute("time", &time);

  if (result == XML_SUCCESS)
  {
    aFrame.SetTime(time);
  }

  //
  // Parse hex attributes
  //
  const char* frameid = elem->Attribute("frameid");

  if (frameid)
  {
    int value;

    try
    {
      value = StrToInt(frameid);
      aFrame.SetFrameId(value);
    }
    catch(...)
    {
    }
  }

  int oldRow = gridAFrames->Row;

  anim.mAFrames.Set(gridAFrames->Row - 1, aFrame);
  mSprite.mAnimsManager.Set(gridAnims->Row - 1, anim);

  edAFrames->Text =
    UnicodeString("frameid=\"") + "0x" + UnicodeString::IntToHex(aFrame.GetFrameId(), 32) + "\" " +
    UnicodeString("time=") + aFrame.GetTime() + " " +
    UnicodeString("x=") + aFrame.GetPos().mX + " " +
    UnicodeString("y=") + aFrame.GetPos().mY + " " +
    UnicodeString("flags=\"") + "0x" + UnicodeString::IntToHex(aFrame.GetFlags(), 32) + "\""
    ;

  AFramesToGrid();

  gridAFrames->Row = oldRow;

  paintAnimsPaint(this);
}
//---------------------------------------------------------------------------

TMouseMode TFormMain::GetAnimMouseMode(TPoint p)
{
  TMouseMode mode = MMM_NONE;

  int animIndex    = gridAnims->Row - 1;

  // avoid out of bounds
  if (animIndex < 0 || animIndex >= mSprite.mAnimsManager.Size())
  {
    return mode;
  }

  CAnim anim = mSprite.mAnimsManager.Get(animIndex);

  int aFrameIndex  = gridAFrames->Row - 1;

  // avoid out of bounds
  if (aFrameIndex < 0 || aFrameIndex >= anim.mAFrames.Size())
  {
    return mode;
  }

  CAnimFrame aFrame = anim.mAFrames.Get(aFrameIndex);

  int frameIndex = mSprite.mFramesManager.GetItemIndexById(aFrame.GetFrameId());

  if (frameIndex == -1)
  {
    return mode;
  }

  CFrame frame = mSprite.mFramesManager.Get(frameIndex);

  // iterate over fmodules in this frame
  for(int i = 0; i < frame.mFModules.Size(); i++)
  {
    CFrameModule fModule = frame.mFModules.Get(i);

    int moduleIndex = mSprite.mModulesManager.GetItemIndexById(fModule.GetModuleId());

    if (moduleIndex == -1)
    {
      continue;
    }

    CModule module = mSprite.mModulesManager.GetItemById(fModule.GetModuleId());

    int left = paintAnims->Width / 2 + fModule.GetPos().mX * animsZoom / 100 - animsPan.x;
    int top  = paintAnims->Height / 2 + fModule.GetPos().mY * animsZoom / 100 - animsPan.y;

    if (PointInRect(p, TRect(left, top, left + module.GetWidth() * animsZoom / 100, top + module.GetHeight() * animsZoom / 100)))
    {
      mode = MMM_MOVE;
    }
  }

  return mode;
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::paintAnimsMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
  ActiveControl = GroupBoxAnimsCanvas;

  if (Button == mbLeft && animMouseMode == MMM_PAN)
  {
    // start to pan
    animMouseMode = MMM_DO_PAN;
    oldAnimsPan.x = X;
    oldAnimsPan.y = Y;
  }

  if (Button == mbLeft && animAvailableMouseMode == MMM_MOVE)
  {
      animMouseMode = MMM_MOVE;
      animsDefine.x = X;
      animsDefine.y = Y;

      return;
  }

  if (Button == mbRight)
  {
    animMouseMode = (animMouseMode == MMM_PAN) ? MMM_NONE : MMM_PAN;
    paintAnims->Cursor = GetModuleCursor(animMouseMode);
  }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::paintAnimsMouseMove(TObject *Sender,
      TShiftState Shift, int X, int Y)
{
  //ActiveControl = GroupBoxAnimsCanvas;

  if (animMouseMode == MMM_DO_PAN)
  {
    animsPan.x += -X + oldAnimsPan.x;
    animsPan.y += -Y + oldAnimsPan.y;

    oldAnimsPan.x = X;
    oldAnimsPan.y = Y;

    paintAnimsPaint(Sender);

    return;
  }

  if (animMouseMode == MMM_MOVE)
  {
    animsCurrentDefine.x = X;
    animsCurrentDefine.y = Y;

    paintAnimsPaint(Sender);

    return;
  }

  animAvailableMouseMode = GetAnimMouseMode(TPoint(X, Y));

  if (animAvailableMouseMode != MMM_NONE)
  {
    paintAnims->Cursor = GetModuleCursor(animAvailableMouseMode);
  }
  else
  {
    paintAnims->Cursor = GetModuleCursor(animMouseMode);
  }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::paintAnimsMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
  ActiveControl = GroupBoxAnimsCanvas;

  if (Button == mbLeft && animMouseMode == MMM_DO_PAN)
  {
    animMouseMode = MMM_PAN;
  }

  if (Button == mbLeft && animMouseMode == MMM_MOVE)
  {
    CAnim      anim   = mSprite.mAnimsManager.Get(gridAnims->Row - 1);
    CAnimFrame aFrame = anim.mAFrames.Get(gridAFrames->Row - 1);

    aFrame.SetX(aFrame.GetPos().mX + (X - animsDefine.x) * 100 / animsZoom);
    aFrame.SetY(aFrame.GetPos().mY + (Y - animsDefine.y) * 100 / animsZoom);

    anim.mAFrames.Set(gridAFrames->Row - 1, aFrame);
    mSprite.mAnimsManager.Set(gridAnims->Row - 1, anim);

    animsDefine.x = animsCurrentDefine.x = 0;
    animsDefine.y = animsCurrentDefine.y = 0;

    int oldAnimRow = gridAnims->Row;
    int oldAFrameRow = gridAFrames->Row;

    AFramesToGrid();
    AnimsToGrid();

    gridAnims->Row = oldAnimRow;
    gridAFrames->Row = oldAFrameRow;

    animMouseMode = MMM_NONE;
    paintAnimsPaint(this);

    gridAFramesClick(Sender);
  }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::paintAnimsPaint(TObject *Sender)
{
  // make clean
  paintAnims->Canvas->Brush->Style = bsClear;
  paintAnims->Canvas->Brush->Color = (TColor)mAppConfig.mAnimsBackgroundColor;
  paintAnims->Canvas->FillRect(paintAnims->ClientRect);

  // set the line style
  paintAnims->Canvas->Pen->Color = (TColor)mAppConfig.mAnimsSelectColor;
  paintAnims->Canvas->Pen->Style = psDash;

  if (gridAnims->Row < gridAnims->RowCount - 1 && gridAnims->RowCount > 2 && mSprite.mAnimsManager.Size() > 0)
  {
    CAnim anim = mSprite.mAnimsManager.Get(gridAnims->Row - 1);

    //for(int j = 0; j < anim.mAFrames.Size(); j++)
    if (gridAFrames->Row < gridAFrames->RowCount - 1 && gridAFrames->RowCount > 2 && anim.mAFrames.Size() > 0)
    {
      CAnimFrame aFrame = anim.mAFrames.Get(gridAFrames->Row - 1);

      int frameIndex = mSprite.mFramesManager.GetItemIndexById(aFrame.GetFrameId());

      if (frameIndex != -1)
      {
        CFrame frame = mSprite.mFramesManager.Get(frameIndex);

        //Graphics::TBitmap* chunk = new Graphics::TBitmap();

        Gdiplus::Graphics graphics(paintAnims->Canvas->Handle);

        graphics.SetSmoothingMode( Gdiplus::SmoothingModeNone );
        graphics.SetPixelOffsetMode( Gdiplus::PixelOffsetModeHalf );
        graphics.SetInterpolationMode( Gdiplus::InterpolationModeNearestNeighbor );

        for(int i = 0; i < frame.mFModules.Size(); i++)
        {
          CFrameModule fmodule = frame.mFModules.Get(i);

          int moduleIndex = mSprite.mModulesManager.GetItemIndexById(fmodule.GetModuleId());

          if (moduleIndex == -1)
          {
            continue;
          }

          CModule module = mSprite.mModulesManager.GetItemById(fmodule.GetModuleId());

          Gdiplus::Bitmap* bitmap = mSprite.GetModuleBitmap(module);
          int transparentColor = mSprite.mImagesManager.GetItemById(module.GetImageId()).GetTransparentColor();

          if (bitmap)
          {
            if (module.GetWidth() != -1 && module.GetHeight() != -1)
            {
              // reimplemented using GDI+
              Gdiplus::Rect destRect;

              CPoint posFMOffset = (cbAnimsFMOffset->Checked) ? fmodule.GetPos() : CPoint(0, 0);
              CPoint posAFOffset = (cbAnimsAFOffset->Checked) ? aFrame.GetPos() : CPoint(0, 0);

              if (animMouseMode == MMM_MOVE)
              {
                destRect = Gdiplus::Rect(
                  paintAnims->Width / 2 - animsPan.x + (posFMOffset.mX + posAFOffset.mX) * animsZoom / 100 + (-animsDefine.x + animsCurrentDefine.x),
                  paintAnims->Height / 2 - animsPan.y + (posFMOffset.mY + posAFOffset.mY) * animsZoom / 100 + (-animsDefine.y + animsCurrentDefine.y),
                  module.GetWidth() * animsZoom / 100,
                  module.GetHeight() * animsZoom / 100
                  );
              }
              else
              {
                destRect = Gdiplus::Rect(
                  paintAnims->Width / 2  - animsPan.x + (posFMOffset.mX + posAFOffset.mX) * animsZoom / 100,
                  paintAnims->Height / 2 - animsPan.y + (posFMOffset.mY + posAFOffset.mY) * animsZoom / 100,
                  module.GetWidth() * animsZoom / 100,
                  module.GetHeight() * animsZoom / 100
                  );
              }

              Gdiplus::ImageAttributes imAtt;

              if (transparentColor == -1 || !cbAnimsTrans->Checked)
              {
              }
              else
              {
                imAtt.SetColorKey(
                  Gdiplus::Color(GetRValue(transparentColor), GetGValue(transparentColor), GetBValue(transparentColor)),
                  Gdiplus::Color(GetRValue(transparentColor), GetGValue(transparentColor), GetBValue(transparentColor)),
                  Gdiplus::ColorAdjustTypeDefault);

              }

              graphics.DrawImage((Gdiplus::Image*)bitmap, destRect, module.GetX(), module.GetY(), module.GetWidth(), module.GetHeight(), Gdiplus::UnitPixel, &imAtt);

              /*
              chunk->Width  = module.GetWidth();
              chunk->Height = module.GetHeight();

              chunk->Canvas->CopyRect(TRect(0, 0, module.GetWidth(), module.GetHeight()), bitmap->Canvas, TRect(module.GetX(), module.GetY(), module.GetX() + module.GetWidth(), module.GetY() + module.GetHeight()));

              TRect rect;

              CPoint posFMOffset = (cbAnimsFMOffset->Checked) ? fmodule.GetPos() : CPoint(0, 0);
              CPoint posAFOffset = (cbAnimsAFOffset->Checked) ? aFrame.GetPos() : CPoint(0, 0);

              if (animMouseMode == MMM_MOVE)
              {
                rect = TRect(
                  paintAnims->Width / 2 - animsPan.x + (posFMOffset.mX + posAFOffset.mX) * animsZoom / 100 + (-animsDefine.x + animsCurrentDefine.x),
                  paintAnims->Height / 2 - animsPan.y + (posFMOffset.mY + posAFOffset.mY) * animsZoom / 100 + (-animsDefine.y + animsCurrentDefine.y),
                  paintAnims->Width / 2 - animsPan.x + (posFMOffset.mX  + posAFOffset.mX) * animsZoom / 100 + (-animsDefine.x + animsCurrentDefine.x) + module.GetWidth() * animsZoom / 100,
                  paintAnims->Height / 2 - animsPan.y + (posFMOffset.mY  + posAFOffset.mY) * animsZoom / 100 + (-animsDefine.y + animsCurrentDefine.y) + module.GetHeight() * animsZoom / 100
                  );
              }
              else
              {
                rect = TRect(
                  paintAnims->Width / 2  - animsPan.x + (posFMOffset.mX + posAFOffset.mX) * animsZoom / 100,
                  paintAnims->Height / 2 - animsPan.y + (posFMOffset.mY + posAFOffset.mY) * animsZoom / 100,
                  paintAnims->Width / 2  - animsPan.x + (posFMOffset.mX + posAFOffset.mX + module.GetWidth()) * animsZoom / 100,
                  paintAnims->Height / 2 - animsPan.y + (posFMOffset.mY + posAFOffset.mY + module.GetHeight()) * animsZoom / 100
                  );
              }

              chunk->Transparent      = cbAnimsTrans->Checked;
              chunk->TransparentMode  = tmFixed;
              chunk->TransparentColor = (TColor)(mSprite.mImagesManager.GetItemById(module.GetImageId()).GetTransparentColor());

              paintAnims->Canvas->StretchDraw(rect , chunk);
              */
            }
          }
        }

        //delete chunk;
      }
    }
  }

  // draw the canvas embelishments (axis, grid and so on)
  if (cbAnimsAxis->Checked && !cbAnimsGrid->Checked)
  {
    paintAnims->Canvas->Pen->Style = psSolid;
    paintAnims->Canvas->Pen->Color = (TColor)mAppConfig.mAnimsAxisColor;

    paintAnims->Canvas->MoveTo(paintAnims->Width / 2 - animsPan.x, 0);
    paintAnims->Canvas->LineTo(paintAnims->Width / 2 - animsPan.x, paintAnims->Height);

    paintAnims->Canvas->MoveTo(0, paintAnims->Height / 2 - animsPan.y);
    paintAnims->Canvas->LineTo(paintAnims->Width, paintAnims->Height / 2 - animsPan.y);
  }

  if (cbAnimsGrid->Checked)
  {
    paintAnims->Canvas->Pen->Style = psSolid;
    paintAnims->Canvas->Pen->Color = (TColor)mAppConfig.mAnimsGridColor;

    for(int x = animsPan.x % mAppConfig.mAnimsGridXSpacing; x < paintAnims->Width; x += mAppConfig.mAnimsGridXSpacing * animsZoom / 100)
    {
      paintAnims->Canvas->MoveTo(x, 0);
      paintAnims->Canvas->LineTo(x, paintAnims->Height);
    }

    for(int y = animsPan.y % mAppConfig.mAnimsGridYSpacing; y < paintAnims->Height; y += mAppConfig.mAnimsGridYSpacing * animsZoom / 100)
    {
      paintAnims->Canvas->MoveTo(0, y);
      paintAnims->Canvas->LineTo(paintAnims->Width, y);
    }
  }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::paintQuickFramesMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
  mQuickFramePos.x = X;
  mQuickFramePos.y = Y;
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::paintQuickFramesDblClick(TObject *Sender)
{
  // double clicking the box is similar to new aframe
  if (gridAnims->Row >= gridAnims->RowCount - 1 || gridAnims->RowCount <= 1)
  {
    return;
  }

  for(int i = 0; i < mSprite.mFramesManager.Size() - scrollQuickFrames->Position; i++)
  {
    CFrame frame = mSprite.mFramesManager.Get(i + scrollQuickFrames->Position);

    if (i * mAppConfig.mQuickFrameSize < mQuickFramePos.x && mQuickFramePos.x < (i + 1) * mAppConfig.mQuickFrameSize &&
        2 < mQuickFramePos.y && mQuickFramePos.y < mAppConfig.mQuickFrameSize)
    {
      // TODO!!!
      CAnim anim = mSprite.mAnimsManager.Get(gridAnims->Row - 1);

      int aFrameIndex = anim.mAFrames.New();

      CAnimFrame aframe = anim.mAFrames.Get(aFrameIndex);

      aframe.SetFrameId(frame.GetId());

      anim.mAFrames.Set(aFrameIndex, aframe);

      mSprite.mAnimsManager.Set(gridAnims->Row - 1, anim);

      int animRow = gridAnims->Row;

      AFramesToGrid();
      AnimsToGrid();

      gridAnims->Row   = animRow;
      gridAFrames->Row = aFrameIndex + 1;

      gridAFramesClick(this);

      // only one frame fits the condition
      break;
    }
  }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::paintQuickFramesPaint(TObject *Sender)
{
  // make clean
  paintQuickFrames->Canvas->Brush->Style = bsClear;
  paintQuickFrames->Canvas->Brush->Color = (TColor)mAppConfig.mQuickFramesBackgroundColor;
  paintQuickFrames->Canvas->FillRect(paintQuickFrames->ClientRect);

  Gdiplus::Graphics graphics(paintQuickFrames->Canvas->Handle);

  graphics.SetSmoothingMode( Gdiplus::SmoothingModeNone );
  graphics.SetPixelOffsetMode( Gdiplus::PixelOffsetModeHalf );
  graphics.SetInterpolationMode( Gdiplus::InterpolationModeNearestNeighbor );

  // draw all frames here
  for(int i = 0; i < mSprite.mFramesManager.Size() - scrollQuickFrames->Position; i++)
  {
    CFrame frame = mSprite.mFramesManager.Get(i + scrollQuickFrames->Position);

    CRect frameBoundRect = frame.GetBoundRect(mSprite.mModulesManager);

    float scaleW = 1.0;
    float scaleH = 1.0;

    if (frameBoundRect.mWidth > mAppConfig.mQuickFrameSize ||
        frameBoundRect.mHeight > mAppConfig.mQuickFrameSize)
    {
      if (frameBoundRect.mWidth > frameBoundRect.mHeight)
      {
        scaleH = (float)frameBoundRect.mHeight / frameBoundRect.mWidth;

        /*
        scaleH = ((float)mAppConfig.mQuickFrameSize * frameBoundRect.mHeight / frameBoundRect.mWidth) / frameBoundRect.mHeight;
        scaleW *= (float)frameBoundRect.mHeight / frameBoundRect.mWidth;
        */
      }
      else
      {
        scaleW = (float)frameBoundRect.mWidth / frameBoundRect.mHeight;

        /*
        scaleW = ((float)mAppConfig.mQuickFrameSize * frameBoundRect.mWidth / frameBoundRect.mHeight) / frameBoundRect.mWidth;
        scaleH *= (float)frameBoundRect.mWidth / frameBoundRect.mHeight;
        */
      }
    }

    for(int j = 0; j < frame.mFModules.Size(); j++)
    {
      CFrameModule fModule = frame.mFModules.Get(j);

      int moduleIndex = mSprite.mModulesManager.GetItemIndexById(fModule.GetModuleId());

      if (moduleIndex == -1)
      {
        continue;
      }

      CModule module = mSprite.mModulesManager.Get(moduleIndex);

      Gdiplus::Bitmap* bitmap = mSprite.GetModuleBitmap(module);
      int transparentColor = mSprite.mImagesManager.GetItemById(module.GetImageId()).GetTransparentColor();

      if (bitmap)
      {
        if (module.GetWidth() != -1 && module.GetHeight() != -1)
        {
          // reimplemented using GDI+
          CPoint fmodulePos = fModule.GetPos();
          
          // translate and scale the fmodules position
          fmodulePos.mX = (float)(fmodulePos.mX - frameBoundRect.mX - frameBoundRect.mWidth / 2) * mAppConfig.mQuickFrameSize * scaleW / frameBoundRect.mWidth;
          fmodulePos.mY = (float)(fmodulePos.mY - frameBoundRect.mY - frameBoundRect.mHeight / 2) * mAppConfig.mQuickFrameSize * scaleH / frameBoundRect.mHeight;
          
          Gdiplus::Rect destRect = Gdiplus::Rect(
            i * mAppConfig.mQuickFrameSize + mAppConfig.mQuickFrameSize / 2 + fmodulePos.mX,
            2 + mAppConfig.mQuickFrameSize / 2 + fmodulePos.mY,
            (float)module.GetWidth() * mAppConfig.mQuickFrameSize * scaleW / frameBoundRect.mWidth,
            (float)module.GetHeight() * mAppConfig.mQuickFrameSize * scaleH / frameBoundRect.mHeight
          );

          Gdiplus::ImageAttributes imAtt;

          if (transparentColor == -1)
          {
          }
          else
          {
            imAtt.SetColorKey(
              Gdiplus::Color(GetRValue(transparentColor), GetGValue(transparentColor), GetBValue(transparentColor)),
              Gdiplus::Color(GetRValue(transparentColor), GetGValue(transparentColor), GetBValue(transparentColor)),
              Gdiplus::ColorAdjustTypeDefault);

          }

          graphics.DrawImage((Gdiplus::Image*)bitmap, destRect, module.GetX(), module.GetY(), module.GetWidth(), module.GetHeight(), Gdiplus::UnitPixel, &imAtt);


          /*
          Graphics::TBitmap* chunk = new Graphics::TBitmap();
          chunk->Width  = module.GetWidth();
          chunk->Height = module.GetHeight();

          chunk->Canvas->CopyRect(TRect(0, 0, module.GetWidth(), module.GetHeight()), bitmap->Canvas, TRect(module.GetX(), module.GetY(), module.GetX() + module.GetWidth(), module.GetY() + module.GetHeight()));

          CPoint fmodulePos = fModule.GetPos();

          // translate and scale the fmodules position
          fmodulePos.mX = (float)(fmodulePos.mX - frameBoundRect.mX - frameBoundRect.mWidth / 2) * mAppConfig.mQuickFrameSize * scaleW / frameBoundRect.mWidth;
          fmodulePos.mY = (float)(fmodulePos.mY - frameBoundRect.mY - frameBoundRect.mHeight / 2) * mAppConfig.mQuickFrameSize * scaleH / frameBoundRect.mHeight;

          TRect rect = TRect(
            i * mAppConfig.mQuickFrameSize + mAppConfig.mQuickFrameSize / 2 + fmodulePos.mX,
            2 + mAppConfig.mQuickFrameSize / 2 + fmodulePos.mY,
            i * mAppConfig.mQuickFrameSize + mAppConfig.mQuickFrameSize / 2 + fmodulePos.mX + (float)module.GetWidth() * mAppConfig.mQuickFrameSize * scaleW / frameBoundRect.mWidth,
            2 + mAppConfig.mQuickFrameSize / 2 + fmodulePos.mY + (float)module.GetHeight() * mAppConfig.mQuickFrameSize * scaleH / frameBoundRect.mHeight
            );

          //TRect rect = TRect(i*mAppConfig.mQuickFrameSize + mAppConfig.mQuickFrameSize / 2 - module.GetWidth() / 2, mAppConfig.mQuickFrameSize / 2 - module.GetHeight() / 2, i*mAppConfig.mQuickFrameSize + mAppConfig.mQuickModuleSize / 2 + module.GetWidth() / 2, mAppConfig.mQuickFrameSize / 2 + module.GetHeight() / 2);

          //if (module.GetWidth() > mAppConfig.mQuickFrameSize || module.GetHeight() > mAppConfig.mQuickFrameSize)
          //{
          //  rect = TRect(i*mAppConfig.mQuickFrameSize, 0, (i + 1)*mAppConfig.mQuickFrameSize, mAppConfig.mQuickFrameSize);
          //}

          chunk->Transparent      = true;
          chunk->TransparentMode  = tmFixed;
          chunk->TransparentColor = (TColor)(mSprite.mImagesManager.GetItemById(module.GetImageId()).GetTransparentColor());

          paintQuickFrames->Canvas->StretchDraw(rect , chunk);

          delete chunk;
          */

          //paintQuickModules->Canvas->CopyRect(TRect(i*64, 0, i*64 + module.GetWidth(), module.GetHeight()), bitmap->Canvas, TRect(module.GetX(), module.GetY(), module.GetX() + module.GetWidth(), module.GetY() + module.GetHeight()));
        }
      }
    }

    paintQuickFrames->Canvas->Font->Color = (TColor)mAppConfig.mQuickFramesIDTextColor;
    SelectObject(paintQuickFrames->Canvas->Handle, paintQuickFrames->Canvas->Font->Handle);
    SetBkMode(paintQuickFrames->Canvas->Handle, TRANSPARENT);

    AnsiString tmp = "0x" + AnsiString::IntToHex(frame.GetId(), 32);
    TSize textSize = paintQuickFrames->Canvas->TextExtent(tmp);
    paintQuickFrames->Canvas->TextOut(i*mAppConfig.mQuickFrameSize + mAppConfig.mQuickFrameSize / 2 - textSize.cx / 2, 2, tmp);

    tmp = frame.GetInfo().c_str();
    textSize = paintQuickFrames->Canvas->TextExtent(tmp);
    paintQuickFrames->Canvas->TextOut(i*mAppConfig.mQuickFrameSize + mAppConfig.mQuickFrameSize / 2 - textSize.cx / 2, mAppConfig.mQuickFrameSize - 2 - textSize.cy, tmp);

  }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::TabSheetAnimShow(TObject *Sender)
{
  SetQuickFramesScroll();
}
//---------------------------------------------------------------------------

void TFormMain::SetQuickFramesScroll()
{
  if (mSprite.mFramesManager.Size() < paintQuickFrames->Width / mAppConfig.mQuickFrameSize)
  {
    //scrollQuickFrames->Enabled = false;
    //paintQuickFrames->Enabled = false;
  }
  else
  {
    scrollQuickFrames->Enabled = true;
    paintQuickFrames->Enabled = true;
    scrollQuickFrames->Max = mSprite.mFramesManager.Size() - paintQuickFrames->Width / mAppConfig.mQuickFrameSize;
  }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::TimerAnimTimer(TObject *Sender)
{
  if (gridAnims->Row < gridAnims->RowCount - 1 && gridAFrames->RowCount > 2)
  {
    if (cbAnimsReverse->Checked)
    {
      // for some reason, if done like increasing the frame, it duplicates
      // the first row of the aframes grid :O
      int tmp = gridAFrames->Row;
      tmp--;

      if (tmp < 1 && cbAnimsLoop->Checked)
      {
        tmp = gridAFrames->RowCount - 2;
      }

      gridAFrames->Row = tmp;
    }
    else
    {
      gridAFrames->Row += 1;

      if (gridAFrames->Row >= gridAFrames->RowCount - 1 && cbAnimsLoop->Checked)
      {
        gridAFrames->Row = 1;
      }
    }

    paintAnimsPaint(Sender);
  }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::cbFPSChange(TObject *Sender)
{
  TimerAnim->Interval = 1000 / cbFPS->Value;
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::btnPlayClick(TObject *Sender)
{
  if (gridAnims->Row < gridAnims->RowCount - 1 && gridAFrames->Row < gridAFrames->RowCount - 1)
  {
    TimerAnim->Enabled = true;
    btnPlay->Enabled   = false;
    btnPause->Enabled  = true;
  }  
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::btnPauseClick(TObject *Sender)
{
  if (gridAnims->Row < gridAnims->RowCount - 1 && gridAFrames->Row < gridAFrames->RowCount - 1)
  {
    TimerAnim->Enabled = false;
    btnPlay->Enabled   = true;
    btnPause->Enabled  = false;
  }  
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::btnFastFwdClick(TObject *Sender)
{
  if (TimerAnim->Enabled)
  {
    if (gridAnims->Row < gridAnims->RowCount - 1)
    {
      CAnim anim = mSprite.mAnimsManager.Get(gridAnims->Row - 1);

      if (gridAFrames->Row < gridAFrames->RowCount - 1)
      {
        gridAFrames->Row = anim.mAFrames.Size();
      }
    }
  }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::btnRewindClick(TObject *Sender)
{
  if (TimerAnim->Enabled)
  {
    if (gridAnims->Row < gridAnims->RowCount - 1)
    {
      if (gridAFrames->Row < gridAFrames->RowCount - 1)
      {
        gridAFrames->Row = 1;
      }
    }
  }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::trackAnimsZoomChange(TObject *Sender)
{
  animsZoom = trackAnimsZoom->Position;

  paintAnimsPaint(this);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::cbAnimsAxisClick(TObject *Sender)
{
  paintAnimsPaint(this);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::cbAnimsGridClick(TObject *Sender)
{
  paintAnimsPaint(this);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::cbAnimsTransClick(TObject *Sender)
{
  paintAnimsPaint(this);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::cbAnimsFMOffsetClick(TObject *Sender)
{
  paintAnimsPaint(this);
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
//
//
//                      FRAME LOGIC EDITING
//
//
//---------------------------------------------------------------------------
void TFormMain::ArrangeFrameLogicPageItems()
{
  edFrameLogic->Top = GroupBoxFrameLogic->ClientHeight - edFrameLogic->Height - 4;
  gridFrameLogic->Height = edFrameLogic->Top - gridFrameLogic->Top - 4;

  edFrameLogicItem->Top = GroupBoxFrameLogicItem->ClientHeight - edFrameLogicItem->Height - 4;
  gridFrameLogicItems->Height = edFrameLogicItem->Top - gridFrameLogic->Top - 4;
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::SplitterFrameLogic1Moved(TObject *Sender)
{
  ArrangeFrameLogicPageItems();    
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::SplitterFrameLogic2Moved(TObject *Sender)
{
  ArrangeFrameLogicPageItems();    
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::SplitterFrameLogic3Moved(TObject *Sender)
{
  ArrangeFrameLogicPageItems();
}
//---------------------------------------------------------------------------

void TFormMain::FramesForLogicToGrid()
{
  gridFramesForLogic->RowCount = 2;
  gridFramesForLogic->Cells[0][0] = "Index";
  gridFramesForLogic->Cells[1][0] = "Id";
  gridFramesForLogic->Cells[2][0] = "FrameLogic";
  gridFramesForLogic->Cells[3][0] = "Info";

  for(int i = 0; i < mSprite.mFramesManager.Size(); i++)
  {
    gridFramesForLogic->Cells[0][i + 1] = AnsiString(i);
    gridFramesForLogic->Cells[1][i + 1] = "0x" + AnsiString::IntToHex(mSprite.mFramesManager.Get(i).GetId(), 32);
    gridFramesForLogic->Cells[2][i + 1] = mSprite.mFramesManager.Get(i).mFLogic.Size();
    gridFramesForLogic->Cells[3][i + 1] = mSprite.mFramesManager.Get(i).GetInfo().c_str();
    gridFramesForLogic->RowCount++;
  }

  if (mSprite.mFramesManager.Size() == 0)
  {
    gridFrameLogic->Enabled = false;
    edFrameLogic->Enabled   = false;
    gridFrameLogic->Color   = clScrollBar;
    edFrameLogic->Color     = clScrollBar;
  }
  else
  {
    gridFrameLogic->Enabled = true;
    edFrameLogic->Enabled   = true;
    gridFrameLogic->Color   = clWindow;
    edFrameLogic->Color     = clWindow;

    gridFramesForLogic->RowCount -= 1;
  }
}
//---------------------------------------------------------------------------

void TFormMain::FrameLogicToGrid()
{
  gridFrameLogic->RowCount = 2;
  gridFrameLogic->Cells[0][0] = "Index";
  gridFrameLogic->Cells[1][0] = "Items";
  gridFrameLogic->Cells[2][0] = "Info";

  if (gridFramesForLogic->Row <= gridFramesForLogic->RowCount && mSprite.mFramesManager.Size() > 0)
  {
    CFrame frame = mSprite.mFramesManager.Get(gridFramesForLogic->Row - 1);

    for(int i = 0; i < frame.mFLogic.Size(); i++)
    {
      CFrameLogic fLogic = frame.mFLogic.Get(i);

      gridFrameLogic->Cells[0][i + 1] = AnsiString(i);
      gridFrameLogic->Cells[1][i + 1] = AnsiString(fLogic.mItems.Size());
      gridFrameLogic->Cells[2][i + 1] = fLogic.GetInfo().c_str();
      gridFrameLogic->RowCount++;
    }

    if (frame.mFLogic.Size() == 0)
    {
      gridFrameLogicItems->Enabled = false;
      edFrameLogicItem->Enabled    = false;
      gridFrameLogicItems->Color   = clScrollBar;
      edFrameLogicItem->Color      = clScrollBar;
    }
    else
    {
      gridFrameLogicItems->Enabled = true;
      edFrameLogicItem->Enabled    = true;
      gridFrameLogicItems->Color   = clWindow;
      edFrameLogicItem->Color      = clWindow;
    }
  }
  else
  {
    gridFrameLogicItems->Enabled = false;
    edFrameLogicItem->Enabled    = false;
    gridFrameLogicItems->Color   = clScrollBar;
    edFrameLogicItem->Color      = clScrollBar;
  }

  gridFrameLogic->Cells[0][gridFrameLogic->RowCount - 1] = "new...";

  for(int i = 1; i < 3; i++)
  {
    gridFrameLogic->Cells[i][gridFrameLogic->RowCount - 1] = "";
  }
}
//---------------------------------------------------------------------------

void TFormMain::FrameLogicItemsToGrid()
{
  gridFrameLogicItems->RowCount = 2;
  gridFrameLogicItems->Cells[0][0] = "Index";
  gridFrameLogicItems->Cells[1][0] = "Type";
  gridFrameLogicItems->Cells[2][0] = "Param";
  
  if (gridFramesForLogic->Row <= gridFramesForLogic->RowCount && mSprite.mFramesManager.Size() > 0)
  {
    CFrame frame = mSprite.mFramesManager.Get(gridFramesForLogic->Row - 1);

    if (gridFrameLogic->Row < gridFrameLogic->RowCount - 1 && gridFrameLogic->RowCount > 1)
    {
      CFrameLogic fLogic = frame.mFLogic.Get(gridFrameLogic->Row - 1);

      for(int i = 0; i < fLogic.mItems.Size(); i++)
      {
        CFrameLogicItem fLogicItem = fLogic.mItems.Get(i);

        gridFrameLogicItems->Cells[0][i + 1] = AnsiString(i);
        gridFrameLogicItems->Cells[1][i + 1] = (fLogicItem.GetType() == FRAME_LOGIC_ITEM_POINT) ? "POINT" : "RECT";
        gridFrameLogicItems->Cells[2][i + 1] = (fLogicItem.GetType() == FRAME_LOGIC_ITEM_POINT) ?
          AnsiString("(") + fLogicItem.mX + ", " + fLogicItem.mY + ")" :
          AnsiString("(") + fLogicItem.mX + ", " + fLogicItem.mY + ", " + fLogicItem.mWidth + ", " + fLogicItem.mHeight + ")";
        gridFrameLogicItems->RowCount++;
      }
    }
  }

  gridFrameLogicItems->Cells[0][gridFrameLogicItems->RowCount - 1] = "new...";

  for(int i = 1; i < 3; i++)
  {
    gridFrameLogicItems->Cells[i][gridFrameLogicItems->RowCount - 1] = "";
  }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::gridFrameLogicDblClick(TObject *Sender)
{
  if (gridFrameLogic->Row == gridFrameLogic->RowCount - 1)
  {
    gridFrameLogicPopNew(Sender);
  }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::gridFrameLogicPopNew(TObject *Sender)
{
  CFrame frame = mSprite.mFramesManager.Get(gridFramesForLogic->Row - 1);

  int fLogicIndex = frame.mFLogic.New();

  mSprite.mFramesManager.Set(gridFramesForLogic->Row - 1, frame);

  int frameRow = gridFramesForLogic->Row;

  FrameLogicToGrid();
  FramesForLogicToGrid();

  gridFramesForLogic->Row = frameRow;
  gridFrameLogic->Row     = fLogicIndex + 1;

  gridFrameLogicClick(this);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::gridFrameLogicClick(TObject *Sender)
{
  FrameLogicItemsToGrid();

  if (gridFrameLogic->Row == gridFrameLogic->RowCount - 1)
  {
    edFrameLogic->Text = "";
    return;
  }

  CFrame frame = mSprite.mFramesManager.Get(gridFramesForLogic->Row - 1);
  CFrameLogic fLogic = frame.mFLogic.Get(gridFrameLogic->Row - 1);

  edFrameLogic->Text =
    AnsiString("info=\"") + AnsiString(fLogic.GetInfo().c_str()) + "\""
    ;

  paintFrameLogicPaint(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::gridFramesForLogicClick(TObject *Sender)
{
  FrameLogicToGrid();
  //FrameLogicItemsToGrid();

  paintFrameLogicPaint(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::gridFrameLogicPopClone(TObject *Sender)
{
  if (gridFrameLogic->Row < gridFrameLogic->RowCount - 1)
  {
    CFrame frame = mSprite.mFramesManager.Get(gridFramesForLogic->Row - 1);

    int row = frame.mFLogic.Clone(gridFrameLogic->Row - 1);

    mSprite.mFramesManager.Set(gridFramesForLogic->Row - 1, frame);

    int frameRow = gridFramesForLogic->Row;

    FrameLogicToGrid();
    FramesForLogicToGrid();

    gridFramesForLogic->Row   = frameRow;
    gridFrameLogic->Row = row + 1;

    gridFrameLogicClick(this);
  }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::gridFrameLogicPopDelete(TObject *Sender)
{
  // TODO: test
  if (gridFrameLogic->Row < gridFrameLogic->RowCount - 1)
  {
    CFrame frame = mSprite.mFramesManager.Get(gridFramesForLogic->Row - 1);

    int row = frame.mFLogic.Delete(gridFrameLogic->Row - 1);
    mSprite.mFramesManager.Set(gridFramesForLogic->Row - 1, frame);

    FramesForLogicToGrid();
    FrameLogicToGrid();
    FrameLogicItemsToGrid();

    if (row != -1)
    {
      gridFrameLogic->Row = row + 1;
    }
    
    gridFrameLogicClick(this);
  }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::gridFrameLogicPopInsert(TObject *Sender)
{
  if (gridFrameLogic->Row < gridFrameLogic->RowCount - 1)
  {
    CFrame frame = mSprite.mFramesManager.Get(gridFramesForLogic->Row - 1);

    int row = frame.mFLogic.Insert(gridFrameLogic->Row - 1, CFrameLogic());

    mSprite.mFramesManager.Set(gridFramesForLogic->Row - 1, frame);

    int frameRow = gridFramesForLogic->Row;

    FrameLogicToGrid();
    FramesForLogicToGrid();

    gridFramesForLogic->Row   = frameRow;
    gridFrameLogic->Row = row + 1;

    gridFrameLogicClick(this);
  }
}
//---------------------------------------------------------------------------


void __fastcall TFormMain::edFrameLogicExit(TObject *Sender)
{
  if (edFrameLogic->Text == "")
  {
    return;
  }

  // if one attribute is wrong, is ignored, all the others that are correct,
  // will be set

  // get the entry that is edited
  CFrame frame = mSprite.mFramesManager.Get(gridFramesForLogic->Row - 1);
  CFrameLogic fLogic = frame.mFLogic.Get(gridFrameLogic->Row - 1);

  // parse the new attributes through tinyxml
  tinyxml2::XMLDocument doc;

  doc.Parse( UTF8Encode("<element " + edFrameLogic->Text + "/>").c_str() );

  auto elem = doc.FirstChildElement("element");

  int result;

  //
  // Parse string attributes
  //
  const char* info = elem->Attribute("info");

  if (info)
  {
    fLogic.SetInfo(info);
  }

  int oldRow = gridFrameLogic->Row;

  frame.mFLogic.Set(gridFrameLogic->Row - 1, fLogic);
  mSprite.mFramesManager.Set(gridFramesForLogic->Row - 1, frame);

  edFrameLogic->Text =
    UnicodeString("info=\"") + UTF8ToString(fLogic.GetInfo().c_str()) + "\""
    ;

  FrameLogicToGrid();

  gridFrameLogic->Row = oldRow;

  paintFrameLogicPaint(this);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::gridFrameLogicPopMoveDown(TObject *Sender)
{
  if (gridFrameLogic->Row < gridFrameLogic->RowCount - 1)
  {
    CFrame frame = mSprite.mFramesManager.Get(gridFramesForLogic->Row - 1);

    int row = frame.mFLogic.MoveDown(gridFrameLogic->Row - 1);
    mSprite.mFramesManager.Set(gridFramesForLogic->Row - 1, frame);

    FrameLogicToGrid();

    if (row != -1)
    {
      gridFrameLogic->Row = row + 1;
      gridFrameLogicClick(this);
    }
  }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::gridFrameLogicPopMoveUp(TObject *Sender)
{
  if (gridFrameLogic->Row < gridFrameLogic->RowCount - 1)
  {
    CFrame frame = mSprite.mFramesManager.Get(gridFramesForLogic->Row - 1);

    int row = frame.mFLogic.MoveUp(gridFrameLogic->Row - 1);
    mSprite.mFramesManager.Set(gridFramesForLogic->Row - 1, frame);

    FrameLogicToGrid();

    if (row != -1)
    {
      gridFrameLogic->Row = row + 1;
      gridFrameLogicClick(this);
    }
  }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::edFrameLogicKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
  if (Key == VK_RETURN)
  {
    edFrameLogicExit(Sender);
  }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::gridFrameLogicItemsClick(TObject *Sender)
{
  if (gridFrameLogicItems->Row == gridFrameLogicItems->RowCount - 1)
  {
    edFrameLogicItem->Text = "";
    return;
  }

  CFrame          frame      = mSprite.mFramesManager.Get(gridFramesForLogic->Row - 1);
  CFrameLogic     fLogic     = frame.mFLogic.Get(gridFrameLogic->Row - 1);
  CFrameLogicItem fLogicItem = fLogic.mItems.Get(gridFrameLogicItems->Row - 1);

  edFrameLogicItem->Text =
    AnsiString("type=\"") + ((fLogicItem.GetType() == FRAME_LOGIC_ITEM_POINT) ? "POINT" : "RECT") + "\" " +
    AnsiString("x=") + fLogicItem.mX + " " +
    AnsiString("y=") + fLogicItem.mY + " "
    ;

  if (fLogicItem.GetType() == FRAME_LOGIC_ITEM_RECT)
  {
    edFrameLogicItem->Text =
    edFrameLogicItem->Text + " " +
    AnsiString("width=") + fLogicItem.mWidth + " " +
    AnsiString("height=") + fLogicItem.mHeight;
  }

  paintFrameLogicPaint(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::gridFrameLogicItemsPopNew(TObject *Sender)
{
  CFrame          frame      = mSprite.mFramesManager.Get(gridFramesForLogic->Row - 1);
  CFrameLogic     fLogic     = frame.mFLogic.Get(gridFrameLogic->Row - 1);

  int fLogicItemIndex = fLogic.mItems.New();

  frame.mFLogic.Set(gridFrameLogic->Row - 1, fLogic);
  mSprite.mFramesManager.Set(gridFramesForLogic->Row - 1, frame);

  // TODO
  int fLogicRow = gridFrameLogic->Row;

  FrameLogicItemsToGrid();
  FrameLogicToGrid();

  gridFrameLogic->Row      = fLogicRow;
  gridFrameLogicItems->Row = fLogicItemIndex + 1;

  gridFrameLogicItemsClick(this);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::gridFrameLogicItemsDblClick(TObject *Sender)
{
  if (gridFrameLogicItems->Row == gridFrameLogicItems->RowCount - 1)
  {
    gridFrameLogicItemsPopNew(Sender);
  }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::gridFrameLogicItemsPopDelete(TObject *Sender)
{
  if (gridFrameLogicItems->Row < gridFrameLogicItems->RowCount - 1)
  {
    CFrame frame       = mSprite.mFramesManager.Get(gridFramesForLogic->Row - 1);
    CFrameLogic fLogic = frame.mFLogic.Get(gridFrameLogic->Row - 1);

    int fLogicItemRow = fLogic.mItems.Delete(gridFrameLogicItems->Row - 1);

    frame.mFLogic.Set(gridFrameLogic->Row - 1, fLogic);
    mSprite.mFramesManager.Set(gridFramesForLogic->Row - 1, frame);

    FrameLogicItemsToGrid();
    FrameLogicToGrid();

    if (fLogicItemRow != -1)
    {
      gridFrameLogicItems->Row = fLogicItemRow + 1;
    }

    gridFrameLogicItemsClick(this);
  }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::gridFrameLogicItemsPopInsert(TObject *Sender)
{
  if (gridFrameLogicItems->Row < gridFrameLogicItems->RowCount - 1)
  {
    CFrame frame       = mSprite.mFramesManager.Get(gridFramesForLogic->Row - 1);
    CFrameLogic fLogic = frame.mFLogic.Get(gridFrameLogic->Row - 1);

    int fLogicItemRow = fLogic.mItems.Insert(gridFrameLogicItems->Row - 1, CFrameLogicItem());

    frame.mFLogic.Set(gridFrameLogic->Row - 1, fLogic);
    mSprite.mFramesManager.Set(gridFramesForLogic->Row - 1, frame);

    int fLogicRow = gridFrameLogic->Row;

    FrameLogicItemsToGrid();
    FrameLogicToGrid();

    gridFrameLogic->Row      = fLogicRow;
    gridFrameLogicItems->Row = fLogicItemRow + 1;

    gridFrameLogicItemsClick(this);
  }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::gridFrameLogicItemsPopMoveDown(TObject *Sender)
{
  if (gridFrameLogicItems->Row < gridFrameLogicItems->RowCount - 1)
  {
    CFrame frame       = mSprite.mFramesManager.Get(gridFramesForLogic->Row - 1);
    CFrameLogic fLogic = frame.mFLogic.Get(gridFrameLogic->Row - 1);

    int fLogicItemRow = fLogic.mItems.MoveDown(gridFrameLogicItems->Row - 1);

    frame.mFLogic.Set(gridFrameLogic->Row - 1, fLogic);
    mSprite.mFramesManager.Set(gridFramesForLogic->Row - 1, frame);

    FrameLogicItemsToGrid();

    if (fLogicItemRow != -1)
    {
      gridFrameLogicItems->Row = fLogicItemRow + 1;
      gridFrameLogicItemsClick(this);
    }
  }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::gridFrameLogicItemsPopMoveUp(TObject *Sender)
{
  if (gridFrameLogicItems->Row < gridFrameLogicItems->RowCount - 1)
  {
    CFrame frame       = mSprite.mFramesManager.Get(gridFramesForLogic->Row - 1);
    CFrameLogic fLogic = frame.mFLogic.Get(gridFrameLogic->Row - 1);

    int fLogicItemRow = fLogic.mItems.MoveUp(gridFrameLogicItems->Row - 1);

    frame.mFLogic.Set(gridFrameLogic->Row - 1, fLogic);
    mSprite.mFramesManager.Set(gridFramesForLogic->Row - 1, frame);

    FrameLogicItemsToGrid();

    if (fLogicItemRow != -1)
    {
      gridFrameLogicItems->Row = fLogicItemRow + 1;
      gridFrameLogicItemsClick(this);
    }
  }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::gridFrameLogicItemsPopClone(TObject *Sender)
{
  if (gridFrameLogicItems->Row < gridFrameLogicItems->RowCount - 1)
  {
    CFrame frame       = mSprite.mFramesManager.Get(gridFramesForLogic->Row - 1);
    CFrameLogic fLogic = frame.mFLogic.Get(gridFrameLogic->Row - 1);

    int fLogicItemRow = fLogic.mItems.Clone(gridFrameLogicItems->Row - 1);

    frame.mFLogic.Set(gridFrameLogic->Row - 1, fLogic);
    mSprite.mFramesManager.Set(gridFramesForLogic->Row - 1, frame);

    int fLogicRow = gridFrameLogic->Row;

    FrameLogicItemsToGrid();
    FrameLogicToGrid();

    gridFrameLogic->Row      = fLogicRow;
    gridFrameLogicItems->Row = fLogicItemRow + 1;

    gridFrameLogicItemsClick(this);
  }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::edFrameLogicItemExit(TObject *Sender)
{
  if (edFrameLogicItem->Text == "")
  {
    return;
  }

  // if one attribute is wrong, is ignored

  // get the entry that is edited
  CFrame frame = mSprite.mFramesManager.Get(gridFramesForLogic->Row - 1);
  CFrameLogic fLogic = frame.mFLogic.Get(gridFrameLogic->Row - 1);
  CFrameLogicItem fLogicItem = fLogic.mItems.Get(gridFrameLogicItems->Row - 1);

  // parse the new attributes through tinyxml
  tinyxml2::XMLDocument doc;

  doc.Parse( UTF8Encode("<element " + edFrameLogicItem->Text + "/>").c_str());

  auto elem = doc.FirstChildElement("element");

  int result;

  //
  // Parse string attributes
  //
  const char* type = elem->Attribute("type");

  int x, y, width, height;

  if (type)
  {
    if (std::string(type) == "RECT")
    {
      fLogicItem.SetType(FRAME_LOGIC_ITEM_RECT);

      result = elem->QueryIntAttribute("w", &width);

      if (result == XML_SUCCESS)
      {
        fLogicItem.mWidth = width;
      }

      result = elem->QueryIntAttribute("h", &height);

      if (result == XML_SUCCESS)
      {
        fLogicItem.mHeight = height;
      }
    }

    if (std::string(type) == "POINT")
    {
      fLogicItem.SetType(FRAME_LOGIC_ITEM_POINT);
    }

    result = elem->QueryIntAttribute("x", &x);

    if (result == XML_SUCCESS)
    {
      fLogicItem.mX = x;
    }

    result = elem->QueryIntAttribute("y", &y);

    if (result == XML_SUCCESS)
    {
      fLogicItem.mY = y;
    }
  }

  int oldRow = gridFrameLogicItems->Row;

  fLogic.mItems.Set(gridFrameLogicItems->Row - 1, fLogicItem);
  frame.mFLogic.Set(gridFrameLogic->Row - 1, fLogic);
  mSprite.mFramesManager.Set(gridFramesForLogic->Row - 1, frame);

  // same code as in
  edFrameLogicItem->Text =
    UnicodeString("type=\"") + ((fLogicItem.GetType() == FRAME_LOGIC_ITEM_POINT) ? "POINT" : "RECT") + "\" " +
    UnicodeString("x=") + fLogicItem.mX + " " +
    UnicodeString("y=") + fLogicItem.mY + " "
    ;

  if (fLogicItem.GetType() == FRAME_LOGIC_ITEM_RECT)
  {
    edFrameLogicItem->Text =
    edFrameLogicItem->Text + " " +
    AnsiString("width=") + fLogicItem.mWidth + " " +
    AnsiString("height=") + fLogicItem.mHeight;
  }

  FrameLogicItemsToGrid();

  gridFrameLogicItems->Row = oldRow;

  paintFrameLogicPaint(this);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::edFrameLogicItemKeyDown(TObject *Sender,
      WORD &Key, TShiftState Shift)
{
  if (Key == VK_RETURN)
  {
    edFrameLogicItemExit(Sender);
  }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::paintFrameLogicPaint(TObject *Sender)
{
  // make clean
  paintFrameLogic->Canvas->Brush->Style = bsClear;
  paintFrameLogic->Canvas->Brush->Color = (TColor)mAppConfig.mFrameLogicBackgroundColor;
  paintFrameLogic->Canvas->FillRect(paintFrameLogic->ClientRect);

  // set the line style
  paintFrameLogic->Canvas->Pen->Color = (TColor)0x0000FF;
  paintFrameLogic->Canvas->Pen->Style = psDash;

  if (gridFramesForLogic->Row <= gridFramesForLogic->RowCount && mSprite.mFramesManager.Size() > 0)
  {
    CFrame frame = mSprite.mFramesManager.Get(gridFramesForLogic->Row - 1);

    Gdiplus::Graphics graphics(paintFrameLogic->Canvas->Handle);

    graphics.SetSmoothingMode( Gdiplus::SmoothingModeNone );
    graphics.SetPixelOffsetMode( Gdiplus::PixelOffsetModeHalf );
    graphics.SetInterpolationMode( Gdiplus::InterpolationModeNearestNeighbor );
    
    //Graphics::TBitmap* chunk = new Graphics::TBitmap();

    for(int i = 0; i < frame.mFModules.Size(); i++)
    {
      CFrameModule fmodule = frame.mFModules.Get(i);

      CModule module = mSprite.mModulesManager.GetItemById(fmodule.GetModuleId());

      if (module.GetId() == -1)
      {
        continue;
      }

      Gdiplus::Bitmap* bitmap = mSprite.GetModuleBitmap(module);
      
      int transparentColor = mSprite.mImagesManager.GetItemById(module.GetImageId()).GetTransparentColor();

      if (bitmap)
      {
        if (module.GetWidth() != -1 && module.GetHeight() != -1)
        {
          // reimplemented using GDI+
          CPoint posOffset = (cbFramesOffset->Checked) ? fmodule.GetPos() : CPoint(0, 0);

          Gdiplus::Rect destRect = Gdiplus::Rect(
              paintFrameLogic->Width  / 2 - fLogicPan.x + posOffset.mX * fLogicZoom / 100,
              paintFrameLogic->Height / 2 - fLogicPan.y + posOffset.mY * fLogicZoom / 100,
              module.GetWidth() * fLogicZoom / 100,
              module.GetHeight() * fLogicZoom / 100
              );

          Gdiplus::ImageAttributes imAtt;

          if (transparentColor == -1 || !cbFrameLogicTrans->Checked)
          {
          }
          else
          {
            imAtt.SetColorKey(
              Gdiplus::Color(GetRValue(transparentColor), GetGValue(transparentColor), GetBValue(transparentColor)),
              Gdiplus::Color(GetRValue(transparentColor), GetGValue(transparentColor), GetBValue(transparentColor)),
              Gdiplus::ColorAdjustTypeDefault);

          }

          graphics.DrawImage((Gdiplus::Image*)bitmap, destRect, module.GetX(), module.GetY(), module.GetWidth(), module.GetHeight(), Gdiplus::UnitPixel, &imAtt);
          
          /*
          chunk->Width  = module.GetWidth();
          chunk->Height = module.GetHeight();

          chunk->Canvas->CopyRect(TRect(0, 0, module.GetWidth(), module.GetHeight()), bitmap->Canvas, TRect(module.GetX(), module.GetY(), module.GetX() + module.GetWidth(), module.GetY() + module.GetHeight()));

          CPoint posOffset = (cbFramesOffset->Checked) ? fmodule.GetPos() : CPoint(0, 0);

          TRect rect = TRect(
              paintFrameLogic->Width  / 2 - fLogicPan.x + posOffset.mX * fLogicZoom / 100,
              paintFrameLogic->Height / 2 - fLogicPan.y + posOffset.mY * fLogicZoom / 100,
              paintFrameLogic->Width  / 2 - fLogicPan.x + (posOffset.mX + module.GetWidth()) * fLogicZoom / 100,
              paintFrameLogic->Height / 2 - fLogicPan.y + (posOffset.mY + module.GetHeight()) * fLogicZoom / 100
              );

          chunk->Transparent      = cbFramesTrans->Checked;
          chunk->TransparentMode  = tmFixed;
          chunk->TransparentColor = (TColor)(mSprite.mImagesManager.GetItemById(module.GetImageId()).GetTransparentColor());

          paintFrameLogic->Canvas->StretchDraw(rect , chunk);
          */
        }
      }
    }

    //delete chunk;

    // paint the logic items (points and rects)
    if ( gridFrameLogic->Row < gridFrameLogic->RowCount - 1 &&
         gridFrameLogicItems->Row < gridFrameLogicItems->RowCount - 1)
    {
      CFrameLogic fLogic = frame.mFLogic.Get(gridFrameLogic->Row - 1);

      for(int i = 0; i < fLogic.mItems.Size(); i++)
      {
        CFrameLogicItem fLogicItem = fLogic.mItems.Get(i);

        if (fLogicItem.GetType() == FRAME_LOGIC_ITEM_POINT)
        {
          TRect rect = TRect(
              paintFrameLogic->Width  / 2 - fLogicPan.x + fLogicItem.mX * fLogicZoom / 100,
              paintFrameLogic->Height / 2 - fLogicPan.y + fLogicItem.mY * fLogicZoom / 100,
              paintFrameLogic->Width  / 2 - fLogicPan.x + (fLogicItem.mX + 1) * fLogicZoom / 100,
              paintFrameLogic->Height / 2 - fLogicPan.y + (fLogicItem.mY + 1) * fLogicZoom / 100
              );

          if (i == gridFrameLogicItems->Row - 1)
          {
            if (fLogicMouseMode == MMM_DO_DEFINE)
            {
              rect.left   = fLogicCurrentDefine.x/* + paintFrameLogic->Width  / 2 - fLogicPan.x*/;
              rect.top    = fLogicCurrentDefine.y/* + paintFrameLogic->Height / 2 - fLogicPan.y*/;
              rect.right  = rect.left + fLogicZoom / 100;
              rect.bottom = rect.top + fLogicZoom / 100;
            }
          }

          paintFrameLogic->Canvas->Brush->Color = (TColor)mAppConfig.mFrameLogicGridPointColor;
          paintFrameLogic->Canvas->FillRect(rect);
        }

        if (fLogicItem.GetType() == FRAME_LOGIC_ITEM_RECT)
        {
          int baseX     = paintFrameLogic->Width  / 2 - fLogicPan.x;
          int baseY     = paintFrameLogic->Height / 2 - fLogicPan.y;
          int newX      = fLogicItem.mX * fLogicZoom / 100;
          int newY      = fLogicItem.mY * fLogicZoom / 100;
          int newWidth  = fLogicItem.mWidth  * fLogicZoom / 100;
          int newHeight = fLogicItem.mHeight * fLogicZoom / 100;

          if (i == gridFrameLogicItems->Row - 1)
          {
            if (fLogicMouseMode == MMM_DO_DEFINE)
            {
              newX     = (int)std::min(fLogicDefine.x, fLogicCurrentDefine.x) - paintFrameLogic->Width  / 2 + fLogicPan.x;
              newY     = (int)std::min(fLogicDefine.y, fLogicCurrentDefine.y) - paintFrameLogic->Height  / 2 + fLogicPan.y;
              newWidth  = abs(-fLogicDefine.x + fLogicCurrentDefine.x);
              newHeight = abs(-fLogicDefine.y + fLogicCurrentDefine.y);
            }
            else if (fLogicMouseMode == MMM_MOVE)
            {
               newX      = fLogicItem.mX * fLogicZoom / 100 + (-fLogicDefine.x + fLogicCurrentDefine.x);
               newY      = fLogicItem.mY * fLogicZoom / 100 + (-fLogicDefine.y + fLogicCurrentDefine.y);
               newWidth  = fLogicItem.mWidth  * fLogicZoom / 100;
               newHeight = fLogicItem.mHeight * fLogicZoom / 100;
            }
            else if (fLogicMouseMode == MMM_RESIZE_RIGHT_BOTTOM)
            {
              int oldX = fLogicItem.mX * fLogicZoom / 100;
              int oldY = fLogicItem.mY * fLogicZoom / 100;
              newX     = (fLogicItem.mX + fLogicItem.mWidth) * fLogicZoom / 100 + (-fLogicDefine.x + fLogicCurrentDefine.x);
              newY     = (fLogicItem.mY + fLogicItem.mHeight) * fLogicZoom / 100 + (-fLogicDefine.y + fLogicCurrentDefine.y);

              newWidth  = abs(newX - oldX);
              newHeight = abs(newY - oldY);
              newX      = (int)std::min(oldX, newX);
              newY      = (int)std::min(oldY, newY);
            }
            else if (fLogicMouseMode == MMM_RESIZE_LEFT_TOP)
            {
              int oldX = (fLogicItem.mX + fLogicItem.mWidth ) * fLogicZoom / 100;
              int oldY = (fLogicItem.mY + fLogicItem.mHeight ) * fLogicZoom / 100;
              newX     = fLogicItem.mX * fLogicZoom / 100 + (-fLogicDefine.x + fLogicCurrentDefine.x);
              newY     = fLogicItem.mY * fLogicZoom / 100 + (-fLogicDefine.y + fLogicCurrentDefine.y);

              newWidth  = abs(newX - oldX);
              newHeight = abs(newY - oldY);
              newX      = (int)std::min(oldX, newX);
              newY      = (int)std::min(oldY, newY);
            }
            else if (fLogicMouseMode == MMM_RESIZE_RIGHT_TOP)
            {
              int oldX = fLogicItem.mX * fLogicZoom / 100;
              int oldY = (fLogicItem.mY + fLogicItem.mHeight ) * fLogicZoom / 100;

              newX     = (fLogicItem.mX + fLogicItem.mWidth) * fLogicZoom / 100 + (-fLogicDefine.x + fLogicCurrentDefine.x);
              newY     = fLogicItem.mY * fLogicZoom / 100 + (-fLogicDefine.y + fLogicCurrentDefine.y);

              newWidth  = abs(newX - oldX);
              newHeight = abs(newY - oldY);
              newX      = (int)std::min(oldX, newX);
              newY      = (int)std::min(oldY, newY);
            }
            else if (fLogicMouseMode == MMM_RESIZE_LEFT_BOTTOM)
            {
              int oldX = (fLogicItem.mX + fLogicItem.mWidth) * fLogicZoom / 100;
              int oldY = fLogicItem.mY * fLogicZoom / 100;

              newX     = fLogicItem.mX * fLogicZoom / 100 + (-fLogicDefine.x + fLogicCurrentDefine.x);
              newY     = (fLogicItem.mY + fLogicItem.mHeight) * fLogicZoom / 100 + (-fLogicDefine.y + fLogicCurrentDefine.y);

              newWidth  = abs(newX - oldX);
              newHeight = abs(newY - oldY);
              newX      = (int)std::min(oldX, newX);
              newY      = (int)std::min(oldY, newY);
            }
            else if (fLogicMouseMode == MMM_RESIZE_RIGHT)
            {
              int oldX = fLogicItem.mX * fLogicZoom / 100;
              newX     = (fLogicItem.mX + fLogicItem.mWidth) * fLogicZoom / 100 + (-fLogicDefine.x + fLogicCurrentDefine.x);
              
              newWidth  = abs(newX - oldX);
              newX      = (int)std::min(oldX, newX);
            }
            else if (fLogicMouseMode == MMM_RESIZE_LEFT)
            {
              int oldX = (fLogicItem.mX + fLogicItem.mWidth) * fLogicZoom / 100;
              newX     = fLogicItem.mX * fLogicZoom / 100 + (-fLogicDefine.x + fLogicCurrentDefine.x);
              
              newWidth  = abs(newX - oldX);
              newX      = (int)std::min(oldX, newX);
            }
            else if (fLogicMouseMode == MMM_RESIZE_BOTTOM)
            {
              int oldY  = fLogicItem.mY * fLogicZoom / 100;
              newY      = (fLogicItem.mY + fLogicItem.mHeight) * fLogicZoom / 100 + (-fLogicDefine.y + fLogicCurrentDefine.y);

              newHeight = abs(newY - oldY);
              newY      = (int)std::min(oldY, newY);
            }
            else if (fLogicMouseMode == MMM_RESIZE_TOP)
            {
              int oldY  = (fLogicItem.mY + fLogicItem.mHeight) * fLogicZoom / 100;
              newY      = fLogicItem.mY * fLogicZoom / 100 + (-fLogicDefine.y + fLogicCurrentDefine.y);

              newHeight = abs(newY - oldY);
              newY      = (int)std::min(oldY, newY);
            }
          }

          paintFrameLogic->Canvas->Brush->Color = (TColor)mAppConfig.mFrameLogicGridRectColor;
          paintFrameLogic->Canvas->FrameRect(TRect(baseX + newX, baseY + newY, baseX + newX + newWidth, baseY + newY + newHeight));
        }
      }
    }
  }

  // draw the canvas embelishments (axis, grid and so on)
  if (cbFrameLogicAxis->Checked && !cbFrameLogicGrid->Checked)
  {
    paintFrameLogic->Canvas->Pen->Style = psSolid;
    paintFrameLogic->Canvas->Pen->Color = (TColor)mAppConfig.mFrameLogicAxisColor;

    paintFrameLogic->Canvas->MoveTo(paintFrameLogic->Width / 2 - fLogicPan.x, 0);
    paintFrameLogic->Canvas->LineTo(paintFrameLogic->Width / 2 - fLogicPan.x, paintFrameLogic->Height);

    paintFrameLogic->Canvas->MoveTo(0, paintFrameLogic->Height / 2 - fLogicPan.y);
    paintFrameLogic->Canvas->LineTo(paintFrameLogic->Width, paintFrameLogic->Height / 2 - fLogicPan.y);
  }

  if (cbFrameLogicGrid->Checked)
  {
    paintFrameLogic->Canvas->Pen->Style = psSolid;
    paintFrameLogic->Canvas->Pen->Color = (TColor)mAppConfig.mFrameLogicGridColor;

    for(int x = fLogicPan.x % mAppConfig.mFrameLogicGridXSpacing; x < paintFrameLogic->Width; x += mAppConfig.mFrameLogicGridXSpacing * fLogicZoom / 100)
    {
      paintFrameLogic->Canvas->MoveTo(x, 0);
      paintFrameLogic->Canvas->LineTo(x, paintFrameLogic->Height);
    }

    for(int y = fLogicPan.y % mAppConfig.mFrameLogicGridYSpacing; y < paintFrameLogic->Height; y += mAppConfig.mFrameLogicGridYSpacing * fLogicZoom / 100)
    {
      paintFrameLogic->Canvas->MoveTo(0, y);
      paintFrameLogic->Canvas->LineTo(paintFrameLogic->Width, y);
    }
  }
}
//---------------------------------------------------------------------------


void __fastcall TFormMain::paintFrameLogicMouseMove(TObject *Sender,
      TShiftState Shift, int X, int Y)
{
  //ActiveControl = GroupBoxFrameLogicCanvas;

  if (fLogicMouseMode == MMM_DO_PAN)
  {
    fLogicPan.x += -X + oldFLogicPan.x;
    fLogicPan.y += -Y + oldFLogicPan.y;

    oldFLogicPan.x = X;
    oldFLogicPan.y = Y;

    paintFrameLogicPaint(Sender);

    return;
  }

  if (fLogicMouseMode == MMM_DO_DEFINE)
  {
    fLogicCurrentDefine.x = X;
    fLogicCurrentDefine.y = Y;

    paintFrameLogicPaint(Sender);

    return;
  }

  if (fLogicMouseMode == MMM_MOVE ||
      fLogicMouseMode == MMM_RESIZE_RIGHT_BOTTOM ||
      fLogicMouseMode == MMM_RESIZE_LEFT_TOP ||
      fLogicMouseMode == MMM_RESIZE_LEFT_BOTTOM ||
      fLogicMouseMode == MMM_RESIZE_RIGHT_TOP)
  {
    fLogicCurrentDefine.x = X;
    fLogicCurrentDefine.y = Y;

    paintFrameLogicPaint(Sender);

    return;
  }

  if (fLogicMouseMode == MMM_RESIZE_RIGHT ||
      fLogicMouseMode == MMM_RESIZE_LEFT)
  {
    fLogicCurrentDefine.x = X;

    paintFrameLogicPaint(this);

    return;
  }

  if (fLogicMouseMode == MMM_RESIZE_TOP ||
      fLogicMouseMode == MMM_RESIZE_BOTTOM)
  {
    fLogicCurrentDefine.y = Y;

    paintFrameLogicPaint(this);

    return;
  }

  fLogicAvailableMouseMode = GetFLogicMouseMode(TPoint(X, Y));

  if (fLogicAvailableMouseMode != MMM_NONE)
  {
    paintFrameLogic->Cursor = GetModuleCursor(fLogicAvailableMouseMode);
  }
  else
  {
    paintFrameLogic->Cursor = GetModuleCursor(fLogicMouseMode);
  }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::cbFrameLogicAxisClick(TObject *Sender)
{
  paintFrameLogicPaint(this);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::cbFrameLogicGridClick(TObject *Sender)
{
  paintFrameLogicPaint(this);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::cbFrameLogicTransClick(TObject *Sender)
{
  paintFrameLogicPaint(this);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::cbFrameLogicOffsetFMClick(TObject *Sender)
{
  paintFrameLogicPaint(this);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::trackFLogicZoomChange(TObject *Sender)
{
  fLogicZoom = trackFLogicZoom->Position;

  paintFrameLogicPaint(this);
}
//---------------------------------------------------------------------------

TMouseMode TFormMain::GetFLogicMouseMode(TPoint p)
{
  TMouseMode mode = MMM_NONE;

  // check for the indexes to be in range
  if ((gridFramesForLogic->Row > gridFramesForLogic->RowCount || mSprite.mFramesManager.Size() <= 0) ||
      gridFrameLogic->Row >= gridFrameLogic->RowCount - 1 ||
      gridFrameLogicItems->Row >= gridFrameLogicItems->RowCount - 1)
  {
    return mode;
  }

  CFrame frame = mSprite.mFramesManager.Get(gridFramesForLogic->Row - 1);
  CFrameLogic fLogic = frame.mFLogic.Get(gridFrameLogic->Row - 1);
  CFrameLogicItem fLogicItem = fLogic.mItems.Get(gridFrameLogicItems->Row - 1);

  if (fLogicItem.GetType() == FRAME_LOGIC_ITEM_POINT)
  {
    TRect rect = TRect(
        paintFrameLogic->Width  / 2 - fLogicPan.x + fLogicItem.mX * fLogicZoom / 100,
        paintFrameLogic->Height / 2 - fLogicPan.y + fLogicItem.mY * fLogicZoom / 100,
        paintFrameLogic->Width  / 2 - fLogicPan.x + (fLogicItem.mX + 1) * fLogicZoom / 100,
        paintFrameLogic->Height / 2 - fLogicPan.y + (fLogicItem.mY + 1) * fLogicZoom / 100
        );

    if (PointInRect(p, rect))
    {
      mode = MMM_MOVE;
    }
  }

  if (fLogicItem.GetType() == FRAME_LOGIC_ITEM_RECT)
  {
    TRect rect = TRect(
        paintFrameLogic->Width  / 2 - fLogicPan.x + fLogicItem.mX * fLogicZoom / 100,
        paintFrameLogic->Height / 2 - fLogicPan.y + fLogicItem.mY * fLogicZoom / 100,
        paintFrameLogic->Width  / 2 - fLogicPan.x + (fLogicItem.mX + fLogicItem.mWidth) * fLogicZoom / 100,
        paintFrameLogic->Height / 2 - fLogicPan.y + (fLogicItem.mY + fLogicItem.mHeight) * fLogicZoom / 100
        );

    if (PointInRect(p, rect))
    {
      mode = MMM_MOVE;
    }
    else
    {
      int edge = mAppConfig.mFrameLogicSelectionEdge;

      if (PointInRect(p, TRect(rect.left - edge, rect.top - edge, rect.left, rect.top)))
      {
        mode = MMM_RESIZE_LEFT_TOP;
      }

      if (PointInRect(p, TRect(rect.right, rect.bottom, rect.right + edge, rect.bottom + edge)))
      {
        mode = MMM_RESIZE_RIGHT_BOTTOM;
      }

      if (PointInRect(p, TRect(rect.left - edge, rect.bottom, rect.left, rect.bottom + edge)))
      {
        mode = MMM_RESIZE_LEFT_BOTTOM;
      }

      if (PointInRect(p, TRect(rect.right, rect.top - edge, rect.right + edge, rect.top)))
      {
        mode = MMM_RESIZE_RIGHT_TOP;
      }

      if (PointInRect(p, TRect(rect.left - edge, rect.top, rect.left, rect.bottom)))
      {
        mode = MMM_RESIZE_LEFT;
      }

      if (PointInRect(p, TRect(rect.right, rect.top, rect.right + edge, rect.bottom)))
      {
        mode = MMM_RESIZE_RIGHT;
      }

      if (PointInRect(p, TRect(rect.left, rect.top - edge, rect.right, rect.top)))
      {
        mode = MMM_RESIZE_TOP;
      }

      if (PointInRect(p, TRect(rect.left, rect.bottom, rect.right, rect.bottom + edge)))
      {
        mode = MMM_RESIZE_BOTTOM;
      }
    }
  }

  return mode;
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::paintFrameLogicMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
  //TODO
  ActiveControl = GroupBoxFrameLogicCanvas;

  if (Button == mbLeft && fLogicMouseMode == MMM_PAN)
  {
    // start to pan
    fLogicMouseMode = MMM_DO_PAN;
    oldFLogicPan.x = X;
    oldFLogicPan.y = Y;
  }

  if (Button == mbLeft && fLogicAvailableMouseMode == MMM_MOVE)
  {
      fLogicMouseMode = MMM_MOVE;
      fLogicDefine.x = X;
      fLogicDefine.y = Y;

      return;
  }

  if (Button == mbLeft && fLogicAvailableMouseMode == MMM_RESIZE_RIGHT_BOTTOM)
  {
      fLogicMouseMode = MMM_RESIZE_RIGHT_BOTTOM;
      fLogicDefine.x = X;
      fLogicDefine.y = Y;

      return;
  }

  if (Button == mbLeft && fLogicAvailableMouseMode == MMM_RESIZE_RIGHT_TOP)
  {
      fLogicMouseMode = MMM_RESIZE_RIGHT_TOP;
      fLogicDefine.x = X;
      fLogicDefine.y = Y;

      return;
  }

  if (Button == mbLeft && fLogicAvailableMouseMode == MMM_RESIZE_LEFT_BOTTOM)
  {
      fLogicMouseMode = MMM_RESIZE_LEFT_BOTTOM;
      fLogicDefine.x = X;
      fLogicDefine.y = Y;

      return;
  }

  if (Button == mbLeft && fLogicAvailableMouseMode == MMM_RESIZE_LEFT_TOP)
  {
      fLogicMouseMode = MMM_RESIZE_LEFT_TOP;
      fLogicDefine.x = X;
      fLogicDefine.y = Y;

      return;
  }

  if (Button == mbLeft && fLogicAvailableMouseMode == MMM_RESIZE_LEFT)
  {
      fLogicMouseMode = MMM_RESIZE_LEFT;
      fLogicDefine.x = X;

      return;
  }

  if (Button == mbLeft && fLogicAvailableMouseMode == MMM_RESIZE_RIGHT)
  {
      fLogicMouseMode = MMM_RESIZE_RIGHT;
      fLogicDefine.x = X;

      return;
  }

  if (Button == mbLeft && fLogicAvailableMouseMode == MMM_RESIZE_TOP)
  {
      fLogicMouseMode = MMM_RESIZE_TOP;
      fLogicDefine.y = Y;

      return;
  }

  if (Button == mbLeft && fLogicAvailableMouseMode == MMM_RESIZE_BOTTOM)
  {
      fLogicMouseMode = MMM_RESIZE_BOTTOM;
      fLogicDefine.y = Y;

      return;
  }

  if (Button == mbLeft && fLogicMouseMode == MMM_DEFINE)
  {
    // TODO: check this
    if ((gridFramesForLogic->Row <= gridFramesForLogic->RowCount && mSprite.mFramesManager.Size() >= 0) &&
        gridFrameLogic->Row < gridFrameLogic->RowCount - 1 &&
        gridFrameLogicItems->Row < gridFrameLogicItems->RowCount - 1)
    {
      // start to define the module
      fLogicMouseMode = MMM_DO_DEFINE;
      fLogicDefine.x = X;
      fLogicDefine.y = Y;
    }

    return;
  }

  if (Button == mbRight)
  {
    fLogicMouseMode = (fLogicMouseMode == MMM_PAN) ? MMM_DEFINE : MMM_PAN;
    paintFrameLogic->Cursor = GetModuleCursor(fLogicMouseMode);
  }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::paintFrameLogicMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
  //TODO
  ActiveControl = GroupBoxFrameLogicCanvas;

  if (Button == mbLeft && fLogicMouseMode == MMM_DO_PAN)
  {
    fLogicMouseMode = MMM_PAN;
  }

  if ( Button           != mbLeft ||
       (fLogicMouseMode != MMM_DO_DEFINE &&
       fLogicMouseMode  != MMM_MOVE &&
       fLogicMouseMode  != MMM_RESIZE_RIGHT_BOTTOM &&
       fLogicMouseMode  != MMM_RESIZE_LEFT_TOP &&
       fLogicMouseMode  != MMM_RESIZE_LEFT_BOTTOM &&
       fLogicMouseMode  != MMM_RESIZE_RIGHT_TOP &&
       fLogicMouseMode  != MMM_RESIZE_LEFT &&
       fLogicMouseMode  != MMM_RESIZE_RIGHT &&
       fLogicMouseMode  != MMM_RESIZE_TOP &&
       fLogicMouseMode  != MMM_RESIZE_BOTTOM)
     )
  {
    return;
  }


  CFrame frame = mSprite.mFramesManager.Get(gridFramesForLogic->Row - 1);
  CFrameLogic fLogic = frame.mFLogic.Get(gridFrameLogic->Row - 1);
  CFrameLogicItem fLogicItem = fLogic.mItems.Get(gridFrameLogicItems->Row - 1);

  if (Button == mbLeft && fLogicMouseMode == MMM_DO_DEFINE)
  {
    // inhibit false defining
    if (abs(X - fLogicDefine.x) != 0 && abs(Y - fLogicDefine.y) != 0)
    {
      if (fLogicItem.GetType() == FRAME_LOGIC_ITEM_RECT)
      {
        fLogicItem.SetX(((-paintFrameLogic->Width / 2 + (int)std::min((long)X, fLogicDefine.x) + fLogicPan.x) * 100) / fLogicZoom );
        fLogicItem.SetY(((-paintFrameLogic->Height / 2 + (int)std::min((long)Y, fLogicDefine.y) + fLogicPan.y) * 100) / fLogicZoom );
        fLogicItem.SetWidth(abs(X - fLogicDefine.x) * 100 / fLogicZoom );
        fLogicItem.SetHeight(abs(Y - fLogicDefine.y) * 100 / fLogicZoom );
      }

      if (fLogicItem.GetType() == FRAME_LOGIC_ITEM_POINT)
      {
        fLogicItem.SetX(((-paintFrameLogic->Width / 2 + X + fLogicPan.x) * 100) / fLogicZoom );
        fLogicItem.SetY(((-paintFrameLogic->Height / 2 + Y + fLogicPan.y) * 100) / fLogicZoom );
      }
    }
    else
    {
      /*
      // TODO: continue
      int rowModules = gridModules->Row;

      // select a module
      for(int i = mSprite.mModulesManager.Size() - 1; i >= 0 ; i--)
      {
        CModule m = mSprite.mModulesManager.Get(i);

        Graphics::TBitmap* bitmap = mSprite.GetModuleBitmap(m);

        if (bitmap)
        {
          int bitmapWidth  = bitmap->Width  * modulesZoom / 100;
          int bitmapHeight = bitmap->Height * modulesZoom / 100;
          int imageX = paintModules->Width / 2 - bitmapWidth / 2 - modulesPan.x;
          int imageY = paintModules->Height / 2 - bitmapHeight / 2 - modulesPan.y;
          int screenModuleWidth  = m.GetWidth()  * modulesZoom / 100;
          int screenModuleHeight = m.GetHeight() * modulesZoom / 100;
          int screenModuleX      = imageX + m.GetX() * modulesZoom / 100;
          int screenModuleY      = imageY + m.GetY() * modulesZoom / 100;

          // check where the cursor is placed and modify the cursor type accordingly
          if (PointInRect(TPoint(X, Y), TRect(screenModuleX, screenModuleY, screenModuleX + screenModuleWidth, screenModuleY + screenModuleHeight)))
          {
            rowModules = i + 1;
            break;
          }
        }
      }

      gridModules->Row = rowModules;
      */
    }
  }

  if (Button == mbLeft && fLogicMouseMode == MMM_MOVE)
  {
    fLogicItem.SetX(fLogicItem.GetX() + ((X - fLogicDefine.x)) * 100 / fLogicZoom );
    fLogicItem.SetY(fLogicItem.GetY() + ((Y - fLogicDefine.y)) * 100 / fLogicZoom );
  }

  if (Button == mbLeft && fLogicMouseMode == MMM_RESIZE_RIGHT_BOTTOM)
  {
    int newX = ((X - fLogicDefine.x)) * 100 / fLogicZoom + fLogicItem.GetX() + fLogicItem.GetWidth();
    int newY = ((Y - fLogicDefine.y)) * 100 / fLogicZoom + fLogicItem.GetY() + fLogicItem.GetHeight();
    int w = abs(newX - fLogicItem.GetX());
    int h = abs(newY - fLogicItem.GetY());

    fLogicItem.SetX((int)std::min(fLogicItem.GetX(), newX));
    fLogicItem.SetY((int)std::min(fLogicItem.GetY(), newY));
    fLogicItem.SetWidth(w);
    fLogicItem.SetHeight(h);
  }

  if (Button == mbLeft && fLogicMouseMode == MMM_RESIZE_LEFT_TOP)
  {
    int newX = ((X - fLogicDefine.x)) * 100 / fLogicZoom + fLogicItem.GetX();
    int newY = ((Y - fLogicDefine.y)) * 100 / fLogicZoom + fLogicItem.GetY();
    int w = abs(newX - fLogicItem.GetX() - fLogicItem.GetWidth());
    int h = abs(newY - fLogicItem.GetY() - fLogicItem.GetHeight());

    fLogicItem.SetX((int)std::min(fLogicItem.GetX() + fLogicItem.GetWidth(), newX));
    fLogicItem.SetY((int)std::min(fLogicItem.GetY() + fLogicItem.GetHeight(), newY));
    fLogicItem.SetWidth(w);
    fLogicItem.SetHeight(h);
  }

  if (Button == mbLeft && fLogicMouseMode == MMM_RESIZE_LEFT_BOTTOM)
  {
    int newX = ((X - fLogicDefine.x)) * 100 / fLogicZoom + fLogicItem.GetX();
    int newY = ((Y - fLogicDefine.y)) * 100 / fLogicZoom + fLogicItem.GetY() + fLogicItem.GetHeight();
    int w = abs(newX - fLogicItem.GetX() - fLogicItem.GetWidth());
    int h = abs(newY - fLogicItem.GetY());

    fLogicItem.SetX((int)std::min(fLogicItem.GetX() + fLogicItem.GetWidth(), newX));
    fLogicItem.SetY((int)std::min(fLogicItem.GetY(), newY));
    fLogicItem.SetWidth(w);
    fLogicItem.SetHeight(h);
  }

  if (Button == mbLeft && fLogicMouseMode == MMM_RESIZE_RIGHT_TOP)
  {
    int newX = ((X - fLogicDefine.x)) * 100 / fLogicZoom + fLogicItem.GetX() + fLogicItem.GetWidth();
    int newY = ((Y - fLogicDefine.y)) * 100 / fLogicZoom + fLogicItem.GetY();
    int w = abs(newX - fLogicItem.GetX());
    int h = abs(newY - fLogicItem.GetY() - fLogicItem.GetHeight());

    fLogicItem.SetX((int)std::min(fLogicItem.GetX(), newX));
    fLogicItem.SetY((int)std::min(fLogicItem.GetY() + fLogicItem.GetHeight(), newY));
    fLogicItem.SetWidth(w);
    fLogicItem.SetHeight(h);
  }

  if (Button == mbLeft && fLogicMouseMode == MMM_RESIZE_RIGHT)
  {
    int newX = ((X - fLogicDefine.x)) * 100 / fLogicZoom + fLogicItem.GetX() + fLogicItem.GetWidth();
    int w = abs(newX - fLogicItem.GetX());

    fLogicItem.SetX((int)std::min(fLogicItem.GetX(), newX));
    fLogicItem.SetWidth(w);
  }

  if (Button == mbLeft && fLogicMouseMode == MMM_RESIZE_LEFT)
  {
    int newX = ((X - fLogicDefine.x)) * 100 / fLogicZoom + fLogicItem.GetX();
    int w = abs(newX - fLogicItem.GetX() - fLogicItem.GetWidth());

    fLogicItem.SetX((int)std::min(fLogicItem.GetX() + fLogicItem.GetWidth(), newX));
    fLogicItem.SetWidth(w);
  }

  if (Button == mbLeft && fLogicMouseMode == MMM_RESIZE_TOP)
  {
    int newY = ((Y - fLogicDefine.y)) * 100 / fLogicZoom + fLogicItem.GetY();
    int h = abs(newY - fLogicItem.GetY() - fLogicItem.GetHeight());

    fLogicItem.SetY((int)std::min(fLogicItem.GetY() + fLogicItem.GetHeight(), newY));
    fLogicItem.SetHeight(h);
  }

  if (Button == mbLeft && fLogicMouseMode == MMM_RESIZE_BOTTOM)
  {
    int newY = ((Y - fLogicDefine.y)) * 100 / fLogicZoom + fLogicItem.GetY() + fLogicItem.GetHeight();
    int h = abs(newY - fLogicItem.GetY());

    fLogicItem.SetY((int)std::min(fLogicItem.GetY(), newY));
    fLogicItem.SetHeight(h);
  }

  fLogic.mItems.Set(gridFrameLogicItems->Row - 1, fLogicItem);
  frame.mFLogic.Set(gridFrameLogic->Row - 1, fLogic);
  mSprite.mFramesManager.Set(gridFramesForLogic->Row - 1, frame);

  int oldRow = gridFrameLogicItems->Row;
  FrameLogicItemsToGrid();
  gridFrameLogicItems->Row = oldRow;

  fLogicMouseMode = MMM_DEFINE;
  paintFrameLogicPaint(this);
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
//
//
//                             SCRIPTING
//
//
//---------------------------------------------------------------------------

void TFormMain::RefreshAllScripts()
{
  RefreshScripts(Import1, "Import");
  RefreshScripts(Export1, "Export");
}
//---------------------------------------------------------------------------

void TFormMain::RefreshScripts(TMenuItem* menuItem, std::string scriptType)
{
  menuItem->Clear();

  TSearchRec sr;

  TMenuItem* childItem;

  SentryScript* script = new SentryScript(&mSprite);

  bool scriptsAdded = false;

  // search all scripts that match the scriptType in the scripts directory
  if (FindFirst(UTF8ToString(mAppConfig.mPathScripts.c_str()) + "\\*.csl", faArchive | faReadOnly, sr) == 0)
  {
    do
    {
      // do something
      if (script->LoadScriptFromFile(mAppConfig.mPathScripts + "\\" + UTF8Encode(sr.Name).c_str()))
      {
        std::string scriptName      = script->GetScriptVar(SCRIPT_VAR_NAME);
        std::string scriptCategory  = script->GetScriptVar(SCRIPT_VAR_CATEGORY);
        std::string paramCountAsStr = script->GetScriptVar(SCRIPT_VAR_PARAM_COUNT);

        if (!scriptName.empty() && scriptCategory == scriptType)
        {
          childItem = new TMenuItem(menuItem);

          int paramCount = 0;

          if (!paramCountAsStr.empty())
          {
            paramCount = AnsiString(paramCountAsStr.c_str()).ToInt();
          }

          childItem->Caption = AnsiString(scriptName.c_str()) + (paramCount > 0 ? "..." : "");

          childItem->Hint = AnsiString(mAppConfig.mPathScripts.c_str()) + AnsiString("\\") + sr.Name + "$" + AnsiString(script->GetScriptVar(SCRIPT_VAR_INFO).c_str());
          childItem->OnClick = OnScriptMenuItemClick;
          
          menuItem->Add(childItem);

          scriptsAdded = true;
        }
      }
      else
      {
        MemoMsg->Lines->Add(UTF8ToString(mAppConfig.mPathScripts.c_str()) + "\\" + sr.Name + ": " + script->GetError().c_str());
      }
    }
    while(FindNext(sr) == 0);
  }

  if (!scriptsAdded)
  {
    childItem = new TMenuItem(menuItem);
    childItem->Caption = "No Scripts Available";
    menuItem->Add(childItem);
  }

  // add default items - separator
  childItem = new TMenuItem(menuItem);
  childItem->Caption = "-";
  menuItem->Add(childItem);

  // refresh
  childItem = new TMenuItem(menuItem);
  childItem->Caption = "Refresh";
  childItem->OnClick = RefreshScriptsClick;
  menuItem->Add(childItem);

  delete script;
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::RefreshScriptsClick(TObject *Sender)
{
  // TODO
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::OnScriptMenuItemClick(TObject *Sender)
{
  TMenuItem* menuItem = (TMenuItem*)Sender;

  if (!menuItem)
  {
    return;
  }

  int sepIndex = menuItem->Hint.Pos("$");

  if (sepIndex == 0)
  {
    return;
  }

  AnsiString scriptFilePath = menuItem->Hint.SubString(1, sepIndex - 1);
  std::string scriptStr = std::string(appPath.c_str()) + std::string(scriptFilePath.c_str());

  mSprite.LoadScriptFromFile(scriptStr);

  if (FormScript->Init(&mSprite))
  {
    FormScript->Show();
  }  
}
//---------------------------------------------------------------------------


void __fastcall TFormMain::New1Click(TObject *Sender)
{
  mSprite.Close();
  mSprite.New();

  ImagesToGrid();
  gridImages->Row = 1;

  ModulesToGrid();
  gridModules->Row = 1;

  FramesToGrid();
  gridFrames->Row = 1;

  FModulesToGrid();
  gridFModules->Row = 1;

  AnimsToGrid();
  gridAnims->Row = 1;

  AFramesToGrid();
  gridAFrames->Row = 1;

  FramesForLogicToGrid();
  gridFramesForLogic->Row = 1;

  FrameLogicToGrid();
  gridFrameLogic->Row = 1;

  FrameLogicItemsToGrid();
  gridFrameLogicItems->Row = 1;

  ImagesForColorMapToGrid();
  gridImagesForColorMap->Row = 1;

  ColorMapToGrid();
  gridColorMaps->Row = 1;

  ColorMapItemsToGrid();
  gridColorMapItems->Row = 1;

  GUIFromDocState(mSprite.GetDocState());
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::FormClose(TObject *Sender, TCloseAction &Action)
{
  SaveConfig();
}
//---------------------------------------------------------------------------


void __fastcall TFormMain::TabSheetFrameLogicShow(TObject *Sender)
{
  FramesForLogicToGrid();
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
//
//
//                      COLOR MAP EDITING
//
//
//---------------------------------------------------------------------------
CImage TFormMain::GetImageForColorMap()
{
  return mSprite.mImagesManager.Get(gridImagesForColorMap->Row - 1);
}
//---------------------------------------------------------------------------

CColorMap TFormMain::GetColorMap()
{
  CImage image = GetImageForColorMap();

  return image.mColorMaps.Get(gridColorMaps->Row - 1);
}
//---------------------------------------------------------------------------

CColorMapItem TFormMain::GetColorMapItem()
{
  CImage image       = GetImageForColorMap();
  CColorMap colorMap = GetColorMap();

  return colorMap.mItems.Get(gridColorMapItems->Row - 1);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::TabSheetColorMappingShow(TObject *Sender)
{
  ImagesForColorMapToGrid();    
}
//---------------------------------------------------------------------------

void TFormMain::ArrangeColorMapPageItems()
{
  editColorMap->Top = GroupBoxColorMaps->ClientHeight - editColorMap->Height - 4;
  gridColorMaps->Height = editColorMap->Top - gridColorMaps->Top - 4;

  editColorMapItem->Top = GroupBoxColorMapItems->ClientHeight - editColorMapItem->Height - 4;
  gridColorMapItems->Height = editColorMapItem->Top - gridColorMapItems->Top - 4;
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::Splitter3Moved(TObject *Sender)
{
  ArrangeColorMapPageItems();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::Splitter11Moved(TObject *Sender)
{
  ArrangeColorMapPageItems();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::Splitter12Moved(TObject *Sender)
{
  ArrangeColorMapPageItems();
}
//---------------------------------------------------------------------------

void TFormMain::ImagesForColorMapToGrid()
{
  gridImagesForColorMap->RowCount = 2;
  gridImagesForColorMap->Cells[0][0] = "Index";
  gridImagesForColorMap->Cells[1][0] = "Id";
  gridImagesForColorMap->Cells[2][0] = "Color Maps";
  gridImagesForColorMap->Cells[3][0] = "File Name";

  for(int i = 0; i < mSprite.mImagesManager.Size(); i++)
  {
    CImage image = mSprite.mImagesManager.Get(i);

    gridImagesForColorMap->Cells[0][i + 1] = AnsiString(i);
    gridImagesForColorMap->Cells[1][i + 1] = "0x" + AnsiString::IntToHex(image.GetId(), 32);
    gridImagesForColorMap->Cells[2][i + 1] = AnsiString(image.mColorMaps.Size());
    gridImagesForColorMap->Cells[3][i + 1] = image.GetFileName().c_str();
    gridImagesForColorMap->RowCount++;
  }

  if (mSprite.mImagesManager.Size() == 0)
  {
    gridColorMaps->Enabled = false;
    editColorMap->Enabled  = false;
    gridColorMaps->Color   = clScrollBar;
    editColorMap->Color    = clScrollBar;
  }
  else
  {
    gridColorMaps->Enabled = true;
    editColorMap->Enabled  = true;
    gridColorMaps->Color   = clWindow;
    editColorMap->Color    = clWindow;

    // adjust so that we won't have an empty row (because of the for loop++)
    gridImagesForColorMap->RowCount -= 1;
  }
}
//---------------------------------------------------------------------------

void TFormMain::ColorMapToGrid()
{
  gridColorMaps->RowCount = 2;
  gridColorMaps->Cells[0][0] = "Index";
  gridColorMaps->Cells[1][0] = "Items";
  gridColorMaps->Cells[2][0] = "Info";

  if (gridImagesForColorMap->Row <= gridImagesForColorMap->RowCount && mSprite.mImagesManager.Size() > 0)
  {
    CImage image = mSprite.mImagesManager.Get(gridImagesForColorMap->Row - 1);

    for(int i = 0; i < image.mColorMaps.Size(); i++)
    {
      CColorMap colorMap = image.mColorMaps.Get(i);

      gridColorMaps->Cells[0][i + 1] = AnsiString(i);
      gridColorMaps->Cells[1][i + 1] = AnsiString(colorMap.mItems.Size());
      gridColorMaps->Cells[2][i + 1] = colorMap.GetInfo().c_str();
      gridColorMaps->RowCount++;
    }

    if (image.mColorMaps.Size() == 0)
    {
      gridColorMapItems->Enabled = false;
      editColorMapItem->Enabled  = false;
      gridColorMapItems->Color   = clScrollBar;
      editColorMapItem->Color    = clScrollBar;
    }
    else
    {
      gridColorMapItems->Enabled = true;
      editColorMapItem->Enabled  = true;
      gridColorMapItems->Color   = clWindow;
      editColorMapItem->Color    = clWindow;
    }
  }
  else
  {
    gridColorMapItems->Enabled = false;
    editColorMapItem->Enabled  = false;
    gridColorMapItems->Color   = clScrollBar;
    editColorMapItem->Color    = clScrollBar;
  }

  gridColorMaps->Cells[0][gridColorMaps->RowCount - 1] = "new...";

  for(int i = 1; i < 3; i++)
  {
    gridColorMaps->Cells[i][gridColorMaps->RowCount - 1] = "";
  }
}
//---------------------------------------------------------------------------

void TFormMain::ColorMapItemsToGrid()
{
  gridColorMapItems->RowCount = 2;
  gridColorMapItems->Cells[0][0] = "Index";
  gridColorMapItems->Cells[1][0] = "Src";
  gridColorMapItems->Cells[2][0] = "Dst";

  if (gridImagesForColorMap->Row <= gridImagesForColorMap->RowCount && mSprite.mImagesManager.Size() > 0)
  {
    CImage image = mSprite.mImagesManager.Get(gridImagesForColorMap->Row - 1);

    if (gridColorMaps->Row < gridColorMaps->RowCount - 1 && gridColorMaps->RowCount > 1)
    {
      CColorMap colorMap = image.mColorMaps.Get(gridColorMaps->Row - 1);

      for(int i = 0; i < colorMap.mItems.Size(); i++)
      {
        CColorMapItem colorMapItem = colorMap.mItems.Get(i);

        gridColorMapItems->Cells[0][i + 1] = AnsiString(i);
        gridColorMapItems->Cells[1][i + 1] = "0x" + AnsiString::IntToHex(colorMapItem.GetSrcColor(), 6);
        gridColorMapItems->Cells[2][i + 1] = "0x" + AnsiString::IntToHex(colorMapItem.GetDstColor(), 6);

        gridColorMapItems->RowCount++;
      }
    }
  }

  gridColorMapItems->Cells[0][gridColorMapItems->RowCount - 1] = "new...";

  for(int i = 1; i < 3; i++)
  {
    gridColorMapItems->Cells[i][gridColorMapItems->RowCount - 1] = "";
  }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::gridImagesForColorMapClick(TObject *Sender)
{
  ColorMapToGrid();

  paintColorMapPaint(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::gridColorMapsClick(TObject *Sender)
{
  ColorMapItemsToGrid();

  if (gridColorMaps->Row == gridColorMaps->RowCount - 1)
  {
    editColorMap->Text = "";
    return;
  }

  CImage image       = mSprite.mImagesManager.Get(gridImagesForColorMap->Row - 1);
  CColorMap colorMap = image.mColorMaps.Get(gridColorMaps->Row - 1);

  editColorMap->Text =
    AnsiString("info=\"") + AnsiString(colorMap.GetInfo().c_str()) + "\""
    ;

  paintColorMapPaint(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::gridColorMapsDblClick(TObject *Sender)
{
  if (gridColorMaps->Row == gridColorMaps->RowCount - 1)
  {
    gridColorMapsPopNew(Sender);
  }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::gridColorMapsPopClone(TObject *Sender)
{
  if (gridColorMaps->Row < gridColorMaps->RowCount - 1)
  {
    CImage image = mSprite.mImagesManager.Get(gridImagesForColorMap->Row - 1);

    int row = image.mColorMaps.Clone(gridColorMaps->Row - 1);

    mSprite.mImagesManager.Set(gridImagesForColorMap->Row - 1, image);

    int imageRow = gridImagesForColorMap->Row;

    ColorMapToGrid();
    ImagesForColorMapToGrid();

    gridImagesForColorMap->Row   = imageRow;
    gridColorMaps->Row = row + 1;

    gridColorMapsClick(this);
  }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::gridColorMapsPopNew(TObject *Sender)
{
  CImage image = mSprite.mImagesManager.Get(gridImagesForColorMap->Row - 1);

  int colorMapIndex = image.mColorMaps.New();

  mSprite.mImagesManager.Set(gridImagesForColorMap->Row - 1, image);

  int imageRow = gridImagesForColorMap->Row;

  ColorMapToGrid();
  ImagesForColorMapToGrid();

  gridImagesForColorMap->Row = imageRow;
  gridColorMaps->Row         = colorMapIndex + 1;

  gridColorMapsClick(this);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::gridColorMapsPopDelete(TObject *Sender)
{
  // TODO: test
  if (gridColorMaps->Row < gridColorMaps->RowCount - 1)
  {
    CImage image = mSprite.mImagesManager.Get(gridImagesForColorMap->Row - 1);

    int row = image.mColorMaps.Delete(gridColorMaps->Row - 1);
    mSprite.mImagesManager.Set(gridImagesForColorMap->Row - 1, image);

    ImagesForColorMapToGrid();
    ColorMapToGrid();
    ColorMapItemsToGrid();

    if (row != -1)
    {
      gridColorMaps->Row = row + 1;
    }

    gridColorMapsClick(this);
  }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::gridColorMapsPopInsert(TObject *Sender)
{
  if (gridColorMaps->Row < gridColorMaps->RowCount - 1)
  {
    CImage image = mSprite.mImagesManager.Get(gridImagesForColorMap->Row - 1);

    int row = image.mColorMaps.Insert(gridColorMaps->Row - 1, CColorMap());

    mSprite.mImagesManager.Set(gridImagesForColorMap->Row - 1, image);

    int imageRow = gridImagesForColorMap->Row;

    ColorMapToGrid();
    ImagesForColorMapToGrid();

    gridImagesForColorMap->Row = imageRow;
    gridColorMaps->Row         = row + 1;

    gridColorMapsClick(this);
  }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::gridColorMapsPopMoveDown(TObject *Sender)
{
  if (gridColorMaps->Row < gridColorMaps->RowCount - 1)
  {
    CImage image = mSprite.mImagesManager.Get(gridImagesForColorMap->Row - 1);

    int row = image.mColorMaps.MoveDown(gridColorMaps->Row - 1);
    mSprite.mImagesManager.Set(gridImagesForColorMap->Row - 1, image);

    ColorMapToGrid();

    if (row != -1)
    {
      gridColorMaps->Row = row + 1;
      gridColorMapsClick(this);
    }
  }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::gridColorMapsPopMoveUp(TObject *Sender)
{
  if (gridColorMaps->Row < gridColorMaps->RowCount - 1)
  {
    CImage image = mSprite.mImagesManager.Get(gridImagesForColorMap->Row - 1);

    int row = image.mColorMaps.MoveUp(gridColorMaps->Row - 1);
    mSprite.mImagesManager.Set(gridImagesForColorMap->Row - 1, image);

    ColorMapToGrid();

    if (row != -1)
    {
      gridColorMaps->Row = row + 1;
      gridColorMapsClick(this);
    }
  }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::editColorMapKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
  if (Key == VK_RETURN)
  {
    editColorMapExit(Sender);
  }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::editColorMapExit(TObject *Sender)
{
  if (editColorMap->Text == "")
  {
    return;
  }

  // if one attribute is wrong, is ignored, all the others that are correct,
  // will be set

  // get the entry that is edited
  CImage image = mSprite.mImagesManager.Get(gridImagesForColorMap->Row - 1);
  CColorMap colorMap = image.mColorMaps.Get(gridColorMaps->Row - 1);

  // parse the new attributes through tinyxml
  tinyxml2::XMLDocument doc;

  doc.Parse( UTF8Encode("<element " + editColorMap->Text + "/>").c_str() );

  auto elem = doc.FirstChildElement("element");

  int result;

  //
  // Parse string attributes
  //
  const char* info = elem->Attribute("info");

  if (info)
  {
    colorMap.SetInfo(info);
  }

  int oldRow = gridColorMaps->Row;

  image.mColorMaps.Set(gridColorMaps->Row - 1, colorMap);
  mSprite.mImagesManager.Set(gridImagesForColorMap->Row - 1, image);

  editColorMap->Text =
    UnicodeString("info=\"") + UTF8ToString(colorMap.GetInfo().c_str()) + "\""
    ;

  ColorMapToGrid();

  gridColorMaps->Row = oldRow;

  paintColorMapPaint(this);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::gridColorMapItemsClick(TObject *Sender)
{
  if (gridColorMapItems->Row == gridColorMapItems->RowCount - 1)
  {
    editColorMapItem->Text = "";
    return;
  }

  CColorMapItem colorMapItem = GetColorMapItem();

  editColorMapItem->Text =
    AnsiString("s=") + "0x" + AnsiString::IntToHex(colorMapItem.GetSrcColor(), 6) + " " +
    AnsiString("d=") + "0x" + AnsiString::IntToHex(colorMapItem.GetDstColor(), 6);

  paintColorMapPaint(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::gridColorMapItemsDblClick(TObject *Sender)
{
  if (gridColorMapItems->Row == gridColorMapItems->RowCount - 1)
  {
    gridColorMapItemsPopNew(Sender);
  }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::gridColorMapItemsPopClone(TObject *Sender)
{
  if (gridColorMapItems->Row < gridColorMapItems->RowCount - 1)
  {
    CImage image       = GetImageForColorMap();
    CColorMap colorMap = GetColorMap();

    int colorMapItemRow = colorMap.mItems.Clone(gridColorMapItems->Row - 1);

    image.mColorMaps.Set(gridColorMaps->Row - 1, colorMap);
    mSprite.mImagesManager.Set(gridImagesForColorMap->Row - 1, image);

    int colorMapRow = gridColorMaps->Row;

    ColorMapItemsToGrid();
    ColorMapToGrid();

    gridColorMaps->Row      = colorMapRow;
    gridColorMapItems->Row  = colorMapItemRow + 1;

    gridColorMapItemsClick(this);
  }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::gridColorMapItemsPopNew(TObject *Sender)
{
  CImage image = GetImageForColorMap();
  CColorMap colorMap = GetColorMap();

  int colorMapItemIndex = colorMap.mItems.New();

  image.mColorMaps.Set(gridColorMaps->Row - 1, colorMap);
  mSprite.mImagesManager.Set(gridImagesForColorMap->Row - 1, image);

  // TODO
  int colorMapRow = gridColorMaps->Row;

  ColorMapItemsToGrid();
  ColorMapToGrid();

  gridColorMaps->Row     = colorMapRow;
  gridColorMapItems->Row = colorMapItemIndex + 1;

  gridColorMapItemsClick(this);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::gridColorMapItemsPopDelete(TObject *Sender)
{
  if (gridColorMapItems->Row < gridColorMapItems->RowCount - 1)
  {
    CImage image = GetImageForColorMap();
    CColorMap colorMap = GetColorMap();

    int colorMapItemRow = colorMap.mItems.Delete(gridColorMapItems->Row - 1);

    image.mColorMaps.Set(gridColorMaps->Row - 1, colorMap);
    mSprite.mImagesManager.Set(gridImagesForColorMap->Row - 1, image);

    ColorMapItemsToGrid();
    ColorMapToGrid();

    if (colorMapItemRow != -1)
    {
      gridColorMapItems->Row = colorMapItemRow + 1;
    }

    gridColorMapItemsClick(this);
  }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::gridColorMapItemsPopInsert(TObject *Sender)
{
  if (gridColorMapItems->Row < gridColorMapItems->RowCount - 1)
  {
    CImage image = GetImageForColorMap();
    CColorMap colorMap = GetColorMap();

    int colorMapItemRow = colorMap.mItems.Insert(gridColorMapItems->Row - 1, CColorMapItem());

    image.mColorMaps.Set(gridColorMaps->Row - 1, colorMap);
    mSprite.mImagesManager.Set(gridImagesForColorMap->Row - 1, image);

    int colorMapRow = gridColorMaps->Row;

    ColorMapItemsToGrid();
    ColorMapToGrid();

    gridColorMaps->Row     = colorMapRow;
    gridColorMapItems->Row = colorMapItemRow + 1;

    gridColorMapItemsClick(this);
  }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::gridColorMapItemsPopMoveDown(TObject *Sender)
{
  if (gridColorMapItems->Row < gridColorMapItems->RowCount - 1)
  {
    CImage image = GetImageForColorMap();
    CColorMap colorMap = GetColorMap();

    int colorMapItemRow = colorMap.mItems.MoveDown(gridColorMapItems->Row - 1);

    image.mColorMaps.Set(gridColorMaps->Row - 1, colorMap);
    mSprite.mImagesManager.Set(gridImagesForColorMap->Row - 1, image);

    ColorMapItemsToGrid();

    if (colorMapItemRow != -1)
    {
      gridColorMapItems->Row = colorMapItemRow + 1;
      gridColorMapItemsClick(this);
    }
  }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::gridColorMapItemsPopMoveUp(TObject *Sender)
{
  if (gridColorMapItems->Row < gridColorMapItems->RowCount - 1)
  {
    CImage image = GetImageForColorMap();
    CColorMap colorMap = GetColorMap();

    int colorMapItemRow = colorMap.mItems.MoveUp(gridColorMapItems->Row - 1);

    image.mColorMaps.Set(gridColorMaps->Row - 1, colorMap);
    mSprite.mImagesManager.Set(gridImagesForColorMap->Row - 1, image);

    ColorMapItemsToGrid();

    if (colorMapItemRow != -1)
    {
      gridColorMapItems->Row = colorMapItemRow + 1;
      gridColorMapItemsClick(this);
    }
  }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::editColorMapItemKeyDown(TObject *Sender,
      WORD &Key, TShiftState Shift)
{
  if (Key == VK_RETURN)
  {
    editColorMapItemExit(Sender);
  }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::editColorMapItemExit(TObject *Sender)
{
  if (editColorMapItem->Text == "")
  {
    return;
  }

  // if one attribute is wrong, is ignored

  // get the entry that is edited
  CImage image = GetImageForColorMap();
  CColorMap colorMap = GetColorMap();
  CColorMapItem colorMapItem = GetColorMapItem();

  // parse the new attributes through tinyxml
  tinyxml2::XMLDocument doc;

  doc.Parse(UTF8Encode("<element " + editColorMapItem->Text + "/>").c_str() );

  auto elem = doc.FirstChildElement("element");

  int result;

  //
  // Parse string attributes
  //
  const char* s = elem->Attribute("s");
  const char* d = elem->Attribute("d");

  if (s)
  {
    int srcColor = 0;

    sscanf(s, "%x", &srcColor);

    colorMapItem.SetSrcColor(srcColor);
  }

  if (d)
  {
    int dstColor = 0;

    sscanf(d, "%x", &dstColor);

    colorMapItem.SetDstColor(dstColor);
  }

  int oldRow = gridColorMapItems->Row;

  colorMap.mItems.Set(gridColorMapItems->Row - 1, colorMapItem);
  image.mColorMaps.Set(gridColorMaps->Row - 1, colorMap);
  mSprite.mImagesManager.Set(gridImagesForColorMap->Row - 1, image);

  // same code as in
  editColorMapItem->Text =
    UnicodeString("s=") + "0x" + UnicodeString::IntToHex(colorMapItem.GetSrcColor(), 6) + " " +
    UnicodeString("d=") + "0x" + UnicodeString::IntToHex(colorMapItem.GetDstColor(), 6) + " "
    ;

  ColorMapItemsToGrid();

  gridColorMapItems->Row = oldRow;

  paintColorMapPaint(this);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::cbColorMapAxisClick(TObject *Sender)
{
  paintColorMapPaint(this);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::cbColorMapGridClick(TObject *Sender)
{
  paintColorMapPaint(this);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::cbColorMapTransClick(TObject *Sender)
{
  paintColorMapPaint(this);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::trackColorMapZoomChange(TObject *Sender)
{
  colorMapZoom = trackColorMapZoom->Position;

  paintColorMapPaint(this);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::paintColorMapPaint(TObject *Sender)
{
  int imageIndex  = gridImagesForColorMap->Row - 1;
  //int moduleIndex = gridModules->Row - 1;

  // make clean
  paintColorMap->Canvas->Brush->Color = (TColor)mAppConfig.mColorMapBackgroundColor;
  paintColorMap->Canvas->FillRect(paintColorMap->ClientRect);

  // paint the selected image
  if (imageIndex < 0 || imageIndex >= mSprite.mImagesManager.Size())
  {
    DrawColorMapOverlay();

    return;
  }

  Gdiplus::Bitmap* bitmap = ImageCache::Instance()->GetBitmap(mSprite.mImagesManager.Get(imageIndex).GetFileName());

  if (!bitmap)
  {
    paintColorMap->Canvas->TextOut(0, 0, "INVALID IMAGE");

    DrawColorMapOverlay();
    return;
  }


  if (gridColorMapItems->Row >= gridColorMapItems->RowCount - 1 ||
      gridColorMaps->Row >= gridColorMaps->RowCount - 1)
  {
    DrawImage(paintColorMap->Canvas, imageIndex, paintColorMap->Width / 2 - colorMapPan.x, paintColorMap->Height / 2 - colorMapPan.y, colorMapZoom, cbColorMapTrans->Checked);

    /*
    Graphics::TBitmap* bitmap = ImageCache::Instance()->GetBitmap(mSprite.mImagesManager.Get(imageIndex).GetFileName());

    if (!bitmap)
    {
      paintColorMap->Canvas->TextOut(0, 0, "INVALID IMAGE");

      DrawColorMapOverlay();
      return;
    }

    bitmap->Transparent      = cbColorMapTrans->Checked;
    bitmap->TransparentMode  = tmFixed;
    bitmap->TransparentColor = (TColor)mSprite.mImagesManager.Get(imageIndex).GetTransparentColor();

    int bitmapWidth  = bitmap->Width  * colorMapZoom / 100;
    int bitmapHeight = bitmap->Height * colorMapZoom / 100;

    paintColorMap->Canvas->StretchDraw(
      TRect(
        paintColorMap->Width / 2 - bitmapWidth / 2 - colorMapPan.x,
        paintColorMap->Height / 2 - bitmapHeight / 2 - colorMapPan.y,
        paintColorMap->Width / 2 + bitmapWidth / 2 - colorMapPan.x,
        paintColorMap->Height / 2 + bitmapHeight / 2 - colorMapPan.y),
      bitmap
    );
    */

    DrawColorMapOverlay();

    return;
  }

  int bitmapWidth  = bitmap->GetWidth()  * colorMapZoom / 100;
  int bitmapHeight = bitmap->GetHeight() * colorMapZoom / 100;

  Gdiplus::Graphics graphics(paintColorMap->Canvas->Handle);

  graphics.SetSmoothingMode( Gdiplus::SmoothingModeNone );
  graphics.SetPixelOffsetMode( Gdiplus::PixelOffsetModeHalf );
  graphics.SetInterpolationMode( Gdiplus::InterpolationModeNearestNeighbor );

  int transparentColor = mSprite.mImagesManager.Get(imageIndex).GetTransparentColor();

  Gdiplus::Rect destRect(
    paintColorMap->Width / 2 - bitmapWidth / 2 - colorMapPan.x,
    paintColorMap->Height / 2 - bitmapHeight / 2 - colorMapPan.y,
    bitmapWidth,
    bitmapHeight
  );

  Gdiplus::ImageAttributes imAtt;

  if (transparentColor == -1 || !cbColorMapTrans->Checked)
  {
  }
  else
  {
    imAtt.SetColorKey(
      Gdiplus::Color(GetRValue(transparentColor), GetGValue(transparentColor), GetBValue(transparentColor)),
      Gdiplus::Color(GetRValue(transparentColor), GetGValue(transparentColor), GetBValue(transparentColor)),
      Gdiplus::ColorAdjustTypeDefault);
  }

  // set the color mapping
  CColorMap colorMap = GetColorMap();

  Gdiplus::ColorMap* gdiplusColorMap = 0;

  if (colorMap.mItems.Size() > 0)
  {
    gdiplusColorMap = new Gdiplus::ColorMap[colorMap.mItems.Size()];

    for(int j = 0; j < colorMap.mItems.Size(); j++)
    {
      CColorMapItem colorMapItem = colorMap.mItems.Get(j);

      gdiplusColorMap[j].oldColor = Gdiplus::Color(colorMapItem.GetSrcColor());      
      gdiplusColorMap[j].newColor = Gdiplus::Color(colorMapItem.GetDstColor());
    }

    imAtt.SetRemapTable(colorMap.mItems.Size(), gdiplusColorMap, Gdiplus::ColorAdjustTypeBitmap);
  }

  graphics.DrawImage((Gdiplus::Image*)bitmap, destRect, 0, 0, bitmap->GetWidth(), bitmap->GetHeight(), Gdiplus::UnitPixel, &imAtt);

  delete [] gdiplusColorMap; 

  // wow :)
  // Graphics::TBitmap* bitmap = ImageCache::Instance()->GetBitmap(mSprite.mImagesManager.Get(imageIndex).GetFileName());
  /*
  Graphics::TBitmap* bitmap = ImageCache::Instance()->CloneBitmap(mSprite.mImagesManager.Get(imageIndex).GetFileName());

  if (!bitmap)
  {
    paintColorMap->Canvas->TextOut(0, 0, "INVALID IMAGE");

    DrawColorMapOverlay();
    return;
  }

  CColorMap colorMap = GetColorMap();

  for(int x = 0; x < bitmap->Width; x++)
  {
    for(int y = 0; y < bitmap->Height; y++)
    {
      int color = (int)bitmap->Canvas->Pixels[x][y];

      for(int j = 0; j < colorMap.mItems.Size(); j++)
      {
        CColorMapItem colorMapItem = colorMap.mItems.Get(j);

        if (color == colorMapItem.GetSrcColor())
        {
          bitmap->Canvas->Pixels[x][y] = (TColor)colorMapItem.GetDstColor();
        }
      }
    }
  }

  bitmap->Transparent      = cbColorMapTrans->Checked;
  bitmap->TransparentMode  = tmFixed;
  bitmap->TransparentColor = (TColor)mSprite.mImagesManager.Get(imageIndex).GetTransparentColor();

  int bitmapWidth  = bitmap->Width  * colorMapZoom / 100;
  int bitmapHeight = bitmap->Height * colorMapZoom / 100;

  paintColorMap->Canvas->StretchDraw(
    TRect(
      paintColorMap->Width / 2 - bitmapWidth / 2 - colorMapPan.x,
      paintColorMap->Height / 2 - bitmapHeight / 2 - colorMapPan.y,
      paintColorMap->Width / 2 + bitmapWidth / 2 - colorMapPan.x,
      paintColorMap->Height / 2 + bitmapHeight / 2 - colorMapPan.y),
    bitmap
  );

  DrawColorMapOverlay();

  //bitmap->Release();
  delete bitmap;
  */
}
//---------------------------------------------------------------------------

void TFormMain::DrawColorMapOverlay()
{
  // draw the canvas embelishments (axis, grid and so on) - extract to method
  if (cbColorMapAxis->Checked && !cbColorMapGrid->Checked)
  {
    paintColorMap->Canvas->Pen->Style = psSolid;
    paintColorMap->Canvas->Pen->Color = (TColor)mAppConfig.mColorMapAxisColor;

    paintColorMap->Canvas->MoveTo(paintColorMap->Width / 2 - colorMapPan.x, 0);
    paintColorMap->Canvas->LineTo(paintColorMap->Width / 2 - colorMapPan.x, paintColorMap->Height);

    paintColorMap->Canvas->MoveTo(0, paintColorMap->Height / 2 - colorMapPan.y);
    paintColorMap->Canvas->LineTo(paintColorMap->Width, paintColorMap->Height / 2 - colorMapPan.y);
  }

  if (cbColorMapGrid->Checked)
  {
    paintColorMap->Canvas->Pen->Style = psSolid;
    paintColorMap->Canvas->Pen->Color = (TColor)mAppConfig.mColorMapGridColor;

    for(int x = colorMapPan.x % mAppConfig.mColorMapGridXSpacing; x < paintColorMap->Width; x += mAppConfig.mColorMapGridXSpacing * colorMapZoom / 100)
    {
      paintColorMap->Canvas->MoveTo(x, 0);
      paintColorMap->Canvas->LineTo(x, paintColorMap->Height);
    }

    for(int y = colorMapPan.y % mAppConfig.mColorMapGridYSpacing; y < paintColorMap->Height; y += mAppConfig.mColorMapGridYSpacing * colorMapZoom / 100)
    {
      paintColorMap->Canvas->MoveTo(0, y);
      paintColorMap->Canvas->LineTo(paintColorMap->Width, y);
    }
  }
}
//---------------------------------------------------------------------------


void __fastcall TFormMain::paintColorMapMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
  //TODO
  ActiveControl = GroupBoxColorMapCanvas;

  if (Button == mbLeft && colorMapMouseMode == MMM_PAN)
  {
    // start to pan
    colorMapMouseMode = MMM_DO_PAN;
    oldColorMapPan.x = X;
    oldColorMapPan.y = Y;
  }
  
  if (Button == mbRight)
  {
    colorMapMouseMode = (colorMapMouseMode == MMM_PAN) ? MMM_PICK_COLOR : MMM_PAN;
    paintColorMap->Cursor = GetModuleCursor(colorMapMouseMode);
  }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::paintColorMapMouseMove(TObject *Sender,
      TShiftState Shift, int X, int Y)
{
  if (colorMapMouseMode == MMM_DO_PAN)
  {
    colorMapPan.x += -X + oldColorMapPan.x;
    colorMapPan.y += -Y + oldColorMapPan.y;

    oldColorMapPan.x = X;
    oldColorMapPan.y = Y;

    paintColorMapPaint(Sender);

    return;
  }

  /*
  fLogicAvailableMouseMode = GetFLogicMouseMode(TPoint(X, Y));

  if (fLogicAvailableMouseMode != MMM_NONE)
  {
    paintFrameLogic->Cursor = GetModuleCursor(fLogicAvailableMouseMode);
  }
  else
  {
    paintFrameLogic->Cursor = GetModuleCursor(fLogicMouseMode);
  }
  */
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::paintColorMapMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
  ActiveControl = GroupBoxColorMapCanvas;

  if (Button == mbLeft && colorMapMouseMode == MMM_DO_PAN)
  {
    colorMapMouseMode = MMM_PAN;
  }

  if (Button == mbLeft && colorMapMouseMode == MMM_PICK_COLOR)
  {
    if (gridColorMapItems->Row >= gridColorMapItems->RowCount - 1 ||
        gridColorMaps->Row >= gridColorMaps->RowCount - 1 ||
        gridImagesForColorMap->Row >= gridImagesForColorMap->RowCount)
    {
      return;
    }

    CImage image               = GetImageForColorMap();
    CColorMap colorMap         = GetColorMap();
    CColorMapItem colorMapItem = GetColorMapItem();

    // TODO: test this
    //Graphics::TBitmap* bitmap = ImageCache::Instance()->GetBitmap(mSprite.mImagesManager.Get(gridImagesForColorMap->Row - 1).GetFileName());
    Gdiplus::Bitmap* bitmap = ImageCache::Instance()->GetBitmap(mSprite.mImagesManager.Get(gridImagesForColorMap->Row - 1).GetFileName());

    if (!bitmap)
    {
      return;
    }

    // pick a color at the mouse position
    //int x = ((-paintColorMap->Width / 2 + X + colorMapPan.x) * 100) / colorMapZoom;
    //int y = ((-paintColorMap->Height / 2 + Y + colorMapPan.y) * 100) / colorMapZoom;
    int x = (-paintColorMap->Width / 2 + X + (bitmap->GetWidth() / 2) * colorMapZoom / 100 + colorMapPan.x) * 100 / colorMapZoom;
    int y = (-paintColorMap->Height / 2 + Y + (bitmap->GetHeight() / 2) * colorMapZoom / 100 + colorMapPan.y) * 100 / colorMapZoom;

    Gdiplus::Color color;

    bitmap->GetPixel(x, y, &color);

    if ((color.GetValue() == image.GetTransparentColor() && cbColorMapTrans->Checked == true) || image.GetTransparentColor() == -1)
    {
      return;
    }

    colorMapItem.SetSrcColor(color.GetValue());
    
    /*
    TColor color = bitmap->Canvas->Pixels[x][y];

    if ((int)color == image.GetTransparentColor() && cbColorMapTrans->Checked == true)
    {
      return;
    }

    colorMapItem.SetSrcColor((int)color);
    */

    // make the changes in the data structures
    colorMap.mItems.Set(gridColorMapItems->Row - 1, colorMapItem);
    image.mColorMaps.Set(gridColorMaps->Row - 1, colorMap);
    mSprite.mImagesManager.Set(gridImagesForColorMap->Row - 1, image);

    int oldRow = gridColorMapItems->Row;
    ColorMapItemsToGrid();
    gridColorMapItems->Row = oldRow;

    // update the edit box
    editColorMapItem->Text =
        AnsiString("s=") + "0x" + AnsiString::IntToHex(colorMapItem.GetSrcColor(), 6) + " " +
        AnsiString("d=") + "0x" + AnsiString::IntToHex(colorMapItem.GetDstColor(), 6) + " ";

    paintColorMapPaint(Sender);

    paintColorMap->Canvas->Font->Color = (TColor)0x00FF00;
    paintColorMap->Canvas->TextOut(0, 0, AnsiString(x) + ", " + AnsiString(y));
  }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::gridColorMapItemsDrawCell(TObject *Sender,
      int ACol, int ARow, TRect &Rect, TGridDrawState State)
{
  if (gridColorMaps->Row >= gridColorMaps->RowCount - 1 ||
      gridImagesForColorMap->Row >= gridImagesForColorMap->RowCount)
  {
    return;
  }

  if (State.Contains(gdFixed) || ACol == 0 || ARow == 0 || ARow >= gridColorMapItems->RowCount - 1)
  {
    return;
  }

  Rect.Left = Rect.Left + 9 * Rect.Width() / 10;

  CColorMap colorMap = GetColorMap();
  CColorMapItem colorMapItem = colorMap.mItems.Get(ARow - 1);

  int color = (ACol == 1) ? (TColor)colorMapItem.GetSrcColor() : (TColor)colorMapItem.GetDstColor();

  // clear the alpha component, because the TColor makes other use for those bytes
  color = color & 0x00FFFFFF;

  // invert the red and blue components (TColor and internal representation differ)
  color = RGB(GetBValue(color), GetGValue(color), GetRValue(color));

  TColor oldBrushColor = gridColorMapItems->Canvas->Brush->Color;

  gridColorMapItems->Canvas->Brush->Color = (TColor)color;
  gridColorMapItems->Canvas->FillRect(Rect);

  gridColorMapItems->Canvas->Brush->Color = oldBrushColor;
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ColorPickerClick(TObject *Sender)
{
  if (gridColorMapItems->Row >= gridColorMapItems->RowCount - 1 ||
      gridColorMaps->Row >= gridColorMaps->RowCount - 1 ||
      gridImagesForColorMap->Row >= gridImagesForColorMap->RowCount)
  {
    return;
  }

  if (ColorDialog->Execute())
  {
    CImage image               = GetImageForColorMap();
    CColorMap colorMap         = GetColorMap();
    CColorMapItem colorMapItem = GetColorMapItem();

    // TODO: swap red with blue and add the full alpha mask (0xFF)
    colorMapItem.SetDstColor(0xFF000000 | (int)ColorDialog->Color);

    colorMap.mItems.Set(gridColorMapItems->Row - 1, colorMapItem);
    image.mColorMaps.Set(gridColorMaps->Row - 1, colorMap);
    mSprite.mImagesManager.Set(gridImagesForColorMap->Row - 1, image);

    int oldRow = gridColorMapItems->Row;
    ColorMapItemsToGrid();
    gridColorMapItems->Row = oldRow;

    // update the edit box
    editColorMapItem->Text =
        AnsiString("s=") + "0x" + AnsiString::IntToHex(colorMapItem.GetSrcColor(), 6) + " " +
        AnsiString("d=") + "0x" + AnsiString::IntToHex(colorMapItem.GetDstColor(), 6) + " ";

    paintColorMapPaint(Sender);
  }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::scrollQuickFramesScroll(TObject *Sender,
      TScrollCode ScrollCode, int &ScrollPos)
{
  scrollQuickFrames->Position = ScrollPos;

  paintQuickFramesPaint(Sender);
}
//---------------------------------------------------------------------------

void TFormMain::DrawImage(Graphics::TCanvas* canvas, int imageIndex, float tx, float ty, float zoom, bool forceTransparent)
{
  Gdiplus::Bitmap* bitmap = ImageCache::Instance()->GetBitmap(mSprite.mImagesManager.Get(imageIndex).GetFileName());

  int bitmapWidth  = bitmap->GetWidth()  * zoom / 100;
  int bitmapHeight = bitmap->GetHeight() * zoom / 100;

  Gdiplus::Graphics graphics(canvas->Handle);

  graphics.SetSmoothingMode( Gdiplus::SmoothingModeNone );
  graphics.SetPixelOffsetMode( Gdiplus::PixelOffsetModeHalf );
  graphics.SetInterpolationMode( Gdiplus::InterpolationModeNearestNeighbor );

  int transparentColor = mSprite.mImagesManager.Get(imageIndex).GetTransparentColor();

  Gdiplus::Rect destRect(
    tx - bitmapWidth / 2,
    ty - bitmapHeight / 2,
    bitmapWidth,
    bitmapHeight
  );

  Gdiplus::ImageAttributes imAtt;

  if (transparentColor == -1 || !forceTransparent)
  {
  }
  else
  {
    imAtt.SetColorKey(
      Gdiplus::Color(GetRValue(transparentColor), GetGValue(transparentColor), GetBValue(transparentColor)),
      Gdiplus::Color(GetRValue(transparentColor), GetGValue(transparentColor), GetBValue(transparentColor)),
      Gdiplus::ColorAdjustTypeDefault);
  }

  graphics.DrawImage((Gdiplus::Image*)bitmap, destRect, 0, 0, bitmap->GetWidth(), bitmap->GetHeight(), Gdiplus::UnitPixel, &imAtt);
}
//---------------------------------------------------------------------------

