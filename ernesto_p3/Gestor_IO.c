#include "gestor_IO.h"
#include "gestorGPIO.h"
#include "eventos.h"
#include "cola.h"
static uint8_t fila_actual = 10 ,columna_actual = 10; // Guardan el ultimo valor de fila y columna que se ha visualizado
volatile int validacion_valor_nuevo = 0;
bool nuevo_valor_introducido = false;
static uint32_t eventoPD;
static bool partida_nueva = true;
static bool mensaje_reset = false;

#define tamanyo_buffer 10
char buffer[tamanyo_buffer];
//static int index_buffer = 0;
static int index_buffer_front;
static int index_buffer_roar;
static bool ha_llegado_hashtag = false;

void guardar_nuevo_caracter(uint32_t nuevo_caracter){
	if (nuevo_caracter == 127 && index_buffer_roar != index_buffer_front) {index_buffer_roar --; if (index_buffer_roar < 0){index_buffer_roar = tamanyo_buffer - 1;}	}
	else{
	if (nuevo_caracter == '#'){ ha_llegado_hashtag = true; index_buffer_front = index_buffer_roar;}
	if (ha_llegado_hashtag){
	buffer[index_buffer_roar] = nuevo_caracter;
	index_buffer_roar = (index_buffer_roar +1)  % 10;
	}
	if (nuevo_caracter == '!'){
		if (buffer[(index_buffer_front + 1) % tamanyo_buffer] == 'R' && buffer[(index_buffer_front + 2) % tamanyo_buffer] == 'S' && buffer[(index_buffer_front + 3) % tamanyo_buffer] == 'T' && buffer[(index_buffer_front + 4) % tamanyo_buffer] == '!'){
			//acabar partida
			//gestorIO_escribir_fila(0);
			//gestorIO_escribir_columna(0);
			//gestorIO_escribir_valor_nuevo(0);
			mensaje_reset = true;
			cola_guardar_eventos(pulsacion1, 0);
		}else if (buffer[(index_buffer_front + 1) % tamanyo_buffer] == 'N' && buffer[(index_buffer_front + 2) % tamanyo_buffer] == 'E' && buffer[(index_buffer_front + 3) % tamanyo_buffer] == 'W' && buffer[(index_buffer_front + 4) % tamanyo_buffer] == '!'){
			if(partida_nueva){
				partida_nueva = false;
				cola_guardar_eventos(pulsacion1,0);
			}
			//nueva partida
		}else if (buffer[(index_buffer_front + 1) % tamanyo_buffer] >= '1' && buffer[(index_buffer_front + 1) % tamanyo_buffer] <= '9' && buffer[(index_buffer_front + 2) % tamanyo_buffer] >= '1' 
							&& buffer[(index_buffer_front + 2) % tamanyo_buffer] <= '9' && buffer[(index_buffer_front + 3) % tamanyo_buffer] >= '0' && buffer[(index_buffer_front + 3) % tamanyo_buffer] <= '9' 
							&& buffer[(index_buffer_front + 4) % tamanyo_buffer] >= '0' && buffer[(index_buffer_front + 4) % tamanyo_buffer] <= '7'){
		int checksum = (buffer[(index_buffer_front + 1) % tamanyo_buffer] -'0') + (buffer[(index_buffer_front + 2) % tamanyo_buffer] -'0') +  (buffer[(index_buffer_front + 3) % tamanyo_buffer] - '0');
			//introducir
		if(checksum % 8 == (buffer[(index_buffer_front + 4) % tamanyo_buffer] -'0')){
			uint32_t auxData  = (buffer[(index_buffer_front + 1) % tamanyo_buffer] - '0') << 16 |
													(buffer[(index_buffer_front + 2) % tamanyo_buffer] - '0') << 8  |
													(buffer[(index_buffer_front + 3) % tamanyo_buffer] - '0');
			
				cola_guardar_eventos(jugada,auxData);
		}
		
		}else{
			//formato incorrecto
		}
	ha_llegado_hashtag =0;
	}
	}
	cola_guardar_eventos(alarmaSet,eventoPD);
}


void gestorIO_iniciar(){
	//uint32_t eventoVisualizar;
	GPIO_iniciar();
	GPIO_marcar_salida(0,14);
	GPIO_marcar_entrada(16,12);
	GPIO_marcar_salida(30,2);
	//eventoVisualizar = visualizar; // Todo lo necesario para la alarma de un evento visualizar
	//eventoVisualizar = eventoVisualizar <<24;
	//eventoVisualizar = eventoVisualizar | 0x00800000;
	//eventoVisualizar = eventoVisualizar + 200;
	
	eventoPD = PDown; // Todo lo necesario para la alarma de un evento visualizar
	eventoPD = eventoPD <<24;
	eventoPD = eventoPD | 0x00800000;
	eventoPD = eventoPD + 15000;
	
	//cola_guardar_eventos(alarmaSet,eventoVisualizar);
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

int gestorIO_leer_idle(){
	return GPIO_leer(31,1);
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

bool final_partida_IO(){
	if(mensaje_reset){
		mensaje_reset = false;
		return true;
	}
	else{
		return false;
	}
}
