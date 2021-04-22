/* ProgAr.cpp
 * Programa principal
 * Desenvolvido para Linux
 * 
 * Função: Controle do ar condicionado e relatório de uso
 * Este programa pode conectar o hardware, ligar, Desligar o 
 * ar condicionado. Também tem a funçãode configurar data e 
 * hora do hardware, e solicitar relatório de uso.
 * 
 * Requisitos: 
 *  - Biblioteca ncurses;
 *  - O hardware deve estar conectado em /dev/ttyUSB0 ou /dev/ttyUSB1
 * 
 * Compilar: g++ -o ProgAr ProgAr.cpp -lncurses
 * Uso: ./ProgAr
 * 
 * Autor: Alexandro Vanderley dos Santos
 * Data : 22/04/2021 
 * */

// C library headers
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>
#include <ctime>
#include <fstream>

// Linux headers
#include <fcntl.h> // Contains file controls like O_RDWR
#include <errno.h> // Error integer and strerror() function
#include <termios.h> // Contains POSIX terminal control definitions
#include <unistd.h>  // write(), read(), close()
#include <ncurses.h> //Terminal screen options

// Bibliotecas desenvolvidas
//#include "Lista.h"

using namespace std;
void  tela_Relatorio(WINDOW * win);
void tela_Config_Hard(WINDOW * win, char* buf);
void tela_Menu(WINDOW * win);

