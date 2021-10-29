#define EXTMODE          (*((volatile unsigned long *) 0xE01FC148))
#define EXTPOLAR          (*((volatile unsigned long *) 0xE01FC14C))
void GPIO_iniciar();
int GPIO_leer(int bit_inicial,int num_bits);
int GPIO_escribir(int bit_inicial,int num_bits,int valor);
void GPIO_marcar_entrada(int bit_inicial,int num_bits);
void GPIO_marcar_salida(int bit_inicial,int num_bits);
