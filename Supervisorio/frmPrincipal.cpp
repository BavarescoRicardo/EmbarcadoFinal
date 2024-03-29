// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "frmPrincipal.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormPrincipal *FormPrincipal;
// ---------------------------------------------------------------------------
__fastcall TFormPrincipal::TFormPrincipal(TComponent* Owner) : TForm(Owner)
{
}
// ---------------------------------------------------------------------------
void __fastcall TFormPrincipal::FormCreate(TObject *Sender)
{
	// Listar portas seriais dispon�veis
	ListarPortasSeriais();
	eTempoDecorrido->Time = EncodeTime(0, 0, 0, 0);
}
// ---------------------------------------------------------------------------
void __fastcall TFormPrincipal::btnAtualizarPortasClick(TObject *Sender)
{
	// Desabilitar bot�o
	btnAtualizarPortas->Enabled = false;

	// Listar portas seriais dispon�veis
	ListarPortasSeriais();

	// Habilitar bot�o
	btnAtualizarPortas->Enabled = true;
}
// ---------------------------------------------------------------------------
void __fastcall TFormPrincipal::btnConectarClick(TObject *Sender)
{
	// valida��o caso n�o tenham portas COM disponiveis/selecionadas
	if ((cmbPortaSerial->ItemIndex < 0) && (cmbPortaSerial->Items->Count <= 0))
	{
		ShowMessage("Nenhuma porta serial dispon�vel!");
		exit(0);
	}

	if (!VerificarCriarPastaDados())
	{
		exit(0);
	};

	// Desabilitar bot�o
	btnConectar->Enabled = false;

	// Iniciar processos
	IniciarProcessos();

	// Habilitar/Desabilitar bot�es
	AtualizarControles();
}
// ---------------------------------------------------------------------------
void __fastcall TFormPrincipal::FecharArquivo()
{
	if (arq_dados != NULL)
	{
		fclose(arq_dados);
		FArquivoAberto = false;
	}
}
// ---------------------------------------------------------------------------
void __fastcall TFormPrincipal::AtualizarArquivo(float temperatura, int batimentos)
{
	if (segundosBpm > 9)
	{
		segundo++;
		segundosBpm = 0;
	}
	// Atualiza bpm ao passar 1 minuto em tempo real = segundos dependem da freq do sinal q est� em 10Hz
	if (segundo >= 59) //
	{
		lblBpm->Caption = IntToStr(batimentos);
		threadAquisicao->ZerarBpm();
		segundo = 0;
	}
	segundosBpm++;
	if (AtualizarRelogio())
	{
		ShowMessage("Tempo de aquisi��o de 24 encerrado!");
		FinalizarProcessos();
	}
	else
	{
		// Quando nenhum arquivo de dados foi criado OU
		// Sempre que completar + 1 hora de execu��o
		if ((arq_dados == NULL) || ((minuto == 0) && (segundo == 0)))
		{
			// Cria pasta do dia
			UnicodeString APastaDiaPath = PathDadosTemperatura + "\\" + getStrData("-");
			if (!TDirectory::Exists(APastaDiaPath))
			{
				TDirectory::CreateDirectory(APastaDiaPath);
			};

			// Cria e abre o arquivo da hora para grava��o
			AnsiString AArquivoDaHoraPath =
				APastaDiaPath + "\\" + "Arq_" + getStrData("") + "_" + getStrTime("") + ".txt";
			if (!FileExists(AArquivoDaHoraPath))
			{
				// Fecha o arquivo atual
				FecharArquivo();
                // Cria o novo arquivo
				arq_dados = fopen(AArquivoDaHoraPath.c_str(), "a+");
			}
		}

        // Gravar os dados no arquivo
		if (arq_dados != NULL)
		{
			dataHoraAtual = getStrData("/") + " - " + getStrTime(":");
			fprintf(arq_dados, "%s - %2.3f �C\n", dataHoraAtual.c_str(), temperatura);
		}
	}
}
// ---------------------------------------------------------------------------
bool __fastcall TFormPrincipal::AtualizarRelogio()
{
	// Fun��o para atualiza��o do relogio
	bool FinalizouTempoAquisicao = false;
	if (segundo >= 60)
	{
		segundo = 0;
		minuto++;
	}

	if (minuto >= 60)
	{
		segundo = 0;
		minuto = 0;
		hora++;
	}

	if (hora >= 24)
	{
		eTempoDecorrido->Time = EncodeTime(24, 0, 0, 0);
		segundo = 0;
		minuto = 0;
		hora = 0;
		FinalizouTempoAquisicao = true;
	}
	else
	{
		eTempoDecorrido->Time = EncodeTime(hora, minuto, segundo, 0);
	}

	return FinalizouTempoAquisicao;
}
// ---------------------------------------------------------------------------
void __fastcall TFormPrincipal::btnDesconectarClick(TObject *Sender)
{
	// Desabilitar bot�o
	btnDesconectar->Enabled = false;

	// Finalizar a thread e fechar a porta serial
	FinalizarProcessos();

	// Habilitar/Desabilitar bot�es
	AtualizarControles();
}
// ---------------------------------------------------------------------------
void __fastcall TFormPrincipal::FormClose(TObject *Sender, TCloseAction &Action)
{
	// N�o executar se n�o for necess�rio
	if (!comunicando)
	{
		return;
	}

	// Finalizar a thread e fechar a porta serial
	FinalizarProcessos();
}
// ---------------------------------------------------------------------------
void __fastcall TFormPrincipal::chtDadosAfterDraw(TObject *Sender)
{
	// Vari�veis dos Eixos:  xInicial, xFinal, yInicial, yFinal
	double xi, xf, yi, yf;
	int posicaoGrafico = 0;
	if (threadAquisicao != NULL)
	{
		posicaoGrafico = threadAquisicao->posicaoNoGrafico;
	}

	// Calcula nova posi��o dos eixos do gr�fico - EIXO X
	xi = chtDados->BottomAxis->CalcPosValue(posicaoGrafico - 1);
	xf = chtDados->BottomAxis->CalcPosValue(posicaoGrafico - 1) + 1;

	// Calcula nova posi��o dos eixos do gr�fico - EIXO Y
	yi = chtDados->LeftAxis->CalcPosValue(chtDados->LeftAxis->Minimum) + 5;
	yf = chtDados->LeftAxis->CalcPosValue(chtDados->LeftAxis->Maximum) - 5;

	// Define a 'caneta', a cor e desenha a linha transversal
	if (xf < chtDados->BottomAxis->CalcPosValue(chtDados->BottomAxis->Maximum) + 1)
	{
		chtDados->Canvas->Pen->Style = psSolid;
		chtDados->Canvas->Pen->Color = clWhite;
		chtDados->Canvas->Rectangle(xi, yi, xf, yf);
	}
}
// ---------------------------------------------------------------------------
void TFormPrincipal::AtualizarControles()
{
	// Habilitar/Desabilitar controles
	cmbPortaSerial->Enabled = !comunicando;
	cmbBaudrate->Enabled = !comunicando;
	btnAtualizarPortas->Enabled = !comunicando;
	btnConectar->Enabled = !comunicando;
	btnDesconectar->Enabled = comunicando;
}
// ---------------------------------------------------------------------------
void TFormPrincipal::ListarPortasSeriais()
{
	// Verifica quais portas seriais est�o dispon�veis/conectadas.
	std::vector<AnsiString>asDetectedPorts;
	asDetectedPorts = portaSerial->CheckSerialPort();

	// Limpar comboBox
	cmbPortaSerial->Clear();

	// Listar comboBox
	for (unsigned int a = 0; a < asDetectedPorts.size(); a++)
	{
		cmbPortaSerial->Items->Add(asDetectedPorts[a]);
	}

	// Definir a primeira porta serial dispon�vel como selecionada
	cmbPortaSerial->ItemIndex = 0;
}
// ---------------------------------------------------------------------------
void TFormPrincipal::IniciarProcessos()
{
	// Status de conex�o
	pgbStatusConexao->Position = 0;

	// Criar e abrir a porta serial
	portaSerial = new SerialPort();
	if (portaSerial->OpenSerialPort(cmbPortaSerial->Text, cmbBaudrate->Text))
	{
		FormMonitorarDados->txtDadosRecebidos->Lines->Add("Porta Serial Aberta...");
		comunicando = true;
	}

    // Configura o gr�fico
    ConfigGrafico();

	// Instancia��o Declara��o da Thread
	threadAquisicao = new Thread(true); 		// Cria a thread suspensa, sem executar ainda.
	threadAquisicao->portaSerial = portaSerial; // Define a porta serial
	threadAquisicao->Resume(); 					// Faz com que a thread seja executada.

	// Status de conex�o
	pgbStatusConexao->Position = 100;
}
// ---------------------------------------------------------------------------
void TFormPrincipal::FinalizarProcessos()
{
	// Verifica��o da inst�ncia da thread de atualiza��o do gr�fico.
	if (threadAquisicao != NULL)
	{
		threadAquisicao->Terminate();
		Sleep(500);

		delete threadAquisicao; // se usar (MyThread->FreeOnTerminate = True;) n�o precisa deletar.
		threadAquisicao = NULL;
		FormMonitorarDados->txtDadosRecebidos->Lines->Add("Finalizou a thread e deletou a inst�ncia...");
	}

	// Status de conex�o
	pgbStatusConexao->Position = 50;

	// Verifica��o da inst�ncia da porta serial.
	if (portaSerial != NULL)
	{
		Sleep(500);
		portaSerial->CloseSerialPort();
		FormMonitorarDados->txtDadosRecebidos->Lines->Add("Porta Serial Fechada...");
		portaSerial = NULL;
		comunicando = false;
	}

    // Status de conex�o
	pgbStatusConexao->Position = 0;
}
// ---------------------------------------------------------------------------
bool TFormPrincipal::VerificarCriarPastaDados()
{
	// Faz a verifica��o da pasta, se n�o existe, cria
	try
	{
		PathDadosTemperatura = System::Ioutils::TPath::GetDocumentsPath() + "\\" + "DadosTemperatura";
		if (!TDirectory::Exists(PathDadosTemperatura))
		{
			TDirectory::CreateDirectory(PathDadosTemperatura);
		};
		return true;
	}
	catch (...)
	{
		ShowMessage("Erro a criar diretorio de dados");
		return false;
	}
}
// ---------------------------------------------------------------------------
void __fastcall TFormPrincipal::Abrirmonitordaportaserial1Click(TObject *Sender)
{
	// Exibe a tela de monitoramento serial
	FormMonitorarDados->Show();
}
// ---------------------------------------------------------------------------
void __fastcall TFormPrincipal::setLabelPaginaAtual()
{
	// 3300 � o resultado da multiplica��o dos 33 caracteres de cada linha pelo tamanho da pagina = 100
	int pagFinal = PosicaoFinal / 3300;
	UnicodeString Pagina = "P�gina " + IntToStr(FPaginaAtual) + " de " + IntToStr(pagFinal);
//	lblPagina->Caption = Pagina;
}
// ---------------------------------------------------------------------------
void __fastcall TFormPrincipal::ConfigGrafico()
{
	// Expande o gr�fico para comportar a quantidade de amostras contidas em max_tela.
	chtDados->BottomAxis->Maximum = max_tela;
	chtDados->LeftAxis->Maximum = max_tela;
	for (unsigned int a = 0; a < max_tela + 1; a++)
	{
		//chtDados->Series[0]->AddY(0);
		serieCH0->AddY(0);
	}

	chtDados->Refresh();
}
// ---------------------------------------------------------------------------
void __fastcall TFormPrincipal::PlotarGraficoArquivo()
{
	// Limpa todas as s�ries criadas.
	chtDados->Series[0]->Clear();
	ConfigGrafico();
	int day, month, year, hour, minute, second;
	float temp = 0.0;
    char grau, celsius;

	// Deve ser preenchido as s�ries com valores iniciais para poderem ser trabalhadas.
	for (unsigned int i = 0; i < 100; i++)
	{
		// Realiza a Leitura da Frequencia de Amostragem do arquivo.
		// Exemplo de dado salvo no arquivo: "05/10/2020 - 17:19:01 - 09.000 �C"
		// Exemplo de dado salvo no arquivo: "05/10/2020 - 17:19:01 - 43.500 �C"
		fscanf(
			arq_dados,
			"%2d/%2d/%4d - %2d:%2d:%2d - %f %c%c",
			&day, &month, &year, &hour, &minute, &second, &temp, &grau, &celsius);
		chtDados->Series[0]->YValues->Value[i] = temp;
	}

	chtDados->Repaint();
}
// ---------------------------------------------------------------------------
void __fastcall TFormPrincipal::Abrir1Click(TObject *Sender)
{
	if (OpenDialog->Execute())
	{
		// PararThreadAquisicao();
		FinalizarProcessos();
		HabDesabIniciarEncerrar(false);
//		gpPaginacao->Visible = true;
		FecharArquivo();
		// Abertura do arquivo para verifica��o configura��o inicial.
		arq_dados = fopen(AnsiString(OpenDialog->FileName).c_str(), "rt");
		FArquivoAberto = true;
		FPaginaAtual = 1;
		// Posiciona o Ponteiro no fim do Arquivo.
		fseek(arq_dados, 0, SEEK_END);
		PosicaoFinal = ftell(arq_dados);
		// Posiciona o Ponteiro do Arquivo.
		PosicaoAtual = 0;
		fseek(arq_dados, 0, SEEK_SET);
		fseek(arq_dados, PosicaoAtual, 1);
		// Modificar a posi��o do ponteiro de arquivo para buscar mais janelas de pontos.
		PosicaoAtual = ftell(arq_dados);
		PlotarGraficoArquivo();
		setLabelPaginaAtual();
	}
}
// ---------------------------------------------------------------------------
void __fastcall TFormPrincipal::btnVoltarPaginaClick(TObject *Sender)
{
	FPaginaAtual--;
	PosicaoAtual = ftell(arq_dados);
	if (FPaginaAtual <= 0)
	{
		FPaginaAtual = 1;
	}

	fseek(arq_dados, FPaginaAtual * 330, SEEK_SET);
	// PosicaoAtual = PosicaoAtual + 330;
	int teste = ftell(arq_dados);
	PlotarGraficoArquivo();
	setLabelPaginaAtual();
}
// ---------------------------------------------------------------------------
void __fastcall TFormPrincipal::btnAvancarPaginaClick(TObject *Sender)
{
	FPaginaAtual++;
	PosicaoAtual = ftell(arq_dados);

	if (PosicaoAtual == PosicaoFinal)
	{
		FPaginaAtual--;
		ShowMessage("Fim do arquivo!");
	}
	else
	{
		fseek(arq_dados, FPaginaAtual * 3300, SEEK_SET);
		PosicaoAtual = PosicaoAtual + 3300;
		//int teste = ftell(arq_dados);
		PlotarGraficoArquivo();
	}

	setLabelPaginaAtual();
}
// ---------------------------------------------------------------------------
void __fastcall TFormPrincipal::FormShow(TObject *Sender)
{
//	gpPaginacao->Visible = false;
}
// ---------------------------------------------------------------------------
void __fastcall TFormPrincipal::btnLimparGraficoClick(TObject *Sender)
{
	chtDados->Series[0]->Clear();
	ConfigGrafico();

//	if (gpPaginacao->Visible == true)
//	{
//		gpPaginacao->Visible = false;
//	}

	segundo = minuto = hora = 0;
	AtualizarRelogio();
	HabDesabIniciarEncerrar(true);

	// Fecha o ponteiro do arquivo de dados caso esteja aberto.
	FecharArquivo();
}
// ---------------------------------------------------------------------------
void __fastcall TFormPrincipal::HabDesabIniciarEncerrar(bool _AHab)
{
	btnConectar->Enabled = _AHab;
	btnDesconectar->Enabled = _AHab;
	eTempoDecorrido->Time = EncodeTime(0, 0, 0, 0);
}
// ---------------------------------------------------------------------------
void __fastcall TFormPrincipal::Abrirfaixadedatas1Click(TObject *Sender)
{
	// Recupera a faixa de dados da tela anterior
	int AQtdeDias = 0;
	if (FormSeletorDatas->ShowModal() == mrOk)
	{
		AnsiString DataIni = DateToStr(FormSeletorDatas->eDataInicial->Date);
		AnsiString DataFim = DateToStr(FormSeletorDatas->eDataFinal->Date);
		// Diferen�a do dia final e dia inicial do filtro de datas.
		AQtdeDias = FormSeletorDatas->eDataFinal->Date - FormSeletorDatas->eDataInicial->Date;
	}

	// Abrir um array de arquivos
	for (int i = 0; i <= AQtdeDias; i++)
	{
		// Percorrer arquivos entre as datas
	}
}
// ---------------------------------------------------------------------------
AnsiString __fastcall TFormPrincipal::getStrData(AnsiString _ASeparador)
{
	return StringReplace(Date(), "/", _ASeparador, TReplaceFlags() << rfReplaceAll);
}
// ---------------------------------------------------------------------------
AnsiString __fastcall TFormPrincipal::getStrTime(AnsiString _ASeparador)
{
	return StringReplace(Time(), ":", _ASeparador, TReplaceFlags() << rfReplaceAll);
}
// ---------------------------------------------------------------------------
