// Circular Queue implementation in C

#include <stdio.h>
#include "cola.h"

struct elemento items[SIZE];
int front = -1, rear = -1;

// Check if the queue is full
int lleno () {
  if ((front == rear + 1) || (front == 0 && rear == SIZE - 1)) return 1;
  return 0;
}

// Check if the queue is empty
int vacio () {
  if (front == -1) return 1;
  return 0;
}

// Adding an element
bool cola_guardar_eventos(uint8_t ID_evento, uint32_t auxData) {
	//Nos interesa que no haga overflow
  if (lleno()){
    printf("\n Queue is full!! \n");
		return false;
	}
  else {
    if (front == -1) front = 0;
    rear = (rear + 1) % SIZE;
		struct elemento aux;
		aux.ID_evento = ID_evento;
		aux.auxData = auxData;
		aux.tiempo = 3; //poner marca de tiempo
    items[rear] = aux;
    //printf("\n Inserted -> %d", element);
		return true;
  }
}

// Removing an element
bool eliminar() {
  
  if (vacio()) {
    printf("\n Queue is empty !! \n");
    return false;
  } else {
    if (front == rear) {
      front = -1;
      rear = -1;
    } 
    // Q has only one element, so we reset the 
    // queue after dequeing it. ?
    else {
      front = (front + 1) % SIZE;
    }
    //printf("\n Deleted element -> %d \n", element);
    return true;
  }
}

// Display the queue
/*void mostrar() {
  int i;
  if (vacio())
    printf(" \n Empty Queue\n");
  else {
    printf("\n Front -> %d ", front);
    printf("\n Items -> ");
    for (i = front; i != rear; i = (i + 1) % SIZE) {
      printf("%d ", items[i]);
    }
    printf("%d ", items[i]);
    printf("\n Rear -> %d \n", rear);
  }
}*/

//funcion que compruebe si la cola tiene nuevos eventos(supongo que se refiere a que no está vacia)
bool hay_eventos(){
	return !vacio();
}

//funcion que lee el evento mas antiguo sin procesar ( supongo que todos los van en orden de tiempo, no van desordenados)
struct elemento leerMasAntiguo(){  
    return items[front];
}

uint8_t leerIDMasAntiguo(){
  return items[front].ID_evento;
}
uint32_t leerDatosMasAntiguo(){
  return items[front].auxData;
}
uint32_t leerTiempoMasAntiguo(){
  return  items[front].tiempo;
}
