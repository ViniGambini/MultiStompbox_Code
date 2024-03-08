#include <Arduino.h>
#include "Fonctions.h"

#define TCON 0x40  // adresse du registre TCON des potentiomètres

void init_board(void){
    SPI.begin();
    Serial.begin(9600);

    // Potentiomètres
    pinMode(IO_CS_POT_VOL, OUTPUT);
    pinMode(IO_CS_POT_MIX, OUTPUT);
    pinMode(IO_CS_POT_A, OUTPUT);
    pinMode(IO_CS_POT_B, OUTPUT);
    digitalWrite(IO_CS_POT_VOL, HIGH);
    digitalWrite(IO_CS_POT_MIX, HIGH);
    digitalWrite(IO_CS_POT_A, HIGH);
    digitalWrite(IO_CS_POT_B, HIGH);

    // Test Point
    pinMode(IO_TP10, OUTPUT);
    digitalWrite(IO_TP10, LOW);
    
    // FV-1
    pinMode(IO_S0_FV1, OUTPUT);
    pinMode(IO_S1_FV1, OUTPUT);
    pinMode(IO_S2_FV1, OUTPUT);
    digitalWrite(IO_S0_FV1, LOW);
    digitalWrite(IO_S1_FV1, LOW);
    digitalWrite(IO_S2_FV1, LOW);
    
    // Mémoires I2C
    pinMode(IO_A0_MEM_1, OUTPUT);
    pinMode(IO_A0_MEM_2, OUTPUT);
    digitalWrite(IO_A0_MEM_1, LOW);
    digitalWrite(IO_A0_MEM_2, HIGH);

    // Encodeur
    pinMode(IO_S1_ENC, INPUT);
    pinMode(IO_S2_ENC, INPUT);
    pinMode(IO_SW_ENC, INPUT);

    delay(100);
}

void init_pots(void){
  digitalPotRegisterWrite(IO_CS_POT_VOL, TCON, 0xFF); // Set les registres TCON
  digitalPotRegisterWrite(IO_CS_POT_MIX, TCON, 0xFF);
  digitalPotRegisterWrite(IO_CS_POT_A, TCON, 0xFF);
  digitalPotRegisterWrite(IO_CS_POT_B, TCON, 0xFF);
  delay(100);
}

void digitalPotWrite(uint8_t CS_pin, uint8_t num_pot, uint8_t pourcentage)
{
  uint8_t val = round((float)(pourcentage * 255 / 100));

  digitalWrite(CS_pin, LOW);  // met la pin CS a 0
  SPI.transfer(num_pot);      // ecrit l'adresse du potentiometre
  SPI.transfer(val);          // ecrit sa valeur
  digitalWrite(CS_pin, HIGH); // met la pin CS a 1
  delay(1);
}

void digitalPotRegisterWrite(uint8_t CS_pin, uint8_t registre, uint8_t valeur)
{
  digitalWrite(CS_pin, LOW);  // met la pin CS a 0
  SPI.transfer(registre);     // ecrit l'adresse du potentiometre
  SPI.transfer(valeur);       // ecrit sa valeur
  digitalWrite(CS_pin, HIGH); // met la pin CS a 1
  delay(1);
}