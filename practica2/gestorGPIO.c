#include <LPC210X.H>    
#include "gestorGPIO.h"
#include <stdint.h>
#include <math.h>
#include "Gestor_Pulsacion.h"


void GPIO_iniciar(){
	PINSEL0 = 0x00000000; 
	PINSEL1 = 0x00000000;
	
	Gestor_Pulsacion_Init();
}
int GPIO_leer(int bit_inicial,int num_bits){
	//Desplazo el trozo de IOPIN a leer hasta el bit 0
	uint32_t IOPIN_desplazado_bit_inicial = IOPIN / pow(2,bit_inicial);
	//Preparo la mascara de longitud num_bits
	uint32_t mascara_num_bits = pow(2,num_bits) - 1;
	
	return IOPIN_desplazado_bit_inicial & mascara_num_bits;
}
int GPIO_escribir(int bit_inicial,int num_bits,int valor){
	int izq = bit_inicial+num_bits;
	uint32_t max = 0xffffffff;
	uint32_t min;
	
	uint32_t aux = 0;
	int i;
	//Preparo el valor que entra en num_bits bits (el valor 3 no entra en 1 bit, por lo que en aux preparo el valor 1)
	for (i = 0; i < num_bits; i++){
		if (i != 0 || valor%2 != 0){
		aux = aux + pow(2*(valor%2),i);}
		valor = valor / 2;
	}
	//Pongo en IOPIN la mascara aux desplazada hasta el bit_inicial
	min = pow(2,bit_inicial)-1;
	max = max - (pow(2,bit_inicial+num_bits) -1)+ min; 
	max = max & IOPIN;
	IOPIN = max | (aux  << bit_inicial);
	//Devuelvo el valor de aux
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
	// ^ -> xor
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
