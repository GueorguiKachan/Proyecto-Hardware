
#include <stdint.h>
#include <stdbool.h>
#define SIZE 32

struct elemento{
	uint8_t ID_evento;
	uint32_t auxData;
	uint32_t tiempo;
};

int lleno ();
int vacio ();
bool cola_guardar_eventos(uint8_t ID_evento, uint32_t auxData);
bool eliminar();
//void mostrar();
bool hay_eventos();
struct elemento elementoMasAntiguo();
int leerIDMasAntiguo();
uint32_t leerDatosMasAntiguo();
uint32_t leerTiempoMasAntiguo();

