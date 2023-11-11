#include "WIFI_Driver.h"
#include "WS2812BMatrix_Driver.h"
#include "WIFI_Func.h"
WiFiUDP ntpUDP;
WIFIDRIVER::wifi_driver* wifi_manager = new WIFIDRIVER::wifi_driver() ;
MATRIXDISPLAY::display_driver* display_manager = new MATRIXDISPLAY::display_driver();
WIFI_FUNC::WIFI_function* wifi_func = new WIFI_FUNC::WIFI_function();
void setup() {
  Serial.begin(115200);
  wifi_manager->initialize();
  display_manager->initialize();
  wifi_func->initialize();
}

void loop() {  
String currentTime = wifi_func->getTime();
int hours = wifi_func->getTimehours();
int minutes = wifi_func->getTimeminutes();
display_manager->showTimeOnMatrix(hours , minutes);
delay(1000);
}
