#include "Gestor_pulsacion.h"
#include "eventos.h"
#include <stdint.h>
#include "cola.h"
#include <LPC210X.H>   
static volatile int nueva_pulsacion_eint1 =0;
static volatile int nueva_pulsacion_eint2 =0;
const int posicion_eint1 = 0x00008000;
const int posicion_eint2 = 0x00010000;


static int button_nueva_pulsacion_1(){
  return nueva_pulsacion_eint1;
}

static int button_nueva_pulsacion_2(){
  return nueva_pulsacion_eint2;
}

static void button_clear_nueva_pulsacion_1(){
  nueva_pulsacion_eint1 = 0;
}

static void button_clear_nueva_pulsacion_2(){
  nueva_pulsacion_eint2 = 0;
}

static bool boton1_estado(){
	EXTINT = EXTINT | 2;
	//si esta pulsado
	if ((VICRawIntr & posicion_eint1) == posicion_eint1) {
		VICIntEnClear = posicion_eint1;
		nueva_pulsacion_eint1 = 1;
	return true;}
	//si no esta pulsado
	else {
		button_clear_nueva_pulsacion_1();
		VICIntEnable |= posicion_eint1;
		return false;
	}
}

static bool boton2_estado(){
	EXTINT = EXTINT | 3;
	//si esta pulsado
	if ((VICRawIntr & posicion_eint2) == posicion_eint2) {
		VICIntEnClear = posicion_eint2;
		nueva_pulsacion_eint2 = 1;
		return true;}
	//si no esta pulsado
	else {
		button_clear_nueva_pulsacion_2();
		VICIntEnable |= posicion_eint2;
		return false;
	}
}

bool eint1_comprobar(){
	uint8_t evento;
	uint32_t aux;
	//si el boton sigue pulsado
	if (boton1_estado()){
		evento = alarmaSet;
		aux = pulsacion1;
		aux = aux << 24;
		aux = aux | 100;
		cola_guardar_eventos(evento, aux);
		return false;
	}
	return true;
}

bool eint2_comprobar(){
	uint8_t evento;
	uint32_t aux;
	//si el boton sigue pulsado
	if (boton2_estado()){
		evento = alarmaSet;
		aux = pulsacion2;
		aux = aux << 24;
		aux = aux | 100;
		cola_guardar_eventos(evento, aux);	
		return false;
	}
	return true;
}

void eint1_ISR (void) __irq {
	uint8_t evento;
	uint32_t aux;
	
	EXTINT = EXTINT | 2;
	VICIntEnClear = posicion_eint1;
	nueva_pulsacion_eint1 = 1;
	
	evento = alarmaSet;
	aux = pulsacion1;
	aux = aux << 24;
	aux = aux | 100;
	cola_guardar_eventos(evento, aux);
	
	VICVectAddr = 0; 
}

void eint2_ISR (void) __irq {
	uint8_t evento;
	uint32_t aux;
	
	EXTINT = EXTINT | 3;
	VICIntEnClear = posicion_eint2;
	nueva_pulsacion_eint2 = 1;
	
	evento = alarmaSet;
	aux = pulsacion2;
	aux = aux << 24;
	aux = aux | 100;
	cola_guardar_eventos(evento, aux);
	
	VICVectAddr = 0; 
}


void Gestor_Pulsacion_Init(){
	button_clear_nueva_pulsacion_1();
	button_clear_nueva_pulsacion_2();
	
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
