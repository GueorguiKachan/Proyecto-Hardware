#include "Gestor_pulsacion.h"
#include <LPC210X.H>    
static volatile int nueva_pulsacion_eint1 =0;
static volatile int nueva_pulsacion_eint2 =0;
const int posicion_eint1 = 0x00008000;
const int posicion_eint2 = 0x00010000;


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
	PINSEL0 = 0xa0000000; //a activa eint1 y eint2
	PINSEL1 = 0x00000000;
	
	EXTMODE = EXTMODE | 0xF9; //set bit 1 and bit 2 to 0 -> eint1 is lvl-sensitive and eint2 is lvl-sensitive
	EXTPOLAR = EXTPOLAR | 0xF9;  //set bit 1 and bit 2 to 0 -> eint1 is low-active and eint2 is low-active

	//Activa la posicion 14 y 15 del VIC para las interrupciones 15(eint1) y 16(eint2)
	VICVectCntl14 = 0x20 | 15; 
	VICVectCntl15 = 0x20 | 16;  
	
	//Asigna las rutinas de excepcion
	VICVectAddr14= (unsigned long)eint1_ISR;
	VICVectAddr15= (unsigned long)eint2_ISR;
	
	//Activa las solicitud de interrupcion por eint1 (15) y eint2 (16)
	VICIntEnable = VICIntEnable | posicion_eint1;
	VICIntEnable = VICIntEnable | posicion_eint2;
}
