//---------------------------------------------------------------------------
#ifndef frmPrincipalH
#define frmPrincipalH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <System.DateUtils.hpp>
#include <System.IOUtils.hpp>
#include <System.SysUtils.hpp>
#include <Vcl.Buttons.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.Dialogs.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Imaging.pngimage.hpp>
#include <Vcl.Menus.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.WinXPickers.hpp>
#include <VCLTee.Chart.hpp>
#include <VCLTee.Series.hpp>
#include <VclTee.TeeGDIPlus.hpp>
#include <VCLTee.TeEngine.hpp>
#include <VCLTee.TeeProcs.hpp>
//---------------------------------------------------------------------------
#include "SerialPort.h"
#include "Thread.h"
#include "frmMonitorarDados.h"
#include "frmSeletorDatas.h"
//---------------------------------------------------------------------------
class TFormPrincipal : public TForm
{
__published:	// IDE-managed Components
	TPanel *pnlConexao;
	TChart *chtDados;
	TFastLineSeries *serieCH0;
	TTimePicker *eTempoDecorrido;
	TMainMenu *menuSuperior;
	TMenuItem *Arquivo1;
	TMenuItem *Monitorar1;
	TMenuItem *Abrir1;
	TMenuItem *Abrirmonitordaportaserial1;
	TOpenDialog *OpenDialog;
	TLabel *lblPortaSerial;
	TComboBox *cmbPortaSerial;
	TBitBtn *btnAtualizarPortas;
	TLabel *lblBaudrate;
	TComboBox *cmbBaudrate;
	TButton *btnConectar;
	TButton *btnDesconectar;
	TProgressBar *pgbStatusConexao;
	TMenuItem *Abrirfaixadedatas1;
	TGroupBox *gpTempoDecorrido;
	TLabel *lblBpm;
	TLabel *lblTituloBpm;
	void __fastcall btnAtualizarPortasClick(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall btnConectarClick(TObject *Sender);
	void __fastcall btnDesconectarClick(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall chtDadosAfterDraw(TObject *Sender);
	void __fastcall Abrirmonitordaportaserial1Click(TObject *Sender);
	void __fastcall Abrir1Click(TObject *Sender);
	void __fastcall btnVoltarPaginaClick(TObject *Sender);
	void __fastcall btnAvancarPaginaClick(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall btnLimparGraficoClick(TObject *Sender);
	void __fastcall Abrirfaixadedatas1Click(TObject *Sender);
private:
	// M�todos
	void __fastcall AtualizarControles();
	void __fastcall ListarPortasSeriais();
	void __fastcall IniciarProcessos();
	void __fastcall FinalizarProcessos();

public:		// User declarations
	__fastcall TFormPrincipal(TComponent* Owner);

    // VARI�VEIS DE CONTROLE
	SerialPort *portaSerial;
	Thread *threadAquisicao;
	bool comunicando = false;
	FILE *arq_dados;
	bool FArquivoAberto = false;
	int FPaginaAtual = 0;
	UnicodeString PathDadosTemperatura;
    AnsiString dataHoraAtual = NULL;
	unsigned long PosicaoInicial;
	unsigned long PosicaoAtual;
	unsigned long PosicaoFinal;
	// Controle de horario
	unsigned int hora = 0;
	unsigned int minuto  = 0;
	unsigned int segundo  = 0;
	unsigned int segundosBpm  = 0;
	unsigned int max_tela = 100;
	// m�todos
	bool __fastcall VerificarCriarPastaDados();
	void __fastcall AtualizarArquivo(float temperaturaLida, int batimentos);
	void __fastcall FecharArquivo();
	bool __fastcall AtualizarRelogio();
	void __fastcall PlotarGraficoArquivo();
	void __fastcall ConfigGrafico();
	void __fastcall setLabelPaginaAtual();
	void __fastcall HabDesabIniciarEncerrar(bool _AHab);
	AnsiString __fastcall getStrData(AnsiString _SubsBarra);
	AnsiString __fastcall getStrTime(AnsiString _ASeparador);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormPrincipal *FormPrincipal;
//---------------------------------------------------------------------------
#endif
