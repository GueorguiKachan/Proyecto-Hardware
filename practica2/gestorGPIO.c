#include <LPC210X.H>    
#include "gestorGPIO.h"
#include <stdint.h>
#include <math.h>

void GPIO_iniciar(){
	PINSEL0 = 0x00000000;
	PINSEL1 = 0x00000000;
}
int GPIO_leer(int bit_inicial,int num_bits){
	int result = 0;
	uint32_t aux = IOPIN;
	aux = aux / pow(2,bit_inicial);
	for (int i = 0; i< num_bits; i++){
		result = result + aux %2;
		aux = aux / 2;
	}
}
int GPIO_escribir(int bit_inicial,int num_bits,int valor){
	uint32_t aux = 0;
	for (int i = 0; i < num_bits; i++){
		aux = aux * 2;
		if (valor % 2 == 1){
			aux = aux + 1;
		}
		valor = valor / 2;
	}
	aux = aux * pow(2,bit_inicial);
	IOPIN = IOPIN | aux;
}	

void GPIO_marcar_entrada(int bit_inicial,int num_bits){
	uint32_t aux = 0;
	uint32_t aux2 = 0xffffffff;
	for (int i = 0; i < num_bits; i++){
		aux = aux * 2;
		aux = aux + 1;
	}
	aux = aux * pow(2,bit_inicial);
	aux = aux ^ aux2;
	IOPIN = IOPIN & aux;
}

void GPIO_marcar_salida(int bit_inicial,int num_bits){
	uint32_t aux = 0;
	for (int i = 0; i < num_bits; i++){
		aux = aux * 2;
		aux = aux + 1;
	}
	aux = aux * pow(2,bit_inicial);
	IOPIN = IOPIN | aux;
}
