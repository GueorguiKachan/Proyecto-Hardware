#include <LPC210X.H>    
#include "gestorGPIO.h"

void GPIO_iniciar(){
	//Si no se debe configurar botones, se iniciaria asi:
	PINSEL0 = 0x00000000;
	PINSEL1 = 0x00000000;
}
int GPIO_leer(int bit_inicial,int num_bits){
	//return IOPIN[bit_inicial+num_bits-1:bit_inicial];
}

int GPIO_escribir(int bit_inicial,int num_bits,int valor){
	//supongo que no se puede escribir directamente en IOPIN, pero si se pudiera seria:
	//IOPIN[bit_inicial+num_bits-1:bit_inicial] = valor;
	//return valor realmente escrito, ya que puede haberse puesto un nº bits en el que no cabe el valor
}
void GPIO_marcar_entrada(int bit_inicial,int num_bits);
	//Entrada -> 0
	//IODIR[bit_inicial+num_bits-1:bit_inicial] = 0;
void GPIO_marcar_salida(int bit_inicial,int num_bits){
	//Salida -> 1
	//IODIR[bit_inicial+num_bits-1:bit_inicial] = 1;
}
