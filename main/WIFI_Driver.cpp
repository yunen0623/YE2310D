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
      WiFi.softAP(ssid_ap, password_ap);


      delay(100);
      server.on("/", HTTP_GET, [this](){
       OnConnect();
      //  handleRoot();
      });

      server.on("/scan" , HTTP_GET, [this](){
        handle_scan();
        // handleScan();
      });

      server.on("/connect" , HTTP_GET , [this](){
        Connect2LocalWiFi();
        // handleConnect();
      });

      server.begin();
      Serial.println("HTTP server started");
      return true;
  }

  void wifi_driver::Connect2LocalWiFi()
  {
      //點擊後顯示password框
      if (server.hasArg("ssid") && isConnectbtnClick == false) 
      {
        user_ssid = server.arg("ssid");
        server.send(200, "text/html", SendHTML(true ,true));
        isConnectbtnClick = true;
      }
      //當點擊提交後
      if (server.hasArg("password") && server.hasArg("ssid") && isConnectbtnClick == true) 
      {
        user_ssid = server.arg("ssid");
        user_password = server.arg("password");
        Serial.println("SSID : " + user_ssid);
        Serial.println("Password : " + user_password);
        WiFi.begin(user_ssid.c_str(), user_password.c_str());
        server.send(200, "text/html", "connect...");
        isConnectbtnClick = false;
      } 
  }

  String wifi_driver::getscanWifistr()
  {
      String wifiList = "";
      int n = WiFi.scanNetworks();
      if (n == 0)
      {
        wifiList += "<li class=\"network-item\">Not NetWork Found</li>\n";
        Serial.println("no networks found");
      }
      else
      {
        Serial.println(" networks found");
        for (int i = 0; i < n; ++i)
        {

            String ssid = WiFi.SSID(i);
            int rssi = WiFi.RSSI(i);
            // wifiList += "<li class=\"network-item\" onclick=\"ClickWifiName(this)\">" + ssid + " (RSSI: " + String(rssi) + ")</li>\n";
            // wifiList += "<li><a class=\"network-item\" href='/connect?ssid=" + WiFi.SSID(i) + "'>" + WiFi.SSID(i) + "</a></li>";
            wifiList += "<li class=\"network-item\"><a href='/connect?ssid=" + ssid + "'>" + ssid + " (RSSI: " + String(rssi) + ")</a></li>\n";

        }
      }
      return wifiList;
  }

  void wifi_driver::handle_scan()
  {
    htmlStrForWifi = getscanWifistr();
    server.send(200, "text/html", SendHTML(true, false));
  }

  void wifi_driver::OnConnect() 
  {
    server.send(200, "text/html", SendHTML(false , false)); 
  }

  String wifi_driver::SendHTML(bool scan_ok  , bool connect_ready)
  {
      String ptr = "<!DOCTYPE html> <html>\n";
      ptr +="<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
      ptr +="<title>YE2310D Config</title>\n";
      ptr +="<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
      ptr +="body{margin-top: 50px;display: flex;flex-direction: column;justify-content: center;align-items: center;background-color: #04364A;} h1 {color: #DAFFFB;margin: 50px auto 30px;} h3 {color: #DAFFFB;margin-bottom: 50px;}\n";
      ptr +=".button {display: block;width: 80px;background-color: #176B87;border: none;color: white;padding: 13px 30px;text-decoration: none;font-size: 18px;margin: 0px auto 35px;cursor: pointer;border-radius: 4px;}\n";
      ptr +=".button-scan {background-color: #176B87;}\n";
      ptr +=".button-scan:active {background-color: #64CCC5;}\n";
      ptr +=".button-connect {background-color: #176B87;}\n";
      ptr +=".button-connect:active {background-color: #64CCC5;}\n";
      ptr +="p {font-size: 14px;color: #888;margin-bottom: 10px;}\n";
      ptr +=".network-list {width: 300px;max-height: 200px;overflow-y: auto;border: none solid #ccc;list-style: none;}\n";
      ptr +=".network-item:nth-child(even) {background-color: #176B87;}\n";
      ptr +=".network-item:nth-child(odd) {background-color: #64CCC5;}\n";
      ptr +=".network-item {padding: 10px;border-bottom: 1px solid #ccc;cursor: pointer;}\n";
      ptr +="input[type='submit'] {background-color: #176B87; color: #DAFFFB; padding: 10px 20px; border: none; border-radius: 4px; cursor: pointer;";
      ptr +="</style>\n";
      ptr +="</head>\n";
      ptr +="<body>\n";
      ptr +="<h1>YE2310D Config</h1>\n";
      ptr +="<h3>Config your WIFI</h3>\n";

      //SCAN 按鈕
      ptr +="<p>SCANWIFI</p><a class=\"button button-scan\" href=\"/scan\">SCAN</a>\n";
      
      //掃描WIFI後顯示
      if(scan_ok)
      {
        ptr +="<p>WIFI LIST</p>\n";
        ptr +="<ul class=\"network-list\">\n";
        ptr += htmlStrForWifi;
        ptr +="</ul>\n";
      }
      
      //點擊後顯示password框
      if (scan_ok && connect_ready) 
      {
        ptr += "<form method='get' action='/connect'>";
        ptr += "<label for='password' style='font-size: 20px; color: #DAFFFB;'>Password:</label>";
        ptr += "<input type='text' name='password' id='password' placeholder='" + user_ssid + " password?' style='font-size: 16px; color: red;'><br>";
        ptr += "<input type='hidden' name='ssid' value='" + user_ssid + "'>";
        ptr += "<br><input type='submit' value='connect' style='margin-top: 10px; font-size: 18px; color: #DAFFFB;'>";
        ptr += "</form>";
      }

      ptr +="</body>\n";
      ptr +="</html>\n";
      return ptr;
  }

  void wifi_driver::start()
  {
      server.handleClient();
  }


}