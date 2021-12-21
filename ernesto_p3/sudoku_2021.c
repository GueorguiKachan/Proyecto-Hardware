#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "sudoku_2021.h"
#include "tableros.h"
#include "eventos.h"
#include "cola.h"
#include "timers.h"
/* *****************************************************************************
 * propaga el valor de una determinada celda en C
 * para actualizar las listas de candidatos
 * de las celdas en su su fila, columna y regi�n */
/* Recibe como parametro la cuadricula, y la fila y columna de

 * la celda a propagar; no devuelve nada
 */
 #define max_message_size 100
 const int sudoku_array_size = 1208;
 static int datos_sudoku[sudoku_array_size];
 static int indice = 0;
 static int fila_cambiada = 0;
 static int columna_cambiada = 0;
 static int valor_cambiado = 0;
 static int index_posible_valor;
 static char datos[sudoku_array_size];
 static char  mensaje_error[max_message_size];
 static char valor_anterior;
 char razon1[] = " porque la celda es una pista\n";
 char razon2[] = " porque el valor no esta entre los candidatos\n";
 static bool error_introducir = false;
  static bool error_introducir_tamanyo = false;
 static int tamanyo_mensaje_error = 0;
 static bool sudoku_solucionado = false;
 static bool confirmar_solucion = false;
 int tiempo_Antes = 0;
 int tiempo_Despues = 0;
 int tiempo_computo = 0;
 
 
void candidatos_propagar_c(CELDA cuadricula[NUM_FILAS][NUM_COLUMNAS],
	uint8_t fila, uint8_t columna)
{
    uint8_t j, i , init_i, init_j, end_i, end_j;
    /* puede ayudar esta "look up table" a mejorar el rendimiento */
    const uint8_t init_region[NUM_FILAS] = {0, 0, 0, 3, 3, 3, 6, 6, 6};

    /* valor que se propaga */
    uint8_t valor = celda_leer_valor(cuadricula[fila][columna]);

    /* recorrer fila descartando valor de listas candidatos */
    for (j=0;j<NUM_FILAS;j++){
				celda_eliminar_candidato(&cuadricula[fila][j],valor);
		}
		
    /* recorrer columna descartando valor de listas candidatos */
    for (i=0;i<NUM_FILAS;i++){
				celda_eliminar_candidato(&cuadricula[i][columna],valor);			
		}

    /* determinar fronteras región */
    init_i = init_region[fila];
    init_j = init_region[columna];
    end_i = init_i + 3;
    end_j = init_j + 3;

    /* recorrer region descartando valor de listas candidatos */
    for (i=init_i; i<end_i; i++) {
      for(j=init_j; j<end_j; j++) {
					celda_eliminar_candidato(&cuadricula[i][j],valor);
			}
    }
}

/* *****************************************************************************
 * calcula todas las listas de candidatos (9x9)
 * necesario tras borrar o cambiar un valor (listas corrompidas)
 * retorna el numero de celdas vacias */

/* Init del sudoku en codigo C invocando a propagar en C
 * Recibe la cuadricula como primer parametro
 * y devuelve en celdas_vacias el n�mero de celdas vacias
 */
int candidatos_actualizar_c(CELDA cuadricula[NUM_FILAS][NUM_COLUMNAS])
{
  int celdas_vacias = 0;
  uint8_t i;
	uint8_t j;
	
	//borrar todos los candidatos
	for ( i=0; i<NUM_FILAS;i++){
		for (j=0; j <NUM_FILAS;j++){
			celda_eliminar_candidatos(&cuadricula[i][j]);
		}
	}
	
	//recalcular candidatos de las celdas vacias calculando cuantas hay vacias
	for ( i=0; i<NUM_FILAS;i++){
		for (j=0; j <NUM_FILAS;j++){
			if (celda_leer_valor(cuadricula[i][j]) == 0){
				celdas_vacias ++;
			}
			else{
				candidatos_propagar_c(cuadricula,i,j);
			}
		}	
	}
	return celdas_vacias; //retornar el numero de celdas vacias
}
static void sudoku_tablero_juego_nuevo(){
	uint8_t i;
	uint8_t j;
	for (i = 0; i<NUM_FILAS; i++){
		for (j = 0; j<NUM_FILAS; j++){
			cuadricula_C_C[i][j] = cuadricula_ARM_C[i][j];
		}
	}
}
	