int main(){
	system("clear");
	
	// Open the serial port. 
	int serial_port = open("/dev/ttyUSB0", O_RDWR);
	// Create new termios struc, we call it 'tty' for convention
	struct termios tty;
	// Read in existing settings, and handle any error
	if(tcgetattr(serial_port, &tty) != 0) {
		//printf("Error %i from tcgetattr: %s\n", errno, strerror(errno));
		int serial_port = open("/dev/ttyUSB1", O_RDWR);
		// Read in existing settings, and handle any error
		if(tcgetattr(serial_port, &tty) != 0) {
			printf("Error %i from tcgetattr: %s\n", errno, strerror(errno));
			printf("\nVerifique as conexões!\n");
		return 1;
		}      
	}  
    tty.c_cflag &= ~PARENB;  // Clear parity bit, disabling parity (most common)
	tty.c_cflag &= ~CSTOPB;  // Clear stop field, only one stop bit used in communication (most common)
	tty.c_cflag &= ~CSIZE;   // Clear all bits that set the data size 
	tty.c_cflag |= CS8;      // 8 bits per byte (most common)
	tty.c_cflag &= ~CRTSCTS; // Disable RTS/CTS hardware flow control (most common)
	tty.c_cflag |= CREAD | CLOCAL; // Turn on READ & ignore ctrl lines (CLOCAL = 1)
	tty.c_lflag &= ~ICANON;
	tty.c_lflag &= ~ECHO;  // Disable echo
	tty.c_lflag &= ~ECHOE; // Disable erasure
	tty.c_lflag &= ~ECHONL;// Disable new-line echo
	tty.c_lflag &= ~ISIG;  // Disable interpretation of INTR, QUIT and SUSP
	tty.c_iflag &= ~(IXON | IXOFF | IXANY); // Turn off s/w flow ctrl
	tty.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL); // Disable any special handling of received bytes	
	tty.c_oflag &= ~OPOST; // Prevent special interpretation of output bytes (e.g. newline chars)
	tty.c_oflag &= ~ONLCR; // Prevent conversion of newline to carriage return/line feed 
	tty.c_cc[VTIME] = 10;    // Wait for up to 1s (10 deciseconds), returning as soon as any data is received.
	tty.c_cc[VMIN] = 0;
	// Set in/out baud rate to be 115200
	cfsetispeed(&tty, B115200);
	// Save tty settings, also checking for error
	if (tcsetattr(serial_port, TCSANOW, &tty) != 0) {
	  printf("Error %i from tcsetattr: %s\n", errno, strerror(errno));
	  printf("\n Verifique as conexões!\n");
	  return 1;
	}	
	
	//Inicia Ncurses
	initscr();
	noecho();
	cbreak();
	curs_set(0);	
	int yMax, xMax;
	getmaxyx(stdscr, yMax, xMax);	
	WINDOW * menuwin = newwin(7, 74, yMax-8,5);
	box(menuwin, 0, 0);
	WINDOW * confwin = newwin(12, 74, yMax-20,5);
	box(confwin, 0, 0);		
	
	refresh();
	wrefresh(menuwin);
	wrefresh(confwin);
	tela_Menu(menuwin);	
	keypad(menuwin, true);
	//Opções do menu principal
	string choices[6] = {"Ligar", 
						"Desligar", 
						"Ler Setup", 
						"Atualizar Setup", 
						"Relatório",
						"Sair "};
	int choice=0;
	int highlight = 0;
	int num_bytes;
	int sair=0;
	char position[6]={3, 11, 22 , 34, 54, 68};	
	char ser_Buf [32];	
	
	while(sair==0){
		box(menuwin, 0, 0);
		//Cria menu e obtem a opção escolhida
		while(choice!=10)
		{			
			for (int i = 0; i<6; i++) 
			{
				if (i==highlight) wattron(menuwin, A_REVERSE);
				mvwprintw(menuwin, 4, position[i], choices[i].c_str());
				wattroff(menuwin, A_REVERSE);
			}
			choice = wgetch(menuwin);
			tela_Menu(menuwin);
			switch(choice){
				case KEY_LEFT:
					highlight--;
					if (highlight == -1) highlight=0;
					break;
				case KEY_RIGHT:
					highlight++;
					if (highlight == 6) highlight=5;
					break;
				default:
					break;
			}			
		}	//Fim do while choices menu
		
		//Ativa o Ar condicionado
		if (highlight==0) write(serial_port, "L", 1);
		//Desativa o Ar condicionado
		if (highlight==1) write(serial_port, "D", 1);
		//Solicita a configuração do Hardware
		if (highlight==2) {
			write(serial_port, "C", 1);
			memset(&ser_Buf, '\0', sizeof(ser_Buf));  		   
			num_bytes = read(serial_port, &ser_Buf, sizeof(ser_Buf));
			tela_Config_Hard(confwin, ser_Buf);			
		}	
		//Atualiza data e hora do Hardware com a do sistema operacional	
		if (highlight==3) {
			time_t now = time(0);
			tm *ltm = localtime(&now);
			ser_Buf[0]='S';
			ser_Buf[1]=ltm->tm_year-100;			
			ser_Buf[2]=1 + ltm->tm_mon;
			ser_Buf[3]=ltm->tm_mday;
			if (ltm->tm_hour>12) {
				ser_Buf[4]=ltm->tm_hour-12;
				ser_Buf[7]=1;
				}
			else{
				ser_Buf[4]=ltm->tm_hour;
				ser_Buf[7]=0;
				}				
			ser_Buf[5]=ltm->tm_min;
			ser_Buf[6]=ltm->tm_sec;			
			write(serial_port, ser_Buf, 8);						
		}	
		
		//Salva em arquivo e apresenta Resumo do Relatório	
		if (highlight==4){
			FILE *fp = fopen("relatorio.dat", "ab+");
			do{
				write(serial_port, "R", 1);
				memset(&ser_Buf, '\0', sizeof(ser_Buf));  		   
				num_bytes = read(serial_port, &ser_Buf, sizeof(ser_Buf));
				if (num_bytes>0){					
					for (int t=1; t<10;t++) fprintf(fp,"%02d ",ser_Buf[t]);
					fprintf(fp,"\n");															
				}
			}while (num_bytes>0);
			fclose(fp);
			tela_Relatorio(confwin);
		}
		
		//Sai do programa	
		if (highlight==5) {sair=1;}			
		choice=0;
	}//Fim do while Sair
	
	endwin();
	system("clear");
	return 0;
}

