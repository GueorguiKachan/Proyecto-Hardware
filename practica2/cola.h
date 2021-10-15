#define SIZE 32

struct evento{
	uint_8t ID_evento;
	uint32_t auxData;
};

int lleno ();
int vacio ();
void cola_guardar_eventos(uint8_t ID_evento, uint32_t auxData)
int eliminar();
void mostrar();
