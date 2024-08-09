object FormActions: TFormActions
  Left = 264
  Top = 408
  Width = 990
  Height = 543
  Caption = 'Actions'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  PopupMenu = popupMenu
  Position = poScreenCenter
  OnClose = FormClose
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object stringGrid: TStringGrid
    Left = 0
    Top = 0
    Width = 982
    Height = 516
    Align = alClient
    Color = clCream
    ColCount = 12
    FixedCols = 0
    RowCount = 2
    Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect, goColSizing, goEditing, goTabs]
    PopupMenu = popupMenu
    TabOrder = 0
    OnContextPopup = stringGridContextPopup
    ColWidths = (
      156
      45
      86
      99
      99
      80
      74
      73
      77
      76
      64
      64)
  end
  object popupMenu: TPopupMenu
    Left = 704
    Top = 456
    object Save1: TMenuItem
      Caption = '&Save...'
      OnClick = Save1Click
    end
    object N4: TMenuItem
      Caption = '-'
    end
    object Export1: TMenuItem
      Caption = '&Export...'
      OnClick = Export1Click
    end
    object N1: TMenuItem
      Caption = '-'
    end
    object Correct1: TMenuItem
      Caption = '&Correct'
      OnClick = Correct1Click
    end
    object N2: TMenuItem
      Caption = '-'
    end
    object PastefromClipboard1: TMenuItem
      Caption = '&Paste from Clipboard'
      ShortCut = 16470
      OnClick = PastefromClipboard1Click
    end
    object N3: TMenuItem
      Caption = '-'
    end
    object Addaction1: TMenuItem
      Caption = '&Add Action'
      OnClick = Addaction1Click
    end
    object Add10actions1: TMenuItem
      Caption = 'Add 10 Actions'
      OnClick = Add10actions1Click
    end
    object Removeaction1: TMenuItem
      Caption = '&Remove Action'
      OnClick = Removeaction1Click
    end
    object RemoveAllACtions1: TMenuItem
      Caption = 'Remove All Actions'
    end
  end
  object dlgSave: TSaveDialog
    DefaultExt = 'actions'
    Filter = 'Actions|*.actions|All Files|*.*'
    InitialDir = './data'
    Options = [ofHideReadOnly, ofNoChangeDir, ofPathMustExist, ofEnableSizing]
    Left = 704
    Top = 424
  end
  object dlgExport: TSaveDialog
    DefaultExt = 'export'
    Filter = 'Exported Actions File|*.export|All Files|*.*'
    InitialDir = './data'
    Options = [ofHideReadOnly, ofNoChangeDir, ofPathMustExist, ofEnableSizing]
    Left = 744
    Top = 424
  end
end
