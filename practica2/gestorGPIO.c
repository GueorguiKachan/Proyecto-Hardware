#include <LPC210X.H>    
#include "gestorGPIO.h"
#include <stdint.h>
#include <math.h>
#include "Gestor_Pulsacion.h"


void GPIO_iniciar(){
	PINSEL0 = 0xa0000000; //a activa eint1 y eint2
	PINSEL1 = 0x00000000;
	
Gestor_Pulsacion_Init();
}
int GPIO_leer(int bit_inicial,int num_bits){
	int i;
	int result = 0;
	uint32_t aux = IOPIN;
	aux = aux / pow(2,bit_inicial);
	for (i = 0; i< num_bits; i++){
		result = result + aux %2;
		aux = aux / 2;
	}
	return result;
}
int GPIO_escribir(int bit_inicial,int num_bits,int valor){
	uint32_t aux = 0;
	int i;
	for (i = 0; i < num_bits; i++){
		aux = aux * 2;
		if (valor % 2 == 1){
			aux = aux + 1;
		}
		valor = valor / 2;
	}
	aux = aux << bit_inicial;
	IOPIN = IOPIN | aux;
	return aux;
}	

void GPIO_marcar_entrada(int bit_inicial,int num_bits){
	uint32_t aux = 0x00000000;
	uint32_t aux2 = 0xffffffff;
	int i;
	for (i = 0; i < num_bits; i++){
		aux = aux * 2;
		aux = aux + 1;
	}
	aux = aux << bit_inicial;
	aux = aux ^ aux2;
	IODIR = IODIR & aux;
}

void GPIO_marcar_salida(int bit_inicial,int num_bits){
	uint32_t aux = 0;
	int i;
	for (i = 0; i < num_bits; i++){
		aux = aux * 2;
		aux = aux + 1;
	}
	aux = aux << bit_inicial;
	IODIR = IODIR | aux;
}
