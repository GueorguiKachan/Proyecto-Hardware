#include <LPC210X.H>    
#include "gestorGPIO.h"

void GPIO_iniciar(){
	PINSEL0 = 0x00000000;
	PINSEL1 = 0x00000000;
}
int GPIO_leer(int bit_inicial,int num_bits);
int GPIO_escribir(int bit_inicial,int num_bits,int valor);
void GPIO_marcar_entrada(int bit_inicial,int num_bits);
void GPIO_marcar_salida(int bit_inicial,int num_bits){
	
}
