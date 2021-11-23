#include "Gestor_IO.h"
#include "gestorGPIO.h"

void GestorIO_iniciar(){
		GPIO_iniciar();
		GPIO_marcar_salida(0,14);
	GPIO_marcar_entrada(16,12);
	GPIO_marcar_salida(30,2);
}


int leerFila(){ 
	return GPIO_leer(16,4);
}
void escribirFila(int X){
	GPIO_escribir(16,4,X);
}

int leerColumna(){
	return GPIO_leer(20,4);
}
void escribirColumna(int X){
	GPIO_escribir(20,4,X);
}

int leerValorCelda(){
	return GPIO_leer(0,4);
}
void escribirValorCelda(int X){
	GPIO_escribir(0,4,X);
}

int leerValorNuevo(){
	return GPIO_leer(24,4);
}
void escribirValorNuevo(int X){
	GPIO_escribir(24,4,X);
}

int leerCandidatos(){
	return GPIO_leer(4,9);
}
void escribirCandidatos(int X){
	GPIO_escribir(4,9,X);
}

int leerError(){
	return GPIO_leer(13,1);
}
void escribirError(int X){
	GPIO_escribir(13,1,X);
}

int leerOverflow(){
	return GPIO_leer(30,1);
}
void escribirOverflow(int X){
	GPIO_escribir(30,1,X);
}

int leerIdle(){
	return GPIO_leer(31,1);
}
void escribirIdle(int X){
	GPIO_escribir(31,1,X);
}