void sudoku_init(){
	sudoku_tablero_juego_nuevo();
	candidatos_actualizar_c(cuadricula_C_C);
}
bool sudoku_fila_columna_validos(uint8_t valor_fila,uint8_t valor_columna){
	if (valor_fila >0 && valor_fila <=9 && valor_columna >0 && valor_columna <= 9){
		return true;
	}
	return false;
}
static bool sudoku_comprobar_borrar_valor(uint8_t fila_actual, uint8_t columna_actual){
	uint8_t esPista;
	if(sudoku_fila_columna_validos(fila_actual,columna_actual)){ // Se quiere borrar, comprobar que no sea una pista
		esPista = (cuadricula_C_C[fila_actual-1][columna_actual-1] >> 4) & 0x1;
	
		//Se comprueba que no sea pista, el valor sea un candidato, y que la columna, fila y valor introducidos sean válidos
		if(esPista == 0){
			return true;
		}
	}	
	return false;
}

static bool sudoku_comprobar_valor_nuevo_valido(uint8_t fila_actual, uint8_t columna_actual, uint8_t valor_nuevo){
	uint8_t esPista = 2;
	uint16_t candidatos;
	bool esCandidato = FALSE;
	//comprobar que las columnas y filas son correctas
	if (sudoku_fila_columna_validos(fila_actual,columna_actual)){ //Comprobar que el valor a introducir es válido
		esPista = (cuadricula_C_C[fila_actual-1][columna_actual-1]>>4) &0x1;
		candidatos = cuadricula_C_C[fila_actual-1][columna_actual-1]>>7;
		//comprobar que el valor sea correcto
		if (valor_nuevo <= 9 && valor_nuevo >= 1){
			candidatos = candidatos >> (valor_nuevo -1);
			esCandidato = 0 == (candidatos &0x1);
		//comprobar que sea valor candidato y no sea pista
			if (esCandidato){
				if (esPista == 0){
					return true;
				}
			}
		}
	}
	return false;
}

bool sudoku_anyadir_valor_nuevo(uint8_t fila_actual, uint8_t columna_actual, uint8_t valor_nuevo){
	int num_vacias;
		if (sudoku_comprobar_valor_nuevo_valido(fila_actual,columna_actual,valor_nuevo)){
			cuadricula_C_C[fila_actual-1][columna_actual-1] =  (cuadricula_C_C[fila_actual-1][columna_actual-1] & 0xfff0) + valor_nuevo;
			tiempo_Antes = clock_gettime();
			num_vacias = candidatos_actualizar_c(cuadricula_C_C);	
			tiempo_computo +=  clock_gettime() - tiempo_Antes;
			if(num_vacias == 0){
				sudoku_solucionado = true;
				cola_guardar_eventos(pulsacion1,0);
			}
			return true;
		}
		else{
			return false;
		}
}

void sudoku_borrar_valor(uint8_t fila_actual, uint8_t columna_actual){
	if (sudoku_comprobar_borrar_valor(fila_actual,columna_actual)){
		cuadricula_C_C[fila_actual-1][columna_actual-1] =  (cuadricula_C_C[fila_actual-1][columna_actual-1] & 0xfff0);
		candidatos_actualizar_c(cuadricula_C_C);
	}
}

uint8_t sudoku_get_valor_celda(uint8_t valor_fila,uint8_t valor_columna){
	return cuadricula_C_C[valor_fila-1][valor_columna-1] & 15;
}

uint8_t sudoku_get_valor_error(uint8_t valor_fila,uint8_t valor_columna){
	return (cuadricula_C_C[valor_fila-1][valor_columna-1]>>5 & 1);
}
bool sudoku_get_pista(uint8_t valor_fila,uint8_t valor_columna){
	return ((cuadricula_C_C[valor_fila-1][valor_columna-1] >>4) & 1) == 1;
}

uint16_t sudoku_get_valor_candidatos(uint8_t valor_fila,uint8_t valor_columna){
	return cuadricula_C_C[valor_fila-1][valor_columna-1]>>7 ;
}

