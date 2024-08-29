//---------------------------------------------------------------------------

#ifndef FMainH
#define FMainH
#include "cspin.h"
#include "StringGridX.h"
#include <System.Classes.hpp>
#include <Vcl.Buttons.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.Dialogs.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.ExtDlgs.hpp>
#include <Vcl.Grids.hpp>
#include <Vcl.Menus.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.ToolWin.hpp>
//---------------------------------------------------------------------------
#include <algorithm>
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <Menus.hpp>
#include <ExtCtrls.hpp>
#include <ToolWin.hpp>
#include "StringGridX.h"
#include <Grids.hpp>
#include "XSpriteDesigner.h"
#include <Dialogs.hpp>
#include <ExtDlgs.hpp>
//---------------------------------------------------------------------------

#include "Defines.h"
#include "MouseWork.h"
#include "AppConfig.h"
#include <Buttons.hpp>
#include <MPlayer.hpp>
#include "CSPIN.h"
#include "Unit1.h"
#include <FileCtrl.hpp>
#include <gdiplus.h>
//---------------------------------------------------------------------------

using std::min;
using std::max;
//---------------------------------------------------------------------------

class TFormMain : public TForm
{
__published:	// IDE-managed Components
    TMainMenu *MainMenu;
    TStatusBar *StatusBar;
    TMenuItem *File1;
    TMenuItem *Tools1;
    TMenuItem *Help1;
    TMenuItem *Open1;
    TMenuItem *Save1;
    TMenuItem *Saveas1;
    TMenuItem *New1;
    TMenuItem *Close1;
    TMenuItem *Import1;
    TMenuItem *Export1;
    TMenuItem *N2;
    TMenuItem *Exit1;
    TMenuItem *About1;
    TMenuItem *N3;
    TMenuItem *Contents1;
    TMenuItem *Scripts1;
    TMenuItem *Edit1;
    TMenuItem *Undo1;
    TMenuItem *Redo1;
    TMenuItem *N4;
    TMenuItem *Copy1;
    TMenuItem *Cut1;
    TMenuItem *Paste1;
    TMenuItem *Delete1;
    TToolBar *ToolBar;
    TPanel *PanelSpriteEditor;
    TPageControl *PageControl1;
    TTabSheet *TabSheetModules;
    TTabSheet *TabSheetFrames;
    TTabSheet *TabSheetAnim;
    TTabSheet *TabSheetMapping;
    TMenuItem *N5;
    TMenuItem *Options1;
    TPanel *PanelModules;
    TPanel *PanelModulesCanvas;
    TSplitter *Splitter1;
    TGroupBox *GroupBoxModules;
    TGroupBox *GroupBoxImages;
    TSplitter *Splitter2;
    TGroupBox *GroupBoxCanvas;
    TPaintBox *paintModules;
    TEdit *edModules;
    TStringGridX *gridModules;
    TStringGridX *gridImages;
    TEdit *edImages;
    TOpenPictureDialog *OpenImage;
    TSaveDialog *SaveDialogXSprite;
    TOpenDialog *OpenDialogXSprite;
    TMemo *MemoMsg;
    TSplitter *Splitter4;
    TPanel *PanelFrames;
    TPanel *PanelFramesCanvas;
    TSplitter *Splitter5;
    TGroupBox *GroupBoxFrames;
    TSplitter *Splitter6;
    TGroupBox *GroupBoxFModules;
    TGroupBox *GroupBoxFramesCanvas;
    TSplitter *Splitter7;
    TGroupBox *GroupBoxModulesQuickSelection;
    TPaintBox *paintFrames;
    TStringGridX *gridFrames;
    TStringGridX *gridFModules;
    TEdit *edFrames;
    TEdit *edFModules;
    TPaintBox *paintQuickModules;
    TScrollBar *scrollQuickModules;
    TPanel *PanelAnims;
    TSplitter *Splitter8;
    TGroupBox *GroupBoxAnims;
    TStringGridX *gridAnims;
    TEdit *edAnims;
    TGroupBox *GroupBoxAFrames;
    TStringGridX *gridAFrames;
    TEdit *edAFrames;
    TSplitter *Splitter9;
    TPanel *Panel1;
    TSplitter *Splitter10;
    TGroupBox *GroupBoxAnimsCanvas;
    TPaintBox *paintAnims;
    TGroupBox *GroupBoxFramesQuickSelection;
    TPaintBox *paintQuickFrames;
    TScrollBar *scrollQuickFrames;
    TTabSheet *TabSheetColorMapping;
    TTimer *TimerAnim;
    TPanel *PanelModulesSetup;
    TCheckBox *cbModulesAxis;
    TCheckBox *cbModulesGrid;
    TCheckBox *cbModulesTrans;
    TCheckBox *cbModulesAll;
    TTrackBar *trackModulesZoom;
    TLabel *Label1;
    TPanel *Panel2;
    TLabel *Label2;
    TCheckBox *cbFramesAxis;
    TCheckBox *cbFramesGrid;
    TCheckBox *cbFramesTrans;
    TTrackBar *trackFramesZoom;
    TPanel *Panel3;
    TLabel *Label3;
    TCheckBox *cbAnimsAxis;
    TCheckBox *cbAnimsGrid;
    TCheckBox *cbAnimsTrans;
    TCheckBox *cbAnimsFMOffset;
    TTrackBar *trackAnimsZoom;
    TCheckBox *cbFramesOffset;
    TCheckBox *cbAnimsAFOffset;
    TSpeedButton *btnPlay;
    TSpeedButton *btnPause;
    TSpeedButton *btnRewind;
    TSpeedButton *btnFastFwd;
    TCheckBox *cbAnimsLoop;
    TCheckBox *cbAnimsReverse;
    TCSpinEdit *cbFPS;
    TLabel *Label4;
    TTabSheet *TabSheetFrameLogic;
    TPanel *PanelFramesLogic;
    TGroupBox *GroupBoxFrameLogic;
    TGroupBox *GroupBoxFrameLogicItem;
    TSplitter *SplitterFrameLogic3;
    TPanel *PanelFrameLogicCanvas;
    TGroupBox *GroupBoxFrameLogicCanvas;
    TPaintBox *paintFrameLogic;
    TGroupBox *GroupBoxFrameForLogic;
    TStringGrid *gridFramesForLogic;
    TSplitter *SplitterFrameLogic1;
    TSplitter *SplitterFrameLogic2;
    TStringGridX *gridFrameLogic;
    TStringGridX *gridFrameLogicItems;
    TEdit *edFrameLogic;
    TEdit *edFrameLogicItem;
    TPanel *Panel4;
    TLabel *Label5;
    TCheckBox *cbFrameLogicAxis;
    TCheckBox *cbFrameLogicGrid;
    TCheckBox *cbFrameLogicTrans;
    TTrackBar *trackFLogicZoom;
    TCheckBox *cbFrameLogicOffsetFM;
    TLabel *Label7;
    TMenuItem *N6;
    TMenuItem *OpenRecent1;
    TCheckBox *cbFramesBoundRect;
    TMenuItem *NoScriptsAvailable1;
    TMenuItem *N1;
    TMenuItem *Refresh1;
    TMenuItem *NoScriptsAvailable2;
    TMenuItem *N7;
    TMenuItem *Refresh2;
    TMenuItem *NoScriptsAvailable3;
    TMenuItem *N8;
    TMenuItem *Refresh3;
    TPanel *Panel6;
    TPanel *Panel5;
    TSplitter *Splitter3;
    TSplitter *Splitter11;
    TGroupBox *GroupBoxColorMaps;
    TStringGridX *gridColorMaps;
    TEdit *editColorMap;
    TGroupBox *GroupBoxColorMapItems;
    TStringGridX *gridColorMapItems;
    TEdit *editColorMapItem;
    TGroupBox *GroupBoxImagesForPalMap;
    TStringGrid *gridImagesForColorMap;
    TSplitter *Splitter12;
    TPanel *Panel7;
    TGroupBox *GroupBoxColorMapCanvas;
    TPaintBox *paintColorMap;
    TPanel *Panel8;
    TLabel *Label6;
    TCheckBox *cbColorMapAxis;
    TCheckBox *cbColorMapGrid;
    TCheckBox *cbColorMapTrans;
    TTrackBar *trackColorMapZoom;
    TColorDialog *ColorDialog;
    void __fastcall About1Click(TObject *Sender);
    void __fastcall Exit1Click(TObject *Sender);
    void __fastcall Splitter2Moved(TObject *Sender);
    void __fastcall gridImagesPopNew(TObject *Sender);
    void __fastcall gridImagesDblClick(TObject *Sender);
    void __fastcall gridModulesPopNew(TObject *Sender);
    void __fastcall gridModulesDblClick(TObject *Sender);
    void __fastcall gridModulesPopInsert(TObject *Sender);
    void __fastcall gridImagesPopInsert(TObject *Sender);
    void __fastcall gridModulesClick(TObject *Sender);
    void __fastcall gridImagesClick(TObject *Sender);
    void __fastcall paintModulesPaint(TObject *Sender);
    void __fastcall Save1Click(TObject *Sender);
    void __fastcall edModulesExit(TObject *Sender);
    void __fastcall edModulesKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
    void __fastcall edImagesExit(TObject *Sender);
    void __fastcall edImagesKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
    void __fastcall paintModulesMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall paintModulesMouseMove(TObject *Sender,
          TShiftState Shift, int X, int Y);
    void __fastcall paintModulesMouseUp(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall gridModulesPopMoveUp(TObject *Sender);
    void __fastcall gridModulesPopMoveDown(TObject *Sender);
    void __fastcall FormMouseWheelUp(TObject *Sender, TShiftState Shift,
          TPoint &MousePos, bool &Handled);
    void __fastcall FormMouseWheelDown(TObject *Sender, TShiftState Shift,
          TPoint &MousePos, bool &Handled);
    void __fastcall Open1Click(TObject *Sender);
    void __fastcall gridModulesPopClone(TObject *Sender);
    void __fastcall gridImagesPopMoveDown(TObject *Sender);
    void __fastcall gridImagesPopMoveUp(TObject *Sender);
    void __fastcall gridImagesPopClone(TObject *Sender);
    void __fastcall Splitter4Moved(TObject *Sender);
    void __fastcall Splitter6Moved(TObject *Sender);
    void __fastcall Splitter5Moved(TObject *Sender);
    void __fastcall Splitter7Moved(TObject *Sender);
    void __fastcall gridFramesClick(TObject *Sender);
    void __fastcall gridFramesDblClick(TObject *Sender);
    void __fastcall gridFramesPopNew(TObject *Sender);
    void __fastcall gridFramesPopClone(TObject *Sender);
    void __fastcall gridFramesPopInsert(TObject *Sender);
    void __fastcall gridFramesPopMoveDown(TObject *Sender);
    void __fastcall gridFramesPopMoveUp(TObject *Sender);
    void __fastcall edFramesKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
    void __fastcall edFramesExit(TObject *Sender);
    void __fastcall paintFramesPaint(TObject *Sender);
    void __fastcall gridFramesSelectCell(TObject *Sender, int ACol,
          int ARow, bool &CanSelect);
    void __fastcall gridFModulesClick(TObject *Sender);
    void __fastcall gridFModulesPopNew(TObject *Sender);
    void __fastcall paintQuickModulesPaint(TObject *Sender);
    void __fastcall gridFModulesDblClick(TObject *Sender);
    void __fastcall edFModulesExit(TObject *Sender);
    void __fastcall edFModulesKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
    void __fastcall gridFModulesPopClone(TObject *Sender);
    void __fastcall gridFModulesPopInsert(TObject *Sender);
    void __fastcall gridFModulesPopMoveDown(TObject *Sender);
    void __fastcall gridFModulesPopMoveUp(TObject *Sender);
    void __fastcall paintFramesMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall paintFramesMouseUp(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall paintFramesMouseMove(TObject *Sender,
          TShiftState Shift, int X, int Y);
    void __fastcall Splitter8Moved(TObject *Sender);
    void __fastcall Splitter9Moved(TObject *Sender);
    void __fastcall Splitter10Moved(TObject *Sender);
    void __fastcall TabSheetFramesShow(TObject *Sender);
    void __fastcall scrollQuickModulesScroll(TObject *Sender,
          TScrollCode ScrollCode, int &ScrollPos);
    void __fastcall FormResize(TObject *Sender);
    void __fastcall paintQuickModulesDblClick(TObject *Sender);
    void __fastcall gridAnimsDblClick(TObject *Sender);
    void __fastcall gridAnimsPopNew(TObject *Sender);
    void __fastcall gridAnimsClick(TObject *Sender);
    void __fastcall paintAnimsPaint(TObject *Sender);
    void __fastcall gridAnimsPopClone(TObject *Sender);
    void __fastcall gridAnimsPopInsert(TObject *Sender);
    void __fastcall gridAnimsPopMoveDown(TObject *Sender);
    void __fastcall gridAnimsPopMoveUp(TObject *Sender);
    void __fastcall gridAnimsSelectCell(TObject *Sender, int ACol,
          int ARow, bool &CanSelect);
    void __fastcall gridAFramesClick(TObject *Sender);
    void __fastcall gridAFramesDblClick(TObject *Sender);
    void __fastcall gridAFramesPopClone(TObject *Sender);
    void __fastcall gridAFramesPopInsert(TObject *Sender);
    void __fastcall gridAFramesPopMoveDown(TObject *Sender);
    void __fastcall gridAFramesPopMoveUp(TObject *Sender);
    void __fastcall gridAFramesPopNew(TObject *Sender);
    void __fastcall edAnimsExit(TObject *Sender);
    void __fastcall edAnimsKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
    void __fastcall edAFramesKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
    void __fastcall edAFramesExit(TObject *Sender);
    void __fastcall paintAnimsMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall paintAnimsMouseMove(TObject *Sender, TShiftState Shift,
          int X, int Y);
    void __fastcall paintAnimsMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
    void __fastcall paintQuickFramesDblClick(TObject *Sender);
    void __fastcall paintQuickFramesPaint(TObject *Sender);
    void __fastcall TabSheetAnimShow(TObject *Sender);
    void __fastcall TimerAnimTimer(TObject *Sender);
    void __fastcall cbModulesAxisClick(TObject *Sender);
    void __fastcall trackModulesZoomChange(TObject *Sender);
    void __fastcall cbModulesTransClick(TObject *Sender);
    void __fastcall cbModulesAllClick(TObject *Sender);
    void __fastcall cbModulesGridClick(TObject *Sender);
    void __fastcall cbFramesAxisClick(TObject *Sender);
    void __fastcall cbFramesGridClick(TObject *Sender);
    void __fastcall cbFramesTransClick(TObject *Sender);
    void __fastcall cbFramesAllClick(TObject *Sender);
    void __fastcall cbFramesOffsetClick(TObject *Sender);
    void __fastcall cbFPSChange(TObject *Sender);
    void __fastcall btnPlayClick(TObject *Sender);
    void __fastcall btnPauseClick(TObject *Sender);
    void __fastcall btnFastFwdClick(TObject *Sender);
    void __fastcall btnRewindClick(TObject *Sender);
    void __fastcall trackAnimsZoomChange(TObject *Sender);
    void __fastcall cbAnimsAxisClick(TObject *Sender);
    void __fastcall cbAnimsGridClick(TObject *Sender);
    void __fastcall cbAnimsTransClick(TObject *Sender);
    void __fastcall cbAnimsFMOffsetClick(TObject *Sender);
    void __fastcall cbAnimsAFOffsetClick(TObject *Sender);
    void __fastcall SplitterFrameLogic1Moved(TObject *Sender);
    void __fastcall SplitterFrameLogic2Moved(TObject *Sender);
    void __fastcall SplitterFrameLogic3Moved(TObject *Sender);
    void __fastcall gridFrameLogicDblClick(TObject *Sender);
    void __fastcall gridFrameLogicPopNew(TObject *Sender);
    void __fastcall gridFrameLogicClick(TObject *Sender);
    void __fastcall gridFramesForLogicClick(TObject *Sender);
    void __fastcall paintFrameLogicPaint(TObject *Sender);
    void __fastcall gridFrameLogicPopClone(TObject *Sender);
    void __fastcall gridFrameLogicPopInsert(TObject *Sender);
    void __fastcall edFrameLogicExit(TObject *Sender);
    void __fastcall gridFrameLogicPopMoveDown(TObject *Sender);
    void __fastcall gridFrameLogicPopMoveUp(TObject *Sender);
    void __fastcall edFrameLogicKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
    void __fastcall gridFrameLogicItemsClick(TObject *Sender);
    void __fastcall gridFrameLogicItemsPopNew(TObject *Sender);
    void __fastcall gridFrameLogicItemsDblClick(TObject *Sender);
    void __fastcall gridFrameLogicItemsPopInsert(TObject *Sender);
    void __fastcall gridFrameLogicItemsPopMoveDown(TObject *Sender);
    void __fastcall gridFrameLogicItemsPopMoveUp(TObject *Sender);
    void __fastcall gridFrameLogicItemsPopClone(TObject *Sender);
    void __fastcall edFrameLogicItemExit(TObject *Sender);
    void __fastcall edFrameLogicItemKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
    void __fastcall paintFrameLogicMouseMove(TObject *Sender,
          TShiftState Shift, int X, int Y);
    void __fastcall cbFrameLogicAxisClick(TObject *Sender);
    void __fastcall cbFrameLogicGridClick(TObject *Sender);
    void __fastcall cbFrameLogicTransClick(TObject *Sender);
    void __fastcall cbFrameLogicOffsetFMClick(TObject *Sender);
    void __fastcall trackFLogicZoomChange(TObject *Sender);
    void __fastcall paintFrameLogicMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall paintFrameLogicMouseUp(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall Saveas1Click(TObject *Sender);
    void __fastcall trackFramesZoomChange(TObject *Sender);
    void __fastcall Close1Click(TObject *Sender);
    void __fastcall gridModulesPopDelete(TObject *Sender);
    void __fastcall gridImagesPopDelete(TObject *Sender);
    void __fastcall gridFramesPopDelete(TObject *Sender);
    void __fastcall gridFModulesPopDelete(TObject *Sender);
    void __fastcall gridAnimsPopDelete(TObject *Sender);
    void __fastcall gridAFramesPopDelete(TObject *Sender);
    void __fastcall gridFrameLogicPopDelete(TObject *Sender);
    void __fastcall gridFrameLogicItemsPopDelete(TObject *Sender);
    void __fastcall paintQuickModulesMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall paintQuickFramesMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall Options1Click(TObject *Sender);
    void __fastcall New1Click(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall TabSheetFrameLogicShow(TObject *Sender);
    void __fastcall paintColorMapPaint(TObject *Sender);
    void __fastcall Splitter3Moved(TObject *Sender);
    void __fastcall Splitter11Moved(TObject *Sender);
    void __fastcall Splitter12Moved(TObject *Sender);
    void __fastcall gridImagesForColorMapClick(TObject *Sender);
    void __fastcall gridColorMapsClick(TObject *Sender);
    void __fastcall gridColorMapsDblClick(TObject *Sender);
    void __fastcall gridColorMapsPopClone(TObject *Sender);
    void __fastcall gridColorMapsPopNew(TObject *Sender);
    void __fastcall gridColorMapsPopDelete(TObject *Sender);
    void __fastcall gridColorMapsPopInsert(TObject *Sender);
    void __fastcall gridColorMapsPopMoveDown(TObject *Sender);
    void __fastcall gridColorMapsPopMoveUp(TObject *Sender);
    void __fastcall editColorMapKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
    void __fastcall editColorMapExit(TObject *Sender);
    void __fastcall gridColorMapItemsClick(TObject *Sender);
    void __fastcall gridColorMapItemsDblClick(TObject *Sender);
    void __fastcall gridColorMapItemsPopClone(TObject *Sender);
    void __fastcall gridColorMapItemsPopNew(TObject *Sender);
    void __fastcall TabSheetColorMappingShow(TObject *Sender);
    void __fastcall gridColorMapItemsPopDelete(TObject *Sender);
    void __fastcall gridColorMapItemsPopInsert(TObject *Sender);
    void __fastcall gridColorMapItemsPopMoveDown(TObject *Sender);
    void __fastcall gridColorMapItemsPopMoveUp(TObject *Sender);
    void __fastcall editColorMapItemKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
    void __fastcall editColorMapItemExit(TObject *Sender);
    void __fastcall cbColorMapAxisClick(TObject *Sender);
    void __fastcall cbColorMapGridClick(TObject *Sender);
    void __fastcall cbColorMapTransClick(TObject *Sender);
    void __fastcall trackColorMapZoomChange(TObject *Sender);
    void __fastcall paintColorMapMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall paintColorMapMouseMove(TObject *Sender,
          TShiftState Shift, int X, int Y);
    void __fastcall paintColorMapMouseUp(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall gridColorMapItemsDrawCell(TObject *Sender, int ACol,
          int ARow, TRect &Rect, TGridDrawState State);
    void __fastcall scrollQuickFramesScroll(TObject *Sender,
          TScrollCode ScrollCode, int &ScrollPos);

private:	// User declarations
    AnsiString appPath;

    int modulesZoom;
    TPoint modulesPan, oldModulesPan, modulesDefine, modulesCurrentDefine;
    TMouseMode moduleMouseMode, moduleAvailableMouseMode;

    int framesZoom;
    TPoint framesPan, oldFramesPan, framesDefine, framesCurrentDefine;
    TMouseMode frameMouseMode, frameAvailableMouseMode;

    int animsZoom;
    TPoint animsPan, oldAnimsPan, animsDefine, animsCurrentDefine;
    TMouseMode animMouseMode, animAvailableMouseMode;

    int fLogicZoom;
    TPoint fLogicPan, oldFLogicPan, fLogicDefine, fLogicCurrentDefine;
    TMouseMode fLogicMouseMode, fLogicAvailableMouseMode;

    int colorMapZoom;
    TPoint colorMapPan, oldColorMapPan;
    TMouseMode colorMapMouseMode, colorMapAvailableMouseMode;

    AppConfig mAppConfig;
    CXSpriteDesigner mSprite;

    TPoint mQuickModulePos;
    TPoint mQuickFramePos;

    void ArrangeModulesPageItems();
    void ArrangeFramesPageItems();
    void ArrangeAnimsPageItems();
    void ArrangeFrameLogicPageItems();
    void ArrangeColorMapPageItems();

    void ImagesToGrid();
    void ModulesToGrid();
    void PalsToGrid();

    void FramesToGrid();
    void FModulesToGrid();

    void AnimsToGrid();
    void AFramesToGrid();

    void FramesForLogicToGrid();
    void FrameLogicToGrid();
    void FrameLogicItemsToGrid();

    void ImagesForColorMapToGrid();
    void ColorMapToGrid();
    void ColorMapItemsToGrid();

    bool PointInRect(TPoint p, TRect r);

    TMouseMode GetModuleMouseMode(TPoint p);
    TMouseMode GetFrameMouseMode(TPoint p);
    TMouseMode GetAnimMouseMode(TPoint p);
    TMouseMode GetFLogicMouseMode(TPoint p);
    TMouseMode GetColorMapMouseMode(TPoint p);

    TCursor GetModuleCursor(TMouseMode mmm);

    void SetQuickModulesScroll();
    void SetQuickFramesScroll();

    void LoadConfig();
    void SaveConfig();
    void GUIDefaultConfig();
    void GUIFromDocState(TSpriteDocState state);
    void MakeHistory();
    void __fastcall HistoryClick(TObject *Sender);
    void LoadDocument(std::string fileName);

    void DrawModulesOverlay();
    void DrawColorMapOverlay();

    void RefreshAllScripts();
    void RefreshScripts(TMenuItem* menuItem, std::string path);
    void __fastcall RefreshScriptsClick(TObject *Sender);
    void __fastcall OnScriptMenuItemClick(TObject *Sender);
    void __fastcall ColorPickerClick(TObject *Sender);

    void __fastcall DisplayHint(TObject* Sender);

private: // Utilities
    CImage GetImageForColorMap();
    CColorMap GetColorMap();
    CColorMapItem GetColorMapItem();

    void DrawImage(Graphics::TCanvas* canvas, int imageIndex, float tx, float ty, float zoom, bool forceTransparent);

private: // Gdiplus interface
    Gdiplus::GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR  gdiplusToken;
    Gdiplus::Graphics* graphicsModules;

public:		// User declarations
    __fastcall TFormMain(TComponent* Owner);
    __fastcall ~TFormMain();
};
//---------------------------------------------------------------------------
extern PACKAGE TFormMain *FormMain;
//---------------------------------------------------------------------------
#endif
