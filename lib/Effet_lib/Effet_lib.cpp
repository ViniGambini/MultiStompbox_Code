// Effet_lib.cpp
// Par Vincent G.
// 2024-01
// Version 1

#include "Effet_lib.h"

char *Effet_lib::get_nom_effet(uint8_t numero)
{
    _numero = numero - 1;
    strcpy_P(_buffer, (char *)pgm_read_ptr(&(noms_effets[_numero])));
    return _buffer;
}

char *Effet_lib::get_nom_ctrl1(uint8_t numero)
{
    _numero = numero - 1;
    strcpy_P(_buffer, (char *)pgm_read_ptr(&(noms_ctrl1[_numero])));
    return _buffer;
}

char *Effet_lib::get_nom_ctrl2(uint8_t numero)
{
    _numero = numero - 1;
    strcpy_P(_buffer, (char *)pgm_read_ptr(&(noms_ctrl2[_numero])));
    return _buffer;
}

char *Effet_lib::get_nom_ctrl3(uint8_t numero)
{
    _numero = numero - 1;
    strcpy_P(_buffer, (char *)pgm_read_ptr(&(noms_ctrl3[_numero])));
    return _buffer;
}

uint8_t Effet_lib::lire_val_pot_vol(void){
    return 100-_val_pot_vol;
}

// Fix sur PCB
// Inverser Réduire et Augmenter
// Enlever les 100- dans les 3 fonction lors du fix physique

uint8_t Effet_lib::reduire_val_pot_vol(uint8_t quantite){
    if(_val_pot_vol + quantite < 100){
        _val_pot_vol = _val_pot_vol + quantite;
    }
    else{
        _val_pot_vol = 100;
    }
    return 100-_val_pot_vol;
}

uint8_t Effet_lib::augmenter_val_pot_vol(uint8_t quantite){
    if(_val_pot_vol - quantite > 0){
        _val_pot_vol = _val_pot_vol - quantite;
    }
    else{
        _val_pot_vol = 0;
    }
    return 100-_val_pot_vol;
}

uint8_t Effet_lib::lire_val_pot(uint8_t no_effet, pot_type no_pot)
{
    return _val_pot_effet[((no_effet - 1) * 4) + no_pot];
}

uint8_t Effet_lib::augmenter_val_pot(uint8_t no_effet, pot_type no_pot, uint8_t quantite)
{
    if (_val_pot_effet[((no_effet - 1) * 4) + no_pot] + quantite < 100)
    {
        _val_pot_effet[((no_effet - 1) * 4) + no_pot] = _val_pot_effet[((no_effet - 1) * 4) + no_pot] + quantite;
    }
    else
    {
        _val_pot_effet[((no_effet - 1) * 4) + no_pot] = 100;
    }
    return _val_pot_effet[((no_effet - 1) * 4) + no_pot];
}

uint8_t Effet_lib::reduire_val_pot(uint8_t no_effet, pot_type no_pot, uint8_t quantite)
{
    if (_val_pot_effet[((no_effet - 1) * 4) + no_pot] - quantite > 0)
    {
        _val_pot_effet[((no_effet - 1) * 4) + no_pot] = _val_pot_effet[((no_effet - 1) * 4) + no_pot] - quantite;
    }
    else
    {
        _val_pot_effet[((no_effet - 1) * 4) + no_pot] = 0;
    }
    return _val_pot_effet[((no_effet - 1) * 4) + no_pot];
}