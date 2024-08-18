object FormMain: TFormMain
  Left = 245
  Top = 214
  Caption = 'XSprite'
  ClientHeight = 581
  ClientWidth = 854
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  Menu = MainMenu1
  OnResize = FormResize
  TextHeight = 13
  object GroupBox1: TGroupBox
    Left = 596
    Top = 0
    Width = 258
    Height = 562
    Align = alRight
    Caption = 'XSprite Tools'
    TabOrder = 0
    object GroupBox3: TGroupBox
      Left = 2
      Top = 15
      Width = 254
      Height = 82
      Align = alTop
      Caption = 'Cells'
      TabOrder = 0
      object cbCells: TComboBox
        Left = 8
        Top = 20
        Width = 237
        Height = 21
        Style = csDropDownList
        TabOrder = 0
        OnChange = cbCellsChange
      end
      object btnNewCell: TButton
        Left = 88
        Top = 48
        Width = 75
        Height = 25
        Caption = '&New'
        TabOrder = 1
        OnClick = btnNewCellClick
      end
      object btnEdit: TButton
        Left = 168
        Top = 48
        Width = 75
        Height = 25
        Caption = '&Edit'
        TabOrder = 2
        OnClick = btnEditClick
      end
    end
    object GroupBox4: TGroupBox
      Left = 2
      Top = 177
      Width = 254
      Height = 140
      Align = alTop
      Caption = 'Tools'
      TabOrder = 1
      object Label1: TLabel
        Left = 12
        Top = 28
        Width = 27
        Height = 13
        Caption = 'Zoom'
      end
      object Label2: TLabel
        Left = 12
        Top = 96
        Width = 45
        Height = 13
        Caption = 'Box Color'
      end
      object shapeBox: TShape
        Left = 12
        Top = 112
        Width = 16
        Height = 16
        Brush.Color = clRed
        OnMouseUp = shapeBoxMouseUp
      end
      object Label3: TLabel
        Left = 184
        Top = 96
        Width = 51
        Height = 13
        Alignment = taRightJustify
        Caption = 'Point Color'
      end
      object shapePoint: TShape
        Left = 216
        Top = 112
        Width = 16
        Height = 16
        OnMouseUp = shapePointMouseUp
      end
      object trackZoom: TTrackBar
        Left = 8
        Top = 44
        Width = 233
        Height = 45
        Max = 16
        Min = 1
        PageSize = 0
        Position = 2
        TabOrder = 0
        TickMarks = tmBoth
        OnChange = trackZoomChange
      end
    end
    object rgZone: TRadioGroup
      Left = 2
      Top = 97
      Width = 254
      Height = 80
      Align = alTop
      Caption = 'Cell Definition'
      ItemIndex = 0
      Items.Strings = (
        'Graphical and Collision Box'
        'Damage Point')
      TabOrder = 2
    end
  end
  object StatusBar1: TStatusBar
    Left = 0
    Top = 562
    Width = 854
    Height = 19
    Panels = <>
  end
  object GroupBox2: TGroupBox
    Left = 0
    Top = 0
    Width = 596
    Height = 562
    Align = alClient
    Caption = 'XSprite'
    TabOrder = 2
    object paintSprite: TPaintBox
      Left = 2
      Top = 15
      Width = 574
      Height = 527
      Align = alClient
      OnMouseDown = paintSpriteMouseDown
      OnMouseMove = paintSpriteMouseMove
      OnMouseUp = paintSpriteMouseUp
      OnPaint = paintSpritePaint
      ExplicitWidth = 582
      ExplicitHeight = 540
    end
    object sbH: TScrollBar
      Left = 2
      Top = 542
      Width = 592
      Height = 18
      Align = alBottom
      PageSize = 0
      TabOrder = 0
      OnChange = sbHChange
    end
    object sbV: TScrollBar
      Left = 576
      Top = 15
      Width = 18
      Height = 527
      Align = alRight
      Kind = sbVertical
      PageSize = 0
      TabOrder = 1
      OnChange = sbVChange
    end
  end
  object MainMenu1: TMainMenu
    Left = 8
    Top = 20
    object File1: TMenuItem
      Caption = '&File'
      object Open1: TMenuItem
        Caption = '&Open...'
        OnClick = Open1Click
      end
      object N2: TMenuItem
        Caption = '-'
      end
      object Export1: TMenuItem
        Caption = '&Export. as definition..'
        OnClick = Export1Click
      end
      object Exportassprite1: TMenuItem
        Caption = 'Export as sprite...'
        OnClick = Exportassprite1Click
      end
      object Exportasindexedsprite1: TMenuItem
        Caption = 'Export as 8-bit indexed sprite...'
        OnClick = Exportasindexedsprite1Click
      end
      object Exportas4bitindexedsprite1: TMenuItem
        Caption = 'Export as 4-bit indexed sprite...'
        OnClick = Exportas4bitindexedsprite1Click
      end
      object Exportas4bitindexedspriteactions1: TMenuItem
        Caption = 'Export as 4-bit indexed sprite&actions...'
        OnClick = Exportas4bitindexedspriteactions1Click
      end
      object N1: TMenuItem
        Caption = '-'
      end
      object Exit1: TMenuItem
        Caption = 'E&xit'
        OnClick = Exit1Click
      end
    end
    object Actions1: TMenuItem
      Caption = '&Actions'
      object ActionEditor1: TMenuItem
        Caption = '&Action Editor...'
        OnClick = ActionEditor1Click
      end
    end
    object Help1: TMenuItem
      Caption = '&Help'
      object About1: TMenuItem
        Caption = '&About...'
      end
    end
  end
  object dlgOpen: TOpenPictureDialog
    InitialDir = '.\data'
    Options = [ofHideReadOnly, ofPathMustExist, ofFileMustExist, ofEnableSizing]
    Left = 40
    Top = 20
  end
  object dlgColor: TColorDialog
    Left = 72
    Top = 20
  end
  object dlgSave: TSaveDialog
    DefaultExt = 'spr'
    Filter = 'Sprite Definition|*.sde|Sprite|*.spr|All Files|*.*'
    InitialDir = '.\data'
    Title = 'Export sprite definition'
    Left = 104
    Top = 20
  end
end
