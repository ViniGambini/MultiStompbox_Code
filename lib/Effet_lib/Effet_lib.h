// Effet_lib.h
// Par Vincent G.
// 2024-01
// Version 1

// Permet de lire les noms des effets
// Permet de lire et écrire des valeurs de Potentiomètre

#ifndef Effet_lib_h
#define Effet_lib_h

#include <Arduino.h>

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
    //                         mix ctrl1 ctrl2 ctrl3
    uint8_t _val_pot_effet[64] = {50, 50, 50, 50,  // effet 1
                                  50, 50, 50, 50,  // effet 2
                                  50, 50, 50, 50,  // effet 3
                                  50, 50, 50, 50,  // effet 4
                                  50, 50, 50, 50,  // effet 5
                                  50, 50, 50, 50,  // effet 6
                                  50, 50, 50, 50,  // effet 7
                                  50, 50, 50, 50,  // effet 8
                                  50, 50, 50, 50,  // effet 9
                                  50, 50, 50, 50,  // effet 10
                                  50, 50, 50, 50,  // effet 11
                                  50, 50, 50, 50,  // effet 12
                                  50, 50, 50, 50,  // effet 13
                                  50, 50, 50, 50,  // effet 14
                                  50, 50, 50, 50,  // effet 15
                                  50, 50, 50, 50}; // effet 16
};
#endif