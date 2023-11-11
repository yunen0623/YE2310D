/* WIFI Manger 
 * only for esp8266 (D1 Mini)
 */

#ifndef WIFIDRIVER_H
#define WIFIDRIVER_H

#include <ESP8266WiFi.h>
#include <Arduino.h>
#include <DNSServer.h>
#include <WiFiManager.h>
#include <PubSubClient.h>
#include <ESP8266WebServer.h>
#include <ArduinoJson.h>
#include <FS.h>

namespace WIFIDRIVER
{
  class wifi_driver
  {
    public:
      wifi_driver();
      ~wifi_driver();
      
      void initialize();
      bool get_connect_statu();
    private:
      //About AP and HTML parameter
      //作為Ap用的 ssid pass
      const char* ssid_ap = "YE2310D";  
      const char* password_ap = "12345678"; 
      //由網頁所設定的ssid pass
      String user_ssid = "";
      String user_password = "";
      //由SPIFFS讀取出來的SSID PASS
      String SPIFFS_ssid = "";
      String SPIFFS_password = "";
      bool isWritetoSPIFFSOK = false;
      bool isAPMode = false;
      bool isConnectSTA_ = false;
      bool click_scan = false;
      bool click_connect = false;
      ESP8266WebServer server;
      int numNetworks = 0 ;
      String htmlStrForWifi = "No Wifi , please Click Scan";
      bool isConnectbtnClick = false;
      //JSON parameter
      File jsonFile;

      void switchMode(bool isAP);
      bool initializeAPWeb();
       void startAPWeb();
      String getscanWifistr();
      void OnConnect();
      void handle_scan();
      void Connect2LocalWiFi();
      String SendHTML(bool scan_ok  , bool connect_ready);
      bool readJsonFromSPIFFS();
      bool writeJsonToSPIFFS();

  };
}
#endif // WIFIDRIVER_H
