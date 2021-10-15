#include <LPC210X.H>                            // LPC21XX Peripheral Registers
#include "gestorAlarmas.h"

void temporizador_iniciar(){
  T0CTCR = 0x0; // Selecciona modo contador, que cuenta al llegar un flanco ascendente del PCLK
  T0TCR = 0x02 // Bit 1: 1, Bit 0: 0, hace reset del contador y el PCLK. No activa el contador
  T0PR: = 0x0 // Cuando el Prescale Counter Register incremente en 1, el Time Counter también lo hará
}
void temporizador_empezar(){
   T0TCR = 1; // Iniciar el contador 
}
