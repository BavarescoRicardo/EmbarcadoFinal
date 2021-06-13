#include "config.h"
#include <xc.h>
#include <stdlib.h>

#include "usart.h"
#include "adc.h"
#include "timers.h"

// Vari�veis globais do sistema.
unsigned char ADC_Buffer[6];
unsigned int contador = 0;

void interrupt ISR(void)
{
	// Interrup��o do TIMER1 para a frequ�ncia de amostragem do sistema.
	if(PIR1bits.TMR1IF) 
	{
        PIR1bits.TMR1IF = 0;		// Resetar a flag do TIMER1 para uma nova contagem.
		// Adicionei o numero 1 ao inicio dos valores abaixco 7936 
		TIMER1_Set(7936);   		// Frequ�ncia de amostragem de X Hz.
        // TIMER1_Set(7936) para 100 Hz
        // Contagem 100ms, a cada 10 estouros, 1 segundo então cada contador vale 0,10 segundos
        contador++;
		// a linha era/ if(contador == 10) eu alterei para 1 pois o grafico esta muito lento
        if(contador == 1)		// 1x100ms ; cada 0,1 segundo é enviado um pulso = freq = 10Hz
        {
	        // Leitura do canal AN0.
			ADC_Read(0);
			ADC_Buffer[0] = '#';
			ADC_Buffer[1] = '$';
			ADC_Buffer[2] = ':';
	        ADC_Buffer[3] = ADRESH;
	        ADC_Buffer[4] = ADRESL;
	        
			// Enviar pacote de dados com as amostras coletadas.
	        unsigned char checksum = 0x00;
	        for(unsigned char index = 0; index < 5; index++) 
			{
	        	USART_WriteChar(ADC_Buffer[index]);
	        	
	        	// Calcular checksum
	           	checksum ^= ADC_Buffer[index];
	        }
	
			// Finalizador do pacote de dados por checksum.	
			ADC_Buffer[5] = checksum;
	        USART_WriteChar(ADC_Buffer[5]);
	        
	        // Resetar contador
	        contador = 0;
		}
	}		
}
//-----------------------------------------------------------------------------
void main(void)
{
	// Inicializa��o das portas do microcontrolador.
    TRISA	= 0b00000011;	// Configura��o dos canais anal�gicos do PORTA.
    PORTA	= 0b00000011;  	// Inicializa��o dos canais anal�gicos do PORTA.
    TRISB	= 0b00000000;	// Configura��o das entradas/sa�das do PORTB.
    PORTB	= 0b00000000;  	// Inicializa��o das entradas/sa�das do PORTB.
	TRISC	= 0b10000000;	// Configura��o das entradas/sa�das do PORTC.
    PORTC	= 0b11000000;  	// Inicializa��o das entradas/sa�das do PORTC.

	// Inicializa��o do m�dulo PWM do microcontrolador.
	USART_Init(115200);	// Inicializa��o da USART passando a velocidade e o modo de transmiss�o.
	TIMER1_Init();			// Inicializa��o das configura��es do TIMER1.
	ADC_Init();				// Inicializa��o do conversor A/D.

	// Inicializa��o dos par�metros de configura��o do TIMER1.
	TIMER1_Set(7936); 		// Frequ�ncia de amostragem de X Hz.

	// Ativa��o das interrup��es do microcontrolador.
	INTCONbits.PEIE	= 1;	// Habilita Interrup��o de Perif?ricos do Microcontrolador.
	INTCONbits.GIE	= 1;	// Habilita Interrup��o Global.

	USART_WriteString("Inicializando USART...");	// Inicializador do pacote de dados.

	while(1)
	{	
	}
}
//-----------------------------------------------------------------------------
