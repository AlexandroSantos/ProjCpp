/*
   Arquivo: ClockCalendar.cpp

   Define as funções das classes inicializadas no cabeçalho  
   ClockCalendar.h
   
   Criado por:
   Eduardo Augusto Bezerra <eduardo.bezerra@ufsc.br>
   Departamento de Engenharia Eletrica
	
      
   Adaptado por: Alexandro Vandeley dos Santos
   Data da ultima alteracao: 11 de abril de 2021. 

*/
#include "ClockCalendar.h" 

Clock::Clock (int h, int m, int s, int pm){
	hr=h; min=m; sec=s;	is_pm=pm; }
	
void Clock::set_Clock (int h, int m, int s, int pm){
	hr=h; min=m; sec=s;	is_pm=pm; }	

Calendar::Calendar (int m, int d, int y){
	mo=m; day=d; yr=y;
}

void Calendar::set_Calendar (int m, int d, int y){
	mo=m; day=d; yr=y;
}
	
void Calendar::advance(){
	if (day++==31) {day=1;if (mo++==13) {mo=1;yr++;}}
}


void Clock::advance(){
	if (++sec==60) {
		sec=0;
		if (++min==60) {
			min=0;
			if ((++hr==12)&&(is_pm)){hr=0;is_pm=(is_pm+1)&1;}
			if (hr==13) hr=1;
		}
	}
}

ClockCalendar::ClockCalendar (int mt, int d, int y, int h, int mn, int s, int pm) : 
				Clock (h, mn, s, pm), 
				Calendar (mt, d, y){}

void ClockCalendar::advance (){ 	// Avançar o calendário, caso o clock
	int wasPm = Clock::is_pm; 		// mude de AM para PM.	
	Clock::advance();	
	if (wasPm && !Clock::is_pm)
		Calendar::advance();
}
void ClockCalendar::set_DataTime (int mt, int d, int y, int h, int mn, int s, int pm) {
	set_Clock (h, mn, s, pm);
	set_Calendar (mt, d, y);
	}	

DtTimecc ClockCalendar::get_DataTime (){		
	dt.yr=yr;
	dt.mo=mo;
	dt.day=day;
	dt.hr=hr;
	dt.min=min;
	dt.sec=sec;
	dt.pm=is_pm;
	return dt;
}
