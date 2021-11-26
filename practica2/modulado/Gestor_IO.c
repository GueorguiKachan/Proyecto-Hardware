#include "gestor_IO.h"
#include "gestorGPIO.h"
#include <stdint.h>
#include "eventos.h"
#include "cola.h"

static uint8_t fila_actual = 10 ,columna_actual = 10; // Guardan el ultimo valor de fila y columna que se ha visualizado
volatile int validacion_valor_nuevo = 0;
bool nuevo_valor_introducido = false;
static uint32_t eventoPD;

void gestorIO_iniciar(){
	uint32_t eventoVisualizar;
	GPIO_iniciar();
	GPIO_marcar_salida(0,14);
	GPIO_marcar_entrada(16,12);
	GPIO_marcar_salida(30,2);
	eventoVisualizar = visualizar; // Todo lo necesario para la alarma de un evento visualizar
	eventoVisualizar = eventoVisualizar <<24;
	eventoVisualizar = eventoVisualizar | 0x00800000;
	eventoVisualizar = eventoVisualizar + 200;
	
	eventoPD = PDown; // Todo lo necesario para la alarma de un evento visualizar
	eventoPD = eventoPD <<24;
	eventoPD = eventoPD | 0x00800000;
	eventoPD = eventoPD + 15000;
	
	cola_guardar_eventos(alarmaSet,eventoVisualizar);
}

void gestorIO_guardar_fila_columna_actual(uint8_t fila_nueva, uint8_t columna_nueva){
	fila_actual = fila_nueva;
	columna_actual = columna_nueva;
}

bool gestorIO_visualizar(){
	int fila = gestorIO_leer_fila();
	int col = gestorIO_leer_columna();
	if (fila_actual != fila || columna_actual != col || nuevo_valor_introducido){
		gestorIO_guardar_fila_columna_actual(fila,col);
		nuevo_valor_introducido = false;
		cola_guardar_eventos(alarmaSet,eventoPD);
		return true;
	}
	nuevo_valor_introducido = false; // Esto igual no hace falta ponerlo
	return false;
}

int gestorIO_leer_fila(){ 
	return GPIO_leer(16,4);
}
void gestorIO_escribir_fila(int valor){
	GPIO_escribir(16,4,valor);
}

int gestorIO_leer_columna(){
	return GPIO_leer(20,4);
}
void gestorIO_escribir_columna(int valor){
	GPIO_escribir(20,4,valor);
}

int gestorIO_leer_valor_celda(){
	return GPIO_leer(0,4);
}
void gestorIO_escribir_valor_celda(int valor){
	GPIO_escribir(0,4,valor);
}

int gestorIO_leer_valor_nuevo(){
	return GPIO_leer(24,4);
}
void gestorIO_escribir_valor_nuevo(int valor){
	GPIO_escribir(24,4,valor);
}

int gestorIO_leer_candidatos(){
	return GPIO_leer(4,9);
}
void gestorIO_escribir_candidatos(int valor){
	GPIO_escribir(4,9,valor);
}

int gestorIO_leer_error(){
	return GPIO_leer(13,1);
}
void gestorIO_escribir_error(int valor){
	if(validacion_valor_nuevo == 0){
		GPIO_escribir(13,1,valor);
	}
}

void gestorIO_escribir_overflow(int valor){
	GPIO_escribir(30,1,valor);
}

void gestorIO_escribir_idle(int valor){
	GPIO_escribir(31,1,valor);
}

void gestorIO_activar_validacion(){
	uint32_t apagarVal = apagarValidacion; // Todo lo necesario para la alarma de un evento para apagar la validación
	apagarVal = apagarVal << 24;
	apagarVal = apagarVal + 1000;
	gestorIO_escribir_error(1);
	validacion_valor_nuevo = 1;
	cola_guardar_eventos(alarmaSet,apagarVal);
	nuevo_valor_introducido = true;
	cola_guardar_eventos(alarmaSet,eventoPD);
}

void gestorIO_apagar_validacion(){
	gestorIO_escribir_error(0);
	validacion_valor_nuevo = 0;
	cola_guardar_eventos(alarmaSet,eventoPD);
}