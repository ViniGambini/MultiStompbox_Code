#ifndef Fonctions_h
#define Fonctions_h

#include "IO_Pin.h"
#include <SPI.h>

void board_init(void);

void digitalPotWrite(uint8_t CS_pin, uint8_t num_pot, uint8_t pourcentage);
void digitalPotRegisterWrite(uint8_t CS_pin, uint8_t registre, uint8_t valeur);

#endif