#ifndef TIMERS_H
#define TIMERS_H

#include <inttypes.h>

uint32_t __swi(0) clock_gettime(void);
void __swi(0xFF) enable_isr (void);
void __swi(0xFE) disable_isr (void);
void __swi(0xFD) enable_isr_fiq (void);
void __swi(0xFC) disable_isr_fiq (void);
extern void temporizador_iniciar(void);
extern void temporizador_empezar(void);
extern unsigned int temporizador_leer(void);
extern unsigned int temporizador_parar();
void temporizador_periodico (int periodo);

#endif