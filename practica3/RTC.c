#include <LPC210X.H>
#include "RTC.h"

void RTC_init(){
	// prepara el RTC para que cuente los segundo (calculado para 60 MHz)
	CCR=0x10; // Para hacer el reset primero?????????????
	PREINT = 0x726; // Resultado de hacer (60000000(60 MHz) / 32768) - 1 = 1830 = 0x726
	PREFRAC = 0x700; // Resultado de hacer 60000000 (60 MHz) - (1831 * 32768) = 1792 = 0x700
	// enable of the RTC
	CCR=0x01;
}

int RTC_leer_minutos(){
	
	return (CTIME0  >> 8)& 0x3F;;
}

int RTC_leer_segundos(){
	return CTIME0 & 0x3F;;
}