#include "planificador.h"
#include "cola.h"
#include "eventos.h"


void main(){
  // Programar alarma periódica para encolar un evento de tipo tempPeriodico
  
  for{
    if(hayEventos()){ // Comprueba si en la cola hay eventos nuevos. Si los hay --> switch-case
      
      switch(leerIDMasAntiguo()){
        case alarmaSet: // Programar una alarma con la info extra del evento
          nuevoEvento(leerDatoMasAntiguo());
        case pulsacion1: 
        case pulsacion2:
        case tempPeriodico: // Establecer un temporizador periodico
          disparaEventos(tiempoAlarma); //!!!!!!!!!!! Habría que poner el tiempo que se pone para la alarma periodica
        case compruebaSuelta1: //Comprobar si se ha soltado la tecla 
          eint1_comprobar();
        case compruebaSuelta2:
          eint2_comprobar();
      }
    }
  }

