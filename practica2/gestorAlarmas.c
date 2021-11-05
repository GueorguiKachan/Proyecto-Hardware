#include <LPC210X.H>                            // LPC21XX Peripheral Registers
#include "timers.h"
#include "gestorAlarmas.h"

struct arraysEventos{
  int eventosProgramados[8];
  int  tiempoEventos[8];
  int estaLibre[8]
  int numEventosProgramados;
  int siguienteHueco;
}

int[] disparaEventos(){
  for(int i = 0;i<8;i++){
    if(eventoProgramado[i]==1 ){ // Si había una alarma programada en este hueco se mira si devuelve el tiempo
      
      if(tiempoEventos[i]-(aqui iria el tiempo) <= 0){ // Si es menor que 0 generar evento
        // Mirar si es periódico o no, en el caso de que lo sea volver a programar
        int esPeriodico= (EventosProgramados[i] >> 23) & 0x1
        if(esPeriodico == 1){
          tiempoEventos[i] = eventosProgramados[i] & 0x007FFFF;
        }
        else{
           tiempoEventos[i] -= tiempo periodico de interrupcion;
        }
        
      }
      else{ // Si no se ha cumplido el tiempo, restar
        
      }
    }
    else if()
  }
}

void nuevoEvento(uint_t32 nuevoEvento){
  //Comprobar el tiempo de la alarma. Si es 0, mirar si existe un evento de este tipo y cancelarlo(numEventos--)
  int tiempoNuevoEvento = nuevoEvento & 0x007FFFF;
  if(tiempoNuevoEvento == 0){
    int tipoEvento = (nuevoEvento >> 24)
  }
  else{ // Hay que registrar nuevo evento
    if(numEventos < 8){
      for(int i=0; i<8;i++){ // Mirar donde hay un hueco libre
        if(estaLibre[i] == 0){
          estaLibre[i] = 1;
          eventosProgramados[i] = nuevoEvento;
          tiempoEventos[i] nuevoEvento & 0x007FFFFF;
          break;
        }
      }
    }
    else { //No se puede registrar mas eventos
      
    }
  }
}
