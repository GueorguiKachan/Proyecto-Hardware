#include <LPC210X.H>                            // LPC21XX Peripheral Registers
#include "timers.h"

static volatile unsigned int timer0_int_count = 0;
static volatile unsigned int timer1_int_count = 0;

void timer0_ISR (void) __irq;    // Generate Interrupt
void timer1_ISR (void) __irq;    // Generate Interrupt

void temporizador_iniciar(void){
	timer1_int_count = 0;
  VPBDIV = 0x1;// --> Esto creo que hace que el PCLK sea el mismo que el CCLK, así el reloj del contador va más rápido. Sección 3.12 del manual LPC
 // T1CTCR = 0x0; // Selecciona modo contador, que cuenta al llegar un flanco ascendente del PCLK
  T1TCR = 0x01; // Bit 0: 1, Bit 1: 0, hace reset del contador y el PCLK. No activa el contador
  T1PR = 0x0; // Cuando el Prescale Counter Register incremente en 1, el Time Counter también lo hará
  T1MR0 = 149999; // Con esto creo que salta una interrupción cuando el TC sea igual a 1, que ocurrirá cada micro 
  T1MCR = 3;
	
  VICVectAddr0 = (unsigned long)timer1_ISR;          // set interrupt vector in 0
    // 0x20 bit 5 enables vectored IRQs. 4 is the number of the interrupt assigned. Number 4 is the Timer 0 (see table 40 of the LPC2105 user manual  
  VICVectCntl0 = 0x20 | 5; // Página 48 del manual, tabla 40. Número asignado a Timer1                   
  VICIntEnable = VICIntEnable | 0x00000020;                  // Enable Timer1	
		
}
void temporizador_empezar(){
   T1TCR = 1; // Iniciar el contador 
}

void timer1_ISR (void) __irq {
    timer1_int_count++;
    T1IR = 1;                              // Clear interrupt flag
    VICVectAddr = 0;                            // Acknowledge Interrupt
}

unsigned int temporizador_leer(){
	return timer1_int_count;
}

unsigned int temporizador_parar(void){
	T1TCR = 0;
	return timer1_int_count;
}

/* Setup the Timer Counter 0 Interrupt */
void temporizador_periodico ( int periodo) {
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
