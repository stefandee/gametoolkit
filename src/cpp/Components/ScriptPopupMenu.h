//---------------------------------------------------------------------------

#ifndef ScriptPopupMenuH
#define ScriptPopupMenuH
//---------------------------------------------------------------------------
#include <SysUtils.hpp>
#include <Controls.hpp>
#include <Classes.hpp>
#include <Forms.hpp>
#include <Menus.hpp>
//---------------------------------------------------------------------------
class PACKAGE TScriptPopupMenu : public TPopupMenu
{
private:
  TMenuItem* mUndo, *mSelect, *mClear, *mCut, *mCopy, *mPaste, *mLoad, *mSave; 

  TOpenDialog* mOpenDialog;
  TSaveDialog* mSaveDialog;

  void LoadFromFile(TMemo* _memo);
  void SaveToFile(TMemo* _memo);
  void __fastcall PopupMenuItemsClick(TObject *Sender);
  void __fastcall PopupMenuOnPopup(TObject *Sender);

protected:
public:
  __fastcall TScriptPopupMenu(TComponent* Owner);
  __fastcall virtual ~TScriptPopupMenu();

  void SetInitialDir(AnsiString _v);
  void SetFilter(AnsiString _v);
  void SetDefaultExt(AnsiString _v);

__published:
};
//---------------------------------------------------------------------------
#endif
