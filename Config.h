#ifndef _CONFIG_H_
#define _CONFIG_H_

#include <Arduino.h>
#include "Adafruit_NeoPixel.h"

#define SerialDebug

//MATRICE
#define PIN_STRIPLED (7) //pwm LED Control

#define SIZE_ALLSTRIP_X (30)
#define SIZE_ALLSTRIP_Y (30)
//#define OPTIM_ADAFRUIT_LIB 

//Mannette
#define PIN_LATCH (2)
#define PIN_CLOCK (3)
#define PIN_DATAMANETTE_1 (4)
#define PIN_DATAMANETTE_2 (5)

//SD
#define PIN_SDCARD (10)

//Color 
static const uint32_t C_OFF     = Adafruit_NeoPixel::Color(0,0,0);        // noir
static const uint32_t C_WHITE   = Adafruit_NeoPixel::Color(255,255,255);  // Blanc

static const uint32_t C_RED     = Adafruit_NeoPixel::Color(255,0,0);      // Rouge
static const uint32_t C_GREEN   = Adafruit_NeoPixel::Color(0,255,0);      // Vert
static const uint32_t C_BLUE    = Adafruit_NeoPixel::Color(0,0,255);      // Bleu

static const uint32_t C_YELLOW  = Adafruit_NeoPixel::Color(255,2555,0);   // Jaune
static const uint32_t C_CYAN    = Adafruit_NeoPixel::Color(0,255,255);    // Cyan
static const uint32_t C_MAGENTA = Adafruit_NeoPixel::Color(255,0,255);    // Magenta

static const uint32_t C_SILVER  = Adafruit_NeoPixel::Color(192,192,192);      
static const uint32_t C_GRAY    = Adafruit_NeoPixel::Color(128,128,128);      
static const uint32_t C_MAROON  = Adafruit_NeoPixel::Color(128,0,0);      
static const uint32_t C_OLIVE  = Adafruit_NeoPixel::Color(128,128,0);      
static const uint32_t C_GREEN_  = Adafruit_NeoPixel::Color(0,128,0);      
static const uint32_t C_PURPLE  = Adafruit_NeoPixel::Color(128,0,128);      
static const uint32_t C_REAL  = Adafruit_NeoPixel::Color(0,128,128);      
static const uint32_t C_NAVY  = Adafruit_NeoPixel::Color(0,0,128);     
 
static const uint32_t C_ORANGE_red = Adafruit_NeoPixel::Color(255,69,0);  
static const uint32_t C_ORANGE_dark = Adafruit_NeoPixel::Color(255,140,0);
static const uint32_t C_ORANGE = Adafruit_NeoPixel::Color(255,165,0);      
static const uint32_t C_GOLD  = Adafruit_NeoPixel::Color(255,215,0);      
static const uint32_t C_TURQUOISE = Adafruit_NeoPixel::Color(64,224,208);   
static const uint32_t C_BLEU_steel = Adafruit_NeoPixel::Color(70,130,180);   
    

static const uint32_t C_GREEN_L = Adafruit_NeoPixel::Color(0,100,0);      // Vert Light
static const uint32_t C_BRIQUE  = Adafruit_NeoPixel::Color(132,46,27);    // Brique


//structure commune
struct Vect2D
{  int8_t X, Y;  };

struct Vect2D_f
{  float X, Y;   };

struct ColorRGB
{  unsigned char R, G, B;};

struct Pixel
{
    Vect2D Possition;
    ColorRGB Color;
};

struct StructManettes
{
    uint16_t DataManette1;
    uint16_t DataManette2;
};

enum GameType
{
    ENUM_GAME_NONE = -1,
    ENUM_GAME_CASSEBRIQUE = 0,
    ENUM_GAME_PONG = 1,
    ENUM_GAME_TETRIX = 2,
    ENUM_GAME_TIME = 3,
    ENUM_GAME_TRON = 4,

    ENUM_GAME_SIZE
};

//text 
#include "DisplayManager.h"
#include "Text.h"
const  Text g_Texts[3]= {Text("YOU"),Text("WIN"),Text("LOSE")}; //TODO a remplire



//OLD
ColorRGB Wheel(byte WheelPos) //recupe de sample neopixel
{
    if(WheelPos < 85) 
    {
        return ColorRGB {(unsigned char)(WheelPos * 3), (unsigned char)(255 - WheelPos * 3), (unsigned char)0};
    } 
    else if(WheelPos < 170) 
    {
       WheelPos -= 85;
       return ColorRGB{(unsigned char)(255 - WheelPos * 3), (unsigned char)0, (unsigned char)(WheelPos * 3)};
    } 
    else 
    {
       WheelPos -= 170;
       return ColorRGB{(unsigned char)0, (unsigned char)(WheelPos * 3), (unsigned char)(255 - WheelPos * 3)};
    }
    return ColorRGB{0,0,0};
}

/* // uniquement pour la mega
int freeRam ()
{
  extern int __heap_start, *__brkval;
  int v;
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
}

void Print_freeRam()
{
  Serial.print("octets restant en variable locales : ");
  Serial.println(freeRam());
}
*/
#endif
