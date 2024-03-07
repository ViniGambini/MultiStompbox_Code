// --------------- Multi-Stompbox --------------- //
//                                                //
// Par Vincent G.                        main.cpp //
// Version 1                           2024-03-01 //
// ---------------------------------------------- //

#include <Arduino.h>
#include "DFRobot_GDL.h"
#include "Effet_lib.h"
#include "Fonctions.h"

//----------------------------------------------//
//                    Define                    //
//----------------------------------------------//

#define POT_0 0x00 // adresse interne du pot 0
#define POT_1 0x10 // adresse interne du pot 1

#define increment_pot 2 // nombre de pourcentage par clic de l'encodeur

#define WIDTH screen.width()   //240
#define HEIGHT screen.height()  //320

//----------------------------------------------//
//                    Objets                    //
//----------------------------------------------//

DFRobot_ST7789_240x320_HW_SPI screen(/*dc=*/IO_DC_ECRAN, /*cs=*/IO_CS_ECRAN, /*rst=*/IO_RST_ECRAN);
Effet_lib effets;

//----------------------------------------------//
//                   Variables                  //
//----------------------------------------------//

enum etat
{
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
  change_ctrl3

} etat_affichage;

uint8_t btnState;                  // Lecture de l'état du bouton
uint8_t currentStateCLK;           // pour encodeur
uint8_t lastStateCLK;              // pour encodeur
unsigned long lastButtonPress = 0; // pour anti rebond du bouton

//unsigned long lastExecutedMillis = 0; // pour timer l'envoie des données sur le terminal

uint8_t effet_actif = 1;

//----------------------------------------------//
//                    Écran                     //
//----------------------------------------------//

#define couleur_fond COLOR_RGB565_BLACK
#define couleur_nor COLOR_RGB565_LGRAY
uint16_t couleur_highlight = COLOR_RGB565_LGRAY;
#define couleur_sel COLOR_RGB565_YELLOW
#define couleur_ligne COLOR_RGB565_WHITE

#define offset_x 5
#define offset_nom_effet 8
uint8_t offset_pot_vol = 0;
uint8_t offset_pot_ctrl = 0;
uint16_t offset_parametre = 0;

#define distance_entre_ligne 8
#define hauteur_texte_2 14
#define hauteur_texte_3 21

// Initialise l'écran
void init_ecran(void)
{
  screen.begin();
  screen.fillScreen(couleur_fond);
  screen.drawRect(0, 0, 240, 320, couleur_ligne);
  screen.setTextWrap(false);
}

// Affiche le menu principale de base
void menu_princ(void){
  // Ajuste les valeurs d'offset des blocs vol et ctrl
  offset_pot_vol = hauteur_texte_2 + offset_nom_effet*2 + distance_entre_ligne;
  offset_pot_ctrl = offset_pot_vol + hauteur_texte_2 + hauteur_texte_3 + distance_entre_ligne*3;
  offset_parametre = offset_pot_ctrl+hauteur_texte_2*3+hauteur_texte_3*3+distance_entre_ligne*7;

  // Print le bloc Nom_effet
  screen.setTextColor(couleur_nor);
  screen.setTextSize(2);
  screen.setCursor(offset_x, offset_nom_effet);
  screen.print(effets.get_nom_effet(effet_actif));
  screen.drawFastHLine(0, hauteur_texte_2 + offset_nom_effet*2, WIDTH, couleur_ligne);

  // Print le bloc pot_vol
  screen.setTextSize(3);
  screen.setCursor(WIDTH/2-75, offset_pot_vol);
  screen.print("Vol");
  screen.setCursor(WIDTH/2+25, offset_pot_vol);
  screen.print("Mix");
  screen.setTextSize(2);
  screen.setCursor(WIDTH/2-65, offset_pot_vol + hauteur_texte_3 + distance_entre_ligne);
  screen.print(effets.lire_val_pot_vol());
  screen.print("%");
  screen.setCursor(WIDTH/2+35, offset_pot_vol + hauteur_texte_3 + distance_entre_ligne);
  screen.print(effets.lire_val_pot(effet_actif, Mix));
  screen.print("%");
  screen.drawFastHLine(0, offset_pot_vol + hauteur_texte_2 + hauteur_texte_3 + distance_entre_ligne*2, WIDTH, couleur_ligne);

  // Print le bloc pot_ctrl
  screen.setTextSize(3);
  screen.setCursor(offset_x, offset_pot_ctrl);
  screen.print(effets.get_nom_ctrl1(effet_actif));
  screen.setTextSize(2);
  screen.setCursor(offset_x, offset_pot_ctrl+hauteur_texte_3+distance_entre_ligne);
  screen.print(effets.lire_val_pot(effet_actif, Ctrl1));
  screen.print("%");

  screen.setTextSize(3);
  screen.setCursor(offset_x, offset_pot_ctrl+hauteur_texte_2+hauteur_texte_3+distance_entre_ligne*2);
  screen.print(effets.get_nom_ctrl2(effet_actif));
  screen.setTextSize(2);
  screen.setCursor(offset_x, offset_pot_ctrl+hauteur_texte_2+hauteur_texte_3*2+distance_entre_ligne*3);
  screen.print(effets.lire_val_pot(effet_actif, Ctrl2));
  screen.print("%");

  screen.setTextSize(3);
  screen.setCursor(offset_x, offset_pot_ctrl+hauteur_texte_2*2+hauteur_texte_3*2+distance_entre_ligne*4);
  screen.print(effets.get_nom_ctrl3(effet_actif));
  screen.setTextSize(2);
  screen.setCursor(offset_x, offset_pot_ctrl+hauteur_texte_2*2+hauteur_texte_3*3+distance_entre_ligne*5);
  screen.print(effets.lire_val_pot(effet_actif, Ctrl3));
  screen.print("%");

  screen.drawFastHLine(0, offset_pot_ctrl+hauteur_texte_2*3+hauteur_texte_3*3+distance_entre_ligne*6, WIDTH, couleur_ligne);

  // Print le bloc paramètre
  screen.setTextSize(3);
  screen.setCursor(offset_x, offset_parametre);
  screen.print("Parametres");
}

