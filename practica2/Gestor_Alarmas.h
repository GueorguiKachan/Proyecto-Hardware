
struct arraysEventos{
  int eventosProgramados[8]; // Array con la info de los eventos (tipo de evento, si es periódico y los milisegundos de retardo)
  int tiempoEventos[8]; // Array para saber cuanto le falta a cada una de las alarmas
  int estaLibre[8]; // Array para saber cuales de las posiciones para las alarmas está libre u ocupada
  int numEventos;
};

void limpiarEspacio();
void disparaEventos(int tiempoTranscurrido);
void nuevoEvento(int nuevoEvento);