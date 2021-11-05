#include "Gestor_pulsacion.h"
#include <LPC210X.H>   
static volatile int nueva_pulsacion_eint1 =0;
static volatile int nueva_pulsacion_eint2 =0;
static volatile unsigned int eint1_count = 0;
static volatile unsigned int eint2_count = 0;
const int posicion_eint1 = 0x00008000;
const int posicion_eint2 = 0x00010000;
static volatile char eint1_estado = 'N';
static volatile char eint2_estado = 'N';


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

void eint1_comprobar(){
	//si el boton no esta pulsado
	EXTINT = EXTINT | 2;
	if ((VICRawIntr & posicion_eint1) != posicion_eint1){ 
		VICVectAddr = 0;
		button_clear_nueva_pulsacion_1();
		VICIntEnable = VICIntEnable | posicion_eint1;
	}else{
	eint1_comprobar();}
	//sino volver a pedir una alarma de 100ms
}

void eint2_comprobar(){
	//si el boton no esta pulsado
	EXTINT = EXTINT | 3;
	if ((VICRawIntr & posicion_eint2) != posicion_eint2){ 
		
		VICVectAddr = 0;
		button_clear_nueva_pulsacion_2();
		VICIntEnable = VICIntEnable | posicion_eint2;
	}
	else{
		eint2_comprobar();}
	//sino volver a pedir una alarma de 100ms
}

void eint1_ISR (void) __irq {
	eint1_count++;
	VICIntEnClear = posicion_eint1;//se desactivan las solicitudes de eint1
	VICVectAddr = 0; 
	EXTINT = EXTINT | 2; //limpio la solicitud de interrupcion
	nueva_pulsacion_eint1 = 1;
	eint1_comprobar();
	//envio una evento de alarma de 100ms q para q ejecute eint2_comprobar()
}

void eint2_ISR (void) __irq {
	eint2_count++;
	VICIntEnClear = posicion_eint2; //se desactivan las solicitudes de eint1
	VICVectAddr = 0; 
	EXTINT = EXTINT | 3; //limpio la solicitud de interrupcion
	nueva_pulsacion_eint2 = 1;	
	//envio una evento de alarma de 100ms q para q ejecute eint1_comprobar()
	eint2_comprobar();
}




void boton1_estado(){
	int entrada = button_nueva_pulsacion_1();
	switch(eint1_estado)
	{
		case 'N': if (entrada == 1) {eint1_estado = 'P';}
							else if (entrada == 0) {eint1_estado = 'N';}
							break;
		case 'P': if (entrada == 1) {eint1_estado = 'P';}
							else if (entrada == 0) {eint1_estado = 'N';}
							break;
	}
}

void Gestor_Pulsacion_Init(){
	button_clear_nueva_pulsacion_1();
	button_clear_nueva_pulsacion_2();
	eint1_count = 0;
	eint2_count = 0;
	
	PINSEL0 = PINSEL0 | 0xa0000000; //a activa eint1 y eint2
	//PINSEL1 = 0x00000000;
	
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
