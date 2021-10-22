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
struct elemento * eliminar();
//void mostrar();
bool hay_eventos();
uint8_t leerMasAntiguo();
