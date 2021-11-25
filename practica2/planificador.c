#include <LPC210x.H>                       /* LPC210x definitions */
#include <stdio.h>
#include "timers.h"
#include "eventos.h"
#include "cola.h"
#include "Gestor_Alarmas.h"
#include "Gestor_Energia.h"
#include "Gestor_Pulsacion.h"
#include "Gestor_IO.h"
#include "sudoku_2021.h"
#include "tableros.h"

#define filaColumnaSudoku 9

int main (void) {
bool powerDownActive = false;
while(1){
	bool idleCambiado = false;
	bool idleRecienApagado = false;
	bool valorCambiado = false;
	bool overflow = false;
	int filaAntes = 20;
	int colAntes = 20;
	int eventoPD;
	int eventoVisualizar;
	int apagarVal;
	int evento;
	int tiempoAntes;
	
	int tiempo;
	bool finPartida = false;
	struct elemento aux;
	int i, j;
	
	temporizador_iniciar();
	temporizador_empezar();
  // Programar alarma periódica para encolar un evento de tipo tempPeriodico
	for (i = 0; i<filaColumnaSudoku; i++){
		for (j = 0; j<filaColumnaSudoku; j++){
	cuadricula_C_C[i][j] = cuadricula_ARM_C[i][j];
	}}
  limpiarEspacio();
	GestorIO_iniciar();
	
	temporizador_periodico(1); 
	
  eventoPD = PDown; // Todo lo necesario para la alarma de un evento PowerDown
	eventoPD = eventoPD <<24;
	eventoPD = eventoPD | 0x00800000;
	eventoPD = eventoPD + 15000;
	cola_guardar_eventos(alarmaSet,eventoPD);
	
	apagarVal = apagarValidacion; // Todo lo necesario para la alarma de un evento para apagar la validación
	apagarVal = apagarVal <<24;
	apagarVal = apagarVal + 1000;
	
	eventoVisualizar = visualizar; // Todo lo necesario para la alarma de un evento visualizar
	eventoVisualizar = eventoVisualizar <<24;
	eventoVisualizar = eventoVisualizar | 0x00800000;
	eventoVisualizar = eventoVisualizar + 200;
	
	cola_guardar_eventos(alarmaSet,eventoVisualizar);
	candidatos_actualizar_c(cuadricula_C_C);
	
	while(!finPartida){
		
    if(hay_eventos()){ // Comprueba si en la cola hay eventos nuevos. Si los hay averigua cual es
			aux = elementoMasAntiguo();
			eliminar();
			
			if(aux.ID_evento == alarmaSet){														// Programar alarma nueva
				evento = aux.auxData;
				nuevoEvento(evento);
			}
			else if(aux.ID_evento == pulsacion1){											// Se pulsa el botón eint1
				if (eint1_comprobar()){  //Si se ha dejado de pulsar
					if (!powerDownActive){ // Comprobar si se ha pulsado el botón para salir de modo Power Down
						int filaAct =  leerFila() ;
						int colAct = leerColumna();
						int valorNuevo = leerValorNuevo();
						int esPista;
						bool esCandidato = false;
						int candidatos;
						
						if(filaAct==0 && colAct==0 && valorNuevo==0){// Se reconoce el final de la partida
							finPartida = true;
							break;
						}
						if (colAct >0 && colAct <= 9 && filaAct > 0 && filaAct <= 9){ //Comprobar que el valor a introducir es válido
							esPista = (cuadricula_C_C[filaAct-1][colAct-1]>>4) &0x1;
							candidatos = cuadricula_C_C[filaAct-1][colAct-1]>>7;
							if (valorNuevo <= 9 && valorNuevo >= 1){
								candidatos = candidatos >> (valorNuevo -1);
								esCandidato = 0 == (candidatos &0x1);
							}
					}
						
						//Se comprueba que no sea pista, el valor sea un candidato, y que la columna, fila y valor introducidos sean válidos
						if(filaAct > 0 && filaAct <= filaColumnaSudoku && colAct > 0 && colAct <= filaColumnaSudoku && 0 < valorNuevo && valorNuevo <= filaColumnaSudoku && esCandidato && esPista == 0){
							cuadricula_C_C[filaAct-1][colAct-1] =  (cuadricula_C_C[filaAct-1][colAct-1] & 0xfff0) + valorNuevo;
							tiempoAntes = temporizador_leer();
							candidatos_actualizar_c(cuadricula_C_C);
							tiempo = temporizador_leer() - tiempoAntes;
							
							valorCambiado=true;
							escribirError(1);
							idleCambiado = true;
							overflow = cola_guardar_eventos(alarmaSet,apagarVal); // Tras 1 segundo, apagar bit de validación
							if(overflow){break;}
						}
					}else{ powerDownActive = false; escribirIdle(0);} // Aquí habría que encender el botón de error mejor no?
					overflow = cola_guardar_eventos(alarmaSet,eventoPD);
					if(overflow){break;}
				}
			}
			else if(aux.ID_evento == pulsacion2){										// Se pulsa el botón eint2
				if (eint2_comprobar()){ //Si se ha dejado de pulsar
					if(!powerDownActive){ //Comprobar si se ha pulsado el botón para salir de modo Power Down
						int filaAct =  leerFila() ;
						int colAct = leerColumna();
						int esPista;
						
						if(colAct >0 && colAct <= 9 && filaAct > 0 && filaAct <= 9){ // Se quiere borrar, comprobar que no sea una pista
							esPista = (cuadricula_C_C[filaAct-1][colAct-1] >> 4) & 0x1;
						}
						
						//Se comprueba que no sea pista, el valor sea un candidato, y que la columna, fila y valor introducidos sean válidos
						if(filaAct > 0 && filaAct <= filaColumnaSudoku && colAct > 0 && colAct <= filaColumnaSudoku && esPista == 0){
							cuadricula_C_C[filaAct-1][colAct-1] =  (cuadricula_C_C[filaAct-1][colAct-1] & 0xfff0);
							
							tiempoAntes = temporizador_leer();
							candidatos_actualizar_c(cuadricula_C_C);
							tiempo = temporizador_leer() - tiempoAntes;
							
							valorCambiado=true;
							escribirError(1);
							idleCambiado = true;
							overflow = cola_guardar_eventos(alarmaSet,apagarVal);// Tras 1 segundo, apagar bit de validación
							if(overflow){break;}
						}			
					}else {
						powerDownActive = false; 
						escribirIdle(0);
					}
				overflow = cola_guardar_eventos(alarmaSet,eventoPD);
				if(overflow){break;}}
			}
			else if(aux.ID_evento == tempPeriodico){                 // Temporizador periódico. Comprobar si alguna alarma salta
				disparaEventos(1); // El temporizador interrumpe cada 1ms. Creo que en vez de 1000 hay que poner 1
			}
			else if(aux.ID_evento == PDown){													// Poner el sistema en modo PowerDown
				powerDownActive = 1;
				escribirIdle(1);
				PM_power_down();
			}
			else if(aux.ID_evento == visualizar){											// Actualizar el valor de la celda y sus candidatos en la GPIO
				int fila = leerFila();
				int col = leerColumna();
				if (filaAntes != fila || colAntes != col || valorCambiado || idleRecienApagado){ //Si columna/Fila distinta o un nuevo valor
					int32_t celda;
					filaAntes = fila;
					colAntes = col;
					overflow = cola_guardar_eventos(alarmaSet,eventoPD);
					if(overflow){break;}
					
					if (fila >0 && fila <=9 && col >0 && col <= 9){ //Escribir solo si el valor de fila/columna es válido
						celda = cuadricula_C_C[fila-1][col-1];
						escribirValorCelda(celda);
						if (!idleCambiado){ //Durante el segundo de validación, esta bit tiene que estar siempre encendido
							escribirError(celda>>4);
						}
						escribirCandidatos(celda>>7);
					}
					
					valorCambiado= false;
					idleRecienApagado = false;
				}
			}
			else if(aux.ID_evento == apagarValidacion){								// Tras un segundo apagar el bit de validacion
				idleCambiado= false;
				idleRecienApagado = true;
				escribirError(0);
			}
			else{} // El valor del evento no coincide con ninguno de los registrados
		}
      else{
				escribirIdle(1);
        PM_idle(); // Poner el procesador en modo reposo hasta que llegue la interrupción del temporizador
				escribirIdle(0);
			}
    }
		if(overflow){break;} // Se sale del programa y se queda en un bucle
		temporizador_parar();
		PM_power_down(); // Modo Powe Down cuando acaba una partida, hasta que se pulse un botón
		powerDownActive = true;
	}
}
