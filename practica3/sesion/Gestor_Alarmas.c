#include <LPC210X.H>                            // LPC21XX Peripheral Registers
#include "timers.h"
#include "Gestor_Alarmas.h"
#include "cola.h"

#define numHuecos 8
#define tiempo_ms 1
#define tamanyo_buffer 10
char buffer[tamanyo_buffer];
int index_buffer = 0;

void guardar_nuevo_caracter(uint32_t nuevo_caracter){
	buffer[index_buffer] = nuevo_caracter;
	index_buffer++;
}

struct arraysEventos{
  int eventosProgramados[numHuecos]; // Array con la info de los eventos (tipo de evento, si es periódico y los milisegundos de retardo)
  int tiempoEventos[numHuecos]; // Array para saber cuanto le falta a cada una de las alarmas
  int estaLibre[numHuecos]; // Array para saber cuales de las posiciones para las alarmas está libre u ocupada
  int numEventos;
};

struct arraysEventos misEventos;

void GestorAlarmaInit(){
	int i;
	for(i=0; i<numHuecos; i++){
		misEventos.estaLibre[i] = 1;
	}
	temporizador_periodico(tiempo_ms);
}

void gestor_alarma_restar_tiempo_alarmas_programadas(){
  volatile int i;
	volatile int tipoEvento;
	volatile int esPeriodico;
	for(i=0; i<numHuecos; i++){
    if(misEventos.estaLibre[i] == 0){ // Si había una alarma programada en este hueco, mira si ha llegado al tiempo programado
      
      if(((misEventos.tiempoEventos[i]-tiempo_ms) <= 0) ){ // Si es menor que 0 generar evento
         esPeriodico= (misEventos.eventosProgramados[i] >> 23) & 0x1;
				
        if(esPeriodico == 1){ // Si es periodico se vuelve a programar la alarma
          misEventos.tiempoEventos[i] = misEventos.eventosProgramados[i] & 0x007FFFF;
        }
        else{ // Si no es periodico se libera espacio
          misEventos.estaLibre[i] = 1;
        }
				
        tipoEvento = (misEventos.eventosProgramados[i] >> 24);
        cola_guardar_eventos(tipoEvento, 0);// Encolar evento 
      }
      else{ // Si no se ha cumplido el tiempo, restar. 
        misEventos.tiempoEventos[i] -= tiempo_ms;  
			}
		}
	}
}

void gestor_alarma_nueva_alarma(uint32_t nuevaAlarma){
  
  uint32_t tiempoNuevoEvento = nuevaAlarma & 0x007FFFF;
  int tipoEvento = (nuevaAlarma >> 24);
  int eventoExiste = -1;
	int i;
  for(i=0; i<numHuecos; i++){ // Comprueba si el tipo de evento a programar ya existe
    if(tipoEvento == (misEventos.eventosProgramados[i] >> 24) && misEventos.estaLibre[i] == 0){
      eventoExiste = i;
    }
  }
  if(eventoExiste > -1){ // Si en alguna posición del array existe un evento de este tipo
    if(tiempoNuevoEvento == 0){ // Cancelar el evento 
      misEventos.estaLibre[eventoExiste] = 1;
    }
    else{ // Reprogramar alarma para el evento
      misEventos.tiempoEventos[eventoExiste] = tiempoNuevoEvento;
    }
  }
  else{ // Hay que registrar nuevo evento
    if(misEventos.numEventos < 8){
      for(i=0; i<8;i++){ // Mirar donde hay un hueco libre
        if(misEventos.estaLibre[i] == 1){
          misEventos.estaLibre[i] = 0;
          misEventos.eventosProgramados[i] = nuevaAlarma;
          misEventos.tiempoEventos[i] = tiempoNuevoEvento;
          break;
        }
      }
    }
    else {} //No se puede registrar mas eventos. Generar error?
  }
}