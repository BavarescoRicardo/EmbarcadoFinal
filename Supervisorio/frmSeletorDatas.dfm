object FormSeletorDatas: TFormSeletorDatas
  Left = 0
  Top = 0
  Caption = 'Per'#237'odo de Visualiza'#231#227'o'
  ClientHeight = 211
  ClientWidth = 324
  Color = clWhite
  Constraints.MaxHeight = 250
  Constraints.MaxWidth = 340
  Constraints.MinHeight = 250
  Constraints.MinWidth = 340
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poMainFormCenter
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object gpFaixaDatas: TGroupBox
    Left = 24
    Top = 32
    Width = 273
    Height = 129
    Caption = 'Informe a faixa de datas'
    TabOrder = 0
    object lblDataInicial: TLabel
      Left = 52
      Top = 37
      Width = 68
      Height = 16
      Alignment = taRightJustify
      Caption = 'Data Inicial:'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
    end
    object lblDataFinal: TLabel
      Left = 58
      Top = 75
      Width = 62
      Height = 16
      Alignment = taRightJustify
      Caption = 'Data Final:'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
    end
    object eDataInicial: TDateTimePicker
      Left = 126
      Top = 37
      Width = 81
      Height = 21
      Date = 44101.000000000000000000
      Time = 0.054656377316860020
      TabOrder = 0
      OnChange = eDataInicialChange
    end
    object eDataFinal: TDateTimePicker
      Left = 126
      Top = 75
      Width = 81
      Height = 21
      Date = 44101.000000000000000000
      Time = 0.054656377316860020
      TabOrder = 1
    end
  end
  object btnConfirmar: TButton
    Left = 222
    Top = 167
    Width = 75
    Height = 25
    Caption = 'Confirmar'
    TabOrder = 1
    OnClick = btnConfirmarClick
  end
end
