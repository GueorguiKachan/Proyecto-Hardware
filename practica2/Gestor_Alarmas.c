#include <LPC210X.H>                            // LPC21XX Peripheral Registers
#include "timers.h"
#include "Gestor_Alarmas.h"
#include "cola.h"

struct arraysEventos{
  int eventosProgramados[8]; // Array con la info de los eventos (tipo de evento, si es periódico y los milisegundos de retardo)
  int  tiempoEventos[8]; // Array para saber cuanto le falta a cada una de las alarmas
  int estaLibre[8] // Array para saber cuales de las posiciones para las alarmas está libre u ocupada
  int numEventosProgramados;
}

void disparaEventos(int tiempoTranscurrido){
  for(int i = 0;i<8;i++){
    if(estaLibre[i]==0 ){ // Si había una alarma programada en este hueco se mira si ha llegado al tiempo programado
      
      if(((tiempoEventos[i]-tiempoTranscurrido) <= 0) ){ // Si es menor que 0 generar evento
        
        int esPeriodico= (EventosProgramados[i] >> 23) & 0x1
        if(esPeriodico == 1){ // Si es periodico se vuelve a programar la alarma
          tiempoEventos[i] = eventosProgramados[i] & 0x007FFFF;
        }
        else{ // Si no es periodico se libera espacio
          estaLibre[i] = 1;
        }
        int tipoEvento = (EventosProgramados[i] >> 24);
        cola_guardar_eventos(tipoEvento, 0); 
      }
      else{ // Si no se ha cumplido el tiempo, restar
        tiempoEventos[i] -= tiempoTranscurrido;
      
    }
  }
}

void nuevoEvento(uint_t32 nuevoEvento){
  
  int tiempoNuevoEvento = nuevoEvento & 0x007FFFF;
  int tipoEvento = (nuevoEvento >> 24);
  int eventoExiste = 0;
  for(int i=0; i<8; i++){ // Comprueba si el tipo de evento a programar ya existe
    if(tipoEvento == (eventosProgramados[i] >> 24)){
      eventoExiste = i;
    }
  }
  if(eventoExiste != 0){ // Si en alguna posición del array existe un evento de este tipo
    if(tiempoNuevoEvento == 0){ // Cancelar el evento 
      estaLibre[i] = 1; // Cancelar la alarma
    }
    else{ // Reprogramar alarma para el evento
      tiempoEventos[eventoExiste] = tiempoNuevoEvento;
    }
  }
  else{ // Hay que registrar nuevo evento
    
    if(numEventos < 8){
      for(int i=0; i<8;i++){ // Mirar donde hay un hueco libre
        if(estaLibre[i] == 1){
          estaLibre[i] = 0;
          eventosProgramados[i] = nuevoEvento;
          tiempoEventos[i] = tiempoNuevoEvento;
          break;
        }
      }
    }
    else { //No se puede registrar mas eventos
      
    }
  }
}
