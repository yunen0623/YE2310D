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
        handle_scan();
      });

      server.on("/connect" , [this](){
        Connect2LocalWiFi();
      });

      server.begin();
      Serial.println("HTTP server started");
      return true;
  }

  void wifi_driver::Connect2LocalWiFi()
  {
    if (server.hasArg("ssid") && server.hasArg("password")) 
    {
        String ssid_ = server.arg("ssid");
        String password_ = server.arg("password");
        
        user_ssid = ssid_;
        user_password = password_;

        Serial.print("User ssid: ");
        Serial.println(user_ssid);
        Serial.print("User password: ");
        Serial.println(user_password);

        String page = "Configuration saved. You can close this page now.";
        server.send(200, "text/html", page);
    }
  }

  String wifi_driver::getscanWifistr()
  {
      String wifiList;
      int n = WiFi.scanNetworks();
      if (n == 0)
      {
        wifiList += "<li class=\"network-item\">Not NetWork Found</li>\n";
        Serial.println("no networks found");
      }
      else
      {
        Serial.print(n);
        Serial.println(" networks found");
        for (int i = 0; i < n; ++i)
        {
           
            String ssid = WiFi.SSID(i);
            int rssi = WiFi.RSSI(i);
            wifiList += "<li class=\"network-item\" onclick=\"ClickWifiName(this)\">" + ssid + " (RSSI: " + String(rssi) + ")</li>\n";
        }
      }
      Serial.println("scan done");
      Serial.println("");
      return wifiList;
      
  }

  void wifi_driver::handle_scan()
  {
    htmlStrForWifi = getscanWifistr();
    server.send(200, "text/html", SendHTML());
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
      ptr +="body{margin-top: 50px;display: flex;flex-direction: column;justify-content: center;align-items: center;background-color: #0f3e51;} h1 {color: #858585;margin: 50px auto 30px;} h3 {color: #858585;margin-bottom: 50px;}\n";
      ptr +=".button {display: block;width: 80px;background-color: #34495e;border: none;color: white;padding: 13px 30px;text-decoration: none;font-size: 18px;margin: 0px auto 35px;cursor: pointer;border-radius: 4px;}\n";
      ptr +=".button-scan {background-color: #34495e;}\n";
      ptr +=".button-scan:active {background-color: #16a085;}\n";
      ptr +=".button-connect {background-color: #34495e;}\n";
      ptr +=".button-connect:active {background-color: #16a085;}\n";
      ptr +="p {font-size: 14px;color: #888;margin-bottom: 10px;}\n";
      ptr +=".network-list {width: 300px;max-height: 200px;overflow-y: auto;border: 1px solid #ccc;list-style: none;}\n";
      ptr +=".network-item:nth-child(even) {background-color: #f2f2f2;}\n";
      ptr +=".network-item:nth-child(odd) {background-color: #ffffff;}\n";
      ptr +=".network-item {padding: 10px;border-bottom: 1px solid #ccc;cursor: pointer;}\n";
      ptr +="input#passwordInput {display: none;width: 200px; height: 40px; font-size: 16px; }\n";
      ptr +="</style>\n";
      ptr +="</head>\n";
      ptr +="<body>\n";
      ptr +="<h1>YE2310D Config</h1>\n";
      ptr +="<h3>Config your WIFI</h3>\n";

      ptr +="<p>SCANWIFI</p><a class=\"button button-scan\" href=\"/scan\">SCAN</a>\n";
      
      //掃描WIFI後顯示
      ptr +="<p>WIFI LIST</p>\n";
      ptr +="<ul class=\"network-list\">\n";
      ptr += htmlStrForWifi;
      // ptr +="<li class=\"network-item\" onclick=\"ClickWifiName(this)\">Network 1</li>\n";
      // ptr +="<li class=\"network-item\" onclick=\"ClickWifiName(this)\">Network 2</li>\n";
      ptr +="</ul>\n";

      ptr +="<input type=\"password\" id=\"passwordInput\" style=\"display: none;\">\n";
      //腳本
      ptr +="<script>\n";
      ptr +="function ClickWifiName(element) {\n";
      ptr +="var ssid = element.textContent;\n";
      ptr +="var password = document.getElementById(\"passwordInput\");\n";
      ptr +="passwordInput.style.display = \"block\";\n";
      ptr +="passwordInput.placeholder = ssid+ \"-password?\";\n";
      // ptr +="passwordInput.onblur = function () {\n";
      // ptr +="passwordInput.style.display = \"none\"; \n";
      // ptr +=" passwordInput.value = "";\n";
      // ptr +="};\n";
      ptr +="}\n";
      ptr +="</script>\n";

      ptr +="<p>CONNECT</p><a class=\"button button-connect\" href=\"/connect\">CONNECT</a>\n";
      ptr +="</body>\n";
      ptr +="</html>\n";
      return ptr;
  }

  void wifi_driver::ClickWifiName(String ssid , String password)
  {
    user_ssid = ssid;
    user_password = password;
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