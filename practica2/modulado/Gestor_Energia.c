#include <LPC210x.H>                       /* LPC210x definitions */
#include "Gestor_Energia.h"
#include "eventos.h"
#include "cola.h"
#include <stdint.h>
// Set the processor into power down state 
// The watchdog cannot wake up the processor from power down

int EINT1_2 = 6;
int Power_Down_Mode = 2;
int Idle_Mode = 1;
static uint32_t eventoPD;

void gestor_energia_init(){
	eventoPD = PDown; // Todo lo necesario para la alarma de un evento PowerDown
	eventoPD = eventoPD <<24;
	eventoPD = eventoPD | 0x00800000;
	eventoPD = eventoPD + 15000;
	cola_guardar_eventos(alarmaSet,eventoPD);
}

void PM_power_down (void)  {
  EXTWAKE = EINT1_2; // Bits 1 y 2 a 1 para que EINT1 y EINT2 despierten al procesador
  PCON |= Power_Down_Mode; // Modo power-down
  Switch_to_PLL(); // Función que habrá que meter en el startup para volver a poner en modo PLL
	cola_guardar_eventos(alarmaSet,eventoPD);
}

void PM_idle(void){

	PCON |= Idle_Mode; // Modo idle
}