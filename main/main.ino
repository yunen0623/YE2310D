#include <FastLED.h>
#include "WIFI_Driver.h"
#define LED_PIN1 12
#define LED_PIN2 13
#define NUM_LEDS 64
CRGB leds[NUM_LEDS];
WIFIDRIVER::wifi_driver wifi_manager;

void setup() {
  Serial.begin(115200);
  wifi_manager.initializeAPWeb();
  // put your setup code here, to run once:
  // FastLED.addLeds<WS2812, LED_PIN1, GRB>(leds, NUM_LEDS);
  // FastLED.addLeds<WS2812, LED_PIN2, GRB>(leds, NUM_LEDS);
}

void loop() {
  wifi_manager.start();
  // wifi_manager.scanWifi();
  // for(int hue = 0; hue < 255; hue += 1) {
  //   for(int i = 0; i < NUM_LEDS; i++) {
  //     leds[i] = CHSV(hue, 128, 255);
  //   }
  //   // FastLED.show();
  //   delay(10);
  // }
}
