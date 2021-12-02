#include <LPC210X.H>                            // LPC21XX Peripheral Registers
#include "timers.h"
#include "eventos.h"
#include "cola.h"


static volatile unsigned int timer0_int_count = 0;
static volatile unsigned int timer1_int_count = 0;

uint32_t __swi(0) clock_gettime(void);
uint32_t __SWI_0 (void) {
   return (((T1TC*T1PR)+T1PC)*16)/1024;
}


void timer0_ISR (void) __irq;    // Generate Interrupt
void timer1_ISR (void) __irq;    // Generate Interrupt

void temporizador_iniciar(void){
	timer1_int_count = 0;
  T1TCR = 0x01; // Bit 0: 1, Bit 1: 0, hace reset del contador y el PCLK. No activa el contador
  T1PR = 0xFFFFFFFE; // Cuando el Prescale Counter Register llegue a ese valor, el Time Counter sumará 1
  T1MR0  = 0xFFFFFFFE; // Saltará una interrupción cuando se llegue al máximo valor posible
  T1MCR = 3;
	
  VICVectAddr0 = (unsigned long)timer1_ISR;          // set interrupt vector in 0
    // 0x20 bit 5 enables vectored IRQs. 4 is the number of the interrupt assigned. Number 4 is the Timer 0 (see table 40 of the LPC2105 user manual  
  VICVectCntl0 = 0x20 | 5; // Página 48 del manual, tabla 40. Número asignado a Timer1                   
  VICIntEnable = VICIntEnable | 0x00000020;                  // Enable Timer1	
		
}
void temporizador_empezar(void){
   T1TCR = 1; // Iniciar el contador 
}

void timer1_ISR (void) __irq {
    timer1_int_count++;
    T1IR = 1;                              // Clear interrupt flag
    VICVectAddr = 0;                            // Acknowledge Interrupt
}

unsigned int temporizador_leer(void){
	return (((T1TC*T1PR)+T1PC)*16)/1024;
}

unsigned int temporizador_parar(void){
	T1TCR = 0;
	return (((T1TC*T1PR)+T1PC)*16)/1024;
}

/* Setup the Timer Counter 0 Interrupt */
void temporizador_periodico (int periodo) {

    timer0_int_count = 0;	
	// configuration of Timer 0
    T0MR0 = (15000*periodo)-1;                        // 1ms = 15.000-1 counts. Periodo=1->1ms. Periodo=2->2ms...
		T0MCR = 3;                     // Generates an interrupt and resets the count when the value of MR0 is reached
    T0TCR = 1;                             // Timer0 Enable
    // configuration of the IRQ slot number 0 of the VIC for Timer 0 Interrupt
		VICVectAddr1 = (unsigned long)timer0_ISR;          // set interrupt vector in 0
    // 0x20 bit 5 enables vectored IRQs. 
		// 4 is the number of the interrupt assigned. Number 4 is the Timer 0 (see table 40 of the LPC2105 user manual  
		VICVectCntl1 = 0x20 | 4;                   
    VICIntEnable = VICIntEnable | 0x00000010;                  // Enable Timer0 Interrupt
		VICIntSelect = VICIntSelect | 0x00000010; 
}


/* Timer Counter 0 Interrupt executes each 10ms @ 60 MHz CPU Clock */
void timer0_ISR (void) __irq {
		cola_guardar_eventos(tempPeriodico,0);
    T0IR = 1;                              // Clear interrupt flag
    VICVectAddr = 0;                            // Acknowledge Interrupt
}
