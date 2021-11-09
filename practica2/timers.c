#include <LPC210X.H>                            // LPC21XX Peripheral Registers
#include "timers.h"
#include "eventos.h"
#include "cola.h"
#include <stdio.h>

static volatile unsigned int timer0_int_count = 0;
static volatile unsigned int timer1_int_count = 0;

void timer0_ISR (void) __irq;    // Generate Interrupt
void timer1_ISR (void) __irq;    // Generate Interrupt

void temporizador_iniciar(void){
	timer1_int_count = 0;
  VPBDIV = 0x1;// --> Esto creo que hace que el PCLK sea el mismo que el CCLK, as� el reloj del contador va m�s r�pido. Secci�n 3.12 del manual LPC
 // T1CTCR = 0x0; // Selecciona modo contador, que cuenta al llegar un flanco ascendente del PCLK
  T1TCR = 0x01; // Bit 0: 1, Bit 1: 0, hace reset del contador y el PCLK. No activa el contador
  T1PR = 0xFFFFFFFE; // Cuando el Prescale Counter Register incremente en 1, el Time Counter tambi�n lo har�
  T1MR0 = 0xFFFFFFFE; // Con esto creo que salta una interrupci�n cuando el TC sea igual a 1, que ocurrir� cada micro 
  T1MCR = 3;
	
  VICVectAddr0 = (unsigned long)timer1_ISR;          // set interrupt vector in 0
    // 0x20 bit 5 enables vectored IRQs. 4 is the number of the interrupt assigned. Number 4 is the Timer 0 (see table 40 of the LPC2105 user manual  
  VICVectCntl0 = 0x20 | 5; // P�gina 48 del manual, tabla 40. N�mero asignado a Timer1                   
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
	return ((T1TC*T1PR)+T1PC)*16;
}

unsigned int temporizador_parar(void){
	T1TCR = 0;
	return timer1_int_count;
}

/* Setup the Timer Counter 0 Interrupt */
void temporizador_periodico (int periodo) {
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
    uint8_t t = tempPeriodico;
		cola_guardar_eventos(t,0);
    T0IR = 1;                              // Clear interrupt flag
    VICVectAddr = 0;                            // Acknowledge Interrupt
}
