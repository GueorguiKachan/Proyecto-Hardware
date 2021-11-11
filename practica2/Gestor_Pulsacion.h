#include <stdbool.h>
#define EXTMODE          (*((volatile unsigned long *) 0xE01FC148))
#define EXTPOLAR          (*((volatile unsigned long *) 0xE01FC14C))
#define VICIntEnClear          (*((volatile unsigned long *) 0xFFFFF014))
static int button_nueva_pulsacion_1();

static int button_nueva_pulsacion_2();

static void button_clear_nueva_pulsacion_1();

static void button_clear_nueva_pulsacion_2();

static bool boton1_estado();
static bool boton2_estado();

void eint1_ISR (void) __irq ;
void eint1_ISR (void) __irq ;

void Gestor_Pulsacion_Init();

void eint2_comprobar();
void eint1_comprobar();
