object FormOptions: TFormOptions
  Left = 746
  Top = 312
  Caption = 'Options'
  ClientHeight = 299
  ClientWidth = 403
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  Position = poMainFormCenter
  DesignSize = (
    403
    299)
  TextHeight = 13
  object PageControl1: TPageControl
    AlignWithMargins = True
    Left = 3
    Top = 3
    Width = 397
    Height = 261
    ActivePage = TabModules
    Align = alTop
    Anchors = [akLeft, akTop, akRight, akBottom]
    TabOrder = 0
    object TabGeneral: TTabSheet
      Caption = 'General'
    end
    object TabModules: TTabSheet
      Caption = 'Modules'
      ImageIndex = 1
      object Label1: TLabel
        Left = 4
        Top = 20
        Width = 88
        Height = 13
        Caption = 'Background Color:'
      end
      object Shape1: TShape
        Left = 156
        Top = 10
        Width = 24
        Height = 24
        OnMouseDown = Shape1MouseDown
      end
      object Label2: TLabel
        Left = 4
        Top = 48
        Width = 49
        Height = 13
        Caption = 'Axis Color:'
      end
      object Shape2: TShape
        Left = 156
        Top = 38
        Width = 24
        Height = 24
        OnMouseDown = Shape1MouseDown
      end
      object Label3: TLabel
        Left = 4
        Top = 76
        Width = 72
        Height = 13
        Caption = 'Selected Color:'
      end
      object Shape3: TShape
        Left = 156
        Top = 66
        Width = 24
        Height = 24
        OnMouseDown = Shape1MouseDown
      end
      object Label4: TLabel
        Left = 4
        Top = 192
        Width = 49
        Height = 13
        Caption = 'Grid Color:'
      end
      object Shape4: TShape
        Left = 156
        Top = 94
        Width = 24
        Height = 24
        OnMouseDown = Shape1MouseDown
      end
      object Label5: TLabel
        Left = 4
        Top = 104
        Width = 92
        Height = 13
        Caption = 'Not Selected Color:'
      end
      object Shape5: TShape
        Left = 156
        Top = 182
        Width = 24
        Height = 24
        OnMouseDown = Shape1MouseDown
      end
      object Label6: TLabel
        Left = 4
        Top = 132
        Width = 114
        Height = 13
        Caption = 'Grid Horizontal Spacing:'
      end
      object Label7: TLabel
        Left = 4
        Top = 160
        Width = 102
        Height = 13
        Caption = 'Grid Vertical Spacing:'
      end
      object Label8: TLabel
        Left = 212
        Top = 20
        Width = 117
        Height = 13
        Caption = 'Grid Selection Edge (px):'
      end
      object CSpinEdit1: TCSpinEdit
        Left = 128
        Top = 124
        Width = 53
        Height = 22
        TabOrder = 0
      end
      object CSpinEdit2: TCSpinEdit
        Left = 128
        Top = 152
        Width = 53
        Height = 22
        TabOrder = 1
      end
      object CSpinEdit3: TCSpinEdit
        Left = 336
        Top = 12
        Width = 53
        Height = 22
        TabOrder = 2
      end
    end
    object TabFrames: TTabSheet
      Caption = 'Frames'
      ImageIndex = 2
      object Label9: TLabel
        Left = 4
        Top = 20
        Width = 88
        Height = 13
        Caption = 'Background Color:'
      end
      object Shape6: TShape
        Left = 156
        Top = 10
        Width = 24
        Height = 24
        OnMouseDown = Shape1MouseDown
      end
      object Shape7: TShape
        Left = 156
        Top = 38
        Width = 24
        Height = 24
        OnMouseDown = Shape1MouseDown
      end
      object Label10: TLabel
        Left = 4
        Top = 48
        Width = 49
        Height = 13
        Caption = 'Axis Color:'
      end
      object Label11: TLabel
        Left = 4
        Top = 76
        Width = 114
        Height = 13
        Caption = 'Grid Horizontal Spacing:'
      end
      object Label12: TLabel
        Left = 4
        Top = 104
        Width = 102
        Height = 13
        Caption = 'Grid Vertical Spacing:'
      end
      object Label13: TLabel
        Left = 4
        Top = 136
        Width = 49
        Height = 13
        Caption = 'Grid Color:'
      end
      object Shape8: TShape
        Left = 156
        Top = 126
        Width = 24
        Height = 24
        OnMouseDown = Shape1MouseDown
      end
      object Label14: TLabel
        Left = 4
        Top = 192
        Width = 97
        Height = 13
        Caption = 'Quick Modules Size:'
      end
      object Label15: TLabel
        Left = 212
        Top = 20
        Width = 123
        Height = 13
        Caption = 'Quick Modules Bkg Color:'
      end
      object Shape10: TShape
        Left = 364
        Top = 10
        Width = 24
        Height = 24
        OnMouseDown = Shape1MouseDown
      end
      object Label16: TLabel
        Left = 212
        Top = 48
        Width = 137
        Height = 13
        Caption = 'Quick Modules Text Id Color:'
      end
      object Shape11: TShape
        Left = 364
        Top = 38
        Width = 24
        Height = 24
        OnMouseDown = Shape1MouseDown
      end
      object Label17: TLabel
        Left = 4
        Top = 164
        Width = 87
        Height = 13
        Caption = 'Bound Rect Color:'
      end
      object Shape9: TShape
        Left = 156
        Top = 154
        Width = 24
        Height = 24
        OnMouseDown = Shape1MouseDown
      end
      object CSpinEdit4: TCSpinEdit
        Left = 128
        Top = 68
        Width = 53
        Height = 22
        TabOrder = 0
      end
      object CSpinEdit5: TCSpinEdit
        Left = 128
        Top = 96
        Width = 53
        Height = 22
        TabOrder = 1
      end
      object CSpinEdit6: TCSpinEdit
        Left = 128
        Top = 184
        Width = 53
        Height = 22
        ReadOnly = True
        TabOrder = 2
      end
    end
    object TabAnims: TTabSheet
      Caption = 'Anims'
      ImageIndex = 3
      object Label18: TLabel
        Left = 4
        Top = 20
        Width = 88
        Height = 13
        Caption = 'Background Color:'
      end
      object Shape12: TShape
        Left = 156
        Top = 10
        Width = 24
        Height = 24
        OnMouseDown = Shape1MouseDown
      end
      object Shape13: TShape
        Left = 156
        Top = 38
        Width = 24
        Height = 24
        OnMouseDown = Shape1MouseDown
      end
      object Label19: TLabel
        Left = 4
        Top = 48
        Width = 49
        Height = 13
        Caption = 'Axis Color:'
      end
      object Label20: TLabel
        Left = 4
        Top = 76
        Width = 60
        Height = 13
        Caption = 'Select Color:'
      end
      object Shape14: TShape
        Left = 156
        Top = 66
        Width = 24
        Height = 24
        OnMouseDown = Shape1MouseDown
      end
      object Label21: TLabel
        Left = 4
        Top = 108
        Width = 114
        Height = 13
        Caption = 'Grid Horizontal Spacing:'
      end
      object Label22: TLabel
        Left = 4
        Top = 132
        Width = 102
        Height = 13
        Caption = 'Grid Vertical Spacing:'
      end
      object Label23: TLabel
        Left = 4
        Top = 164
        Width = 49
        Height = 13
        Caption = 'Grid Color:'
      end
      object Shape15: TShape
        Left = 156
        Top = 154
        Width = 24
        Height = 24
        OnMouseDown = Shape1MouseDown
      end
      object Label24: TLabel
        Left = 4
        Top = 192
        Width = 91
        Height = 13
        Caption = 'Quick Frames Size:'
      end
      object Label25: TLabel
        Left = 212
        Top = 20
        Width = 117
        Height = 13
        Caption = 'Quick Frames Bkg Color:'
      end
      object Label26: TLabel
        Left = 212
        Top = 48
        Width = 131
        Height = 13
        Caption = 'Quick Frames Text Id Color:'
      end
      object Shape16: TShape
        Left = 364
        Top = 10
        Width = 24
        Height = 24
        OnMouseDown = Shape1MouseDown
      end
      object Shape17: TShape
        Left = 364
        Top = 38
        Width = 24
        Height = 24
        OnMouseDown = Shape1MouseDown
      end
      object CSpinEdit7: TCSpinEdit
        Left = 128
        Top = 96
        Width = 53
        Height = 22
        TabOrder = 0
      end
      object CSpinEdit8: TCSpinEdit
        Left = 128
        Top = 124
        Width = 53
        Height = 22
        TabOrder = 1
      end
      object CSpinEdit9: TCSpinEdit
        Left = 128
        Top = 184
        Width = 53
        Height = 22
        ReadOnly = True
        TabOrder = 2
      end
    end
    object TabFLogic: TTabSheet
      Caption = 'Frame Logic'
      ImageIndex = 4
      object Label27: TLabel
        Left = 4
        Top = 20
        Width = 88
        Height = 13
        Caption = 'Background Color:'
      end
      object Shape18: TShape
        Left = 156
        Top = 10
        Width = 24
        Height = 24
        OnMouseDown = Shape1MouseDown
      end
      object Shape19: TShape
        Left = 156
        Top = 38
        Width = 24
        Height = 24
        OnMouseDown = Shape1MouseDown
      end
      object Label28: TLabel
        Left = 4
        Top = 48
        Width = 49
        Height = 13
        Caption = 'Axis Color:'
      end
      object Label29: TLabel
        Left = 4
        Top = 76
        Width = 60
        Height = 13
        Caption = 'Select Color:'
      end
      object Shape20: TShape
        Left = 156
        Top = 66
        Width = 24
        Height = 24
        OnMouseDown = Shape1MouseDown
      end
      object Label30: TLabel
        Left = 4
        Top = 108
        Width = 114
        Height = 13
        Caption = 'Grid Horizontal Spacing:'
      end
      object Label31: TLabel
        Left = 4
        Top = 132
        Width = 102
        Height = 13
        Caption = 'Grid Vertical Spacing:'
      end
      object Shape21: TShape
        Left = 156
        Top = 154
        Width = 24
        Height = 24
        OnMouseDown = Shape1MouseDown
      end
      object Label32: TLabel
        Left = 4
        Top = 164
        Width = 49
        Height = 13
        Caption = 'Grid Color:'
      end
      object Label33: TLabel
        Left = 4
        Top = 192
        Width = 80
        Height = 13
        Caption = 'Type Rect Color:'
      end
      object Shape22: TShape
        Left = 156
        Top = 182
        Width = 24
        Height = 24
        OnMouseDown = Shape1MouseDown
      end
      object Label34: TLabel
        Left = 212
        Top = 20
        Width = 81
        Height = 13
        Caption = 'Type Point Color:'
      end
      object Shape23: TShape
        Left = 364
        Top = 10
        Width = 24
        Height = 24
        OnMouseDown = Shape1MouseDown
      end
      object Label35: TLabel
        Left = 212
        Top = 48
        Width = 75
        Height = 13
        Caption = 'Selection Edge:'
      end
      object CSpinEdit10: TCSpinEdit
        Left = 128
        Top = 96
        Width = 53
        Height = 22
        TabOrder = 0
      end
      object CSpinEdit11: TCSpinEdit
        Left = 128
        Top = 124
        Width = 53
        Height = 22
        TabOrder = 1
      end
      object CSpinEdit12: TCSpinEdit
        Left = 336
        Top = 40
        Width = 53
        Height = 22
        TabOrder = 2
      end
    end
    object TabColorMap: TTabSheet
      Caption = 'Color Map'
      ImageIndex = 5
      object Label36: TLabel
        Left = 4
        Top = 136
        Width = 49
        Height = 13
        Caption = 'Grid Color:'
      end
      object Shape24: TShape
        Left = 156
        Top = 126
        Width = 24
        Height = 24
        OnMouseDown = Shape1MouseDown
      end
      object Label37: TLabel
        Left = 4
        Top = 104
        Width = 102
        Height = 13
        Caption = 'Grid Vertical Spacing:'
      end
      object Label38: TLabel
        Left = 4
        Top = 80
        Width = 114
        Height = 13
        Caption = 'Grid Horizontal Spacing:'
      end
      object Label39: TLabel
        Left = 4
        Top = 48
        Width = 49
        Height = 13
        Caption = 'Axis Color:'
      end
      object Shape25: TShape
        Left = 156
        Top = 38
        Width = 24
        Height = 24
        OnMouseDown = Shape1MouseDown
      end
      object Shape26: TShape
        Left = 156
        Top = 10
        Width = 24
        Height = 24
        OnMouseDown = Shape1MouseDown
      end
      object Label40: TLabel
        Left = 4
        Top = 20
        Width = 88
        Height = 13
        Caption = 'Background Color:'
      end
      object CSpinEdit13: TCSpinEdit
        Left = 128
        Top = 96
        Width = 53
        Height = 22
        TabOrder = 0
      end
      object CSpinEdit14: TCSpinEdit
        Left = 128
        Top = 68
        Width = 53
        Height = 22
        TabOrder = 1
      end
    end
    object Scripts: TTabSheet
      Caption = 'Scripts'
      ImageIndex = 6
      object Label41: TLabel
        Left = 3
        Top = 10
        Width = 62
        Height = 13
        Caption = 'Search Path:'
      end
      object edScriptsPath: TEdit
        Left = 68
        Top = 5
        Width = 247
        Height = 21
        Enabled = False
        TabOrder = 0
      end
      object btnBrowseScriptsPath: TButton
        Left = 318
        Top = 2
        Width = 75
        Height = 25
        Caption = 'Browse...'
        TabOrder = 1
        OnClick = btnBrowseScriptsPathClick
      end
    end
  end
  object BtnOk: TButton
    Left = 243
    Top = 270
    Width = 75
    Height = 25
    Anchors = [akRight, akBottom]
    Caption = '&Ok'
    ModalResult = 1
    TabOrder = 1
    OnClick = BtnOkClick
  end
  object BtnCancel: TButton
    Left = 323
    Top = 270
    Width = 75
    Height = 25
    Anchors = [akRight, akBottom]
    Caption = '&Cancel'
    ModalResult = 2
    TabOrder = 2
  end
  object btnReset: TButton
    Left = 4
    Top = 270
    Width = 75
    Height = 25
    Anchors = [akLeft, akBottom]
    Caption = '&Reset'
    TabOrder = 3
    OnClick = btnResetClick
  end
  object ColorDialog: TColorDialog
    Left = 372
    Top = 224
  end
  object OpenScriptsPathDialog: TOpenDialog
    Options = [ofHideReadOnly, ofPathMustExist, ofEnableSizing]
    Left = 340
    Top = 224
  end
end
