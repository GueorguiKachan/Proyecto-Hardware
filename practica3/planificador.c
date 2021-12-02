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

int main (void) {
bool power_down_active = false;
while(1){
	bool idle_recien_apagado = false;
	int tiempoAntes;
	int tiempo; // Guardar el valor de antes en esta variable y luego hacer tiempo = leer_tiempo - tiempo
	uint8_t evento_id;
	bool pulsacion1_recien_pulsado = true;
	bool pulsacion2_recien_pulsado = true;
	
	temporizador_iniciar();
	temporizador_empezar();
	sudoku_init();
  GestorAlarmaInit();
	gestorIO_iniciar();
	gestor_pulsacion_inicializar_botones();
	gestor_energia_init();
	
	while(1){
    if(cola_hay_eventos()){ // Comprueba si en la cola hay eventos nuevos. Si los hay averigua cual es
			evento_id = cola_leer_evento_mas_antiguo();
			
			if(evento_id == alarmaSet){														// Programar alarma nueva
				gestor_alarma_nueva_alarma(cola_leer_datos_aux_mas_antiguo());
			}
			else if(evento_id == pulsacion1){											// Se pulsa el botón eint1
				if(pulsacion1_recien_pulsado){
					if(!power_down_active){
						tiempoAntes = clock_gettime();
						//falta comprobar si todo es 0
						sudoku_anyadir_valor_nuevo(gestorIO_leer_fila(),gestorIO_leer_columna(),gestorIO_leer_valor_nuevo());
						tiempo = temporizador_leer() - tiempoAntes;
						gestorIO_activar_validacion(); // Tras 1 segundo, apagar bit de validación.!!!!!!!!!!!!!!!!!! Visualizar el cambio aqui directamente?
						// Habría que hacer que si la celda es pista se encienda el bit de error mientras se mantenga pulsado el boton
					}
					else{ 
						power_down_active = false;
						gestorIO_escribir_idle(0);// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!Esto no sobra?
					}
					pulsacion1_recien_pulsado = false;
				}
				if (!gestor_pulsacion_eint1_comprobar_sigue_pulsado()){
					pulsacion1_recien_pulsado = true;
				}
			}
			else if(evento_id == pulsacion2){										// Se pulsa el botón eint2
					if (pulsacion2_recien_pulsado){ //Si se ha dejado de pulsar
						if(!power_down_active){ //Comprobar si se ha pulsado el botón para salir de modo Power Down
							tiempoAntes = temporizador_leer();
							sudoku_borrar_valor(gestorIO_leer_fila(),gestorIO_leer_columna());
							tiempo = temporizador_leer() - tiempoAntes;
							gestorIO_activar_validacion();
						}
						else{ 
							power_down_active = false;
							gestorIO_escribir_idle(0);// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!Esto no sobra?
						}
					pulsacion2_recien_pulsado = false;
				}
				if (!gestor_pulsacion_eint2_comprobar_sigue_pulsado()){
						pulsacion2_recien_pulsado = true;
				}
			}
			else if(evento_id == tempPeriodico){                 // Temporizador periódico. Comprobar si alguna alarma salta
				gestor_alarma_restar_tiempo_alarmas_programadas();
			}
			else if(evento_id == PDown){													// Poner el sistema en modo PowerDown
				power_down_active = true;
				gestorIO_escribir_idle(1); // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!Esto no sobra?
				PM_power_down();
			}
			else if(evento_id == visualizar){											// Actualizar el valor de la celda y sus candidatos en la GPIO
					if (gestorIO_visualizar() || idle_recien_apagado){ 
						if(sudoku_fila_columna_validos(gestorIO_leer_fila(),gestorIO_leer_columna())){
							//************** Unir estas 3 en una? Pasarle el valor de la celda y ya que internamente llame a estas 3*******************
							gestorIO_escribir_valor_celda(sudoku_get_valor_celda(gestorIO_leer_fila(), gestorIO_leer_columna())); 
							gestorIO_escribir_error(sudoku_get_valor_error(gestorIO_leer_fila(), gestorIO_leer_columna()));
							gestorIO_escribir_candidatos(sudoku_get_valor_candidatos(gestorIO_leer_fila(), gestorIO_leer_columna()));
						}		
					}
					idle_recien_apagado = false;
			}
			else if(evento_id == apagarValidacion){								// Tras un segundo apagar el bit de validacion
				idle_recien_apagado = true;
				gestorIO_apagar_validacion();
			}
			else{} // El valor del evento no coincide con ninguno de los registrados
			cola_eliminar_evento_mas_antiguo();
		}
      else{
				gestorIO_escribir_idle(1);
        PM_idle(); // Poner el procesador en modo reposo hasta que llegue la interrupción del temporizador
				gestorIO_escribir_idle(0);
			}
    }
		temporizador_parar();
		PM_power_down(); // Modo Power Down cuando acaba una partida, hasta que se pulse un botón
		power_down_active = true;
	}
}