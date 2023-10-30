#include "WIFI_Driver.h"

namespace WIFIDRIVER
{
  wifi_driver::wifi_driver():server{80}
  {

  }

  wifi_driver::~wifi_driver()
  {
    
  }

  void wifi_driver::switchMode()
  {

  }

  bool wifi_driver::initializeAPWeb()
  {
      WiFi.mode(WIFI_AP);
      WiFi.softAP(ssid, password);


      delay(100);
      server.on("/", [this](){
       OnConnect();
      });

      server.on("/scan" , [this](){
        scanWifi();
      });

      server.on("/connect" , [this](){
        Connect2LocalWiFi();
      });

      // server.onNotFound([](this){
      //   NotFound();
      // });
      server.begin();
      Serial.println("HTTP server started");


      return true;
  }

  void wifi_driver::Connect2LocalWiFi()
  {

  }

  void wifi_driver::scanWifi()
  {
      server.send(200, "text/html", "SCAN 按钮被点击！");
      Serial.println("");
      Serial.println("scan start");
      WiFi.disconnect();
      delay(100);
      // WiFi.scanNetworks will return the number of networks found
      int n = WiFi.scanNetworks();
      if (n == 0)
        Serial.println("no networks found");
      else
      {
        Serial.print(n);
        Serial.println(" networks found");
        for (int i = 0; i < n; ++i)
        {
          // Print SSID and RSSI for each network found
          Serial.print(i + 1);
          Serial.print(": ");
          Serial.print(WiFi.SSID(i));
          Serial.print(" (");
          Serial.print(WiFi.RSSI(i));
          Serial.print(") ");
          Serial.print("\n");
          delay(10);
        }
      }
      Serial.println("scan done");
      Serial.println("");
  }


  void wifi_driver::OnConnect() 
  {
    server.send(200, "text/html", SendHTML()); 
  }

  String wifi_driver::SendHTML()
  {
      String ptr = "<!DOCTYPE html> <html>\n";
      ptr +="<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
      ptr +="<title>YE2310D Config</title>\n";
      ptr +="<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
      ptr +="body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;} h3 {color: #444444;margin-bottom: 50px;}\n";
      ptr +=".button {display: block;width: 80px;background-color: #1abc9c;border: none;color: white;padding: 13px 30px;text-decoration: none;font-size: 18px;margin: 0px auto 35px;cursor: pointer;border-radius: 4px;}\n";
      ptr +=".button-scan {background-color: #1abc9c;}\n";
      // ptr +=".button-scan:active {background-color: #16a085;}\n";
      ptr +=".button-connect {background-color: #34495e;}\n";
      ptr +=".button-connect:active {background-color: #2c3e50;}\n";
      ptr +="p {font-size: 14px;color: #888;margin-bottom: 10px;}\n";
      ptr +="</style>\n";
      ptr +="</head>\n";
      ptr +="<body>\n";
      ptr +="<h1>YE2310D Config</h1>\n";
      ptr +="<h3>Config your WIFI</h3>\n";

      ptr +="<p>SCANWIFI</p><a class=\"button button-scan\" href=\"/scan\">SCAN</a>\n";
      ptr +="<p>CONNECT</p><a class=\"button button-connect\" href=\"/connect\">CONNECT</a>\n";

      ptr +="</body>\n";
      ptr +="</html>\n";
      return ptr;
  }

  void wifi_driver::NotFound()
  {
      server.send(404, "text/plain", "Not found");
  }

  void wifi_driver::start()
  {
      server.handleClient();
  }

}