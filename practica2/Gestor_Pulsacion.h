#define EXTMODE          (*((volatile unsigned long *) 0xE01FC148))
#define EXTPOLAR          (*((volatile unsigned long *) 0xE01FC14C))
#define VICIntEnClear          (*((volatile unsigned long *) 0xFFFFF014))
int button_nueva_pulsacion_1();

int button_nueva_pulsacion_2();

void button_clear_nueva_pulsacion_1();

void button_clear_nueva_pulsacion_2();
void eint1_ISR (void) __irq ;
void eint1_ISR (void) __irq ;

void Gestor_Pulsacion_Init();

void eint2_comprobar();
void eint1_comprobar();
