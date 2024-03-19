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

#define increment_pot 5 // nombre de pourcentage par clic de l'encodeur

#define WIDTH screen.width()   // 240
#define HEIGHT screen.height() // 320

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
  select_bloc_nom,
  change_nom,
  select_bloc_vol,
  select_vol,
  change_val_vol,
  select_mix,
  change_val_mix,
  select_bloc_ctrl,
  change_ctrl1,
  change_ctrl2,
  change_ctrl3,
  select_bloc_para

} etat_affichage;

uint8_t btnState;                  // Lecture de l'état du bouton
uint8_t currentStateCLK;           // pour encodeur
uint8_t lastStateCLK;              // pour encodeur
unsigned long lastButtonPress = 0; // pour anti rebond du bouton

// unsigned long lastExecutedMillis = 0; // pour timer l'envoie des données sur le terminal

uint8_t effet_actif = 1;


//----------------------------------------------//
//                    Écran                     //
//----------------------------------------------//

#define couleur_fond COLOR_RGB565_BLACK
#define couleur_normal COLOR_RGB565_GREEN
#define couleur_select COLOR_RGB565_YELLOW
#define couleur_ligne COLOR_RGB565_GREEN

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
  screen.setTextWrap(false);
}

// Affiche le menu principale de base
void menu_princ(void)
{
  // Ajuste les valeurs d'offset des blocs vol et ctrl
  offset_pot_vol = hauteur_texte_2 + offset_nom_effet * 2 + distance_entre_ligne;
  offset_pot_ctrl = offset_pot_vol + hauteur_texte_2 + hauteur_texte_3 + distance_entre_ligne * 3;
  offset_parametre = offset_pot_ctrl + hauteur_texte_2 * 3 + hauteur_texte_3 * 3 + distance_entre_ligne * 7;

  // Print le bloc Nom_effet
  screen.setTextColor(couleur_normal);
  screen.setTextSize(2);
  screen.setCursor(offset_x, offset_nom_effet);
  screen.print(effets.get_nom_effet(effet_actif));
  screen.drawFastHLine(0, hauteur_texte_2 + offset_nom_effet * 2, WIDTH, couleur_ligne);

  // Print le bloc pot_vol
  screen.setTextSize(3);
  screen.setCursor(WIDTH / 2 - 75, offset_pot_vol);
  screen.print("Vol");
  screen.setCursor(WIDTH / 2 + 25, offset_pot_vol);
  screen.print("Mix");
  screen.setTextSize(2);
  screen.setCursor(WIDTH / 2 - 65, offset_pot_vol + hauteur_texte_3 + distance_entre_ligne);
  screen.print(effets.lire_val_pot_vol());
  screen.print("%");
  screen.setCursor(WIDTH / 2 + 35, offset_pot_vol + hauteur_texte_3 + distance_entre_ligne);
  screen.print(effets.lire_val_pot(effet_actif, Mix));
  screen.print("%");
  screen.drawFastHLine(0, offset_pot_vol + hauteur_texte_2 + hauteur_texte_3 + distance_entre_ligne * 2, WIDTH, couleur_ligne);

  // Print le bloc pot_ctrl
  screen.setTextSize(3);
  screen.setCursor(offset_x, offset_pot_ctrl);
  screen.print(effets.get_nom_ctrl1(effet_actif));
  screen.setTextSize(2);
  screen.setCursor(offset_x, offset_pot_ctrl + hauteur_texte_3 + distance_entre_ligne);
  screen.print(effets.lire_val_pot(effet_actif, Ctrl1));
  screen.print("%");

  screen.setTextSize(3);
  screen.setCursor(offset_x, offset_pot_ctrl + hauteur_texte_2 + hauteur_texte_3 + distance_entre_ligne * 2);
  screen.print(effets.get_nom_ctrl2(effet_actif));
  screen.setTextSize(2);
  screen.setCursor(offset_x, offset_pot_ctrl + hauteur_texte_2 + hauteur_texte_3 * 2 + distance_entre_ligne * 3);
  screen.print(effets.lire_val_pot(effet_actif, Ctrl2));
  screen.print("%");

  screen.setTextSize(3);
  screen.setCursor(offset_x, offset_pot_ctrl + hauteur_texte_2 * 2 + hauteur_texte_3 * 2 + distance_entre_ligne * 4);
  screen.print(effets.get_nom_ctrl3(effet_actif));
  screen.setTextSize(2);
  screen.setCursor(offset_x, offset_pot_ctrl + hauteur_texte_2 * 2 + hauteur_texte_3 * 3 + distance_entre_ligne * 5);
  screen.print(effets.lire_val_pot(effet_actif, Ctrl3));
  screen.print("%");

  screen.drawFastHLine(0, offset_pot_ctrl + hauteur_texte_2 * 3 + hauteur_texte_3 * 3 + distance_entre_ligne * 6, WIDTH, couleur_ligne);

  // Print le bloc paramètre
  /*screen.setTextSize(3);
  screen.setCursor(offset_x, offset_parametre);
  screen.print("Parametres");*/
}

