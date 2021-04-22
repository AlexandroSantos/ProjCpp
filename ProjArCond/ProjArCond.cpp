/*
 * ProjArCond.cpp
 * 
 * Programa principal
 * Função: Automação e controle de Ar Condicionado
 * Desenvolvido para Raspberry PI PICO
 * Harware principais:
 * 	- Rapberry PI PICO
 * 	- 1 reles
 * 	- 1 botões 
 * 	- 2 Sensores (Porta e Janela)
 * 	- Porta serial ou Adaptador serial/USB	
 *  - Adaptador WiFi - Telnet (Ligado a aporta serial)
 * 
 * Requisitos: Arquivos de instalação e tutorial para instalação do SDK
 * 			   C/C++, do Raspberry PI pico, estão neste pdf (página 4):
 * https://datasheets.raspberrypi.org/pico/getting-started-with-pico.pdf
 * Acessado em 22/04/2021.				
 * 
 * OBS.:1) Utilizado a distribuição Linux Mint 20.1 Cinnamon 
 * 		   para o desenvimento.
 * 
 * 		2) No arquivo ProjCpp/ProjArCond/CMakeLists.txt a linha contendo
 * 			set(PICO_SDK_PATH "/home/alex/pico/pico-sdk")
 * 		deverá ser alterada apontando para o diretório de instalação do 
 * 		SDK C/C++.
 * 		
 * 		3) No arquivo ProjCpp/ProjArCond/build/Makefile todas as linhas
 * 		contendo "/home/alex/ProjCpp/ProjArCond" deverão ser modificadas
 * 		a fim de apontar o caminho correto do código fonte.
 * 
 * Compilação: Dentro do diretório ProjCpp/ProjArCond/build deverá ser
 * 			   executado o comando: 
 * 			   # make		
 * 
 * Autor: Alexandro Vanderley dos Santos
 * Data : 22/04/2021
 */
#include <stdio.h>

//Bibliotecas para o raspberry pi pico
#include "pico/stdlib.h"
#include "hardware/gpio.h"

//Classes desenvolvidas 
#include "lib/ClockCalendar.h"
#include "lib/List.h"

//Seleciona as portas do Raspberry PI Pico
int sensor_1  = 2; //Porta
int sensor_2  = 3; //Janela
int btn_start = 4; //Botão Start
int rele_ar   = 5; //Relé do ar condicionado 
int led_Alerta= 6; //Led indicador de porta/janela aberta

//Definição da porta serial
#define UART_ID uart0
#define BAUD_RATE 115200
#define DATA_BITS 8
#define STOP_BITS 1
#define PARITY    UART_PARITY_NONE

// Pinos da porta serial
#define UART_TX_PIN 0
#define UART_RX_PIN 1

//**********************************************************
//Retorna o tempo após o boot em ms 
uint32_t millis(){	 
    return to_ms_since_boot(get_absolute_time());
} 

