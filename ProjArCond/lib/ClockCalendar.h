#ifndef CLOCKCALENDAR_H_
#define CLOCKCALENDAR_H_
/*
   Arquivo: ClockCalendar.h

   Classes Calendar, Clock e ClockCalendar   
   
   Criado por:
   Eduardo Augusto Bezerra <eduardo.bezerra@ufsc.br>
   Departamento de Engenharia Eletrica

      
   Adaptado por: Alexandro Vandeley dos Santos
   Data da ultima alteracao: 11 de abril de 2021. 

*/

#include <stdio.h>
#include "header.h"

class Calendar {
	protected:
		int mo, day, yr;		
	public:
		Calendar (int m, int d, int y);
		void set_Calendar (int m, int d, int y);		
		void advance ();
};

class Clock {
	protected:
		int hr, min, sec, is_pm;
	public:
		Clock (int h, int m, int s, int pm);
		void set_Clock (int h, int s, int m, int pm);
		void advance ();
};

class ClockCalendar : public Clock, public Calendar {
	protected:
	DtTimecc dt;
	public:
		ClockCalendar (int mt, int d, int y, int h, int m, int s, int pm);		
		void advance ();
        void set_DataTime (int mt, int d, int y, int h, int mn, int s, int pm);
		DtTimecc get_DataTime();
		
};

#endif //CLOCKCALENDAR_H_