//----------------------------------------------//
//                    Setup                     //
//----------------------------------------------//

void setup()
{
  init_board();

  // Setup de l'écran
  init_ecran();

  // "Setup" Encodeur
  lastStateCLK = digitalRead(IO_S1_ENC);

  // Setup des potentiomètres
  init_pots();

  digitalPotWrite(IO_CS_POT_VOL, POT_0, effets.lire_val_pot_vol());
  digitalPotWrite(IO_CS_POT_MIX, POT_0, effets.lire_val_pot(effet_actif, Mix));
  digitalPotWrite(IO_CS_POT_A, POT_0, effets.lire_val_pot(effet_actif, Ctrl1));
  digitalPotWrite(IO_CS_POT_A, POT_1, effets.lire_val_pot(effet_actif, Ctrl2));
  digitalPotWrite(IO_CS_POT_B, POT_0, effets.lire_val_pot(effet_actif, Ctrl3));

  // État de départ
  etat_affichage = select_effet;

  // Menu Principal
  menu_princ();
}

//----------------------------------------------//
//                     Loop                     //
//----------------------------------------------//

void loop()
{
  // lit l'état de la pin CLK
  currentStateCLK = digitalRead(IO_S1_ENC);

  switch (etat_affichage)
  {
  case select_effet:
    // Lecture de l'encodeur
    if (currentStateCLK != lastStateCLK && currentStateCLK == 1)
    {
      if (digitalRead(IO_S2_ENC) != currentStateCLK)
      { // CCW
      }
      else
      { // CW
      }
    }

    // Lecture du bouton
    btnState = digitalRead(IO_SW_ENC);
    if (btnState == LOW)
    {
      if (millis() - lastButtonPress > 50)
      {
        // code ici
      }
      lastButtonPress = millis();
    }
    break;

  case change_effet:
    // Lecture de l'encodeur
    if (currentStateCLK != lastStateCLK && currentStateCLK == 1)
    {
      if (digitalRead(IO_S2_ENC) != currentStateCLK)
      { // CCW
      }
      else
      { // CW
      }
    }

    // Lecture du bouton
    btnState = digitalRead(IO_SW_ENC);
    if (btnState == LOW)
    {
      if (millis() - lastButtonPress > 50)
      {
        // code ici
      }
      lastButtonPress = millis();
    }
    break;

  case select_vol:
    // Lecture de l'encodeur
    if (currentStateCLK != lastStateCLK && currentStateCLK == 1)
    {
      if (digitalRead(IO_S2_ENC) != currentStateCLK)
      { // CCW
      }
      else
      { // CW
      }
    }

    // Lecture du bouton
    btnState = digitalRead(IO_SW_ENC);
    if (btnState == LOW)
    {
      if (millis() - lastButtonPress > 50)
      {
        // code ici
      }
      lastButtonPress = millis();
    }
    break;

  case change_vol:
    // Lecture de l'encodeur
    if (currentStateCLK != lastStateCLK && currentStateCLK == 1)
    {
      if (digitalRead(IO_S2_ENC) != currentStateCLK)
      { // CCW
      }
      else
      { // CW
      }
    }

    // Lecture du bouton
    btnState = digitalRead(IO_SW_ENC);
    if (btnState == LOW)
    {
      if (millis() - lastButtonPress > 50)
      {
        // code ici
      }
      lastButtonPress = millis();
    }
    break;

  case select_mix:
    // Lecture de l'encodeur
    if (currentStateCLK != lastStateCLK && currentStateCLK == 1)
    {
      if (digitalRead(IO_S2_ENC) != currentStateCLK)
      { // CCW
      }
      else
      { // CW
      }
    }

    // Lecture du bouton
    btnState = digitalRead(IO_SW_ENC);
    if (btnState == LOW)
    {
      if (millis() - lastButtonPress > 50)
      {
        // code ici
      }
      lastButtonPress = millis();
    }
    break;

  case change_mix:
    // Lecture de l'encodeur
    if (currentStateCLK != lastStateCLK && currentStateCLK == 1)
    {
      if (digitalRead(IO_S2_ENC) != currentStateCLK)
      { // CCW
      }
      else
      { // CW
      }
    }

    // Lecture du bouton
    btnState = digitalRead(IO_SW_ENC);
    if (btnState == LOW)
    {
      if (millis() - lastButtonPress > 50)
      {
        // code ici
      }
      lastButtonPress = millis();
    }
    break;

  case select_ctrl1:
    // Lecture de l'encodeur
    if (currentStateCLK != lastStateCLK && currentStateCLK == 1)
    {
      if (digitalRead(IO_S2_ENC) != currentStateCLK)
      { // CCW
      }
      else
      { // CW
      }
    }

    // Lecture du bouton
    btnState = digitalRead(IO_SW_ENC);
    if (btnState == LOW)
    {
      if (millis() - lastButtonPress > 50)
      {
        // code ici
      }
      lastButtonPress = millis();
    }
    break;

  case change_ctrl1:
    // Lecture de l'encodeur
    if (currentStateCLK != lastStateCLK && currentStateCLK == 1)
    {
      if (digitalRead(IO_S2_ENC) != currentStateCLK)
      { // CCW
      }
      else
      { // CW
      }
    }

    // Lecture du bouton
    btnState = digitalRead(IO_SW_ENC);
    if (btnState == LOW)
    {
      if (millis() - lastButtonPress > 50)
      {
        // code ici
      }
      lastButtonPress = millis();
    }
    break;

  case select_ctrl2:
    // Lecture de l'encodeur
    if (currentStateCLK != lastStateCLK && currentStateCLK == 1)
    {
      if (digitalRead(IO_S2_ENC) != currentStateCLK)
      { // CCW
      }
      else
      { // CW
      }
    }

    // Lecture du bouton
    btnState = digitalRead(IO_SW_ENC);
    if (btnState == LOW)
    {
      if (millis() - lastButtonPress > 50)
      {
        // code ici
      }
      lastButtonPress = millis();
    }
    break;

  case change_ctrl2:
    // Lecture de l'encodeur
    if (currentStateCLK != lastStateCLK && currentStateCLK == 1)
    {
      if (digitalRead(IO_S2_ENC) != currentStateCLK)
      { // CCW
      }
      else
      { // CW
      }
    }

    // Lecture du bouton
    btnState = digitalRead(IO_SW_ENC);
    if (btnState == LOW)
    {
      if (millis() - lastButtonPress > 50)
      {
        // code ici
      }
      lastButtonPress = millis();
    }
    break;

  case select_ctrl3:
    // Lecture de l'encodeur
    if (currentStateCLK != lastStateCLK && currentStateCLK == 1)
    {
      if (digitalRead(IO_S2_ENC) != currentStateCLK)
      { // CCW
      }
      else
      { // CW
      }
    }

    // Lecture du bouton
    btnState = digitalRead(IO_SW_ENC);
    if (btnState == LOW)
    {
      if (millis() - lastButtonPress > 50)
      {
        // code ici
      }
      lastButtonPress = millis();
    }
    break;

  case change_ctrl3:
    // Lecture de l'encodeur
    if (currentStateCLK != lastStateCLK && currentStateCLK == 1)
    {
      if (digitalRead(IO_S2_ENC) != currentStateCLK)
      { // CCW
      }
      else
      { // CW
      }
    }

    // Lecture du bouton
    btnState = digitalRead(IO_SW_ENC);
    if (btnState == LOW)
    {
      if (millis() - lastButtonPress > 50)
      {
        // code ici
      }
      lastButtonPress = millis();
    }
    break;
  }

  lastStateCLK = currentStateCLK;

  delay(1);
}
