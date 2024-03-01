// IO_Pin.h
// Par Vincent G.
// 2024-03-01
// Version 1

// SPI
// SCK  pin 9 - PB1
// MOSI pin 10 - PB2
// MISO pin 11 - PB3

// DELS
// LED2 pin 8 - PB0
// LED3 pin 22 - PD5

// Écran DFR0664
#define IO_CS_ECRAN 6   // pin 27 - PD7
#define IO_RST_ECRAN 12 // pin 26 - PD6
#define IO_DC_ECRAN 4   // pin 25 - PD4

// Potentiomètres
#define IO_CS_POT_VOL 13 // pin 32 - PC7
#define IO_CS_POT_MIX 5  // pin 31 - PC6
#define IO_CS_POT_A 19   // pin 37 - PF6
#define IO_CS_POT_B 18   // pin 36 - PF7

// Test Point
#define IO_TP10 20 // pin 38 - PF5

// FV-1
#define IO_S0_FV1 23 // pin 41 - PF0
#define IO_S1_FV1 22 // pin 40 - PF1
#define IO_S2_FV1 21 // pin 39 - PF4

// Mémoires I2C
#define IO_A0_MEM_1 8 // pin 28 - PB4
#define IO_A0_MEM_2 9 // pin 29 - PB5

// Encodeur
#define IO_S1_ENC 1 // pin 21 - PD3
#define IO_S2_ENC 0 // pin 20 - PD2
#define IO_SW_ENC 2 // pin 19 - PD1
