#include "config.h"
#include <xc.h>

#include "usart.h"

void USART_Init(long BaudRate)
{
	// Tempo de envio de um bit:
	// BaudRate =   9600 -> 1 / baudrate = 1 /   9600 = 1,041667e-4 = 1000000 /   9600 = 104,16us
	// BaudRate = 115200 -> 1 / baudrate = 1 / 115200 = 8,6856e-6   = 1000000 / 115200 =   8,68us
	  
	unsigned char BR = 0;

	// C�lculo do valor para o registrador SPBRG para uma determinada velocidade em bps.	
	BR = (_XTAL_FREQ / (16 * BaudRate)) - 1;
	SPBRG = BR;
	
    // Configuração do Registrador TXSTA.
    TXSTAbits.CSRC	= 0;	// Seleção MASTER/SLAVE para o Modo Síncrono.
    TXSTAbits.TX9	= 0;	// Transmição de Dados em 8 Bits.
    TXSTAbits.TXEN	= 1; 	// Habilita a Transmição de Dados.
    TXSTAbits.SYNC	= 0; 	// Modo de Comunicação Assíncrono.
    TXSTAbits.BRGH	= 1;	// Baud Rate em alta(1) ou baixa(0) velocidade.
    TXSTAbits.TRMT	= 1;	// Situação do Registrador Interno de Transmissão (TSR).
    TXSTAbits.TX9D	= 0;	// Valor a Ser Transmitido como 9º bit (Paridade/Endereçamento).

    // Configuração do Registrador RCSTA.
    RCSTAbits.SPEN	= 1;	// Habilita o Sistema USART.
    RCSTAbits.RX9	= 0;    // Recepção de Dados em 8 Bits.
    RCSTAbits.SREN	= 0;	// Desabilita Recepção Unitária (Somente Modo Síncrono em MASTER).
    RCSTAbits.CREN	= 1;   	// Habilita a Recepção Contínua de Dados.
    RCSTAbits.ADDEN	= 0;  	// Desabilita o Sistema de Endereçamento.
    RCSTAbits.FERR	= 0;	// Erro de Stop Bit.
    RCSTAbits.OERR	= 0;	// Erro de Muitos Bytes Recebidos sem Leitura.
    RCSTAbits.RX9D	= 0;	// Valor a Ser Recebido como 9º bit (Paridade/Endereçamento).

    // Configuração da Interrupção USART.
	PIE1bits.RCIE 	= 1;	// Habilita a Interrupção Serial.
	PIR1bits.RCIF 	= 0;	// Habilita a Interrupção Serial de Recepção.
}
//-----------------------------------------------------------------------------
void USART_WriteChar(unsigned char USART_Data)
{
	while(!PIR1bits.TXIF);
	TXREG = USART_Data;
}
//-----------------------------------------------------------------------------
void USART_WriteString(const char *str)
{
	// Efetua a transmissão da string para a USART.
	while(*str != '\0')
  	{
    	// Envio da string byte a byte.
		USART_WriteChar(*str);
      	str++;
  	}
}
//-----------------------------------------------------------------------------
unsigned char USART_ReceiveChar(void)
{
	unsigned char USART_Data;

	__delay_us(75);					// Tempo necessário para recepção do próximo byte.
	
	if (!OERR)						// Erro de Muitos Bytes Recebidos sem Nenhuma Leitura.
	{
		USART_Data 		= RCREG;	// Recebe o byte da USART e atribui a variável USARTData.
    	PIR1bits.RCIF 	= 0;		// Limpa a Flag da Interrupção de Recepção.
	}
	else
	{
		USART_WriteString("\n\r ------- ESTOURO DE PILHA ------- \n\r ");

		RCSTAbits.CREN 	= 0;   		// Desabilita a Recepção Contínua de Dados Momentaneamente.
		USART_Data 		= RCREG;	// Recebe o byte da USART e atribui a variável USARTData.
		RCSTAbits.CREN 	= 1;   		// Habilita a Recepção Contínua de Dados Novamente.
    	PIR1bits.RCIF 	= 0;		// Limpa a Flag da Interrupção de Recepção.				
	}
	
	return(USART_Data);
}