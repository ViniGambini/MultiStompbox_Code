/*!
 * @file bitmap.ino
 * @brief Draw monochrome, colorful, and grayscale images based on the image array generated by the bitmap software. 
 * @n The demo requires to be run on a main-controller with 40000 bytes of dynamic memory, like FireBeetle-ESP8266 and FireBeetle-ESP32 since there are 380,000 bytes in this project.
 * @n This demo supports mainboard FireBeetle-ESP32, FireBeetle-ESP8266, and FireBeetle-M0.
 * @copyright Copyright (c) 2010 DFRobot Co. Ltd (http://www.dfrobot.com)
 * @license The MIT License (MIT)
 * @author [fary] (feng.yang@dfrobot.com)
 * @version V1.0
 * @date 2021-11-04
 * @url https://github.com/DFRobot/DFRobot_GDL
 */
#include "DFRobot_GDL.h"
#include "Bitmap.h"
#include "XBitmap.h"
#include "RGBBitmap.h"
//Custom communication pins
/*M0*/
#if defined ARDUINO_SAM_ZERO
#define TFT_DC  7
#define TFT_CS  5
#define TFT_RST 6
/*ESP32 ESP8266*/
#elif defined(ESP32)
#define TFT_DC  D2
#define TFT_CS  D6
#define TFT_RST D3
/*ESP8266*/
#elif defined(ESP8266)
#define TFT_DC  D4
#define TFT_CS  D6
#define TFT_RST D5
/* AVR series mainboard */
#else
#define TFT_DC  2
#define TFT_CS  3
#define TFT_RST 4
#endif

/**
 * @brief Constructor Constructor of hardware SPI communication
 * @param dc Command/data line pin for SPI communication
 * @param cs Chip select pin for SPI communication
 * @param rst reset pin of the screen
 */
//DFRobot_ST7735_128x160_HW_SPI screen(/*dc=*/TFT_DC,/*cs=*/TFT_CS,/*rst=*/TFT_RST);
/* M0 mainboard DMA transfer */
//DFRobot_ST7735_128x160_DMA_SPI screen(/*dc=*/TFT_DC,/*cs=*/TFT_CS,/*rst=*/TFT_RST);


void setup() {
  Serial.begin(115200);
  screen.begin();
}

void loop() {
  /**
   * @brief Paint the screen white
   * @param 0xFFFF white parameter
   */
  screen.fillScreen(COLOR_RGB565_WHITE );
  /**
   * @brief Draw monochrome pictures with different drawing orders
   * @param x 0 Set the starting point to be at the upper left of the screen, near the left side
   * @param y 0 Near the upper side 
   * @param bitmap gImage_XBitmap The array in the header file XBitmap, the array elements are single bytes
   * @param w 240 Picture width
   * @param h 240 Picture height
   * @param color 0x0000 The black picture part is set to black
   */
  screen.drawXBitmap(/*x=*/(screen.width()-146)/2,/*y=*/(screen.height()-128)/2,/*bitmap gImage_Bitmap=*/gImage_XBitmap,/*w=*/146,/*h=*/128,/*color=*/0x0000);
  screen.fillScreen(COLOR_RGB565_WHITE);
  /**
   * @brief Draw colorful picture
   * @param x 0 Set the starting point to be at the upper left of the screen, near the left side
   * @param y 0 Near the upper side
   * @param bitmap gImage_RGBBitmap The array of RGBBitmap header files, the array elements are single bytes, here forced to convert to 2 bytes
   * @param w 240 picture width
   * @param h 240 Picture height
   */
  screen.drawRGBBitmap(/*x=*/(screen.width()-146)/2,/*y=*/(screen.height()-128)/2,/*bitmap gImage_Bitmap=*/(const unsigned uint16_t*)gImage_RGBBitmap,/*w=*/146,/*h=*/128);
  screen.fillScreen(COLOR_RGB565_BLACK);
  /**
   * @brief Draw a monochrome picture without background circularly, and the color is changing constantly.
   * @param x 0 Set the starting point to be at the upper left of the screen, near the left side
   * @param y 0 Near the upper side
   * @param bitmap gImage_Bitmap The array in the header file Bitmap, the array elements are single bytes
   * @param w 240 Picture width
   * @param h 240 Picture height
   * @param color i Set the black image part to bluish color
   */
  for (int16_t i = 0x00ff; ; i+=0x3300) {
    screen.drawBitmap(/*x=*/(screen.width()-146)/2,/*y=*/(screen.height()-128)/2,/*bitmap gImage_Bitmap=*/gImage_Bitmap,/*w=*/146,/*h=*/128,/*color=*/i);
  }

}
