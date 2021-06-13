object FormMonitorarDados: TFormMonitorarDados
  Left = 0
  Top = 0
  Caption = 'Monitor'
  ClientHeight = 388
  ClientWidth = 265
  Color = clWhite
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object lblDadosRecebidos: TLabel
    Left = 0
    Top = 0
    Width = 265
    Height = 21
    Margins.Top = 10
    Align = alTop
    Alignment = taCenter
    Caption = 'Dados Recebidos'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -17
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    ExplicitWidth = 128
  end
  object txtDadosRecebidos: TMemo
    Left = 0
    Top = 21
    Width = 265
    Height = 367
    Margins.Top = 10
    Margins.Bottom = 10
    Align = alClient
    ReadOnly = True
    TabOrder = 0
  end
end
