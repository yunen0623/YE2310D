#include <Arduino.h>
#include <FastLED.h>
#define LED_PIN1 12
#define LED_PIN2 13
#define NUM_LEDS 64


namespace MATRIXDISPLAY
{
  class display_driver
  {
      public:
        display_driver();
        ~display_driver();
        void initialize();
        void allMatrixDisplaControl(bool isOn);
        void setOut(int digitalpin , int hue , bool isON);
        void clearMatrix();
        void showTimeOnMatrix(int hours, int minutes);
        
      private:
        CRGB leds_1[NUM_LEDS];
        CRGB leds_2[NUM_LEDS];    
        void bitSetDisplayForNumber(int number, int pos , CRGB  leds[] , int hue);
        byte numbers[10][8] = {
          {B11100000, B10100000, B10100000, B10100000, B10100000, B10100000, B10100000, B11100000}, // 0
          {B00100000, B01100000, B00100000, B00100000, B00100000, B00100000, B00100000, B00110000}, // 1
          {B11100000, B00100000, B00100000, B11100000, B10000000, B10000000, B10000000, B11100000}, // 2
          {B11100000, B00100000, B00100000, B11100000, B00100000, B00100000, B00100000, B11100000}, // 3
          {B10100000, B10100000, B10100000, B11100000, B00100000, B00100000, B00100000, B00100000}, // 4
          {B11100000, B10000000, B10000000, B11100000, B00100000, B00100000, B00100000, B11100000}, // 5
          {B11100000, B10000000, B10000000, B11100000, B10100000, B10100000, B10100000, B11100000}, // 6
          {B11100000, B00100000, B00100000, B00100000, B00100000, B00100000, B00100000, B00100000}, // 7
          {B11100000, B10100000, B10100000, B11100000, B10100000, B10100000, B10100000, B11100000}, // 8
          {B11100000, B10100000, B10100000, B11100000, B00100000, B00100000, B00100000, B11100000}  // 9
      };

      const byte alphabet[26][8] = {
      { B01110,
        B10001,
        B10001,
        B10001,
        B10001,
        B10001,
        B10001,
        B01110 }, // A
    
      { B11110,
        B10001,
        B10001,
        B11110,
        B10001,
        B10001,
        B10001,
        B11110 }, // B
    
      { B01110,
        B10001,
        B10000,
        B10000,
        B10000,
        B10001,
        B10001,
        B01110 }, // C
    
      { B11100,
        B10010,
        B10001,
        B10001,
        B10001,
        B10010,
        B10010,
        B11100 }, // D
    
      { B11111,
        B10000,
        B10000,
        B11110,
        B10000,
        B10000,
        B10000,
        B11111 }, // E
    
      { B11111,
        B10000,
        B10000,
        B11110,
        B10000,
        B10000,
        B10000,
        B10000 }, // F
    
      { B01110,
        B10001,
        B10000,
        B10011,
        B10001,
        B10001,
        B10001,
        B01110 }, // G
    
      { B10001,
        B10001,
        B10001,
        B11111,
        B10001,
        B10001,
        B10001,
        B10001 }, // H
    
      { B01110,
        B00100,
        B00100,
        B00100,
        B00100,
        B00100,
        B00100,
        B01110 }, // I
    
      { B00111,
        B00010,
        B00010,
        B00010,
        B10010,
        B10010,
        B10010,
        B01100 }, // J
    
      { B10001,
        B10010,
        B10100,
        B11000,
        B10100,
        B10010,
        B10001,
        B10001 }, // K
    
      { B10000,
        B10000,
        B10000,
        B10000,
        B10000,
        B10000,
        B10000,
        B11111 }, // L
    
      { B10001,
        B11011,
        B10101,
        B10001,
        B10001,
        B10001,
        B10001,
        B10001 }, // M
    
      { B10001,
        B11001,
        B10101,
        B10011,
        B10001,
        B10001,
        B10001,
        B10001 }, // N
    
      { B01110,
        B10001,
        B10001,
        B10001,
        B10001,
        B10001,
        B10001,
        B01110 }, // O
    
      { B11110,
        B10001,
        B10001,
        B11110,
        B10000,
        B10000,
        B10000,
        B10000 }, // P
    
      { B01110,
        B10001,
        B10001,
        B10001,
        B10001,
        B10001,
        B10010,
        B01101 }, // Q
    
      { B11110,
        B10001,
        B10001,
        B11110,
        B10010,
        B10001,
        B10001,
        B10001 }, // R
    
      { B01111,
        B10000,
        B01110,
        B00001,
        B11110,
        B10000,
        B10000,
        B01110 }, // S
    
      { B11111,
        B00100,
        B00100,
        B00100,
        B00100,
        B00100,
        B00100,
        B00100 }, // T
    
      { B10001,
        B10001,
        B10001,
        B10001,
        B10001,
        B10001,
        B10001,
        B01110 }, // U
    
      { B10001,
        B10001,
        B10001,
        B01010,
        B00100,
        B01010,
        B10001,
        B10001 }, // V
    
      { B10001,
        B10001,
        B10001,
        B10101,
        B11011,
        B10001,
        B10001,
        B10001 }, // W
    
      { B10001,
        B01010,
        B00100,
        B00100,
        B00100,
        B00100,
        B01010,
        B10001 }, // X
    
      { B10001,
        B01010,
        B00100,
        B00100,
        B00100,
        B00100,
        B00100,
        B00100 }, // Y
    
      { B11111,
        B00010,
        B00100,
        B01000,
        B10000,
        B10001,
        B10001,
        B11111 }  // Z
    };


  };
}
