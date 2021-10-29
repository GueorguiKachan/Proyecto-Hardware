#include <LPC210x.H>                       /* LPC210x definitions */
// Set the processor into power down state 
// The watchdog cannot wake up the processor from power down
void PM_power_down (void)  {
  EXTWAKE = 6; // Bits 1 y 2 a 1 para que EINT1 y EINT2 despierten al procesador
	PCON |= 0x02; // Modo power-down
}

void PM_idle(void){

	PCON |= 0x01; // Modo idle
}