#ifndef Fonctions_h
#define Fonctions_h

#include "IO_Pin.h"
#include <SPI.h>

// Initialisation du SPI/Serial
// pinMode() des IOs
void init_board(void);

// Initialisation des potentiomètres
// Registre TCON
void init_pots(void);

/*Envoie une valeur au potentiomètre donnée
  CS_pin : numéro de pin CS de la pièce
  num_pot : numéro du potentiomètre dans la pièce (Pot_0 ou Pot_1)
  pourcentage : valeur envoyé au potentiomètre, de 0 à 100*/
void digitalPotWrite(uint8_t CS_pin, uint8_t num_pot, uint8_t pourcentage);

/*Écrit sur un registre au potentiomètre donnée
  CS_pin : numéro de pin CS de la pièce
  registre : numéro du potentiomètre dans la pièce (Pot_0 ou Pot_1)
  valeur : valeur envoyé au registre, de 0x00 à 0xFF*/
void digitalPotRegisterWrite(uint8_t CS_pin, uint8_t registre, uint8_t valeur);

#endif