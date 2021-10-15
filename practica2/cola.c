// Circular Queue implementation in C

#include <stdio.h>
#include "cola.h"

//evento colaEventos [SIZE]; 
int items[SIZE];
int front = -1, rear = -1;
// int ultimoProcesado = rear;

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
void cola_guardar_eventos(uint8_t ID_evento, uint32_t auxData) {
  if (lleno())
    printf("\n Queue is full!! \n");
  else {
    if (front == -1) front = 0;
    rear = (rear + 1) % SIZE;
    
    /***************************************
    if(rear == ultimoProcesado){
        return overflow;
    }
    evento nuevo;
    nuevo.ID_evento = ID_evento;
    nuevo.auxData = auxData;
    //colaEventos[rear] = nuevo;
    */
    
    items[rear] = element;
    printf("\n Inserted -> %d", element);
  }
}

// Removing an element
int eliminar() {
  int element;
  if (vacio()) {
    printf("\n Queue is empty !! \n");
    return (-1);
  } else {
    element = items[front];
    if (front == rear) {
      front = -1;
      rear = -1;
    } 
    // Q has only one element, so we reset the 
    // queue after dequeing it. ?
    else {
      front = (front + 1) % SIZE;
    }
    printf("\n Deleted element -> %d \n", element);
    return (element);
  }
}

// Display the queue
void mostrar() {
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
}
/************************
bool hayEventoNuevo(){
  if(rear != ultimoProcesado){
      return true; 
  }
  else{
      return false;   
  }
}*/

evento leerMasAntiguo(){
   ultimoProcesado = (ultimoProcesado + 1) % SIZE;
  
    return colaEventos[ultimoProcesado);
}