// Affiche le menu qui permet de changer les effets
void menu_change_nom(void)
{
  screen.fillRect(offset_x, offset_nom_effet, WIDTH - offset_x * 2, hauteur_texte_2 * 2 + distance_entre_ligne, couleur_fond);
  screen.setTextColor(couleur_normal);
  screen.setTextSize(2);
  screen.setCursor(offset_x, offset_nom_effet);
  screen.print(effets.get_nom_effet(effet_actif));
  screen.setCursor(offset_x, offset_nom_effet + hauteur_texte_2 + distance_entre_ligne);
  screen.print(effet_actif);
}

// Vide l'écran et affiche le contour
void clear_screen(void)
{
  screen.fillScreen(couleur_fond);
  screen.drawRect(0, 0, WIDTH, HEIGHT, couleur_ligne);
}

// Les fonctions suivante permettent de mettre en évidence certains aspects de l'affichage

void highlight_bloc_nom(bool type)
{
  if (type == 1)
  {
    screen.drawRect(0, 0, WIDTH, hauteur_texte_2 + offset_nom_effet * 2 + 1, couleur_select);
  }
  else
  {
    screen.drawRect(0, 0, WIDTH, hauteur_texte_2 + offset_nom_effet * 2 + 1, couleur_ligne);
  }
}

void highlight_bloc_vol(bool type)
{
  if (type == 1)
  {
    screen.drawRect(0, offset_pot_vol - distance_entre_ligne, WIDTH, hauteur_texte_2 + hauteur_texte_3 + distance_entre_ligne * 3 + 1, couleur_select);
  }
  else
  {
    screen.drawRect(0, offset_pot_vol - distance_entre_ligne, WIDTH, hauteur_texte_2 + hauteur_texte_3 + distance_entre_ligne * 3 + 1, couleur_ligne);
  }
}

void highlight_bloc_ctrl(bool type)
{
  if (type == 1)
  {
    screen.drawRect(0, offset_pot_ctrl - distance_entre_ligne, WIDTH, hauteur_texte_2 * 3 + hauteur_texte_3 * 3 + distance_entre_ligne * 7 + 1, couleur_select);
  }
  else
  {
    screen.drawRect(0, offset_pot_ctrl - distance_entre_ligne, WIDTH, hauteur_texte_2 * 3 + hauteur_texte_3 * 3 + distance_entre_ligne * 7 + 1, couleur_ligne);
  }
}

void highlight_bloc_para(bool type)
{
  if (type == 1)
  {
    screen.drawRect(0, offset_parametre - distance_entre_ligne, WIDTH, HEIGHT - offset_parametre + distance_entre_ligne, couleur_select);
  }
  else
  {
    screen.drawRect(0, offset_parametre - distance_entre_ligne, WIDTH, HEIGHT - offset_parametre + distance_entre_ligne, couleur_ligne);
  }
}

void highlight_texte_vol(bool type)
{
  if (type == 1)
  {
    screen.setTextSize(3);
    screen.setTextColor(couleur_select);
    screen.setCursor(WIDTH / 2 - 75, offset_pot_vol);
    screen.print("Vol");
  }
  else
  {
    screen.setTextSize(3);
    screen.setTextColor(couleur_normal);
    screen.setCursor(WIDTH / 2 - 75, offset_pot_vol);
    screen.print("Vol");
  }
}

void highlight_texte_mix(bool type)
{
  if (type == 1)
  {
    screen.setTextSize(3);
    screen.setTextColor(couleur_select);
    screen.setCursor(WIDTH / 2 + 25, offset_pot_vol);
    screen.print("Mix");
  }
  else
  {
    screen.setTextSize(3);
    screen.setTextColor(couleur_normal);
    screen.setCursor(WIDTH / 2 + 25, offset_pot_vol);
    screen.print("Mix");
  }
}