char sudoku_get_candidato_9_6 (uint8_t valor_fila,uint8_t valor_columna){
	uint16_t aux = ((sudoku_get_valor_candidatos(valor_fila, valor_columna) >> 5 ) & 0xf);
	if (aux >= 10){
		return aux -10 + 'a';
	}
	else {
		return aux + '0'; 
	}
}
char sudoku_get_candidato_5_2 (uint8_t valor_fila,uint8_t valor_columna){
	uint16_t aux = ((sudoku_get_valor_candidatos(valor_fila, valor_columna) >> 1 ) & 0xf);
	if (aux >= 10){
		return aux -10 + 'a';
	}
	else {
		return aux + '0'; 
	}
}

char sudoku_get_candidato_1 (uint8_t valor_fila,uint8_t valor_columna){
	return ((sudoku_get_valor_candidatos(valor_fila, valor_columna)) & 0x1) +'0';
}
static char sudoku_get_candidato (uint8_t valor_fila,uint8_t valor_columna, int candidato){
	int aux = ((sudoku_get_valor_candidatos(valor_fila, valor_columna)) >> (candidato -1) & 0x1);
	if (aux == 1){
		return '_';
	}
	return candidato + '0';
}


char *sudoku_print(){
	int i;
	int j;
	int celda;
	int index = 0;
	int index_candidatos = 0;
	int col;
	int candidato_uno = 0;
	int candidato_dos = 0;
	int candidato_tres = 0;
	for (i = 0; i < sudoku_array_size; i++){
		datos[i] = ' ';
	}
	for (i = 0; i<20; i += 2){
		datos[i] = '+';
		datos[i+1] = '-';
	}
	datos[19] = '\n';
	index = 20;
	while (index <= (sudoku_array_size - 20)){
		datos[index] = '|';
		datos[index+2]= '.';
		datos[index+4]= '.';
		datos[index+6] = '|';
		datos[index+8]= '.';
		datos[index+10]= '.';
		datos[index+12] = '|';
		datos[index+14]= '.';
		datos[index+16]= '.';
		datos[index+18] = '|';
		datos[index+19]= ' ';
		datos[index+20] = '|';
		datos[index+30] = '.';
		datos[index+40] = '.';
		datos[index+50] = '|';
		datos[index+60] = '.';
		datos[index+70] = '.';
		datos[index+80] = '|';
		datos[index+90] = '.';
		datos[index+100] = '.';
		datos[index+110] = '|';
		datos[index+111] = '\n';
		index += 112;
		datos[index] = '+';
		datos[index+2]= '+';
		datos[index+4]= '+';
		datos[index+6] = '+';
		datos[index+8]= '+';
		datos[index+10]= '+';
		datos[index+12] = '+';
		datos[index+14]= '+';
		datos[index+16]= '+';
		datos[index+18] = '+';
		datos[index+19]= '\n';
		index +=20;
	}
index = 20;
	for (i = 1 ; i<=9; i++){
		for (j = 1; j <= 9; j++){
			datos[index+1] = '0' + sudoku_get_valor_celda(i,j);
			if (datos[index+1] == '0'){
				datos[index +1] = ' ';
				datos[index + 113] = '-';
			}else{
			if (sudoku_get_valor_error(i,j)){
				datos[index + 113] = '^';
			}else if (sudoku_get_pista(i,j)){
					datos[index + 113] = '~';
			}
		}
			index += 2;
			index_candidatos = ((( 40 + (i-1)* 132) + ((j-1) * 10))) ;
		
			if (!sudoku_get_pista(i,j)){
				int candidato;
				for (candidato = 1; candidato <= 9; candidato++){
					datos[index_candidatos + candidato] = sudoku_get_candidato(i,j,candidato);
				}
		}else{			
					datos[index_candidatos + 1 ] = ' ';
					datos[index_candidatos + 2 ] = ' ';
					datos[index_candidatos + 3 ] = 'P';
					datos[index_candidatos + 4 ] = 'I';
					datos[index_candidatos + 5 ] = 'S';
					datos[index_candidatos + 6 ] = 'T';
					datos[index_candidatos + 7 ] = 'A';
					datos[index_candidatos + 8 ] = ' ';
					datos[index_candidatos + 9 ] = ' ';
				}
		
			
	}
		index +=114;
}
	
  	if(!error_introducir){
		return datos;
	}else{
		return mensaje_error;
	}
}

