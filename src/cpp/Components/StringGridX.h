//---------------------------------------------------------------------------

#ifndef StringGridXH
#define StringGridXH
//---------------------------------------------------------------------------
#include <SysUtils.hpp>
#include <Classes.hpp>
#include <Controls.hpp>
#include <Grids.hpp>
//---------------------------------------------------------------------------
class PACKAGE TStringGridX : public TStringGrid
{
private:
    TPopupMenu* mPopMenu;

    void AddSep();

    void __fastcall PopupMenuNewClick(TObject* Sender);
    void __fastcall PopupMenuInsertClick(TObject* Sender);
    void __fastcall PopupMenuCloneClick(TObject* Sender);
    void __fastcall PopupMenuDeleteClick(TObject* Sender);
    void __fastcall PopupMenuCutClick(TObject* Sender);
    void __fastcall PopupMenuCopyClick(TObject* Sender);
    void __fastcall PopupMenuPasteClick(TObject* Sender);
    void __fastcall PopupMenuMoveUpClick(TObject* Sender);
    void __fastcall PopupMenuMoveDownClick(TObject* Sender);

// events
private:
	TNotifyEvent FOnPopNew;
    TNotifyEvent FOnPopInsert;
    TNotifyEvent FOnPopClone;
    TNotifyEvent FOnPopDelete;
    TNotifyEvent FOnPopCut;
    TNotifyEvent FOnPopCopy;
    TNotifyEvent FOnPopPaste;
    TNotifyEvent FOnPopMoveUp;
    TNotifyEvent FOnPopMoveDown;
    
protected:

public:
    __fastcall TStringGridX(TComponent* Owner);

__published:

    __property Classes::TNotifyEvent OnPopNew      = {read=FOnPopNew,      write=FOnPopNew};
    __property Classes::TNotifyEvent OnPopInsert   = {read=FOnPopInsert,   write=FOnPopInsert};
    __property Classes::TNotifyEvent OnPopClone    = {read=FOnPopClone,    write=FOnPopClone};
    __property Classes::TNotifyEvent OnPopDelete   = {read=FOnPopDelete,   write=FOnPopDelete};
    __property Classes::TNotifyEvent OnPopCut      = {read=FOnPopCut,      write=FOnPopCut};
    __property Classes::TNotifyEvent OnPopCopy     = {read=FOnPopCopy,     write=FOnPopCopy};
    __property Classes::TNotifyEvent OnPopPaste    = {read=FOnPopPaste,    write=FOnPopPaste};
    __property Classes::TNotifyEvent OnPopMoveUp   = {read=FOnPopMoveUp,   write=FOnPopMoveUp};
    __property Classes::TNotifyEvent OnPopMoveDown = {read=FOnPopMoveDown, write=FOnPopMoveDown};

};
//---------------------------------------------------------------------------
#endif
