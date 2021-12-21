#include <LPC210x.H> 
#include "cola.h"
#include <stdlib.h>
#include <string.h>
#include "eventos.h"
#define CR     0x0D
#define MAX_MSG_SIZE 1000

static int tamanyo_string;
static volatile int index;
char mensaje_inicial[MAX_MSG_SIZE];
char mensaje_final[MAX_MSG_SIZE];
char *buffer_string;
char *aux_buffer;
static int tamanyo_aux_buffer = 0;
bool pendiente = false;
static int fin = 0;
static int ini = 0;
static bool mensaje_error = false;
static int num_partida = 0;
static int tamanyo_mensaje_final = 0;

void uart0_ISR (void) __irq{
		
	if(((U0IIR >> 1) & 0x111) == 1 ){
		cola_guardar_eventos(cout_caracter,0);
	}	
	else{
		uint32_t hola = U0RBR;
		VICVectAddr = 0;
		cola_guardar_eventos(nuevo_caracter,hola);
	}
}


	
void init_serial (void)  {               /* Initialize Serial Interface       */
  PINSEL0 |= 0x00000005;                  /* Enable RxD1 and TxD1              */ //RxD1 =  Receiver input for UART1 //TxD1 =  Transmitter output for UART1 // UART0 --> 0x00000005
  U0LCR = 0x83;                          /* 8 bits, no Parity, 1 Stop bit     */
  U0DLL = 97;                            /* 9600 Baud Rate @ 15MHz VPB Clock  */
  U0LCR = 0x03;                          /* DLAB = 0                          */
	U0IER = 0x07; //es de 32bits
	VICVectAddr13= (unsigned long)uart0_ISR;
	VICVectCntl13 = 0x20 | 6;     
	VICIntEnable = VICIntEnable | 0x00000040;                  // Enable Timer0 Interrupt
	//VICIntSelect = VICIntSelect | 0x00000080; 
}

int sendchar (int ch)  {                 /* Write character to Serial Port    */

  /*if (ch == '\n')  {
    while (!(U1LSR & 0x20));
    U1THR = CR;                           output CR */
  //}
  while (!(U0LSR & 0x20));
  return (U0THR = ch);
}

void Gvisualizacion_print_array(int lista[]){
	int i;
	int size;
	int value = 0;
	size = 1208;
	for(i = 0; i <= size; i++){
		sendchar(lista[i]);
	}
	//sendchar('n');
}
void enviar_string(char *mensaje, int tam){
	int t ;
	//if(index == 0){ // Ya se ha acabado de copiar lo que se tenía que copiar
	index = 0;
		buffer_string = mensaje;
		
		if(buffer_string[index] == '+'){
			tamanyo_string = 1208;
		}
		else{
			tamanyo_string = tam;
			t = tam;
			mensaje_error = true;
		}
		sendchar(buffer_string[index]);
		index++;
	
	/*}
	else{ // Todavia faltan cosas del antiguo string por copiar
		t = index;
		if(mensaje[0] == '+'){
			tamanyo_aux_buffer += 722;
		}else{
			tamanyo_aux_buffer += tam;
		}
		aux_buffer = mensaje;
		pendiente = true;
		sendchar(buffer_string[index]);
		index++;
	}*/
}

void continuar_mensaje(){
	int val = buffer_string[index];
	if(index < tamanyo_string){
		if(buffer_string[index] != '\0'){
		
			sendchar(buffer_string[index]);
			index++;
		}
		else{
			index = 0;
			if(fin ==1 ){
				cola_guardar_eventos(fin_partida,0);
				fin=0;
			}
	
			if(pendiente){
				buffer_string = aux_buffer;
				tamanyo_string = tamanyo_aux_buffer;
				pendiente = false;
				sendchar(buffer_string[index]);
				index++;
			}
			if(mensaje_error){
				mensaje_error = false;
				cola_guardar_eventos(pulsacion2,0);
			}
		}
	}
	else{
		if(mensaje_error){
				mensaje_error = false;
				cola_guardar_eventos(pulsacion2,0);
			}
		index = 0;
		val = index;
		tamanyo_string = 0;
	}
}
	
