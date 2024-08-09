object FormEditObjectStream: TFormEditObjectStream
  Left = 573
  Top = 286
  Caption = 'Object Stream Editor'
  ClientHeight = 576
  ClientWidth = 447
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  Menu = MainMenu
  TextHeight = 13
  object ToolBar: TToolBar
    Left = 0
    Top = 0
    Width = 447
    Height = 29
    Caption = 'ToolBar'
    TabOrder = 0
    ExplicitWidth = 443
    object ToolButton1: TToolButton
      Left = 0
      Top = 0
      Caption = 'Erase'
      ImageIndex = 0
      ParentShowHint = False
      ShowHint = False
    end
    object ToolButton2: TToolButton
      Left = 23
      Top = 0
      Caption = 'ToolButton2'
      ImageIndex = 1
    end
    object ToolButton3: TToolButton
      Left = 46
      Top = 0
      Caption = 'ToolButton3'
      ImageIndex = 2
    end
  end
  object StatusBar: TStatusBar
    Left = 0
    Top = 557
    Width = 447
    Height = 19
    Panels = <
      item
        Width = 50
      end
      item
        Width = 50
      end>
    ExplicitTop = 556
    ExplicitWidth = 443
  end
  object StringGrid: TStringGrid
    Left = 0
    Top = 29
    Width = 447
    Height = 528
    Align = alClient
    FixedCols = 0
    Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect, goColSizing, goRowSelect]
    PopupMenu = PopupMenu
    TabOrder = 2
    ExplicitWidth = 443
    ExplicitHeight = 527
    ColWidths = (
      19
      78
      74
      73
      64)
  end
  object MainMenu: TMainMenu
    Left = 360
    Top = 8
    object File1: TMenuItem
      Caption = 'File'
      ShortCut = 49240
      object New1: TMenuItem
        Caption = '&New'
        OnClick = New1Click
      end
      object Open1: TMenuItem
        Caption = '&Open...'
        OnClick = Open1Click
      end
      object OpenRecent1: TMenuItem
        Caption = '&Open Recent'
      end
      object N5: TMenuItem
        Caption = '-'
      end
      object Save1: TMenuItem
        Caption = '&Save'
        OnClick = Save1Click
      end
      object SaveAs2: TMenuItem
        Caption = 'Save As...'
        OnClick = SaveAs2Click
      end
      object N2: TMenuItem
        Caption = '-'
      end
      object Exit1: TMenuItem
        Caption = 'E&xit'
        ShortCut = 49240
        OnClick = Exit1Click
      end
    end
    object About1: TMenuItem
      Caption = 'Help'
      object About2: TMenuItem
        Caption = 'About...'
        OnClick = About2Click
      end
    end
  end
  object PopupMenu: TPopupMenu
    Left = 360
    Top = 40
    object Edit1: TMenuItem
      Caption = '&Edit'
      OnClick = Edit1Click
    end
    object N3: TMenuItem
      Caption = '-'
    end
    object AddRecord1: TMenuItem
      Caption = '&Add Record...'
      OnClick = AddRecord1Click
    end
    object InsertRecord1: TMenuItem
      Caption = '&Insert Record...'
      OnClick = InsertRecord1Click
    end
    object RemoveRecord1: TMenuItem
      Caption = '&Remove Record'
      OnClick = RemoveRecord1Click
    end
    object Clear1: TMenuItem
      Caption = 'Cl&ear'
      OnClick = Clear1Click
    end
    object N1: TMenuItem
      Caption = '-'
    end
    object Save2: TMenuItem
      Caption = '&Save'
      OnClick = Save2Click
    end
    object SaveAs1: TMenuItem
      Caption = 'Sa&ve As...'
    end
    object Load1: TMenuItem
      Caption = '&Load...'
      OnClick = Load1Click
    end
  end
  object OpenDialog: TOpenDialog
    Left = 360
    Top = 72
  end
  object SaveDialog: TSaveDialog
    Left = 360
    Top = 104
  end
end
