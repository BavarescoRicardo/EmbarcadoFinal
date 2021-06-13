//---------------------------------------------------------------------------
#include <System.hpp>
#pragma hdrstop

#include "Thread.h"
#pragma package(smart_init)
//---------------------------------------------------------------------------
// Cria variaveis globais para caulcular os batimentos
	float maximo = 0;
	float minimo = 1000;
	float valor_ant=0;
	// Variavel para saber o estado do sinal
	boolean desce_sobe = 0;
	boolean desce_sobe_ant = 0;
	int npulsos = 0;
	int tempo_decorrido = 0;

	boolean erroLeitura = false;

// fun��o que zera os batimentos apos passar 1 minuto e os valores forem exibidos na tela
void Thread::ZerarBpm()
{
    npulsos = 0;
}

// Fun��o para calcular os batimentos usando maximo e minimo e estado subindo descendo
void __fastcall Thread::CalcularBpm(unsigned char high, unsigned char low, float antigo)
{
        // Vari�veis de controle
	float sinal_cardioLido = 0;
	char sinal_cardio[30];

	// Juntando dois LSB com os 8 MSB gerando um valor entre 0 e 1023
	float valor = ((high << 8) + low);

	// Multiplicando pelo fator de convers�o do conversor A/D (5/1023) para obter a tens�o
	//sinal_cardioLido = (valor * fatorConversaoADC) * 100;

	// Express�o ternaria para eliminar valores muito altos e ampliar os sinais provavelmente corretos do fluxo sanguineio
	sinal_cardioLido = valor > 20 ? 0 : (valor*5);
	// Grava os valores em um arquivo de textto em documentos/dados temp
	FormPrincipal->AtualizarArquivo(sinal_cardioLido, npulsos);

	// Adicionar no grafico
	sprintf(sinal_cardio, "%.3f", sinal_cardioLido);
	FormMonitorarDados->txtDadosRecebidos->Lines->Add(sinal_cardio);

	// Tratamento para contar pulsos
	// condi��es para determinar o estado � subindo um pulso
	if (valor_ant <= sinal_cardioLido)
	{
		desce_sobe = true;
		// Se o sentido do sinal mudou ent�o um pulso de batimento esta completo
		if (desce_sobe != desce_sobe_ant)
		{
			npulsos++;
		}
	}   // sinal do pulso esta descendo
	else
	{
		desce_sobe = false;
	}


    // Por fim salva o valor lido como antigo para a prox conferencia na proxima itera��o do la�o
	valor_ant = sinal_cardioLido;
    desce_sobe_ant = desce_sobe;


	// Plotar no gr�fico
	FormPrincipal->serieCH0->YValues->Value[posicaoNoGrafico] = sinal_cardioLido;
	posicaoNoGrafico = posicaoNoGrafico >= FormPrincipal->max_tela ? 0 : posicaoNoGrafico + 1;
	Synchronize(&AtualizarInterface);


}

__fastcall Thread::Thread(bool CreateSuspended)
	: TThread(CreateSuspended)
{
	Priority = tpHigher;
	FormMonitorarDados->txtDadosRecebidos->Lines->Add("Iniciou a thread...");
}

void __fastcall Thread::ResetarValoresIniciaisbpm(void)
{
	maximo = 0;
	minimo = 1000;
	valor_ant=0;
	desce_sobe = 0;
	desce_sobe_ant = 0;
}
//---------------------------------------------------------------------------
void __fastcall Thread::Execute()
{

	// Enquanto a thread estiver em execu��o
	while (!Terminated)
	{
		// Armazena em buffer a resposta enviada pelo PIC.
		std::vector<unsigned char>bufferTmp = portaSerial->ReadABuffer();
		buffer.insert(buffer.end(), bufferTmp.begin(), bufferTmp.end());

        // Enquanto existir ao menos um pacote
		while(buffer.size() >= 6)
		{
			// Validar cabe�alho
			if (buffer.at(0) == '#' && buffer.at(1) == '$' && buffer.at(2) == ':')
			{
				// Obt�m o primeiro pacote e apaga do buffer
				std::vector<unsigned char>pacote = {buffer.begin(), buffer.begin() + 6};
				buffer.erase(buffer.begin(), buffer.begin() + 6);

				// Calcular o checksum
				unsigned char checksum = 0;
				for (int index = 0; index < 5; index++)
				{
					checksum ^= pacote.at(index);
				}

				// Pacote v�lido
				if (checksum == pacote.at(5))
				{
					CalcularBpm(pacote.at(3), pacote.at(4), 0);
				}
			}
			else
			{
				// Mensagem de erro
//				FormMonitorarDados->txtDadosRecebidos->Lines->Add("Erro de pacote!");

				// Limpar pacotes inv�lidos
				ClearBufferSerial();
			}
		}
	}
}
//---------------------------------------------------------------------------
// Apagar o primeiro caractere do buffer at� encontrar um cabe�alho de pacote v�lido
void Thread::ClearBufferSerial()
{
	// Validar o cabe�alho
	while ((buffer.size() >= 3) && ((buffer.at(0) != '#') || (buffer.at(1) != '$') || buffer.at(2) != ':'))
	{
		// Apaga do buffer
		buffer.erase(buffer.begin(), buffer.begin() + 1);
	}
}
//---------------------------------------------------------------------------
// Converter dado recebido para sinal_cardio, gravar no arquivo e plotar no gr�fico
void Thread::CalcularTemperatura(unsigned char high, unsigned char low)
{
    // Vari�veis de controle
	float sinal_cardioLido = 0;
   	char sinal_cardio[30];

	// Juntando dois LSB com os 8 MSB gerando um valor entre 0 e 1023
	float valor = ((high << 8) + low);

	// Multiplicando pelo fator de convers�o do conversor A/D (5/1023) para obter a tens�o
	//sinal_cardioLido = (valor * fatorConversaoADC) * 100;
	sinal_cardioLido = valor > 20 ? 0 : (valor*5);

	// Grava os valores em um arquivo de textto em documentos/dados temp
	FormPrincipal->AtualizarArquivo(sinal_cardioLido, npulsos);

	// Adicionar no grafico
	sprintf(sinal_cardio, "%.3f", sinal_cardioLido);
	FormMonitorarDados->txtDadosRecebidos->Lines->Add(sinal_cardio);

	// Plotar no gr�fico
	FormPrincipal->serieCH0->YValues->Value[posicaoNoGrafico] = sinal_cardioLido;
	posicaoNoGrafico = posicaoNoGrafico >= FormPrincipal->max_tela ? 0 : posicaoNoGrafico + 1;
	Synchronize(&AtualizarInterface);
}
//---------------------------------------------------------------------------
void __fastcall Thread::AtualizarInterface()
{
	// Atualizar o gr�fico
	FormPrincipal->chtDados->Refresh();
}
//---------------------------------------------------------------------------
