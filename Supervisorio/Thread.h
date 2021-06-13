//---------------------------------------------------------------------------
#include <System.Classes.hpp>
//---------------------------------------------------------------------------
#include "SerialPort.h"
#include "frmPrincipal.h"
#include "frmMonitorarDados.h"
//---------------------------------------------------------------------------
#ifndef ThreadH
#define ThreadH
//---------------------------------------------------------------------------
class Thread : public TThread
{
private:
protected:
	void __fastcall Execute();
public:
	__fastcall Thread(bool CreateSuspended);

	// Constantes
	const double fatorConversaoADC = 0.004887585532746823069403714565;
    TDateTime dataHoraCriacaoUltimoArquivo = 0;
    TDateTime dataHoraCriacaoProximoArquivo = 1;

	// Propriedades de controle de comunica��o serial e convers�o
	SerialPort *portaSerial;
	int posicaoNoGrafico = 0;
	std::vector<unsigned char> buffer;

	void ClearBufferSerial();
    void ZerarBpm();
	void CalcularTemperatura(unsigned char high, unsigned char low);
	void __fastcall CalcularBpm(unsigned char high, unsigned char low, float antigo);
	void __fastcall AtualizarInterface();
    void __fastcall ResetarValoresIniciaisbpm(void);
};
//---------------------------------------------------------------------------
#endif