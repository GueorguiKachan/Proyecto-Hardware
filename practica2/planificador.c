#include "planificador.h"
#include "cola.h"
#include "eventos.h"
#include "Gestor_Energia.h"
#include "Gestor_Pulsacion.h"
#include "Gestor_Alarmas.h"


void main(){
  // Programar alarma periódica para encolar un evento de tipo tempPeriodico
  temporizador_periodico(59); // 59+1/60MHz creo que son 1us !!!!!!!!!!!!!! Igual habría que hacer una función en gestor Alarmas para programar esto
  for{
    if(hayEventos()){ // Comprueba si en la cola hay eventos nuevos. Si los hay --> switch-case
      
      switch(leerIDMasAntiguo()){
        case alarmaSet: // Programar una alarma con la info extra del evento
          nuevoEvento(leerDatoMasAntiguo());
        case pulsacion1: 
        case pulsacion2:
        case tempPeriodico: // Establecer un temporizador periodico
          disparaEventos(1000); // El temporizador interrumpe cada 1us = 1000ms (en el Gestor los tiempos de alarma se guardan en ms)
        case compruebaSuelta1: //Comprobar si se ha soltado la tecla 
          eint1_comprobar();
        case compruebaSuelta2:
          eint2_comprobar();
      }
      else{
        PM_idle(); // Poner el procesador en modo reposo hasta que llegue la interrupción del temporizador
      }
    }
  }