void mensaje_inicial_print(){
	static char explicacion_juego[] =  "Juego sudoku 9x9:\n El mismo numero no puede estar en la misma fila columna o cuadrado\n";
	static char explicacion_simbolos[] = " ~: El valor es una pista(no se modifica) --- >: Es un nuevo valor a introducir\n";
	static char explicacion_candidatos [] = " Tabla de candidatos a la derecha. Las casillas marcadas con 'P' contienen una pista\n";
	static char explicacion_inicio[] = " Para empezar la partida escribir #NEW! en el uart o pulsar cualquier boton\n";
	static char explicacion_reset[] = " Para acabar la partida escribir #RST! en la uart\n";
	static char explicacion_jugada[] = " Introducir una jugada: #FCVS! donde: F = fila, C = columna, V = nuevo valor S = F+C+V % 8 \n";
	static char explicacion_aceptar[] = " Tras introducir la jugada, pulsar eint0 para aceptarla o eint1 para cancelar.\n";
	static char explicacion_jugada_anulada[] = " Si tras 3 segundos no se toma una decisión la jugada queda anulada.\n";
	if(ini == 0){
	strcpy(mensaje_inicial,explicacion_juego);
	//strcpy(buffer_string,explicacion_juego);
	strcat(mensaje_inicial,explicacion_simbolos);
	strcat(mensaje_inicial,explicacion_candidatos);
	strcat(mensaje_inicial,explicacion_inicio);
	strcat(mensaje_inicial,explicacion_reset);
	strcat(mensaje_inicial,explicacion_jugada);
	strcat(mensaje_inicial,explicacion_aceptar);
	strcat(mensaje_inicial,explicacion_jugada_anulada);
	}
	index = 0;
	tamanyo_string = sizeof(explicacion_inicio) + sizeof(explicacion_juego) + sizeof(explicacion_jugada) + sizeof(explicacion_candidatos)+
													sizeof(explicacion_reset)+sizeof(explicacion_simbolos)+sizeof(explicacion_aceptar) + sizeof(explicacion_jugada_anulada);
	buffer_string = mensaje_inicial;
	sendchar(buffer_string[index]);
	index++;
}
void enviar_mensaje_final(int minutos, int segundos,int razon,int tiempo_computo){
	static char mins[2];
	static char secs[2];
	static char t_computo[2];
	 char m[] = " minutos y ";
	 char s[] = " segundos. \n";
	static char explicacion_final[] =  "Fin de la partida despues de ";//+ itoa(minutos) + " minutos y " + segundos + " segundos."
	static char info_tiempo_computo[] = "El total del tiempo de computo es: ";
	static char medida_tiempo[] = "ms\n";
	char explicacion_reiniciar[] = "Para volver a jugar pulsar cualquiera de los 2 botones\n\n";
	static char razon_fin_1[] = "\nPartida finalizada por haber tecleado #RST!\n";
	static char razon_fin_2[] = "\nPartida finalizada por haber solcuionado el sudoku\n";
	int tamanyo_string_razon;
	char mensaje_completo[MAX_MSG_SIZE] = "\0";
	sprintf(mins,"%d",minutos);
	sprintf(secs,"%d",segundos);
	sprintf(t_computo,"%d",tiempo_computo);
	//if(ini == 0){
	
		strcpy(mensaje_final,explicacion_final);
		strcat(mensaje_final,mins);
		strcat(mensaje_final, m);
		strcat(mensaje_final,secs);
		strcat(mensaje_final,s);
		strcat(mensaje_final,info_tiempo_computo);
		strcat(mensaje_final,t_computo);
		strcat(mensaje_final,medida_tiempo);
		strcat(mensaje_final,explicacion_reiniciar);
		ini =1;
		tamanyo_mensaje_final = sizeof(explicacion_final)+sizeof(m) + sizeof(s) + sizeof(mins) + sizeof(secs) +
														sizeof(explicacion_reiniciar)+ sizeof(info_tiempo_computo) + sizeof(t_computo)+ sizeof(medida_tiempo);
	//}
	index = 0;
	if(razon == 1 && num_partida == 0){
		num_partida = 1;
		tamanyo_aux_buffer = sizeof(razon_fin_1);
		strcat(mensaje_completo,razon_fin_1);
		strcat(mensaje_completo,mensaje_final);
		buffer_string = mensaje_completo;
	}else{
		tamanyo_aux_buffer = sizeof(razon_fin_2);
		strcat(mensaje_completo,razon_fin_2);
		strcat(mensaje_completo,mensaje_final);
		buffer_string = mensaje_completo;
	}
	buffer_string = mensaje_completo;
	tamanyo_string = tamanyo_mensaje_final+tamanyo_aux_buffer;
	sendchar(buffer_string[index]);
	index++;
	fin = 1;
}
