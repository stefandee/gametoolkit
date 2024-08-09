object FormMain: TFormMain
  Left = 611
  Top = 267
  Caption = 'Data Growth Tool'
  ClientHeight = 512
  ClientWidth = 303
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  Menu = MainMenu
  TextHeight = 13
  object Bevel1: TBevel
    Left = 0
    Top = 0
    Width = 303
    Height = 2
    Align = alTop
    Shape = bsTopLine
    ExplicitWidth = 315
  end
  object StatusBar1: TStatusBar
    Left = 0
    Top = 493
    Width = 303
    Height = 19
    Panels = <>
    ExplicitTop = 494
    ExplicitWidth = 307
  end
  object GroupBox1: TGroupBox
    Left = 0
    Top = 2
    Width = 303
    Height = 491
    Align = alClient
    Caption = 'File list'
    TabOrder = 1
    ExplicitWidth = 307
    ExplicitHeight = 492
    object Grid: TStringGrid
      Left = 2
      Top = 15
      Width = 303
      Height = 475
      Align = alClient
      ColCount = 3
      FixedCols = 0
      RowCount = 4
      Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect, goRowSelect]
      TabOrder = 0
      ColWidths = (
        97
        110
        64)
    end
  end
  object MainMenu: TMainMenu
    Left = 280
    Top = 8
    object File1: TMenuItem
      Caption = '&File'
      object NewDataFile1: TMenuItem
        Caption = 'New Data File'
        OnClick = NewDataFile1Click
      end
      object N3: TMenuItem
        Caption = '-'
      end
      object Open1: TMenuItem
        Caption = '&Open Data File...'
        OnClick = Open1Click
      end
      object N2: TMenuItem
        Caption = '-'
      end
      object Save1: TMenuItem
        Caption = '&Save Data File'
        OnClick = Save1Click
      end
      object SaveAs1: TMenuItem
        Caption = '&Save Data File As...'
        OnClick = SaveAs1Click
      end
      object N4: TMenuItem
        Caption = '-'
      end
      object Close1: TMenuItem
        Caption = '&Close'
        OnClick = Close1Click
      end
      object N1: TMenuItem
        Caption = '-'
      end
      object Exit1: TMenuItem
        Caption = 'E&xit'
      end
    end
    object Tool1: TMenuItem
      Caption = '&Entries'
      object Addfile1: TMenuItem
        Caption = '&Add file...'
        OnClick = Addfile1Click
      end
      object ReplaceEntry1: TMenuItem
        Caption = '&Replace Entry...'
        OnClick = ReplaceEntry1Click
      end
      object Deleteentry1: TMenuItem
        Caption = '&Delete Selected'
        OnClick = Deleteentry1Click
      end
    end
    object Help1: TMenuItem
      Caption = '&Help'
      object About1: TMenuItem
        Caption = '&About...'
      end
    end
  end
  object PopupMenuList: TPopupMenu
    Left = 248
    Top = 8
    object AddFile2: TMenuItem
      Caption = 'Add File...'
    end
    object DeleteEntry2: TMenuItem
      Caption = 'Delete Entry'
    end
  end
  object OpenDialog: TOpenDialog
    Filter = 'All Files|*.*'
    Options = [ofHideReadOnly, ofNoChangeDir, ofAllowMultiSelect, ofPathMustExist, ofFileMustExist, ofEnableSizing]
    Left = 248
    Top = 40
  end
  object SaveDialog: TSaveDialog
    DefaultExt = 'data'
    Filter = 'Data File|*.data|All Files|*.*'
    Left = 280
    Top = 40
  end
  object OpenDataDialog: TOpenDialog
    Filter = 'All Files|*.*'
    Options = [ofHideReadOnly, ofAllowMultiSelect, ofPathMustExist, ofFileMustExist, ofEnableSizing]
    Left = 248
    Top = 72
  end
end
