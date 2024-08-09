//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FOptions.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "CSPIN"
#pragma resource "*.dfm"
TFormOptions *FormOptions;
//---------------------------------------------------------------------------
__fastcall TFormOptions::TFormOptions(TComponent* Owner)
    : TForm(Owner)
{
  mAppConfig = 0;
}
//---------------------------------------------------------------------------

void TFormOptions::AppConfigToGUI()
{
  // modules
  Shape1->Brush->Color   = (TColor)mAppConfig->mModulesBackgroundColor;
  Shape2->Brush->Color   = (TColor)mAppConfig->mModulesAxisColor;
  Shape3->Brush->Color   = (TColor)mAppConfig->mModulesSelectedColor;
  Shape4->Brush->Color   = (TColor)mAppConfig->mModulesNotSelectedColor;
  CSpinEdit1->Value      = mAppConfig->mModulesGridXSpacing;
  CSpinEdit2->Value      = mAppConfig->mModulesGridYSpacing;
  Shape5->Brush->Color   = (TColor)mAppConfig->mModulesGridColor;
  CSpinEdit3->Value      = mAppConfig->mModulesSelectionEdge;

  // frames
  Shape6->Brush->Color   = (TColor)mAppConfig->mFramesBackgroundColor;
  Shape7->Brush->Color   = (TColor)mAppConfig->mFramesAxisColor;
  CSpinEdit4->Value      = mAppConfig->mFramesGridXSpacing;
  CSpinEdit5->Value      = mAppConfig->mFramesGridYSpacing;
  Shape8->Brush->Color   = (TColor)mAppConfig->mFramesGridColor;
  Shape9->Brush->Color   = (TColor)mAppConfig->mFramesBoundRectColor;

  CSpinEdit6->Value      = mAppConfig->mQuickModuleSize;
  Shape10->Brush->Color  = (TColor)mAppConfig->mQuickModulesBackgroundColor;
  Shape11->Brush->Color  = (TColor)mAppConfig->mQuickModulesIdTextColor;

  // anims
  Shape12->Brush->Color  = (TColor)mAppConfig->mAnimsBackgroundColor;
  Shape13->Brush->Color  = (TColor)mAppConfig->mAnimsAxisColor;
  Shape14->Brush->Color  = (TColor)mAppConfig->mAnimsSelectColor;
  CSpinEdit7->Value      = mAppConfig->mAnimsGridXSpacing;
  CSpinEdit8->Value      = mAppConfig->mAnimsGridYSpacing;
  Shape15->Brush->Color  = (TColor)mAppConfig->mAnimsGridColor;

  CSpinEdit9->Value      = mAppConfig->mQuickFrameSize;
  Shape16->Brush->Color  = (TColor)mAppConfig->mQuickFramesBackgroundColor;
  Shape17->Brush->Color  = (TColor)mAppConfig->mQuickFramesIDTextColor;

  // frame logic
  Shape18->Brush->Color  = (TColor)mAppConfig->mFrameLogicBackgroundColor;
  Shape19->Brush->Color  = (TColor)mAppConfig->mFrameLogicAxisColor;
  Shape20->Brush->Color  = (TColor)mAppConfig->mFrameLogicSelectColor;
  CSpinEdit10->Value     = mAppConfig->mFrameLogicGridXSpacing;
  CSpinEdit11->Value     = mAppConfig->mFrameLogicGridYSpacing;
  Shape21->Brush->Color  = (TColor)mAppConfig->mFrameLogicGridColor;
  Shape22->Brush->Color  = (TColor)mAppConfig->mFrameLogicGridRectColor;
  Shape23->Brush->Color  = (TColor)mAppConfig->mFrameLogicGridPointColor;
  CSpinEdit12->Value     = mAppConfig->mFrameLogicSelectionEdge;

  // color map
  Shape26->Brush->Color = (TColor)mAppConfig->mColorMapBackgroundColor;
  Shape25->Brush->Color = (TColor)mAppConfig->mColorMapAxisColor;
  CSpinEdit14->Value    = mAppConfig->mColorMapGridXSpacing;
  CSpinEdit13->Value    = mAppConfig->mColorMapGridYSpacing;
  Shape24->Brush->Color = (TColor)mAppConfig->mColorMapGridColor;
}
//---------------------------------------------------------------------------

void TFormOptions::SetAppConfig(AppConfig& appConfig)
{
  mAppConfig = &appConfig;

  if (!mAppConfig)
  {
    return;
  }

  AppConfigToGUI();
}
//---------------------------------------------------------------------------

