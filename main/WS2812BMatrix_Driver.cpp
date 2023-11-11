#include "WS2812BMatrix_Driver.h"

/**
hVal – 色調數值
sVal – 飽和度數值
vVal – 明亮度數值
**/


namespace MATRIXDISPLAY
{
  display_driver::display_driver()
  {
    
  }
  
  display_driver::~display_driver()
  {
    
  }
  
  void display_driver::initialize()
  {
       FastLED.addLeds<WS2812, LED_PIN1, GRB>(leds_1, NUM_LEDS);
       FastLED.addLeds<WS2812, LED_PIN2, GRB>(leds_2, NUM_LEDS);
       Serial.println("Matrix Display Already initialize finish");
  }

  void display_driver::allMatrixDisplaControl(bool isOn)
  {
    if (isOn)
    {
      for (int hue = 0; hue < 255; hue += 1)
      {
        for (int i = 0; i < NUM_LEDS; i++)
        {
          leds_1[i] = CHSV(hue, 128, 255);
          leds_2[i] = CHSV(hue, 128, 255);
        }
        FastLED.show(LED_PIN1);
        FastLED.show(LED_PIN2);
        delay(10);
      }
    }
    else
    {
      for (int i = 0; i < NUM_LEDS; i++)
      {
        leds_1[i] = CHSV(0, 0, 0);
        leds_2[i] = CHSV(0, 0, 0);
      }
      FastLED.show(LED_PIN1);
      FastLED.show(LED_PIN2);
      delay(10);
    }
  }

  void display_driver::setOut(int digitalpin , int hue , bool isON)
  {
//    FastLED.clear();
    // 0 ~ 63 
    if(digitalpin < 64 )
    {
      if(isON)
      {
        leds_1[digitalpin] = CHSV(hue , 128 , 255);  
      }
      else
      {
        leds_1[digitalpin] = CHSV(hue , 0 , 0);  
      }
      FastLED.show(LED_PIN1);
      FastLED.show(LED_PIN2);
    }
    else if(digitalpin >= 64 && digitalpin < 128 ) // 64 ~ 127
    {
      if(isON)
      {
        leds_2[digitalpin-64] = CHSV(hue , 128 , 255);  
      }
      else
      {
        leds_2[digitalpin-64] = CHSV(hue , 0 , 0);  
      }
      FastLED.show(LED_PIN1);
      FastLED.show(LED_PIN2);
    }
  }

  void display_driver::showTimeOnMatrix(int hours, int minutes)
  {
    FastLED.clear();

    bitSetDisplayForNumber((hours / 10), 0, leds_1 , 127); 
    bitSetDisplayForNumber((hours % 10), 4, leds_1 , 127); 
    FastLED.show(LED_PIN1);
    bitSetDisplayForNumber((minutes / 10), 1, leds_2 , 240); 
    bitSetDisplayForNumber((minutes % 10) , 5, leds_2 , 240); 

    
    FastLED.show(LED_PIN2);
  }

  void display_driver::bitSetDisplayForNumber(int number, int pos , CRGB  leds[] , int hue)
  {
    int digitHeight = 8;
    int digitWidth = 3; 
    for (int row = 0; row < digitHeight; row++)
    {
        byte rowData = numbers[number][row]; //選擇數字以及每個橫排
        for (int col = 0; col < digitWidth; col++) // 最多3位
        {
            byte bit = bitRead(rowData, 7 - col); //讀最高位
            int index = row * 8 + pos + col; // 0 * 8 + 0 + 0,1,2
            if (bit == 1)
            {
                leds[index] = CHSV(hue, 128, 120); 
            }
            else
            {
                leds[index] = CHSV(0, 0, 0); 
            }
        }
    }
      
  }
  
  void display_driver::clearMatrix()
  {
    FastLED.clear();
    FastLED.show();
  }
}
