#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266Wifi.h>
#include <DNSServer.h>
#include <WiFiManager.h>
#include <PubSubClient.h>
#include <ESP8266WebServer.h>
namespace WIFIDRIVER
{
  class wifi_driver
  {
    public:
      wifi_driver();
      ~wifi_driver();
      void switchMode();
      bool initializeAPWeb();
      void start();
      

    private:
      const char* ssid = "YE2310D";  
      const char* password = "12345678"; 
      bool click_scan = false;
      bool click_connect = false;
      ESP8266WebServer server;
      int numNetworks = 0 ;
      void scanWifi();
      void OnConnect();
      void NotFound();
      void Connect2LocalWiFi();
      String SendHTML();
  };
}