void __fastcall TFormOptions::Shape1MouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
  TShape* shape = dynamic_cast<TShape*>(Sender);

  if (shape)
  {
    /*
    ColorDialog->CustomColors->Add("ColorA=" +
      IntToHex(GetRValue(shape->Brush->Color), 2) +
      IntToHex(GetGValue(shape->Brush->Color), 2) +
      IntToHex(GetBValue(shape->Brush->Color), 2));
    */

    if (ColorDialog->Execute())
    {
        shape->Brush->Color = ColorDialog->Color;
    }
  }
}
//---------------------------------------------------------------------------

void __fastcall TFormOptions::BtnOkClick(TObject *Sender)
{
  if (!mAppConfig)
  {
    return;
  }

  // modules
  mAppConfig->mModulesBackgroundColor        = (int)Shape1->Brush->Color;
  mAppConfig->mModulesAxisColor              = (int)Shape2->Brush->Color;
  mAppConfig->mModulesSelectedColor          = (int)Shape3->Brush->Color;
  mAppConfig->mModulesNotSelectedColor       = (int)Shape4->Brush->Color;
  mAppConfig->mModulesGridXSpacing           = CSpinEdit1->Value;
  mAppConfig->mModulesGridYSpacing           = CSpinEdit2->Value;
  mAppConfig->mModulesGridColor              = (int)Shape5->Brush->Color;
  mAppConfig->mModulesSelectionEdge          = CSpinEdit3->Value;

  // frames
  mAppConfig->mFramesBackgroundColor         = (int)Shape6->Brush->Color;
  mAppConfig->mFramesAxisColor               = (int)Shape7->Brush->Color;
  mAppConfig->mFramesGridXSpacing            = CSpinEdit4->Value;
  mAppConfig->mFramesGridYSpacing            = CSpinEdit5->Value;
  mAppConfig->mFramesGridColor               = (int)Shape8->Brush->Color;
  mAppConfig->mFramesBoundRectColor          = (int)Shape9->Brush->Color;

  mAppConfig->mQuickModuleSize               = CSpinEdit6->Value;
  mAppConfig->mQuickModulesBackgroundColor   = (int)Shape10->Brush->Color;
  mAppConfig->mQuickModulesIdTextColor       = (int)Shape11->Brush->Color;

  // anims
  mAppConfig->mAnimsBackgroundColor          = (int)Shape12->Brush->Color;
  mAppConfig->mAnimsAxisColor                = (int)Shape13->Brush->Color;
  mAppConfig->mAnimsSelectColor              = (int)Shape14->Brush->Color;
  mAppConfig->mAnimsGridXSpacing             = CSpinEdit7->Value;
  mAppConfig->mAnimsGridYSpacing             = CSpinEdit8->Value;
  mAppConfig->mAnimsGridColor                = (int)Shape15->Brush->Color;

  mAppConfig->mQuickFrameSize                = CSpinEdit9->Value;
  mAppConfig->mQuickFramesBackgroundColor    = (int)Shape16->Brush->Color;
  mAppConfig->mQuickFramesIDTextColor        = (int)Shape17->Brush->Color;

  // framelogic
  mAppConfig->mFrameLogicBackgroundColor     = (int)Shape18->Brush->Color;
  mAppConfig->mFrameLogicAxisColor           = (int)Shape19->Brush->Color;
  mAppConfig->mFrameLogicSelectColor         = (int)Shape20->Brush->Color;
  mAppConfig->mFrameLogicGridXSpacing        = CSpinEdit10->Value;
  mAppConfig->mFrameLogicGridYSpacing        = CSpinEdit11->Value;
  mAppConfig->mFrameLogicGridColor           = (int)Shape21->Brush->Color;
  mAppConfig->mFrameLogicGridRectColor       = (int)Shape22->Brush->Color;
  mAppConfig->mFrameLogicGridPointColor      = (int)Shape23->Brush->Color;
  mAppConfig->mFrameLogicSelectionEdge       = CSpinEdit12->Value;

  // color map
  mAppConfig->mColorMapBackgroundColor       = (int)Shape26->Brush->Color;
  mAppConfig->mColorMapAxisColor             = (int)Shape25->Brush->Color;
  mAppConfig->mColorMapGridXSpacing          = CSpinEdit14->Value;
  mAppConfig->mColorMapGridYSpacing          = CSpinEdit13->Value;
  mAppConfig->mColorMapGridColor             = (int)Shape24->Brush->Color;
}
//---------------------------------------------------------------------------

void __fastcall TFormOptions::btnResetClick(TObject *Sender)
{
  if (Application->MessageBox("Reset the configuration to default settings?", "Reset Config", MB_OKCANCEL) == ID_OK)
  {
    mAppConfig->Default();
    AppConfigToGUI();
  }
}
//---------------------------------------------------------------------------