void highlight_pourcent_vol(bool type)
{
  if (type == 1)
  {
    screen.setTextSize(2);
    screen.setTextColor(couleur_select);
    screen.setCursor(WIDTH / 2 - 65, offset_pot_vol + hauteur_texte_3 + distance_entre_ligne);
    screen.print(effets.lire_val_pot_vol());
    screen.print("%");
  }
  else
  {
    screen.setTextSize(2);
    screen.setTextColor(couleur_normal);
    screen.setCursor(WIDTH / 2 - 65, offset_pot_vol + hauteur_texte_3 + distance_entre_ligne);
    screen.print(effets.lire_val_pot_vol());
    screen.print("%");
  }
}

void highlight_pourcent_mix(bool type)
{
  if (type == 1)
  {
    screen.setTextSize(2);
    screen.setTextColor(couleur_select);
    screen.setCursor(WIDTH / 2 + 35, offset_pot_vol + hauteur_texte_3 + distance_entre_ligne);
    screen.print(effets.lire_val_pot(effet_actif, Mix));
    screen.print("%");
  }
  else
  {
    screen.setTextSize(2);
    screen.setTextColor(couleur_normal);
    screen.setCursor(WIDTH / 2 + 35, offset_pot_vol + hauteur_texte_3 + distance_entre_ligne);
    screen.print(effets.lire_val_pot(effet_actif, Mix));
    screen.print("%");
  }
}

void highlight_pourcent_ctrl1(bool type)
{
  if (type == 1)
  {
    screen.setTextSize(2);
    screen.setTextColor(couleur_select);
    screen.setCursor(offset_x, offset_pot_ctrl + hauteur_texte_3 + distance_entre_ligne);
    screen.print(effets.lire_val_pot(effet_actif, Ctrl1));
    screen.print("%");
  }
  else
  {
    screen.setTextSize(2);
    screen.setTextColor(couleur_normal);
    screen.setCursor(offset_x, offset_pot_ctrl + hauteur_texte_3 + distance_entre_ligne);
    screen.print(effets.lire_val_pot(effet_actif, Ctrl1));
    screen.print("%");
  }
}

void highlight_pourcent_ctrl2(bool type)
{
  if (type == 1)
  {
    screen.setTextSize(2);
    screen.setTextColor(couleur_select);
    screen.setCursor(offset_x, offset_pot_ctrl + hauteur_texte_2 + hauteur_texte_3 * 2 + distance_entre_ligne * 3);
    screen.print(effets.lire_val_pot(effet_actif, Ctrl2));
    screen.print("%");
  }
  else
  {
    screen.setTextSize(2);
    screen.setTextColor(couleur_normal);
    screen.setCursor(offset_x, offset_pot_ctrl + hauteur_texte_2 + hauteur_texte_3 * 2 + distance_entre_ligne * 3);
    screen.print(effets.lire_val_pot(effet_actif, Ctrl2));
    screen.print("%");
  }
}

