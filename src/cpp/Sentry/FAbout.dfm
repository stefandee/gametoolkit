object FormAbout: TFormAbout
  Left = 491
  Top = 403
  BorderIcons = [biSystemMenu]
  BorderStyle = bsToolWindow
  Caption = 'About'
  ClientHeight = 269
  ClientWidth = 373
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  FormStyle = fsStayOnTop
  Position = poDesktopCenter
  TextHeight = 13
  object MemoInfo: TMemo
    Left = 4
    Top = 4
    Width = 405
    Height = 185
    Lines.Strings = (
      'Sentry - An Advanced Sprite Editing Tool')
    ReadOnly = True
    ScrollBars = ssVertical
    TabOrder = 0
  end
  object GroupBox1: TGroupBox
    Left = 4
    Top = 196
    Width = 405
    Height = 93
    TabOrder = 1
    object Label1: TLabel
      Left = 8
      Top = 16
      Width = 193
      Height = 13
      Caption = 'Sentry - An Advanced Sprite Editing Tool'
    end
    object Label2: TLabel
      Left = 8
      Top = 36
      Width = 42
      Height = 13
      Caption = '(c) 2006 '
    end
    object HttpLabel1: THttpLabel
      Left = 52
      Top = 36
      Width = 87
      Height = 13
      Link = 'https://www.pirongames.com'
      FontMouseOver.Charset = DEFAULT_CHARSET
      FontMouseOver.Color = clWindowText
      FontMouseOver.Height = -11
      FontMouseOver.Name = 'MS Sans Serif'
      FontMouseOver.Style = []
      Caption = 'Karg/Piron Games'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlue
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsUnderline]
    end
  end
  object ButtonOk: TButton
    Left = 332
    Top = 300
    Width = 75
    Height = 25
    Caption = '&Ok'
    ModalResult = 1
    TabOrder = 2
  end
end
