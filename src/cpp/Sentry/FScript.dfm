object FormScript: TFormScript
  Left = 362
  Top = 215
  Caption = 'Script'
  ClientHeight = 617
  ClientWidth = 514
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  TextHeight = 13
  object gbParams: TGroupBox
    Left = 0
    Top = 0
    Width = 514
    Height = 253
    Align = alTop
    Caption = 'Script Parameters'
    TabOrder = 0
    ExplicitWidth = 538
  end
  object GroupBox1: TGroupBox
    Left = 0
    Top = 253
    Width = 514
    Height = 340
    Align = alTop
    Caption = 'Script'
    TabOrder = 1
    ExplicitWidth = 538
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
