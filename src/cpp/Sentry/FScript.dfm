object FormScript: TFormScript
  Left = 362
  Top = 215
  Width = 546
  Height = 660
  Caption = 'Script'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object gbParams: TGroupBox
    Left = 0
    Top = 0
    Width = 538
    Height = 253
    Align = alTop
    Caption = 'Script Parameters'
    TabOrder = 0
  end
  object GroupBox1: TGroupBox
    Left = 0
    Top = 253
    Width = 538
    Height = 340
    Align = alTop
    Caption = 'Script'
    TabOrder = 1
    object memoScript: TMemo
      Left = 2
      Top = 15
      Width = 534
      Height = 176
      Align = alTop
      ScrollBars = ssBoth
      TabOrder = 0
    end
    object memoErrors: TMemo
      Left = 2
      Top = 191
      Width = 534
      Height = 147
      Align = alClient
      ReadOnly = True
      ScrollBars = ssBoth
      TabOrder = 1
    end
  end
  object Button1: TButton
    Left = 460
    Top = 604
    Width = 75
    Height = 25
    Caption = '&Run'
    TabOrder = 2
    OnClick = Button1Click
  end
  object OpenDialog: TOpenDialog
    Options = [ofOverwritePrompt, ofHideReadOnly, ofPathMustExist, ofEnableSizing]
    Left = 500
    Top = 12
  end
end