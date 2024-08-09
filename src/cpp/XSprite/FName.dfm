object FormName: TFormName
  Left = 520
  Top = 325
  BorderIcons = []
  BorderStyle = bsSingle
  Caption = 'Name'
  ClientHeight = 78
  ClientWidth = 199
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 8
    Top = 20
    Width = 28
    Height = 13
    Caption = 'Name'
  end
  object edName: TEdit
    Left = 68
    Top = 12
    Width = 121
    Height = 21
    TabOrder = 0
    Text = 'NewName'
  end
  object btnOk: TButton
    Left = 16
    Top = 48
    Width = 75
    Height = 25
    Caption = '&Ok'
    ModalResult = 1
    TabOrder = 1
  end
  object btnCancel: TButton
    Left = 108
    Top = 48
    Width = 75
    Height = 25
    Caption = '&Cancel'
    ModalResult = 2
    TabOrder = 2
  end
end
