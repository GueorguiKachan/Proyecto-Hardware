#include "Gestor_pulsacion.h"
#include <LPC210X.H>    
static volatile int nueva_pulsacion_eint1 =0;
static volatile int nueva_pulsacion_eint2 =0;



int button_nueva_pulsacion_1(){
  return nueva_pulsacion_eint1;
}

int button_nueva_pulsacion_2(){
  return nueva_pulsacion_eint2;
}

void button_clear_nueva_pulsacion_1(){
  nueva_pulsacion_eint1 = 0;
}

void button_clear_nueva_pulsacion_2(){
  nueva_pulsacion_eint2 = 0;
}

void eint1_ISR (void) __irq {
	VICIntEnable = VICIntEnable | ~posicion_eint1; //se desactivan las solicitudes de eint1
	nueva_pulsacion_eint1 = 1;
}

void eint2_ISR (void) __irq {
	VICIntEnable = VICIntEnable | ~posicion_eint2; //se desactivan las solicitudes de eint1
	nueva_pulsacion_eint2 = 1;
}


void Gestor_Pulsacion_Init(){
	
	EXTMODE = EXTMODE | 0xF9; //set bit 1 and bit 2 to 0 -> eint1 is lvl-sensitive and eint2 is lvl-sensitive
	EXTPOLAR = EXTPOLAR | 0xF9;  //set bit 1 and bit 2 to 0 -> eint1 is low-active and eint2 is low-active

	//Activa las solicitud de interrupcion por eint1 (15) y eint2 (16)
	VICIntEnable = VICIntEnable | posicion_eint1;
	VICIntEnable = VICIntEnable | posicion_eint2;
}
