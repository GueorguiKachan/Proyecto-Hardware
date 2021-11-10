   #include <LPC210X.H>                            // LPC21XX Peripheral Registers
#include "timer0.h"
#include "Gestor_Alarmas.h"
#include "cola.h"

#define numHuecos 8;

struct arraysEventos misEventos;

void limpiarEspacio(){
	int i;
	for(i=0; i<8; i++){
		misEventos.estaLibre[i] = 1;
	}
}

void disparaEventos(int tiempoTranscurrido){
  int i;
	int tipoEvento;
	int esPeriodico;
	for(i=0; i<8; i++){
    if(misEventos.estaLibre[i] == 0){ // Si había una alarma programada en este hueco se mira si ha llegado al tiempo programado
      
      if(((misEventos.tiempoEventos[i]-tiempoTranscurrido) <= 0) ){ // Si es menor que 0 generar evento
        
         esPeriodico= (misEventos.eventosProgramados[i] >> 23) & 0x1;
        if(esPeriodico == 1){ // Si es periodico se vuelve a programar la alarma
          misEventos.tiempoEventos[i] = misEventos.eventosProgramados[i] & 0x007FFFF;
        }
        else{ // Si no es periodico se libera espacio
          misEventos.estaLibre[i] = 1;
        }
        tipoEvento = (misEventos.eventosProgramados[i] >> 24);
        cola_guardar_eventos(tipoEvento, 0); 
      }
      else{ // Si no se ha cumplido el tiempo, restar
        misEventos.tiempoEventos[i] -= tiempoTranscurrido;
      
			}
		}
	}
}

void nuevoEvento(int nuevoEvento){
  
  uint32_t tiempoNuevoEvento = nuevoEvento & 0x007FFFF;
  int tipoEvento = (nuevoEvento >> 24);
  int eventoExiste = 0;
	int i;
  for(i=0; i<8; i++){ // Comprueba si el tipo de evento a programar ya existe
    if(tipoEvento == (misEventos.eventosProgramados[i] >> 24)){
      eventoExiste = i;
    }
  }
  if(eventoExiste != 0){ // Si en alguna posición del array existe un evento de este tipo
    if(tiempoNuevoEvento == 0){ // Cancelar el evento 
      misEventos.estaLibre[eventoExiste] = 1; // Cancelar la alarma
    }
    else{ // Reprogramar alarma para el evento
      misEventos.tiempoEventos[eventoExiste] = tiempoNuevoEvento;
    }
  }
  else{ // Hay que registrar nuevo evento
    
    if(misEventos.numEventos < 8){
			int i;
      for(i=0; i<8;i++){ // Mirar donde hay un hueco libre
        if(misEventos.estaLibre[i] == 1){
          misEventos.estaLibre[i] = 0;
          misEventos.eventosProgramados[i] = nuevoEvento;
          misEventos.tiempoEventos[i] = tiempoNuevoEvento;
          break;
        }
      }
    }
    else { //No se puede registrar mas eventos
      
    }
  }
}