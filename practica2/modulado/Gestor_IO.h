#ifndef 	GESTOR_IO_H
#define 	GESTOR_IO_H
#include <stdbool.h>
bool gestorIO_visualizar();

void gestorIO_iniciar();
int gestorIO_leer_fila();
void gestorIO_escribir_fila(int valor);
int gestorIO_leer_columna();
void gestorIO_escribir_columna(int valor);
int gestorIO_leer_valor_celda();
void gestorIO_escribir_valor_celda(int valor);
int gestorIO_leer_valor_nuevo();
void gestorIO_escribir_valor_nuevo(int valor);
int gestorIO_leer_candidatos();
void gestorIO_escribir_candidatos(int valor);
int gestorIO_leer_error();
void gestorIO_escribir_error(int valor);
void gestorIO_escribir_overflow(int valor);
void gestorIO_escribir_idle(int valor);

void gestorIO_activar_validacion();
void gestorIO_apagar_validacion();
#endif