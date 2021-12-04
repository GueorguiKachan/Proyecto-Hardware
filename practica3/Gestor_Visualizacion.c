#include <LPC210x.H> 
#define CR     0x0D

void uart1_ISR (void) __irq{
	int hola = U1RBR;
	VICVectAddr = 0;
	U1THR = hola;
}
	
void init_serial (void)  {               /* Initialize Serial Interface       */
  PINSEL0 = 0x00050000;                  /* Enable RxD1 and TxD1              */ //RxD1 =  Receiver input for UART1 //TxD1 =  Transmitter output for UART1 // UART0 --> 0x00000005
  U1LCR = 0x83;                          /* 8 bits, no Parity, 1 Stop bit     */
  U1DLL = 97;                            /* 9600 Baud Rate @ 15MHz VPB Clock  */
  U1LCR = 0x03;                          /* DLAB = 0                          */
	U1IER = 0x05;
	VICVectAddr13= (unsigned long)uart1_ISR;
	VICVectCntl13 = 0x20 | 7;     
	VICIntEnable = VICIntEnable | 0x00000080;                  // Enable Timer0 Interrupt
	//VICIntSelect = VICIntSelect | 0x00000080; 
}

static int sendchar (int ch)  {                 /* Write character to Serial Port    */

  /*if (ch == '\n')  {
    while (!(U1LSR & 0x20));
    U1THR = CR;                           output CR */
  //}
  while (!(U1LSR & 0x20));
  return (U1THR = ch);
}

void Gvisualizacion_print_array(int lista[]){
	int i;
	int size;
	int value = 0;
	size = 380;
	for(i = 0; i <= size; i++){
		sendchar(lista[i]);
	}
	sendchar('n');
}

