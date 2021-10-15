#include <LPC210X.H>                            // LPC21XX Peripheral Registers
#include "gestorAlarmas.h"

void temporizador_iniciar(){
  T1CTCR = 0x0; // Selecciona modo contador, que cuenta al llegar un flanco ascendente del PCLK
  T1TCR = 0x02 // Bit 1: 1, Bit 0: 0, hace reset del contador y el PCLK. No activa el contador
  T1PR: = 0x0 // Cuando el Prescale Counter Register incremente en 1, el Time Counter también lo hará
}
void temporizador_empezar(){
   T1TCR = 1; // Iniciar el contador 
}

void timer0_ISR (void) __irq;    // Generate Interrupt 


/* Setup the Timer Counter 0 Interrupt */
void temporizador_periodico ( uintt32_t periodo) {
		timer0_int_count = 0;	
	// configuration of Timer 0
		T0MR0 = periodo;                        // Interrumpe cada 0,05ms = 150.000-1 counts
    T0MCR = 3;                     // Generates an interrupt and resets the count when the value of MR0 is reached
    T0TCR = 1;                             // Timer0 Enable
    // configuration of the IRQ slot number 0 of the VIC for Timer 0 Interrupt
		VICVectAddr0 = (unsigned long)timer0_ISR;          // set interrupt vector in 0
    // 0x20 bit 5 enables vectored IRQs. 
		// 4 is the number of the interrupt assigned. Number 4 is the Timer 0 (see table 40 of the LPC2105 user manual  
		VICVectCntl0 = 0x20 | 4;                   
    VICIntEnable = VICIntEnable | 0x00000010;                  // Enable Timer0 Interrupt
}


/* Timer Counter 0 Interrupt executes each 10ms @ 60 MHz CPU Clock */
void timer0_ISR (void) __irq {
    cola_guardar_eventos(numEvento, auxData);
    T0IR = 1;                              // Clear interrupt flag
    VICVectAddr = 0;                            // Acknowledge Interrupt
}