//**********************************************************
// Programa principal
int main()
{
	//Inicia os periféricos e portas
    stdio_init_all();         
    gpio_init(rele_ar);   
    gpio_set_dir(rele_ar, GPIO_OUT);     
    gpio_put(rele_ar,0); 
    gpio_set_dir(led_Alerta, GPIO_OUT);     
    gpio_put(led_Alerta,0);    
    gpio_init(btn_start);   
    gpio_set_dir(btn_start, GPIO_IN);        
	
	//Iniciando objetos
	ClockCalendar cc(4,8,2021,10,59,56,0);
	List Relatorio;
	Mens mensagem;
	    
	//**********************************************************
	// Configura a velocidade da porta serial
    uart_init(UART_ID, BAUD_RATE);
    // Configura os pinos RX e TX da porta serial
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);
    // Desabilita o controle de fluxo CTS/RTS da porta serial
    uart_set_hw_flow(UART_ID, false, false);
    // Formato dos dados em trânsito pela serial
    uart_set_format(UART_ID, DATA_BITS, STOP_BITS, PARITY);
    uart_set_fifo_enabled(UART_ID, false);
    
    //**********************************************************
    /* Espaço reservado para configurar e iniciar o serviço Telnet
     * via porta serial utilizando o módulo WiFi.
     */
       
    //**********************************************************
    //Variáveis para o loop infinito    
	uint32_t timerSec=millis();
	uint8_t dados_Serial[15];
	char ligado=0;
		
	//loop infinito
    while(true){
		//***********************************************************
		//Atualiza o calendário a cada (1000) milisegundos = 1s		
		if (millis() - timerSec >= 1000) { 
			timerSec += 1000;		  
			cc.advance();		  
		}
		
		//***********************************************************
		//Verifica se o botão start foi pressionado [com debounce]
		if ((gpio_get(btn_start)==1)&&(ligado==0)) {			
			while(gpio_get(btn_start)==1);			
			sleep_ms(150);		
			gpio_put(rele_ar,1);
			ligado=1;					
			mensagem.ID=1;
			mensagem.dt=cc.get_DataTime();
			mensagem.oper=1;
			Relatorio.insertAfterLast(mensagem);						
        }	
        //***********************************************************
		//Verifica se os sensores foram ativados
		if (((gpio_get(sensor_1)==1)||(gpio_get(sensor_2)==1))&&(ligado==1)) {	
			char sensor=(gpio_get(sensor_1)<<2)+(gpio_get(sensor_2)<<3);
			gpio_put(rele_ar,0);			
			ligado=0;				
			mensagem.ID=1;
			mensagem.dt=cc.get_DataTime();
			mensagem.oper=2+sensor;
			Relatorio.insertAfterLast(mensagem);						
        }     
        
        if ((gpio_get(sensor_1))&&(gpio_get(sensor_2))) gpio_put(led_Alerta,0);
        else gpio_put(led_Alerta,1);		
        
		//***********************************************************
		/* 		Verifica se a serial está recebendo dados e decodifica.
		 * Controla o equipamento, transmite dados da lista e dados de 
		 * configuração. Neste trecho pode-se configurar data e hora do
		 * hardware.
		 *
		 * OBS.: O If abaixo deverá ser alterado para uma possível 
		 * entrada de dados através da Telnet-WiFi.
		 * ex.: if ((uart_is_readable(UART_ID))||(telnet_readable()))
		 * 		 
		 * 		Também deverá ser alterado a forma de captura da 
		 * variável	ch, dependendo da porta que recebeu os dados.
		 * ex.: uint8_t ch = telnet_getc();
		 * 
		 */
		 
		if (uart_is_readable(UART_ID)){
			uint8_t ch = uart_getc(UART_ID);//Recebe o comando		
			switch (ch) {	
				case 0x4C: if (ligado==0){	//L - Ligar equipamento	
					gpio_put(rele_ar,1);			
					ligado=1;
					mensagem.ID=1;
					mensagem.dt=cc.get_DataTime();
					mensagem.oper=1;
					Relatorio.insertAfterLast(mensagem);
				}					
			break;
				case 0x44: if (ligado==1){	//D - Desligar equipamento		
					gpio_put(rele_ar,0);
					ligado=0;							
					mensagem.ID=1;
					mensagem.dt=cc.get_DataTime();
					mensagem.oper=2;
					Relatorio.insertAfterLast(mensagem);				
				}
			break;
				case 0x52:					//R - Relatório solicitado					
					mensagem=Relatorio.removeFirst();
					if (mensagem.ID>0) {
						uart_putc(UART_ID, 0xAA); 			// Inicio de pacote
						uart_putc(UART_ID, mensagem.ID); 	//ID do equipamento
						uart_putc(UART_ID, mensagem.dt.yr%2000); // ano - 2000
						uart_putc(UART_ID, mensagem.dt.mo); // mes
						uart_putc(UART_ID, mensagem.dt.day);// dia
						uart_putc(UART_ID, mensagem.dt.hr); //hora
						uart_putc(UART_ID, mensagem.dt.min);//minutos
						uart_putc(UART_ID, mensagem.dt.sec);//segundos
						uart_putc(UART_ID, mensagem.dt.pm); //am ou pm
						uart_putc(UART_ID, mensagem.oper); 	// operação
						uart_putc(UART_ID, 0xBB); 			// Fim de pacote
					}					
			break;
				case 0x43:					// C - Configuração solicitada
					mensagem.ID=1;
					mensagem.dt=cc.get_DataTime();
					if (ligado==0) mensagem.oper=0; else mensagem.oper=10;
					uart_putc(UART_ID, 0xCC);
					uart_putc(UART_ID, mensagem.ID); 	//ID do equipamento
					uart_putc(UART_ID, mensagem.dt.yr%2000); // ano - 2000
					uart_putc(UART_ID, mensagem.dt.mo); // mes
					uart_putc(UART_ID, mensagem.dt.day);// dia
					uart_putc(UART_ID, mensagem.dt.hr); //hora
					uart_putc(UART_ID, mensagem.dt.min);//minutos
					uart_putc(UART_ID, mensagem.dt.sec);//segundos
					uart_putc(UART_ID, mensagem.dt.pm); //am ou pm
					uart_putc(UART_ID, mensagem.oper); 	// operação						
					uart_putc(UART_ID, 0xDD);			
			break;
				case 0x53:					// S - Salva Configuração				
					uart_read_blocking(UART_ID, dados_Serial, (size_t) 6);						
					cc.set_DataTime(dados_Serial[1],dados_Serial[2],
					dados_Serial[0]+2000,dados_Serial[3],dados_Serial[4],
					dados_Serial[5],dados_Serial[6]);					
					
			break;
			} //Fim do switch
			ch=0;
		}//Fim do IF serial
	}//Fim do loop
    return 0;
}