void highlight_pourcent_ctrl3(bool type)
{
  if (type == 1)
  {
    screen.setTextSize(2);
    screen.setTextColor(couleur_select);
    screen.setCursor(offset_x, offset_pot_ctrl + hauteur_texte_2 * 2 + hauteur_texte_3 * 3 + distance_entre_ligne * 5);
    screen.print(effets.lire_val_pot(effet_actif, Ctrl3));
    screen.print("%");
  }
  else
  {
    screen.setTextSize(2);
    screen.setTextColor(couleur_normal);
    screen.setCursor(offset_x, offset_pot_ctrl + hauteur_texte_2 * 2 + hauteur_texte_3 * 3 + distance_entre_ligne * 5);
    screen.print(effets.lire_val_pot(effet_actif, Ctrl3));
    screen.print("%");
  }
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

  //digitalPotWrite(IO_CS_POT_VOL, POT_0, effets.lire_val_pot_vol());
  //digitalPotWrite(IO_CS_POT_MIX, POT_0, effets.lire_val_pot(effet_actif, Mix));
  digitalPotWrite(IO_CS_POT_A, POT_0, effets.lire_val_pot(effet_actif, Ctrl1));
  digitalPotWrite(IO_CS_POT_A, POT_1, effets.lire_val_pot(effet_actif, Ctrl2));
  digitalPotWrite(IO_CS_POT_B, POT_0, effets.lire_val_pot(effet_actif, Ctrl3));

  // État de départ
  etat_affichage = select_bloc_nom;

  // Menu Principal
  clear_screen();
  menu_princ();
  highlight_bloc_nom(1);
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
  case select_bloc_nom:
    if (currentStateCLK != lastStateCLK && currentStateCLK == 1)
    {
      if (digitalRead(IO_S2_ENC) != currentStateCLK)
      { // CCW
      }
      else
      { // CW
        etat_affichage = select_bloc_vol;
        highlight_bloc_nom(0);
        highlight_bloc_vol(1);
      }
    }
    lastStateCLK = currentStateCLK;
    // Lecture du bouton
    btnState = digitalRead(IO_SW_ENC);
    if (btnState == LOW)
    {
      if (millis() - lastButtonPress > 50)
      {
        etat_affichage = change_nom;
        clear_screen();
        menu_change_nom();
      }
      lastButtonPress = millis();
    }
    break;

  case change_nom:
    if (currentStateCLK != lastStateCLK && currentStateCLK == 1)
    {
      if (digitalRead(IO_S2_ENC) != currentStateCLK)
      { // CCW
        effet_actif--;
        if (effet_actif < 1)
        {
          effet_actif = 16;
        }
      }
      else
      { // CW
        effet_actif++;
        if (effet_actif > 16)
        {
          effet_actif = 1;
        }
      }
      digitalWrite(IO_A0_MEM_1, bitRead(effet_actif-1, 3));
      digitalWrite(IO_A0_MEM_2, !digitalRead(IO_A0_MEM_1));
      delay(10);
      digitalWrite(IO_S2_FV1, bitRead(effet_actif-1, 2));
      digitalWrite(IO_S1_FV1, bitRead(effet_actif-1, 1));
      digitalWrite(IO_S0_FV1, bitRead(effet_actif-1, 0));
      menu_change_nom();
    }
    lastStateCLK = currentStateCLK;
    // Lecture du bouton
    btnState = digitalRead(IO_SW_ENC);
    if (btnState == LOW)
    {
      if (millis() - lastButtonPress > 50)
      {
        etat_affichage = select_bloc_nom;
        screen.fillRect(offset_x, offset_nom_effet + hauteur_texte_2 + distance_entre_ligne, 50, hauteur_texte_2, couleur_fond);
        menu_princ();
        highlight_bloc_nom(1);
      }
      lastButtonPress = millis();
    }
    break;

  case select_bloc_vol:
    if (currentStateCLK != lastStateCLK && currentStateCLK == 1)
    {
      if (digitalRead(IO_S2_ENC) != currentStateCLK)
      { // CCW
        etat_affichage = select_bloc_nom;
        highlight_bloc_vol(0);
        highlight_bloc_nom(1);
      }
      else
      { // CW
        etat_affichage = select_bloc_ctrl;
        highlight_bloc_vol(0);
        highlight_bloc_ctrl(1);
      }
    }
    lastStateCLK = currentStateCLK;
    // Lecture du bouton
    btnState = digitalRead(IO_SW_ENC);
    if (btnState == LOW)
    {
      if (millis() - lastButtonPress > 50)
      {
        etat_affichage = select_vol;
        highlight_bloc_vol(0);
        highlight_texte_vol(1);
      }
      lastButtonPress = millis();
    }
    break;

  case select_vol:
    if (currentStateCLK != lastStateCLK && currentStateCLK == 1)
    {
      if (digitalRead(IO_S2_ENC) != currentStateCLK)
      { // CCW
      }
      else
      { // CW
        etat_affichage = select_mix;
        highlight_texte_vol(0);
        highlight_texte_mix(1);
      }
    }
    lastStateCLK = currentStateCLK;
    // Lecture du bouton
    btnState = digitalRead(IO_SW_ENC);
    if (btnState == LOW)
    {
      if (millis() - lastButtonPress > 50)
      {
        etat_affichage = change_val_vol;
        highlight_texte_vol(0);
        highlight_pourcent_vol(1);
      }
      lastButtonPress = millis();
    }
    break;

  case change_val_vol:
    // Lecture de l'encodeur
    if (currentStateCLK != lastStateCLK && currentStateCLK == 1)
    {
      screen.fillRect(WIDTH / 2 - 65, offset_pot_vol + hauteur_texte_3 + distance_entre_ligne, 50, hauteur_texte_2, couleur_fond);
      if (digitalRead(IO_S2_ENC) != currentStateCLK)
      { // CCW
        screen.setCursor(WIDTH / 2 - 65, offset_pot_vol + hauteur_texte_3 + distance_entre_ligne);
        screen.print(effets.reduire_val_pot_vol(increment_pot));
        screen.print("%");
      }
      else
      { // CW
        screen.setCursor(WIDTH / 2 - 65, offset_pot_vol + hauteur_texte_3 + distance_entre_ligne);
        screen.print(effets.augmenter_val_pot_vol(increment_pot));
        screen.print("%");
      }
      digitalPotWrite(IO_CS_POT_VOL, POT_0,effets.lire_val_pot_vol());
    }
    lastStateCLK = currentStateCLK;
    // Lecture du bouton
    btnState = digitalRead(IO_SW_ENC);
    if (btnState == LOW)
    {
      if (millis() - lastButtonPress > 50)
      {
        etat_affichage = select_bloc_vol;
        highlight_pourcent_vol(0);
        highlight_bloc_vol(1);
      }
      lastButtonPress = millis();
    }
    break;

  case select_mix:
    if (currentStateCLK != lastStateCLK && currentStateCLK == 1)
    {
      if (digitalRead(IO_S2_ENC) != currentStateCLK)
      { // CCW
        etat_affichage = select_vol;
        highlight_texte_mix(0);
        highlight_texte_vol(1);
      }
      else
      { // CW
      }
    }
    lastStateCLK = currentStateCLK;
    // Lecture du bouton
    btnState = digitalRead(IO_SW_ENC);
    if (btnState == LOW)
    {
      if (millis() - lastButtonPress > 50)
      {
        etat_affichage = change_val_mix;
        highlight_texte_mix(0);
        highlight_pourcent_mix(1);
      }
      lastButtonPress = millis();
    }
    break;

  case change_val_mix:
    // Lecture de l'encodeur
    if (currentStateCLK != lastStateCLK && currentStateCLK == 1)
    {
      screen.fillRect(WIDTH / 2 + 35, offset_pot_vol + hauteur_texte_3 + distance_entre_ligne, 50, hauteur_texte_2, couleur_fond);
      if (digitalRead(IO_S2_ENC) != currentStateCLK)
      { // CCW
        screen.setCursor(WIDTH / 2 + 35, offset_pot_vol + hauteur_texte_3 + distance_entre_ligne);
        screen.print(effets.reduire_val_pot(effet_actif, Mix, increment_pot));
        screen.print("%");
      }
      else
      { // CW
        screen.setCursor(WIDTH / 2 + 35, offset_pot_vol + hauteur_texte_3 + distance_entre_ligne);
        screen.print(effets.augmenter_val_pot(effet_actif, Mix, increment_pot));
        screen.print("%");
      }
      digitalPotWrite(IO_CS_POT_MIX, POT_0, effets.lire_val_pot(effet_actif, Mix));
    }
    lastStateCLK = currentStateCLK;
    // Lecture du bouton
    btnState = digitalRead(IO_SW_ENC);
    if (btnState == LOW)
    {
      if (millis() - lastButtonPress > 50)
      {
        etat_affichage = select_bloc_vol;
        highlight_pourcent_mix(0);
        highlight_bloc_vol(1);
      }
      lastButtonPress = millis();
    }
    break;

  case select_bloc_ctrl:
    if (currentStateCLK != lastStateCLK && currentStateCLK == 1)
    {
      if (digitalRead(IO_S2_ENC) != currentStateCLK)
      { // CCW
        etat_affichage = select_bloc_vol;
        highlight_bloc_ctrl(0);
        highlight_bloc_vol(1);
      }
      else
      { // CW
        /*etat_affichage = select_bloc_para;
        highlight_bloc_ctrl(0);
        highlight_bloc_para(1);*/
      }
    }
    lastStateCLK = currentStateCLK;
    // Lecture du bouton
    btnState = digitalRead(IO_SW_ENC);
    if (btnState == LOW)
    {
      if (millis() - lastButtonPress > 50)
      {
        etat_affichage = change_ctrl1;
        highlight_bloc_ctrl(0);
        highlight_pourcent_ctrl1(1);
      }
      lastButtonPress = millis();
    }
    break;

  case change_ctrl1:
    if (currentStateCLK != lastStateCLK && currentStateCLK == 1)
    {
      screen.fillRect(offset_x, offset_pot_ctrl + hauteur_texte_3 + distance_entre_ligne, 50, hauteur_texte_2, couleur_fond);
      if (digitalRead(IO_S2_ENC) != currentStateCLK)
      { // CCW
        screen.setCursor(offset_x, offset_pot_ctrl + hauteur_texte_3 + distance_entre_ligne);
        screen.print(effets.reduire_val_pot(effet_actif, Ctrl1, increment_pot));
        screen.print("%");
      }
      else
      { // CW
        screen.setCursor(offset_x, offset_pot_ctrl + hauteur_texte_3 + distance_entre_ligne);
        screen.print(effets.augmenter_val_pot(effet_actif, Ctrl1, increment_pot));
        screen.print("%");
      }
      digitalPotWrite(IO_CS_POT_A, POT_0, effets.lire_val_pot(effet_actif, Ctrl1));
    }
    lastStateCLK = currentStateCLK;
    // Lecture du bouton
    btnState = digitalRead(IO_SW_ENC);
    if (btnState == LOW)
    {
      if (millis() - lastButtonPress > 50)
      {
        etat_affichage = change_ctrl2;
        highlight_pourcent_ctrl1(0);
        highlight_pourcent_ctrl2(1);
      }
      lastButtonPress = millis();
    }
    break;

  case change_ctrl2:
    if (currentStateCLK != lastStateCLK && currentStateCLK == 1)
    {
      screen.fillRect(offset_x, offset_pot_ctrl + hauteur_texte_2 + hauteur_texte_3 * 2 + distance_entre_ligne * 3, 50, hauteur_texte_2, couleur_fond);
      if (digitalRead(IO_S2_ENC) != currentStateCLK)
      { // CCW
        screen.setCursor(offset_x, offset_pot_ctrl + hauteur_texte_2 + hauteur_texte_3 * 2 + distance_entre_ligne * 3);
        screen.print(effets.reduire_val_pot(effet_actif, Ctrl2, increment_pot));
        screen.print("%");
      }
      else
      { // CW
        screen.setCursor(offset_x, offset_pot_ctrl + hauteur_texte_2 + hauteur_texte_3 * 2 + distance_entre_ligne * 3);
        screen.print(effets.augmenter_val_pot(effet_actif, Ctrl2, increment_pot));
        screen.print("%");
      }
      digitalPotWrite(IO_CS_POT_A, POT_1, effets.lire_val_pot(effet_actif, Ctrl2));
    }
    lastStateCLK = currentStateCLK;
    // Lecture du bouton
    btnState = digitalRead(IO_SW_ENC);
    if (btnState == LOW)
    {
      if (millis() - lastButtonPress > 50)
      {
        etat_affichage = change_ctrl3;
        highlight_pourcent_ctrl2(0);
        highlight_pourcent_ctrl3(1);
      }
      lastButtonPress = millis();
    }
    break;  

  case change_ctrl3:
    if (currentStateCLK != lastStateCLK && currentStateCLK == 1)
    {
      screen.fillRect(offset_x,  offset_pot_ctrl + hauteur_texte_2 * 2 + hauteur_texte_3 * 3 + distance_entre_ligne * 5, 50, hauteur_texte_2, couleur_fond);
      if (digitalRead(IO_S2_ENC) != currentStateCLK)
      { // CCW
        screen.setCursor(offset_x, offset_pot_ctrl + hauteur_texte_2 * 2 + hauteur_texte_3 * 3 + distance_entre_ligne * 5);
        screen.print(effets.reduire_val_pot(effet_actif, Ctrl3, increment_pot));
        screen.print("%");
      }
      else
      { // CW
        screen.setCursor(offset_x, offset_pot_ctrl + hauteur_texte_2 * 2 + hauteur_texte_3 * 3 + distance_entre_ligne * 5);
        screen.print(effets.augmenter_val_pot(effet_actif, Ctrl3, increment_pot));
        screen.print("%");
      }
      digitalPotWrite(IO_CS_POT_B, POT_0, effets.lire_val_pot(effet_actif, Ctrl3));
    }
    lastStateCLK = currentStateCLK;
    // Lecture du bouton
    btnState = digitalRead(IO_SW_ENC);
    if (btnState == LOW)
    {
      if (millis() - lastButtonPress > 50)
      {
        etat_affichage = select_bloc_ctrl;
        highlight_pourcent_ctrl3(0);
        highlight_bloc_ctrl(1);
      }
      lastButtonPress = millis();
    }
    break;

  case select_bloc_para:
    if (currentStateCLK != lastStateCLK && currentStateCLK == 1)
    {
      if (digitalRead(IO_S2_ENC) != currentStateCLK)
      { // CCW
        etat_affichage = select_bloc_ctrl;
        highlight_bloc_para(0);
        highlight_bloc_ctrl(1);
      }
      else
      { // CW
      }
    }
    lastStateCLK = currentStateCLK;
    break;
  }
  delay(1);
}
