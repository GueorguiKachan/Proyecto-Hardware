#include <LPC210x.H>                       /* LPC210x definitions */
//#include "Timer.h"
//#include "Power_management.h"
//#include "boton_eint0.h"
#include <stdio.h>
#include "timers.h"
#include "eventos.h"
#include "cola.h"
#include "Gestor_Alarmas.h"
#include "Gestor_Energia.h"

//#pragma import(__use_no_semihosting_swi)

// Nota: wait es una espera activa. Se puede eliminar poniendo el procesador en modo iddle. Probad a hacerlo
/*void wait (void)  {                         //wait function 
  unsigned int i;

  i = temporizador_leer(); // reads the number of previous timer IRQs
	//temporizador_parar();
  //while ((i + 50) != temporizador_leer());              //waits for 10 interrupts, i.e. 50ms 
	while (500 != temporizador_leer());
}*/

int main (void) {
	int evento;
	struct elemento aux;
  // Programar alarma periódica para encolar un evento de tipo tempPeriodico
  limpiarEspacio();
	temporizador_periodico(599); // 59+1/60MHz creo que son 1us !!!!!!!!!!!!!! Igual habría que hacer una función en gestor Alarmas para programar esto
  evento = 0x011003E8;
	cola_guardar_eventos(alarmaSet,evento);
	while(1){
		
    if(hay_eventos()){ // Comprueba si en la cola hay eventos nuevos. Si los hay --> switch-case
			aux = elementoMasAntiguo();
			eliminar();
			if(aux.ID_evento == alarmaSet){
				evento = aux.auxData;
				 nuevoEvento(evento);
			}
			else if(aux.ID_evento == pulsacion1){}
			else if(aux.ID_evento == pulsacion2){}
			else if(aux.ID_evento == tempPeriodico){
				disparaEventos(1000); // El temporizador interrumpe cada 1us = 1000ms (en el Gestor los tiempos de alarma se guardan en ms)
				
			}
			else{}
      /*switch(evento)
			{
        case alarmaSet: // Programar una alarma con la info extra del evento
          nuevoEvento(leerDatosMasAntiguo());
					break;
        case pulsacion1: 
					break;
        case pulsacion2:
					break;
        case tempPeriodico: // Establecer un temporizador periodico
          disparaEventos(1000); // El temporizador interrumpe cada 1us = 1000ms (en el Gestor los tiempos de alarma se guardan en ms)
					break;
				case compruebaSuelta1: //Comprobar si se ha soltado la tecla 
          break;
					//eint1_comprobar();
        case compruebaSuelta2:
          //eint2_comprobar();
					break;
				default:
					break;
      }*/
		}
      else{
        PM_idle(); // Poner el procesador en modo reposo hasta que llegue la interrupción del temporizador
      }
    }
	}