#include <LPC210X.H>
#include "WT.h"

void WD_init(int sec){

}

void WD_feed(){ 
	// Antes de escribir el AA hay que desactivar las interrupciones
	WDFEED = 0xAA;						   
  WDFEED = 0x55;  
	// Volver a activar las interrupciones
}