void generar_evento_sudoku(){
	indice = 0;
	if (indice< sudoku_array_size){
		cola_guardar_eventos(cout_caracter,0);

	}
}

int siguiente_caracter_sudoku(){
	int siguiente_dato_escribir = datos_sudoku[indice];
	//cola_guardar_eventos(cout_caracter,0);
	indice ++;
	return siguiente_dato_escribir;
}

bool existe_siguiente_caracter_sudoku(){
	return indice < sudoku_array_size;
}

void introducir_posible_valor(int fila, int columna, int valor){
	index_posible_valor = /* (21 + ((fila -1) * 2)*20);*/ /* (21 * (fila - 1) + (columna - 1) * 2) + ((fila - 1)*78);*/((( 21 + (fila-1)* 132) + ((columna-1) * 2)));
	fila_cambiada = fila;
	columna_cambiada = columna;
	valor_cambiado =  valor;
	valor_anterior = datos[index_posible_valor];
	//index_posible_valor += (columna-1)*2;
	if (valor_cambiado == 0){
		datos[index_posible_valor] = ' ';
	}
	else{
		datos[index_posible_valor] = '0' + valor;
	}
	//solo se pone '^' si es error
		//datos[index_posible_valor+20] = '^';
	// Fila 1, Columna 1 = index:21
	// Fila 2, Columna 1 = index:61
}

void sudoku_confirmar_valor_nuevo(){
	char fila[1];
	char columna[1];
	char valor[1];
	int tam;
	static char error[] = "Error al introducir el valor ";
	char frase1[] = " en la fila ";
	char frase2[] = " y columna ";
	
	if (valor_cambiado == 0){
		sudoku_borrar_valor(fila_cambiada,columna_cambiada);
	}else{
		if(sudoku_anyadir_valor_nuevo(fila_cambiada,columna_cambiada,valor_cambiado)){
			error_introducir = false;
		}else{
			error_introducir = true;
			error_introducir_tamanyo = true;
			sprintf(fila,"%d",fila_cambiada);
			sprintf(columna,"%d",columna_cambiada);
			sprintf(valor,"%d",valor_cambiado);
			strcpy(mensaje_error,error);
			strcat(mensaje_error,valor);
			strcat(mensaje_error,frase1);
			strcat(mensaje_error,fila);
			strcat(mensaje_error,frase2);
			strcat(mensaje_error,columna);
			if(sudoku_get_pista(fila_cambiada,columna_cambiada)){
				tam = sizeof(razon1);
				strcat(mensaje_error,razon1);
			}
			else{
				tam = sizeof(razon2);
				strcat(mensaje_error,razon2);
			}
			tamanyo_mensaje_error = sizeof(error)+sizeof(frase1)+ sizeof(frase2) +  sizeof(fila) +  sizeof(columna) +  sizeof(valor) + tam;
		}
	}
}

void sudoku_cancelar_valor_nuevo(){
	//index_posible_valor = fila_cambiada * 21 + ((columna_cambiada-1)*2);
	datos[index_posible_valor] = valor_anterior;
	//datos[index_posible_valor+21] = '+';
	valor_anterior = ' ';
	index_posible_valor = 0;
	fila_cambiada = 0;
	columna_cambiada = 0;
	valor_cambiado = 0;
}

char * obtener_sudoku(){
	return datos;
}

int tamanyo_string(){
	if(error_introducir_tamanyo){
		error_introducir_tamanyo =  false;
		return tamanyo_mensaje_error;
	}else{
		return sudoku_array_size;
	}
}

bool final_partida_sudoku(){
	if(sudoku_solucionado || confirmar_solucion){
		if(confirmar_solucion){
			sudoku_solucionado = false;
			confirmar_solucion = true;
		}else{
			confirmar_solucion = false;
		}
			return true;
	}
	else{
		return false;
	}
}
int sudoku_tiempo_computo(){
	return tiempo_computo;
}
