object FormVisualFactory: TFormVisualFactory
  Left = 491
  Top = 265
  BorderStyle = bsSingle
  Caption = 'Select Curve Type to Create'
  ClientHeight = 123
  ClientWidth = 237
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  TextHeight = 13
  object Label1: TLabel
    Left = 8
    Top = 40
    Width = 58
    Height = 13
    Caption = 'Curve Type:'
  end
  object btnOk: TButton
    Left = 41
    Top = 96
    Width = 75
    Height = 25
    Caption = '&Ok'
    ModalResult = 1
    TabOrder = 0
    OnClick = btnOkClick
  end
  object btnCancel: TButton
    Left = 121
    Top = 96
    Width = 75
    Height = 25
    Caption = '&Cancel'
    ModalResult = 2
    TabOrder = 1
  end
  object cbType: TComboBox
    Left = 80
    Top = 32
    Width = 145
    Height = 21
    Style = csDropDownList
    TabOrder = 2
    Items.Strings = (
      'Linear'
      'Elipsoid'
      'Scripted'
      'Composite'
      'Polynome')
  end
end
