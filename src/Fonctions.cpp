#include <Arduino.h>
#include "Fonctions.h"

void init_board(void){
    SPI.begin();
    Serial.begin(9600);

    // Potentiomètres
    pinMode(IO_CS_POT_VOL, OUTPUT);
    pinMode(IO_CS_POT_MIX, OUTPUT);
    pinMode(IO_CS_POT_A, OUTPUT);
    pinMode(IO_CS_POT_B, OUTPUT);

    // Test Point
    pinMode(IO_TP10, OUTPUT);
    
    // FV-1
    pinMode(IO_S0_FV1, OUTPUT);
    pinMode(IO_S1_FV1, OUTPUT);
    pinMode(IO_S2_FV1, OUTPUT);
    
    // Mémoires I2C
    pinMode(IO_A0_MEM_1, OUTPUT);
    pinMode(IO_A0_MEM_2, OUTPUT);

    // Encodeur
    pinMode(IO_S1_ENC, OUTPUT);
    pinMode(IO_S2_ENC, OUTPUT);
    pinMode(IO_SW_ENC, OUTPUT);

    delay(200);

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