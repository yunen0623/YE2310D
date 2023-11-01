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
      const char* ssid_ap = "YE2310D";  
      const char* password_ap = "12345678"; 
      String user_ssid = "";
      String user_password = "";
      bool click_scan = false;
      bool click_connect = false;
      ESP8266WebServer server;
      int numNetworks = 0 ;
      String htmlStrForWifi = "No Wifi , please Click Scan";
      bool isConnectbtnClick = false;

      String getscanWifistr();
      void OnConnect();
      void handle_scan();
      void Connect2LocalWiFi();
      String SendHTML(bool scan_ok  , bool connect_ready);

  };
}