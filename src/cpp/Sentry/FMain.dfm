object FormMain: TFormMain
  Left = 321
  Top = 45
  Caption = 'Sentry Sprite Editor'
  ClientHeight = 873
  ClientWidth = 1073
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  Menu = MainMenu
  Position = poDesktopCenter
  ShowHint = True
  OnClose = FormClose
  OnMouseWheelDown = FormMouseWheelDown
  OnMouseWheelUp = FormMouseWheelUp
  OnResize = FormResize
  TextHeight = 13
  object StatusBar: TStatusBar
    Left = 0
    Top = 851
    Width = 1073
    Height = 22
    Panels = <
      item
        Width = 250
      end
      item
        Width = 50
      end>
    ExplicitTop = 850
    ExplicitWidth = 1069
  end
  object ToolBar: TToolBar
    Left = 0
    Top = 0
    Width = 1073
    Height = 29
    Caption = 'ToolBar'
    Color = clBtnFace
    ParentColor = False
    TabOrder = 1
    ExplicitWidth = 1069
  end
  object PanelSpriteEditor: TPanel
    Left = 0
    Top = 29
    Width = 1073
    Height = 822
    Align = alClient
    BevelInner = bvRaised
    BevelOuter = bvLowered
    TabOrder = 2
    ExplicitWidth = 1069
    ExplicitHeight = 821
    object Splitter4: TSplitter
      Left = 2
      Top = 734
      Width = 1069
      Height = 3
      Cursor = crVSplit
      Align = alTop
      OnMoved = Splitter4Moved
      ExplicitTop = 754
      ExplicitWidth = 1077
    end
    object PageControl1: TPageControl
      Left = 2
      Top = 2
      Width = 1069
      Height = 732
      ActivePage = TabSheetColorMapping
      Align = alTop
      Anchors = [akLeft, akTop, akRight, akBottom]
      TabOrder = 0
      TabPosition = tpBottom
      ExplicitWidth = 1065
      ExplicitHeight = 731
      object TabSheetModules: TTabSheet
        Caption = 'Modules'
        object Splitter1: TSplitter
          Left = 297
          Top = 0
          Height = 706
          ExplicitHeight = 726
        end
        object PanelModules: TPanel
          Left = 0
          Top = 0
          Width = 297
          Height = 706
          Align = alLeft
          BevelInner = bvLowered
          BevelOuter = bvNone
          TabOrder = 0
          object Splitter2: TSplitter
            Left = 1
            Top = 361
            Width = 295
            Height = 3
            Cursor = crVSplit
            Align = alTop
            OnMoved = Splitter2Moved
          end
          object GroupBoxModules: TGroupBox
            Left = 1
            Top = 1
            Width = 295
            Height = 360
            Align = alTop
            Caption = 'Modules'
            TabOrder = 0
            DesignSize = (
              295
              360)
            object edModules: TEdit
              Left = 4
              Top = 332
              Width = 285
              Height = 21
              Anchors = [akLeft, akTop, akRight]
              AutoSize = False
              TabOrder = 0
              OnExit = edModulesExit
              OnKeyDown = edModulesKeyDown
            end
            object gridModules: TStringGridX
              Left = 2
              Top = 15
              Width = 291
              Height = 310
              Align = alTop
              ColCount = 8
              DefaultRowHeight = 16
              FixedCols = 0
              RowCount = 2
              Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect, goColSizing, goRowSelect]
              TabOrder = 1
              OnClick = gridModulesClick
              OnDblClick = gridModulesDblClick
              OnPopNew = gridModulesPopNew
              OnPopInsert = gridModulesPopInsert
              OnPopClone = gridModulesPopClone
              OnPopDelete = gridModulesPopDelete
              OnPopMoveUp = gridModulesPopMoveUp
              OnPopMoveDown = gridModulesPopMoveDown
            end
          end
          object GroupBoxImages: TGroupBox
            Left = 1
            Top = 364
            Width = 295
            Height = 341
            Align = alClient
            Caption = 'Images'
            TabOrder = 1
            DesignSize = (
              295
              341)
            object gridImages: TStringGridX
              Left = 2
              Top = 15
              Width = 291
              Height = 310
              Align = alTop
              DefaultRowHeight = 16
              FixedCols = 0
              RowCount = 2
              Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect, goColSizing, goRowSelect]
              TabOrder = 0
              OnClick = gridImagesClick
              OnDblClick = gridImagesDblClick
              OnPopNew = gridImagesPopNew
              OnPopInsert = gridImagesPopInsert
              OnPopClone = gridImagesPopClone
              OnPopDelete = gridImagesPopDelete
              OnPopMoveUp = gridImagesPopMoveUp
              OnPopMoveDown = gridImagesPopMoveDown
            end
            object edImages: TEdit
              Left = 4
              Top = 332
              Width = 285
              Height = 21
              Anchors = [akLeft, akTop, akRight]
              TabOrder = 1
              OnExit = edImagesExit
              OnKeyDown = edImagesKeyDown
            end
          end
        end
        object PanelModulesCanvas: TPanel
          Left = 300
          Top = 0
          Width = 761
          Height = 706
          Align = alClient
          BevelInner = bvLowered
          BevelOuter = bvNone
          TabOrder = 1
          object GroupBoxCanvas: TGroupBox
            Left = 1
            Top = 1
            Width = 759
            Height = 704
            Align = alClient
            Caption = 'Modules Canvas'
            TabOrder = 0
            object paintModules: TPaintBox
              Left = 2
              Top = 109
              Width = 755
              Height = 593
              Align = alClient
              OnMouseDown = paintModulesMouseDown
              OnMouseMove = paintModulesMouseMove
              OnMouseUp = paintModulesMouseUp
              OnPaint = paintModulesPaint
              ExplicitWidth = 763
              ExplicitHeight = 613
            end
            object PanelModulesSetup: TPanel
              Left = 2
              Top = 15
              Width = 755
              Height = 94
              Align = alTop
              BevelInner = bvLowered
              TabOrder = 0
              DesignSize = (
                755
                94)
              object Label1: TLabel
                Left = 569
                Top = 68
                Width = 30
                Height = 13
                Anchors = [akRight, akBottom]
                Caption = 'Zoom:'
                ExplicitLeft = 577
              end
              object cbModulesAxis: TCheckBox
                Left = 8
                Top = 8
                Width = 97
                Height = 17
                Caption = 'Axis'
                TabOrder = 0
                OnClick = cbModulesAxisClick
              end
              object cbModulesGrid: TCheckBox
                Left = 8
                Top = 28
                Width = 97
                Height = 17
                Caption = 'Grid'
                TabOrder = 1
                OnClick = cbModulesGridClick
              end
              object cbModulesTrans: TCheckBox
                Left = 8
                Top = 48
                Width = 97
                Height = 17
                Caption = 'Transparent'
                TabOrder = 2
                OnClick = cbModulesTransClick
              end
              object cbModulesAll: TCheckBox
                Left = 8
                Top = 68
                Width = 97
                Height = 17
                Caption = 'All'
                TabOrder = 3
                OnClick = cbModulesAllClick
              end
              object trackModulesZoom: TTrackBar
                Left = 601
                Top = 52
                Width = 150
                Height = 37
                Anchors = [akRight, akBottom]
                Max = 1600
                Min = 10
                Frequency = 100
                Position = 100
                TabOrder = 4
                TickMarks = tmBoth
                OnChange = trackModulesZoomChange
              end
            end
          end
        end
      end
      object TabSheetFrames: TTabSheet
        Caption = 'Frames'
        ImageIndex = 1
        OnShow = TabSheetFramesShow
        object Splitter5: TSplitter
          Left = 281
          Top = 0
          Height = 706
          OnMoved = Splitter5Moved
          ExplicitHeight = 726
        end
        object PanelFrames: TPanel
          Left = 0
          Top = 0
          Width = 281
          Height = 706
          Align = alLeft
          BevelInner = bvLowered
          BevelOuter = bvNone
          TabOrder = 0
          object Splitter6: TSplitter
            Left = 1
            Top = 361
            Width = 279
            Height = 3
            Cursor = crVSplit
            Align = alTop
            OnMoved = Splitter6Moved
          end
          object GroupBoxFrames: TGroupBox
            Left = 1
            Top = 1
            Width = 279
            Height = 360
            Align = alTop
            Caption = 'Frames'
            TabOrder = 0
            DesignSize = (
              279
              360)
            object gridFrames: TStringGridX
              Left = 2
              Top = 15
              Width = 275
              Height = 310
              Align = alTop
              ColCount = 4
              DefaultRowHeight = 16
              FixedCols = 0
              RowCount = 2
              Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect, goColSizing, goRowSelect]
              TabOrder = 0
              OnClick = gridFramesClick
              OnDblClick = gridFramesDblClick
              OnSelectCell = gridFramesSelectCell
              OnPopNew = gridFramesPopNew
              OnPopInsert = gridFramesPopInsert
              OnPopClone = gridFramesPopClone
              OnPopDelete = gridFramesPopDelete
              OnPopMoveUp = gridFramesPopMoveUp
              OnPopMoveDown = gridFramesPopMoveDown
            end
            object edFrames: TEdit
              Left = 4
              Top = 332
              Width = 269
              Height = 21
              Anchors = [akLeft, akTop, akRight]
              TabOrder = 1
              OnExit = edFramesExit
              OnKeyDown = edFramesKeyDown
            end
          end
          object GroupBoxFModules: TGroupBox
            Left = 1
            Top = 364
            Width = 279
            Height = 341
            Align = alClient
            Caption = 'Frame Modules'
            TabOrder = 1
            DesignSize = (
              279
              341)
            object gridFModules: TStringGridX
              Left = 2
              Top = 15
              Width = 275
              Height = 310
              Align = alTop
              DefaultRowHeight = 16
              FixedCols = 0
              RowCount = 2
              Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect, goColSizing, goRowSelect]
              TabOrder = 0
              OnClick = gridFModulesClick
              OnDblClick = gridFModulesDblClick
              OnPopNew = gridFModulesPopNew
              OnPopInsert = gridFModulesPopInsert
              OnPopClone = gridFModulesPopClone
              OnPopDelete = gridFModulesPopDelete
              OnPopMoveUp = gridFModulesPopMoveUp
              OnPopMoveDown = gridFModulesPopMoveDown
            end
            object edFModules: TEdit
              Left = 4
              Top = 332
              Width = 269
              Height = 21
              Anchors = [akLeft, akTop, akRight]
              TabOrder = 1
              OnExit = edFModulesExit
              OnKeyDown = edFModulesKeyDown
            end
          end
        end
        object PanelFramesCanvas: TPanel
          Left = 284
          Top = 0
          Width = 777
          Height = 706
          Align = alClient
          BevelInner = bvLowered
          BevelOuter = bvNone
          TabOrder = 1
          object Splitter7: TSplitter
            Left = 1
            Top = 581
            Width = 775
            Height = 3
            Cursor = crVSplit
            Align = alTop
            OnMoved = Splitter7Moved
            ExplicitWidth = 783
          end
          object GroupBoxFramesCanvas: TGroupBox
            Left = 1
            Top = 1
            Width = 775
            Height = 580
            Align = alTop
            Caption = 'Frame Canvas'
            TabOrder = 0
            object paintFrames: TPaintBox
              Left = 2
              Top = 69
              Width = 771
              Height = 509
              Align = alClient
              OnMouseDown = paintFramesMouseDown
              OnMouseMove = paintFramesMouseMove
              OnMouseUp = paintFramesMouseUp
              OnPaint = paintFramesPaint
              ExplicitWidth = 779
            end
            object Panel2: TPanel
              Left = 2
              Top = 15
              Width = 771
              Height = 54
              Align = alTop
              BevelInner = bvLowered
              TabOrder = 0
              DesignSize = (
                771
                54)
              object Label2: TLabel
                Left = 581
                Top = 24
                Width = 30
                Height = 13
                Anchors = [akRight, akBottom]
                Caption = 'Zoom:'
                ExplicitLeft = 589
              end
              object cbFramesAxis: TCheckBox
                Left = 8
                Top = 8
                Width = 97
                Height = 17
                Caption = 'Axis'
                TabOrder = 0
                OnClick = cbFramesAxisClick
              end
              object cbFramesGrid: TCheckBox
                Left = 8
                Top = 28
                Width = 97
                Height = 17
                Caption = 'Grid'
                TabOrder = 1
                OnClick = cbFramesGridClick
              end
              object cbFramesTrans: TCheckBox
                Left = 98
                Top = 8
                Width = 97
                Height = 17
                Caption = 'Transparent'
                TabOrder = 2
                OnClick = cbFramesTransClick
              end
              object trackFramesZoom: TTrackBar
                Left = 617
                Top = 8
                Width = 150
                Height = 37
                Anchors = [akRight, akBottom]
                Max = 1600
                Min = 10
                Frequency = 100
                Position = 100
                TabOrder = 3
                TickMarks = tmBoth
                OnChange = trackFramesZoomChange
              end
              object cbFramesOffset: TCheckBox
                Left = 98
                Top = 28
                Width = 97
                Height = 17
                Caption = 'Offset FM'
                TabOrder = 4
                OnClick = cbFramesOffsetClick
              end
              object cbFramesBoundRect: TCheckBox
                Left = 188
                Top = 8
                Width = 97
                Height = 17
                Caption = 'Bound Rect'
                TabOrder = 5
                OnClick = cbFramesTransClick
              end
            end
          end
          object GroupBoxModulesQuickSelection: TGroupBox
            Left = 1
            Top = 584
            Width = 775
            Height = 121
            Align = alClient
            Caption = 'Modules Quick Selection'
            TabOrder = 1
            object paintQuickModules: TPaintBox
              Left = 2
              Top = 15
              Width = 771
              Height = 88
              Align = alClient
              OnDblClick = paintQuickModulesDblClick
              OnMouseDown = paintQuickModulesMouseDown
              OnPaint = paintQuickModulesPaint
              ExplicitWidth = 779
              ExplicitHeight = 108
            end
            object scrollQuickModules: TScrollBar
              Left = 2
              Top = 103
              Width = 771
              Height = 16
              Align = alBottom
              PageSize = 0
              TabOrder = 0
              OnScroll = scrollQuickModulesScroll
            end
          end
        end
      end
      object TabSheetAnim: TTabSheet
        Caption = 'Animation'
        ImageIndex = 2
        OnShow = TabSheetAnimShow
        object Splitter9: TSplitter
          Left = 309
          Top = 0
          Height = 706
          OnMoved = Splitter9Moved
          ExplicitHeight = 726
        end
        object PanelAnims: TPanel
          Left = 0
          Top = 0
          Width = 309
          Height = 706
          Align = alLeft
          BevelInner = bvLowered
          BevelOuter = bvNone
          TabOrder = 0
          object Splitter8: TSplitter
            Left = 1
            Top = 361
            Width = 307
            Height = 3
            Cursor = crVSplit
            Align = alTop
            OnMoved = Splitter8Moved
          end
          object GroupBoxAnims: TGroupBox
            Left = 1
            Top = 1
            Width = 307
            Height = 360
            Align = alTop
            Caption = 'Anims'
            TabOrder = 0
            DesignSize = (
              307
              360)
            object gridAnims: TStringGridX
              Left = 2
              Top = 15
              Width = 303
              Height = 310
              Align = alTop
              ColCount = 4
              DefaultRowHeight = 16
              FixedCols = 0
              RowCount = 2
              Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect, goColSizing, goRowSelect]
              TabOrder = 0
              OnClick = gridAnimsClick
              OnDblClick = gridAnimsDblClick
              OnSelectCell = gridAnimsSelectCell
              OnPopNew = gridAnimsPopNew
              OnPopInsert = gridAnimsPopInsert
              OnPopClone = gridAnimsPopClone
              OnPopDelete = gridAnimsPopDelete
              OnPopMoveUp = gridAnimsPopMoveUp
              OnPopMoveDown = gridAnimsPopMoveDown
            end
            object edAnims: TEdit
              Left = 4
              Top = 332
              Width = 297
              Height = 21
              Anchors = [akLeft, akTop, akRight]
              TabOrder = 1
              OnExit = edAnimsExit
              OnKeyDown = edAnimsKeyDown
            end
          end
          object GroupBoxAFrames: TGroupBox
            Left = 1
            Top = 364
            Width = 307
            Height = 341
            Align = alClient
            Caption = 'Anim Frames'
            TabOrder = 1
            DesignSize = (
              307
              341)
            object gridAFrames: TStringGridX
              Left = 2
              Top = 15
              Width = 303
              Height = 310
              Align = alTop
              ColCount = 6
              DefaultRowHeight = 16
              FixedCols = 0
              RowCount = 2
              Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect, goColSizing, goRowSelect]
              TabOrder = 0
              OnClick = gridAFramesClick
              OnDblClick = gridAFramesDblClick
              OnPopNew = gridAFramesPopNew
              OnPopInsert = gridAFramesPopInsert
              OnPopClone = gridAFramesPopClone
              OnPopDelete = gridAFramesPopDelete
              OnPopMoveUp = gridAFramesPopMoveUp
              OnPopMoveDown = gridAFramesPopMoveDown
            end
            object edAFrames: TEdit
              Left = 4
              Top = 332
              Width = 297
              Height = 21
              Anchors = [akLeft, akTop, akRight]
              TabOrder = 1
              OnExit = edAFramesExit
              OnKeyDown = edAFramesKeyDown
            end
          end
        end
        object Panel1: TPanel
          Left = 312
          Top = 0
          Width = 749
          Height = 706
          Align = alClient
          BevelInner = bvLowered
          BevelOuter = bvNone
          TabOrder = 1
          object Splitter10: TSplitter
            Left = 1
            Top = 581
            Width = 747
            Height = 3
            Cursor = crVSplit
            Align = alTop
            OnMoved = Splitter10Moved
            ExplicitWidth = 755
          end
          object GroupBoxAnimsCanvas: TGroupBox
            Left = 1
            Top = 1
            Width = 747
            Height = 580
            Align = alTop
            Caption = 'Anim Canvas'
            TabOrder = 0
            object paintAnims: TPaintBox
              Left = 2
              Top = 109
              Width = 743
              Height = 469
              Align = alClient
              OnMouseDown = paintAnimsMouseDown
              OnMouseMove = paintAnimsMouseMove
              OnMouseUp = paintAnimsMouseUp
              OnPaint = paintAnimsPaint
              ExplicitWidth = 751
            end
            object Panel3: TPanel
              Left = 2
              Top = 15
              Width = 743
              Height = 94
              Align = alTop
              BevelInner = bvLowered
              TabOrder = 0
              DesignSize = (
                743
                94)
              object Label3: TLabel
                Left = 557
                Top = 68
                Width = 30
                Height = 13
                Anchors = [akRight, akBottom]
                Caption = 'Zoom:'
                ExplicitLeft = 565
              end
              object btnPlay: TSpeedButton
                Left = 413
                Top = 60
                Width = 23
                Height = 22
                Anchors = [akRight, akBottom]
                Caption = '>'
                OnClick = btnPlayClick
                ExplicitLeft = 421
              end
              object btnPause: TSpeedButton
                Left = 389
                Top = 60
                Width = 23
                Height = 22
                Anchors = [akRight, akBottom]
                Caption = '| |'
                OnClick = btnPauseClick
                ExplicitLeft = 397
              end
              object btnRewind: TSpeedButton
                Left = 365
                Top = 60
                Width = 23
                Height = 22
                Anchors = [akRight, akBottom]
                Caption = '| <'
                OnClick = btnRewindClick
                ExplicitLeft = 373
              end
              object btnFastFwd: TSpeedButton
                Left = 437
                Top = 60
                Width = 23
                Height = 22
                Anchors = [akRight, akBottom]
                Caption = '| >'
                OnClick = btnFastFwdClick
                ExplicitLeft = 445
              end
              object Label4: TLabel
                Left = 473
                Top = 68
                Width = 23
                Height = 13
                Anchors = [akRight, akBottom]
                Caption = 'FPS:'
                ExplicitLeft = 481
              end
              object cbAnimsAxis: TCheckBox
                Left = 8
                Top = 8
                Width = 97
                Height = 17
                Caption = 'Axis'
                TabOrder = 0
                OnClick = cbAnimsAxisClick
              end
              object cbAnimsGrid: TCheckBox
                Left = 8
                Top = 28
                Width = 97
                Height = 17
                Caption = 'Grid'
                TabOrder = 1
                OnClick = cbAnimsGridClick
              end
              object cbAnimsTrans: TCheckBox
                Left = 8
                Top = 48
                Width = 97
                Height = 17
                Caption = 'Transparent'
                TabOrder = 2
                OnClick = cbAnimsTransClick
              end
              object cbAnimsFMOffset: TCheckBox
                Left = 8
                Top = 68
                Width = 97
                Height = 17
                Caption = 'Offset FM'
                TabOrder = 3
                OnClick = cbAnimsFMOffsetClick
              end
              object trackAnimsZoom: TTrackBar
                Left = 589
                Top = 52
                Width = 150
                Height = 37
                Anchors = [akRight, akBottom]
                Max = 1600
                Min = 10
                Frequency = 100
                Position = 100
                TabOrder = 4
                TickMarks = tmBoth
                OnChange = trackAnimsZoomChange
              end
              object cbAnimsAFOffset: TCheckBox
                Left = 112
                Top = 8
                Width = 97
                Height = 17
                Caption = 'Offset AF'
                TabOrder = 5
                OnClick = cbAnimsAFOffsetClick
              end
              object cbAnimsLoop: TCheckBox
                Left = 112
                Top = 28
                Width = 97
                Height = 17
                Caption = 'Loop'
                TabOrder = 6
              end
              object cbAnimsReverse: TCheckBox
                Left = 112
                Top = 48
                Width = 97
                Height = 17
                Caption = 'Reverse'
                TabOrder = 7
              end
              object cbFPS: TCSpinEdit
                Left = 505
                Top = 60
                Width = 41
                Height = 22
                Anchors = [akRight, akBottom]
                MaxValue = 100
                MinValue = 1
                TabOrder = 8
                Value = 10
                OnChange = cbFPSChange
              end
            end
          end
          object GroupBoxFramesQuickSelection: TGroupBox
            Left = 1
            Top = 584
            Width = 747
            Height = 121
            Align = alClient
            Caption = 'Frames Quick Selection'
            TabOrder = 1
            object paintQuickFrames: TPaintBox
              Left = 2
              Top = 15
              Width = 743
              Height = 88
              Align = alClient
              OnDblClick = paintQuickFramesDblClick
              OnMouseDown = paintQuickFramesMouseDown
              OnPaint = paintQuickFramesPaint
              ExplicitWidth = 751
              ExplicitHeight = 108
            end
            object scrollQuickFrames: TScrollBar
              Left = 2
              Top = 103
              Width = 743
              Height = 16
              Align = alBottom
              PageSize = 0
              TabOrder = 0
              OnScroll = scrollQuickFramesScroll
            end
          end
        end
      end
      object TabSheetMapping: TTabSheet
        Caption = 'Module Map'
        ImageIndex = 3
        object Label7: TLabel
          Left = 460
          Top = 320
          Width = 228
          Height = 30
          Align = alCustom
          Alignment = taCenter
          Caption = 'NOT IMPLEMENTED'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clRed
          Font.Height = -21
          Font.Name = 'Arial Black'
          Font.Style = []
          ParentFont = False
        end
      end
      object TabSheetColorMapping: TTabSheet
        Caption = 'Color Map'
        ImageIndex = 4
        OnShow = TabSheetColorMappingShow
        object Panel6: TPanel
          Left = 0
          Top = 0
          Width = 1061
          Height = 706
          Align = alClient
          BevelInner = bvLowered
          BevelOuter = bvNone
          TabOrder = 0
          ExplicitWidth = 1057
          ExplicitHeight = 705
          object Splitter12: TSplitter
            Left = 270
            Top = 1
            Height = 704
            OnMoved = Splitter12Moved
            ExplicitHeight = 724
          end
          object Panel5: TPanel
            Left = 1
            Top = 1
            Width = 269
            Height = 704
            Align = alLeft
            BevelInner = bvLowered
            BevelOuter = bvNone
            TabOrder = 0
            ExplicitHeight = 703
            object Splitter3: TSplitter
              Left = 1
              Top = 202
              Width = 267
              Height = 3
              Cursor = crVSplit
              Align = alTop
              OnMoved = Splitter3Moved
            end
            object Splitter11: TSplitter
              Left = 1
              Top = 462
              Width = 267
              Height = 3
              Cursor = crVSplit
              Align = alTop
              OnMoved = Splitter11Moved
            end
            object GroupBoxColorMaps: TGroupBox
              Left = 1
              Top = 205
              Width = 267
              Height = 257
              Align = alTop
              Caption = 'Color Maps'
              TabOrder = 0
              DesignSize = (
                267
                257)
              object gridColorMaps: TStringGridX
                Left = 4
                Top = 16
                Width = 257
                Height = 209
                Anchors = [akLeft, akTop, akRight]
                ColCount = 3
                DefaultRowHeight = 16
                FixedCols = 0
                RowCount = 2
                Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect, goColSizing, goRowSelect]
                TabOrder = 0
                OnClick = gridColorMapsClick
                OnDblClick = gridColorMapsDblClick
                OnPopNew = gridColorMapsPopNew
                OnPopInsert = gridColorMapsPopInsert
                OnPopClone = gridColorMapsPopClone
                OnPopDelete = gridColorMapsPopDelete
                OnPopMoveUp = gridColorMapsPopMoveUp
                OnPopMoveDown = gridColorMapsPopMoveDown
              end
              object editColorMap: TEdit
                Left = 4
                Top = 228
                Width = 257
                Height = 21
                Anchors = [akLeft, akTop, akRight]
                TabOrder = 1
                OnExit = editColorMapExit
                OnKeyDown = editColorMapKeyDown
              end
            end
            object GroupBoxColorMapItems: TGroupBox
              Left = 1
              Top = 465
              Width = 267
              Height = 238
              Align = alClient
              Caption = 'Color Map Items'
              TabOrder = 1
              ExplicitHeight = 237
              DesignSize = (
                267
                238)
              object gridColorMapItems: TStringGridX
                Left = 4
                Top = 16
                Width = 257
                Height = 213
                Anchors = [akLeft, akTop, akRight]
                ColCount = 3
                DefaultRowHeight = 16
                FixedCols = 0
                RowCount = 2
                Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect, goColSizing, goRowSelect]
                TabOrder = 0
                OnClick = gridColorMapItemsClick
                OnDblClick = gridColorMapItemsDblClick
                OnDrawCell = gridColorMapItemsDrawCell
                OnPopNew = gridColorMapItemsPopNew
                OnPopInsert = gridColorMapItemsPopInsert
                OnPopClone = gridColorMapItemsPopClone
                OnPopDelete = gridColorMapItemsPopDelete
                OnPopMoveUp = gridColorMapItemsPopMoveUp
                OnPopMoveDown = gridColorMapItemsPopMoveDown
              end
              object editColorMapItem: TEdit
                Left = 4
                Top = 232
                Width = 257
                Height = 21
                Anchors = [akLeft, akTop, akRight]
                TabOrder = 1
                OnExit = editColorMapItemExit
                OnKeyDown = editColorMapItemKeyDown
              end
            end
            object GroupBoxImagesForPalMap: TGroupBox
              Left = 1
              Top = 1
              Width = 267
              Height = 201
              Align = alTop
              Caption = 'Images'
              TabOrder = 2
              object gridImagesForColorMap: TStringGrid
                Left = 2
                Top = 15
                Width = 263
                Height = 184
                Align = alClient
                DefaultRowHeight = 16
                FixedCols = 0
                Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect, goColSizing, goRowSelect]
                TabOrder = 0
                OnClick = gridImagesForColorMapClick
              end
            end
          end
          object Panel7: TPanel
            Left = 273
            Top = 1
            Width = 787
            Height = 704
            Align = alClient
            BevelInner = bvLowered
            BevelOuter = bvNone
            TabOrder = 1
            ExplicitWidth = 783
            ExplicitHeight = 703
            object GroupBoxColorMapCanvas: TGroupBox
              Left = 1
              Top = 1
              Width = 785
              Height = 702
              Align = alClient
              Caption = 'Color Map Canvas'
              TabOrder = 0
              ExplicitWidth = 781
              ExplicitHeight = 701
              object paintColorMap: TPaintBox
                Left = 2
                Top = 69
                Width = 781
                Height = 631
                Align = alClient
                OnMouseDown = paintColorMapMouseDown
                OnMouseMove = paintColorMapMouseMove
                OnMouseUp = paintColorMapMouseUp
                OnPaint = paintColorMapPaint
                ExplicitWidth = 789
                ExplicitHeight = 651
              end
              object Panel8: TPanel
                Left = 2
                Top = 15
                Width = 781
                Height = 54
                Align = alTop
                BevelInner = bvLowered
                TabOrder = 0
                ExplicitWidth = 777
                DesignSize = (
                  781
                  54)
                object Label6: TLabel
                  Left = 581
                  Top = 24
                  Width = 30
                  Height = 13
                  Anchors = [akRight, akBottom]
                  Caption = 'Zoom:'
                  ExplicitLeft = 589
                end
                object cbColorMapAxis: TCheckBox
                  Left = 8
                  Top = 8
                  Width = 97
                  Height = 17
                  Caption = 'Axis'
                  TabOrder = 0
                  OnClick = cbColorMapAxisClick
                end
                object cbColorMapGrid: TCheckBox
                  Left = 8
                  Top = 28
                  Width = 97
                  Height = 17
                  Caption = 'Grid'
                  TabOrder = 1
                  OnClick = cbColorMapGridClick
                end
                object cbColorMapTrans: TCheckBox
                  Left = 88
                  Top = 8
                  Width = 97
                  Height = 17
                  Caption = 'Transparent'
                  TabOrder = 2
                  OnClick = cbColorMapTransClick
                end
                object trackColorMapZoom: TTrackBar
                  Left = 617
                  Top = 8
                  Width = 150
                  Height = 37
                  Anchors = [akRight, akBottom]
                  Max = 1600
                  Min = 10
                  Frequency = 100
                  Position = 100
                  TabOrder = 3
                  TickMarks = tmBoth
                  OnChange = trackColorMapZoomChange
                  ExplicitLeft = 613
                end
              end
            end
          end
        end
      end
      object TabSheetFrameLogic: TTabSheet
        Caption = 'Frame Logic'
        ImageIndex = 5
        OnShow = TabSheetFrameLogicShow
        object SplitterFrameLogic3: TSplitter
          Left = 269
          Top = 0
          Height = 706
          OnMoved = SplitterFrameLogic3Moved
          ExplicitHeight = 726
        end
        object PanelFramesLogic: TPanel
          Left = 0
          Top = 0
          Width = 269
          Height = 706
          Align = alLeft
          BevelInner = bvLowered
          BevelOuter = bvNone
          TabOrder = 0
          object SplitterFrameLogic1: TSplitter
            Left = 1
            Top = 202
            Width = 267
            Height = 3
            Cursor = crVSplit
            Align = alTop
            OnMoved = SplitterFrameLogic1Moved
          end
          object SplitterFrameLogic2: TSplitter
            Left = 1
            Top = 462
            Width = 267
            Height = 3
            Cursor = crVSplit
            Align = alTop
            OnMoved = SplitterFrameLogic2Moved
          end
          object GroupBoxFrameLogic: TGroupBox
            Left = 1
            Top = 205
            Width = 267
            Height = 257
            Align = alTop
            Caption = 'Frame Logic'
            TabOrder = 0
            DesignSize = (
              267
              257)
            object gridFrameLogic: TStringGridX
              Left = 4
              Top = 16
              Width = 257
              Height = 209
              Anchors = [akLeft, akTop, akRight]
              DefaultRowHeight = 16
              FixedCols = 0
              RowCount = 2
              Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect, goColSizing, goRowSelect]
              TabOrder = 0
              OnClick = gridFrameLogicClick
              OnDblClick = gridFrameLogicDblClick
              OnPopNew = gridFrameLogicPopNew
              OnPopInsert = gridFrameLogicPopInsert
              OnPopClone = gridFrameLogicPopClone
              OnPopDelete = gridFrameLogicPopDelete
              OnPopMoveUp = gridFrameLogicPopMoveUp
              OnPopMoveDown = gridFrameLogicPopMoveDown
            end
            object edFrameLogic: TEdit
              Left = 4
              Top = 228
              Width = 257
              Height = 21
              Anchors = [akLeft, akTop, akRight]
              TabOrder = 1
              OnExit = edFrameLogicExit
              OnKeyDown = edFrameLogicKeyDown
            end
          end
          object GroupBoxFrameLogicItem: TGroupBox
            Left = 1
            Top = 465
            Width = 267
            Height = 240
            Align = alClient
            Caption = 'Frame Logic Item'
            TabOrder = 1
            DesignSize = (
              267
              240)
            object gridFrameLogicItems: TStringGridX
              Left = 4
              Top = 16
              Width = 257
              Height = 213
              Anchors = [akLeft, akTop, akRight]
              DefaultRowHeight = 16
              FixedCols = 0
              RowCount = 2
              Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect, goColSizing, goRowSelect]
              TabOrder = 0
              OnClick = gridFrameLogicItemsClick
              OnDblClick = gridFrameLogicItemsDblClick
              OnPopNew = gridFrameLogicItemsPopNew
              OnPopInsert = gridFrameLogicItemsPopInsert
              OnPopClone = gridFrameLogicItemsPopClone
              OnPopDelete = gridFrameLogicItemsPopDelete
              OnPopMoveUp = gridFrameLogicItemsPopMoveUp
              OnPopMoveDown = gridFrameLogicItemsPopMoveDown
            end
            object edFrameLogicItem: TEdit
              Left = 4
              Top = 232
              Width = 257
              Height = 21
              Anchors = [akLeft, akTop, akRight]
              TabOrder = 1
              OnExit = edFrameLogicItemExit
              OnKeyDown = edFrameLogicItemKeyDown
            end
          end
          object GroupBoxFrameForLogic: TGroupBox
            Left = 1
            Top = 1
            Width = 267
            Height = 201
            Align = alTop
            Caption = 'Frames'
            TabOrder = 2
            object gridFramesForLogic: TStringGrid
              Left = 2
              Top = 15
              Width = 263
              Height = 184
              Align = alClient
              DefaultRowHeight = 16
              FixedCols = 0
              Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect, goColSizing, goRowSelect]
              TabOrder = 0
              OnClick = gridFramesForLogicClick
            end
          end
        end
        object PanelFrameLogicCanvas: TPanel
          Left = 272
          Top = 0
          Width = 789
          Height = 706
          Align = alClient
          BevelInner = bvLowered
          BevelOuter = bvNone
          TabOrder = 1
          object GroupBoxFrameLogicCanvas: TGroupBox
            Left = 1
            Top = 1
            Width = 787
            Height = 704
            Align = alClient
            Caption = 'Frame Logic Canvas'
            TabOrder = 0
            object paintFrameLogic: TPaintBox
              Left = 2
              Top = 69
              Width = 783
              Height = 633
              Align = alClient
              OnMouseDown = paintFrameLogicMouseDown
              OnMouseMove = paintFrameLogicMouseMove
              OnMouseUp = paintFrameLogicMouseUp
              OnPaint = paintFrameLogicPaint
              ExplicitWidth = 791
              ExplicitHeight = 653
            end
            object Panel4: TPanel
              Left = 2
              Top = 15
              Width = 783
              Height = 54
              Align = alTop
              BevelInner = bvLowered
              TabOrder = 0
              DesignSize = (
                783
                54)
              object Label5: TLabel
                Left = 581
                Top = 24
                Width = 30
                Height = 13
                Anchors = [akRight, akBottom]
                Caption = 'Zoom:'
                ExplicitLeft = 589
              end
              object cbFrameLogicAxis: TCheckBox
                Left = 8
                Top = 8
                Width = 97
                Height = 17
                Caption = 'Axis'
                TabOrder = 0
                OnClick = cbFrameLogicAxisClick
              end
              object cbFrameLogicGrid: TCheckBox
                Left = 8
                Top = 28
                Width = 97
                Height = 17
                Caption = 'Grid'
                TabOrder = 1
                OnClick = cbFrameLogicGridClick
              end
              object cbFrameLogicTrans: TCheckBox
                Left = 88
                Top = 8
                Width = 97
                Height = 17
                Caption = 'Transparent'
                TabOrder = 2
                OnClick = cbFrameLogicTransClick
              end
              object trackFLogicZoom: TTrackBar
                Left = 617
                Top = 8
                Width = 150
                Height = 37
                Anchors = [akRight, akBottom]
                Max = 1600
                Min = 10
                Frequency = 100
                Position = 100
                TabOrder = 3
                TickMarks = tmBoth
                OnChange = trackFLogicZoomChange
              end
              object cbFrameLogicOffsetFM: TCheckBox
                Left = 88
                Top = 28
                Width = 97
                Height = 17
                Caption = 'Offset FM'
                TabOrder = 4
                OnClick = cbFrameLogicOffsetFMClick
              end
            end
          end
        end
      end
    end
    object MemoMsg: TMemo
      Left = 2
      Top = 737
      Width = 1069
      Height = 83
      Align = alClient
      TabOrder = 1
      ExplicitTop = 736
      ExplicitWidth = 1065
    end
  end
  object MainMenu: TMainMenu
    Left = 1036
    object File1: TMenuItem
      Caption = '&File'
      object New1: TMenuItem
        Caption = '&New'
        OnClick = New1Click
      end
      object Open1: TMenuItem
        Caption = '&Open...'
        OnClick = Open1Click
      end
      object OpenRecent1: TMenuItem
        Caption = 'Open Recent'
        object TMenuItem
        end
      end
      object Save1: TMenuItem
        Caption = '&Save'
        OnClick = Save1Click
      end
      object Saveas1: TMenuItem
        Caption = 'Save &as...'
        OnClick = Saveas1Click
      end
      object Close1: TMenuItem
        Caption = '&Close'
        OnClick = Close1Click
      end
      object N6: TMenuItem
        Caption = '-'
      end
      object Import1: TMenuItem
        Caption = '&Import'
        object NoScriptsAvailable2: TMenuItem
          Caption = 'No Scripts Available'
          Enabled = False
        end
        object N7: TMenuItem
          Caption = '-'
        end
        object Refresh2: TMenuItem
          Caption = 'Refresh'
        end
      end
      object Export1: TMenuItem
        Caption = '&Export'
        object NoScriptsAvailable1: TMenuItem
          Caption = 'No Scripts Available'
          Enabled = False
        end
        object N1: TMenuItem
          Caption = '-'
        end
        object Refresh1: TMenuItem
          Caption = 'Refresh'
        end
      end
      object N2: TMenuItem
        Caption = '-'
      end
      object Exit1: TMenuItem
        Caption = 'E&xit'
        ShortCut = 32856
        OnClick = Exit1Click
      end
    end
    object Edit1: TMenuItem
      Caption = '&Edit'
      object Undo1: TMenuItem
        Caption = '&Undo'
      end
      object Redo1: TMenuItem
        Caption = '&Redo'
      end
      object N4: TMenuItem
        Caption = '-'
      end
      object Copy1: TMenuItem
        Caption = '&Copy'
      end
      object Cut1: TMenuItem
        Caption = 'C&ut'
      end
      object Paste1: TMenuItem
        Caption = '&Paste'
      end
      object Delete1: TMenuItem
        Caption = '&Delete'
      end
    end
    object Tools1: TMenuItem
      Caption = '&Tools'
      object Scripts1: TMenuItem
        Caption = '&Scripts'
        object NoScriptsAvailable3: TMenuItem
          Caption = 'No Scripts Available'
        end
        object N8: TMenuItem
          Caption = '-'
        end
        object Refresh3: TMenuItem
          Caption = 'Refresh'
        end
      end
      object N5: TMenuItem
        Caption = '-'
      end
      object Options1: TMenuItem
        Caption = '&Options...'
        OnClick = Options1Click
      end
    end
    object Help1: TMenuItem
      Caption = '&Help'
      object Contents1: TMenuItem
        Caption = 'Contents'
      end
      object N3: TMenuItem
        Caption = '-'
      end
      object About1: TMenuItem
        Caption = '&About...'
        OnClick = About1Click
      end
    end
  end
  object OpenImage: TOpenPictureDialog
    Left = 1004
  end
  object SaveDialogXSprite: TSaveDialog
    DefaultExt = 'xsprite'
    Filter = 'XSprite (*.xsprite)|*.xsprite|All Files (*.*)|*.*'
    InitialDir = '.'
    Left = 968
  end
  object OpenDialogXSprite: TOpenDialog
    DefaultExt = 'xsprite'
    Filter = 'XSprite (*.xsprite)|*.xsprite|All Files (*.*)|*.*'
    InitialDir = '.'
    Left = 936
  end
  object TimerAnim: TTimer
    Enabled = False
    Interval = 100
    OnTimer = TimerAnimTimer
    Left = 904
  end
  object ColorDialog: TColorDialog
    Left = 872
  end
end
