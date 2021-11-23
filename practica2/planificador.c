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
	int filaAntes = 20;
	int colAntes = 20;
	int eventoPD;
	int eventoVisualizar;
	int apagarVal;
	int evento;
	
	int tiempo;
	bool finPartida = false;
	struct elemento aux;
	int i, j;
  // Programar alarma periódica para encolar un evento de tipo tempPeriodico
	for (i = 0; i<filaColumnaSudoku; i++){
		for (j = 0; j<filaColumnaSudoku; j++){
	cuadricula_C_C[i][j] = cuadricula_ARM_C[i][j];
	}}
  limpiarEspacio();
	GestorIO_iniciar();
	
	temporizador_periodico(1); // 14999 = 1ms
  eventoPD = PDown;
	eventoPD = eventoPD <<24;
	eventoPD = eventoPD | 0x00800000;
	eventoPD = eventoPD + 15000;
	cola_guardar_eventos(alarmaSet,eventoPD);
	
	apagarVal = apagarValidacion;
	apagarVal = apagarVal <<24;
	apagarVal = apagarVal + 1000;
	
	eventoVisualizar = visualizar;
	eventoVisualizar = eventoVisualizar <<24;
	eventoVisualizar = eventoVisualizar | 0x00800000;
	eventoVisualizar = eventoVisualizar + 200;
	cola_guardar_eventos(alarmaSet,eventoVisualizar);
	candidatos_actualizar_c(cuadricula_C_C);
	while(!finPartida){
		
    if(hay_eventos()){ // Comprueba si en la cola hay eventos nuevos. Si los hay --> switch-case
			aux = elementoMasAntiguo();
			eliminar();
			if(aux.ID_evento == alarmaSet){
				evento = aux.auxData;
				 nuevoEvento(evento);
			}
			else if(aux.ID_evento == pulsacion1){
				if (eint1_comprobar()){ //si se ha dejado de pulsar
					if (!powerDownActive){
						int filaAct =  leerFila() ;
						int colAct = leerColumna();
						int valorNuevo = leerValorNuevo();
						int esPista;//
						bool esCandidato = false;
						int candidatos;//
						int i;
						int uno;
						if(filaAct==0 && colAct==0 && valorNuevo==0){
							finPartida = true;
							break;
						}
						if (colAct >0 && colAct <= 9 && filaAct > 0 && filaAct <= 9){
							esPista = (cuadricula_C_C[filaAct-1][colAct-1]>>4) &0x1;
							 candidatos = cuadricula_C_C[filaAct-1][colAct-1]>>7;
						if (valorNuevo <= 9 && valorNuevo >= 1){
							candidatos = candidatos >> (valorNuevo -1);
							esCandidato = 0 == (candidatos &0x1);
						}
					}
						
						//se comprueba que se pueda escribir en la celda
						//q no sea pista, q el valor este entre los candidatos, y que la columna, fila y valor introducidos sean validos
						
						if(filaAct > 0 && filaAct <= filaColumnaSudoku && colAct > 0 && colAct <= filaColumnaSudoku && 0 < valorNuevo && valorNuevo <= filaColumnaSudoku && esCandidato && esPista == 0){
						cuadricula_C_C[filaAct-1][colAct-1] =  (cuadricula_C_C[filaAct-1][colAct-1] & 0xfff0) + valorNuevo;
						temporizador_iniciar();
						temporizador_empezar();
						candidatos_actualizar_c(cuadricula_C_C);
						tiempo = temporizador_parar();
						valorCambiado=true;
						escribirError(1);
						idleCambiado = true;
						cola_guardar_eventos(alarmaSet,apagarVal);}
					}else{ powerDownActive = false; escribirIdle(0);}
					cola_guardar_eventos(alarmaSet,eventoPD);
				}
			}else if(aux.ID_evento == pulsacion2){
				if (eint2_comprobar()){ //si se ha dejado de pulsar
					if (!powerDownActive){ // Se quiere borrar, comprobar que no sea una pista
						
						int filaAct =  leerFila() ;
						int colAct = leerColumna();
						int esPista;
						if (colAct >0 && colAct <= 9 && filaAct > 0 && filaAct <= 9){
							esPista = (cuadricula_C_C[filaAct-1][colAct-1] >> 4) & 0x1;
						}
						
						if(filaAct > 0 && filaAct <= filaColumnaSudoku && colAct > 0 && colAct <= filaColumnaSudoku && esPista == 0){
							cuadricula_C_C[filaAct-1][colAct-1] =  (cuadricula_C_C[filaAct-1][colAct-1] & 0xfff0);//+ valorNuevo;
							temporizador_iniciar();
							temporizador_empezar();
							candidatos_actualizar_c(cuadricula_C_C);
							tiempo = temporizador_parar();
							valorCambiado=true;
							escribirError(1);
							idleCambiado = true;
							cola_guardar_eventos(alarmaSet,apagarVal);
						}
						
					}else {
						powerDownActive = false; 
						escribirIdle(0);
					}
				cola_guardar_eventos(alarmaSet,eventoPD);}
			}else if(aux.ID_evento == tempPeriodico){
				disparaEventos(1); // El temporizador interrumpe cada 1ms. Creo que en vez de 1000 hay que poner 1
				
			}
			else if(aux.ID_evento == PDown){
				powerDownActive = 1;
				escribirIdle(1);
				PM_power_down();
			}
			else if(aux.ID_evento == visualizar){
				int fila = leerFila();//visualizar todo
				int col = leerColumna();
				if (filaAntes != fila || colAntes != col || valorCambiado || idleRecienApagado){
					int32_t celda;
					filaAntes = fila;
					colAntes = col;
					cola_guardar_eventos(alarmaSet,eventoPD);
					if (fila >0 && fila <=9 && col >0 && col <= 9){
					celda = cuadricula_C_C[fila-1][col-1];
					escribirValorCelda(celda);
					if (!idleCambiado){
						escribirError(celda>>4);
					}
					escribirCandidatos(celda>>7);
				}
					valorCambiado= false;
					idleRecienApagado = false;
				}
				
			}
			else if(aux.ID_evento == apagarValidacion){
				idleCambiado= false;
				idleRecienApagado = true;
				escribirError(0);
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
				escribirIdle(1);
        PM_idle(); // Poner el procesador en modo reposo hasta que llegue la interrupción del temporizador
				escribirIdle(0);
      }
    }
		PM_power_down();
		powerDownActive = true;
	}

}

