// --------------- Multi-Stompbox --------------- //
//                                                //
// Par Vincent G.                        main.cpp //
// Version 1                           2024-03-01 //
// ---------------------------------------------- //

#include <Arduino.h>
//#include "DFRobot_GDL.h"
#include "Effet_lib.h"
#include "IO_Pin.h"
#include "Fonctions.h"

//----------------------------------------------//
//                    Define                    //
//----------------------------------------------//

#define POT_0 0x00 // adresse interne du pot 0
#define POT_1 0x10 // adresse interne du pot 1
#define TCON 0x40  // adresse du registre TCON des potentiomètres

#define increment_pot 2 // nombre de pourcentage par clic de l'encodeur

//----------------------------------------------//
//                    Objets                    //
//----------------------------------------------//

DFRobot_ST7789_240x320_HW_SPI screen(/*dc=*/IO_DC_ECRAN, /*cs=*/IO_CS_ECRAN, /*rst=*/IO_RST_ECRAN);
Effet_lib effets;

//----------------------------------------------//
//                   Variables                  //
//----------------------------------------------//

enum etat { 
  select_effet,
  change_effet,
  select_vol,
  change_vol,
  select_mix,
  change_mix,
  select_ctrl1,
  change_ctrl1,
  select_ctrl2,
  change_ctrl2,
  select_ctrl3,
  change_ctrl3,

  
} etat_affichage;

uint8_t currentStateCLK;           // pour encodeur
uint8_t lastStateCLK;              // pour encodeur
unsigned long lastButtonPress = 0; // pour anti rebond du bouton

unsigned long lastExecutedMillis = 0; // pour timer l'envoie des données sur le terminal

uint8_t effet_actif = 1;

void init_ecran(void){
  screen.begin();
  screen.fillScreen(COLOR_RGB565_DGRAY);
}

//----------------------------------------------//
//                    Setup                     //
//----------------------------------------------//

void setup()
{
  init_board();

  // Setup de l'écran
  init_ecran();

  lastStateCLK = digitalRead(IO_S1_ENC);

  // Setup des potentiomètres
  digitalPotRegisterWrite(IO_CS_POT_VOL, TCON, 0xFF); // Set les registres TCON
  digitalPotRegisterWrite(IO_CS_POT_MIX, TCON, 0xFF);
  digitalPotRegisterWrite(IO_CS_POT_A, TCON, 0xFF);
  digitalPotRegisterWrite(IO_CS_POT_B, TCON, 0xFF);
  delay(200);
  digitalPotWrite(IO_CS_POT_VOL, POT_0, effets.lire_val_pot_vol());
  digitalPotWrite(IO_CS_POT_MIX, POT_0, effets.lire_val_pot(effet_actif, Mix));
  digitalPotWrite(IO_CS_POT_A, POT_0, effets.lire_val_pot(effet_actif, Ctrl1));
  digitalPotWrite(IO_CS_POT_A, POT_1, effets.lire_val_pot(effet_actif, Ctrl2));
  digitalPotWrite(IO_CS_POT_B, POT_0, effets.lire_val_pot(effet_actif, Ctrl3));

  //screen.fillScreen(COLOR_RGB565_BLUE);

  // Menu Principal
}

//----------------------------------------------//
//                     Loop                     //
//----------------------------------------------//

void loop()
{
  // lit l'état de la pin CLK
  currentStateCLK = digitalRead(IO_S1_ENC);

  // switch case

  if (currentStateCLK != lastStateCLK && currentStateCLK == 1)
  {
    if (digitalRead(IO_S2_ENC) != currentStateCLK)
    { // CCW
    }
    else
    { // CW
    }
  }

  // fin switch case

  lastStateCLK = currentStateCLK;

  //----------------------Lecture bouton----------------------

  int btnState = digitalRead(IO_SW_ENC);
  if (btnState == LOW)
  {
    if (millis() - lastButtonPress > 50)
    {
      // code ici
    }
    lastButtonPress = millis();
  }
  
  delay(1);
}
