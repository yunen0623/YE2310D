/*
 * IS for ESP8266 WIFI Function
 * openweather fadeb6864879b82d2a7b2ce0717b565f
 */

#ifndef WIFI_FUNC_H
#define WIFI_FUNC_H
#include <stdio.h>
#include <ArduinoJson.h>
#include <Arduino.h>
#include <TimeLib.h>
#include <WiFiUdp.h>
#include <NTPClient.h>
#include <ESP8266HTTPClient.h>

#define SUN  0
#define SUN_CLOUD  1
#define CLOUD 2
#define RAIN 3
#define THUNDER 4

namespace WIFI_FUNC
{
  class WIFI_function
  {
     public:
        WIFI_function();
        ~WIFI_function();
    
        void initialize();
        
        String getTime();
        int getTimehours();
        int getTimeminutes();
        int getTimeseconds();
        
     private:
        void updateTime();
        WiFiUDP udp;
        NTPClient timeClient;
        String Timestr = "";
        int hours , minutes , seconds = 0;
  };
}
#endif