//Apresenta o relatório
void tela_Relatorio(WINDOW * win){	
	wclear(win);
	int pag=0;
	int ini=0;	
	int fim=0;
	int numOfLines = 0;
	int ID;
	char buf[6][9];
	string line;
	box(win, 0, 0);
	ifstream fp;
    fp.open ("relatorio.dat", ios::in);
    // Verifica se o arquivo existe
    if ( !fp ){
		mvwprintw(win, 2,2,"Importar Relatório!");
    }
    else{
		while(getline(fp, line)){			
            numOfLines++;            
        } 
        if (numOfLines>4) fim=5;              
        mvwprintw(win, 1,30,"Relatório");
        mvwprintw(win, 2,2,"Número de operações: %d",numOfLines);                
        
        //***********************************************************
        keypad(win, true);
		//Opções do menu secundário
		string choices[3] = {"Próximo", 
							"Anterior", 							
							"Sair "};
		int choice=0;
		int highlight = 0;		
		int sair=0;
		char position[6]={11, 34, 60};	
		int n=5;  //Número de linhas por página
		if (numOfLines<6) {n=numOfLines;pag=2;}              
        while(sair==0){		
		//Cria menu secundário e obtém a opção escolhida
			while(choice!=10)
			{			
				fp.clear();				
				fp.seekg((ini)*28, ios::beg);
				mvwprintw(win, 2,40,"Listando de %04d a %04d",ini+1,ini+fim);				
				while(n>0){ 					
					getline(fp, line);
					for (int i=0; i<9; i++)	buf[n][i]=stoi(line.substr(i*3,2));
					mvwprintw(win, 4+(5-n),2,"ID:%02d Data:%02d/%02d/%04d Hora:%02d:%02d:%02d %s Oper.:%02d",
							buf[n][0], buf[n][3], buf[n][2], 2000+buf[n][1],
							buf[n][4],buf[n][5],buf[n][6],buf[n][7]==0?"am":"pm",
							buf[n][8]);
					n--;
				}
				wrefresh(win);
				if (pag==0){
					for (int i = pag; i<3; i++) 
					{
						if (i==highlight) wattron(win, A_REVERSE);
						mvwprintw(win, 10, position[i], choices[i].c_str());
						wattroff(win, A_REVERSE);
					}
					choice = wgetch(win);						
					switch(choice){
						case KEY_LEFT:
							highlight--;
							if (highlight == -1) highlight=0;
							break;
						case KEY_RIGHT:
							highlight++;
							if (highlight == 3) highlight=2;
							break;
						default:
							break;
					}		
				}	
				else {
				choice=10;
				highlight=2;
				}
			}	//Fim do while choices menu	
			
			if (pag==0) {					
				//Próxima página
				if ((highlight==0)&&(fim==5)) {															
					//Limpa a lista
					for (int i=0;i<6;i++) mvwprintw(win, 3+i, 2, "%60s", " ");				
					wrefresh(win);
					ini=ini+4;				
					if (numOfLines<ini) {
						ini=ini-4;
						n=5;
					}					
					else {
						if ((numOfLines-ini)>4) n=5;
						else n=numOfLines-ini;
					}
					fim=n;				
				}
				
				//Página anterior
				if (highlight==1) {	
					//Limpa a lista
					for (int i=0;i<6;i++) mvwprintw(win, 3+i, 2, "%60s", " ");				
					wrefresh(win);					
					if (ini>3) {ini=ini-4;n=5;}	
					else {ini=0;n=5;}	
					fim=n;
				}					
			}					
			
			//Sai do programa	
			if (highlight==2) {sair=1;}			
			choice=0;
		}//Fim do while Sair                	
	}	
	fp.close();
	mvwprintw(win, 10, 6, "%60s", " ");
	wrefresh(win);	
}

//Apresenta a configura de Hardware
void tela_Config_Hard(WINDOW * win, char* buf){
	wclear(win);
	box(win, 0, 0);
	mvwprintw(win, 1,25,"Configuração do Hardware");
	mvwprintw(win, 3,7,"ID: %04d   Ligado: %s", buf[1],buf[9]==10?"Sim":"Não");
	mvwprintw(win, 3,32,"Data: %02d/%02d/%d  Hora: %02d:%02d:%02d %s", 
			  buf[4],buf[3],buf[2]+2000,buf[5],buf[6],buf[7],buf[8]==0?"am":"pm");
	mvwprintw(win, 6,20,"Configuração do Sistema Operacional");
	time_t now = time(0);
	tm *ltm = localtime(&now);
	mvwprintw(win, 8,21,"Data: %02d/%02d/%d   Hora: %02d:%02d:%02d ", 
			  ltm->tm_mday,1 + ltm->tm_mon,1900 + ltm->tm_year,
			  ltm->tm_hour,ltm->tm_min,ltm->tm_sec);
	wrefresh(win);		  
}

//Apresenta menu
void tela_Menu(WINDOW * win){
	wclear(win);
	box(win, 0, 0);
	mvwprintw(win, 1, 23 , "Controle do Ar Condicionado");
	mvwprintw(win, 2, 32 , "<- Menu ->");
	wrefresh(win);		  
}
