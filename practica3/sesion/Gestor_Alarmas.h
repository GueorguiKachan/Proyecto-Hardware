#ifndef GESTOR_ALARMAS_H
#define GESTOR_ALARMAS_H

#include <stdint.h>
void GestorAlarmaInit();
void guardar_nuevo_caracter(uint32_t nuevo_caracter);
void gestor_alarma_restar_tiempo_alarmas_programadas();
void gestor_alarma_nueva_alarma(uint32_t nuevaAlarma);

#endif