#include <LPC210X.H>
#include "WT.h"
#include "timers.h"

void WD_init(int sec){

}

void WD_feed(){ 
	// Antes de escribir el AA hay que desactivar las interrupciones
	disable_isr();
	disable_isr_fiq();
	WDFEED = 0xAA;						   
  WDFEED = 0x55;
  enable_isr_fiq();	
	enable_isr ();
	// Volver a activar las interrupciones
}
