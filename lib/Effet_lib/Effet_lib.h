// Effet_lib.h
// Par Vincent G.
// 2024-01
// Version 1

// Permet de lire les noms des effets
// Permet de lire et écrire des valeurs de Potentiomètre

#ifndef Effet_lib_h
#define Effet_lib_h

#include <Arduino.h>
#include "Effet_nom.h"
#include <EEPROM.h>

enum pot_type
{
    Mix,
    Ctrl1,
    Ctrl2,
    Ctrl3
};

class Effet_lib
{
public:

    // Initialise les effets
    void init(void);

    void save_EEPROM(void);

    // Permet de lire les noms des différents effets
    // entrée : numréro de 1 à 16
    //
    // sortie : tableau de char qui contient les noms
    char *get_nom_effet(uint8_t numero);

    // Permet de lire les noms des différents effets
    // entrée : numréro de 1 à 16
    //
    // sortie : tableau de char qui contient les noms
    char *get_nom_ctrl1(uint8_t numero);

    // Permet de lire les noms des différents effets
    // entrée : numréro de 1 à 16
    //
    // sortie : tableau de char qui contient les noms
    char *get_nom_ctrl2(uint8_t numero);

    // Permet de lire les noms des différents effets
    // entrée : numréro de 1 à 16
    //
    // sortie : tableau de char qui contient les noms
    char *get_nom_ctrl3(uint8_t numero);

    // Permet de lire la valeur du potentiomètre de volume
    //
    // retourne le pourcentage du potentiomètre désiré
    uint8_t lire_val_pot_vol(void);

    // Permet d'augmenter la valeur d'un potentiomètre d'une valeur donnée
    //
    // quantite : nombre de % en plus par appel de fonction
    // retourne la valeur modifiée
    uint8_t augmenter_val_pot_vol(uint8_t quantite);

    // Permet de réduire la valeur d'un potentiomètre d'une valeur donnée
    //
    // quantite : nombre de % en moins par appel de fonction
    // retourne la valeur modifiée
    uint8_t reduire_val_pot_vol(uint8_t quantite);

    // Permet de lire la valeur d'un potentiomètre
    //
    // no_effet : numéro de l'effet (1 à 16)
    // no_pot   : potentiomètre(Mix, Ctrl1, Ctrl2, Ctrl3)
    // retourne le pourcentage du potentiomètre désiré
    uint8_t lire_val_pot(uint8_t no_effet, pot_type no_pot);

    // Permet d'augmenter la valeur d'un potentiomètre d'une valeur donnée
    //
    // no_effet : numéro de l'effet (1 à 16),
    // no_pot   : potentiomètre(Mix, Ctrl1, Ctrl2, Ctrl3)
    // quantite : nombre de % en plus par appel de fonction
    // retourne la valeur modifiée
    uint8_t augmenter_val_pot(uint8_t no_effet, pot_type no_pot, uint8_t quantite);

    // Permet de réduire la valeur d'un potentiomètre d'une valeur donnée
    //
    // no_effet : numéro de l'effet (1 à 16),
    // no_pot   : potentiomètre(Mix, Ctrl1, Ctrl2, Ctrl3)
    // quantite : nombre de % en moins par appel de fonction
    // retourne la valeur modifiée
    uint8_t reduire_val_pot(uint8_t no_effet, pot_type no_pot, uint8_t quantite);

private:
    char _buffer[20];
    uint8_t _numero;

    uint8_t _val_pot_vol = 25;
    //                         mix ctrl1 ctrl2 ctrl3
    uint8_t _val_pot_effet[64] = {00, 00, 00, 00,  // effet 1  Shimmer Reverb
                                  50, 50, 50, 50,  // effet 2  Modulated Reverb
                                  50, 50, 10, 25,  // effet 3  Modulated Delay
                                  40, 5, 40, 0,  // effet 4  Pitch Delay
                                  75, 45, 65, 75,  // effet 5  Bit Crusher
                                  50, 0, 50, 100,  // effet 6  Dual Pitch Shifter
                                  50, 25, 60, 30,  // effet 7  Hamonic Tremolo
                                  50, 0, 35, 65,  // effet 8  Daydream
                                  50, 35, 40, 60,  // effet 9  Phaser
                                  50, 40, 75, 75,  // effet 10 Flanger
                                  75, 25, 100, 25,  // effet 11 Chorus
                                  50, 50, 50, 50,  // effet 12 Harmonic Tremolo
                                  50, 60, 40, 0,  // effet 13 Pulse Tremolo
                                  50, 50, 50, 50,  // effet 14 Ring Modulation
                                  75, 50, 25, 0,  // effet 15 Pitch Modulation
                                  50, 50, 65, 55}; // effet 16 Auto-Filter
};
#endif