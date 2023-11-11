#include "WIFI_Driver.h"

namespace WIFIDRIVER
{
  wifi_driver::wifi_driver():server{80}
  {

  }

  wifi_driver::~wifi_driver()
  {
    
  }

  bool wifi_driver::get_connect_statu()
  {
    return isConnectSTA_;
  }
  
  ///搜尋SPIFFS中的JSON 存不存在SSID PASS
  ///不存在or附近無相同SSID 則開啟AP mode 建立WEB
  ///存在的話直接STA MDOE連線
  ///切換到STA1並連線後return true
  void wifi_driver::initialize()
  {
    Serial.println("初始化");
    bool isReadOK = readJsonFromSPIFFS();
    int n = WiFi.scanNetworks();
    if(n != 0 && isReadOK == true)
    { 
      for (int i = 0; i < n; ++i)
      {
          String ssid = WiFi.SSID(i);
          if(ssid == SPIFFS_ssid)
          {
            Serial.println("初始化中找到配對的SSID");
            isAPMode = false;
            //切換為STA模式
            switchMode(isAPMode);
            break;   
          }
      }      
    }
    else
    {
      // 切換到AP模式
      Serial.println("初始化未找到 切換為AP模式");
      isAPMode = true;
      initializeAPWeb();
      while(1)
      {
        startAPWeb();
        if(isWritetoSPIFFSOK)
        {
           Serial.println("初始化While Loop ,isWritetoSPIFFSOK 設定OK");
          if(user_ssid != "" && user_ssid != "")
          {
            Serial.println("初始化While Loop , userSSID PASS != NULL");
            break;
          }
          else
            Serial.println("JSON parameter is Null");
        }
      }
      //設定完成後 遞規自己 切換成STA並重新嘗試連線 
      initialize();
    }
  }


  void wifi_driver::switchMode(bool isAP)
  {
    if(isAP == true) //AP
    {
      Serial.println("切換模式中 , AP 模式");
      WiFi.mode(WIFI_AP);
      WiFi.softAP(ssid_ap, password_ap);
      isConnectSTA_ = false;
    }
    else //STA
    {
      Serial.println("切換模式中 , STA 模式");
      WiFi.mode(WIFI_STA);
      WiFi.begin(SPIFFS_ssid, SPIFFS_password , 15);
      int connectionAttempts = 0;
      while (WiFi.status() != WL_CONNECTED) 
      {
        delay(1000);
        Serial.println("Connecting to WiFi...");
        connectionAttempts++;
        if (connectionAttempts >= 15) 
        {
           Serial.println("Connection timed out");
           isConnectSTA_ = false;
            break;
        }
      }

      if (WiFi.status() == WL_CONNECTED) 
      {
        Serial.println("Connected to WiFi");
        isConnectSTA_ = true;
      } 
      else //連線超時
      { //遞歸自己 並開啟AP
        Serial.println("連線超時 , 開始AP");
        switchMode(true);
      }
    }
  }

  bool wifi_driver::initializeAPWeb()
  {
      switchMode(isAPMode);
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
        server.send(200, "text/html", "connect...");
        //將資料寫入JSON當中
        isWritetoSPIFFSOK = writeJsonToSPIFFS();
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

  void wifi_driver::startAPWeb()
  {
      server.handleClient();
  }

  bool wifi_driver::readJsonFromSPIFFS()
  {
    //init SPIFFS
    if (!SPIFFS.begin()) 
    {
      Serial.println("SPIFFS 初始化失敗!");
      return false;
    }
    else
      Serial.println("SPIFFS 初始化成功!"); 
    //"r"（只讀）、"w"（只寫）、"a"（附加，如果文件不存在則創建）、"r+"（讀寫，如果文件不存在則失敗）
    jsonFile = SPIFFS.open("/config.json", "r+");
    if (!jsonFile) 
    {
      Serial.println("開啟JSON失敗");
      return false;
    }
    else
      Serial.println("開啟JSON成功");

    //read JSON ssid and password
    
    DynamicJsonDocument doc(200);
    DeserializationError error = deserializeJson(doc, jsonFile);
    if (error) 
    {
      Serial.println("JSON轉換失敗");
      jsonFile.close();
      return false;
    }
    else
      Serial.println("JSON轉換成功");
    SPIFFS_ssid = doc["user_ssid"].as<String>();
    SPIFFS_password = doc["user_password"].as<String>();

    if(SPIFFS_ssid != "" && SPIFFS_password != "")
    {
      Serial.println("JSON中數據正確");
      jsonFile.close();
      return true;      
    }
    else
    {
      Serial.println("JSON中數據錯誤");
      jsonFile.close();
      return false;
    }
  }

  bool wifi_driver::writeJsonToSPIFFS()
  {
    //init SPIFFS
    if (!SPIFFS.begin()) 
    {
      Serial.println("SPIFFS initialization failed!");
      return false;
    }
    //"r"（只讀）、"w"（只寫）、"a"（附加，如果文件不存在則創建）、"r+"（讀寫，如果文件不存在則失敗）
    jsonFile = SPIFFS.open("/config.json", "r+");
    if (!jsonFile) 
    {
      Serial.println("Failed to open JSON file");
      return false;
    }
    // 2是字段數，40是JSON數據大致估算的容量
    const size_t capacity = JSON_OBJECT_SIZE(2) + 40;
    DynamicJsonDocument doc(capacity);

    DeserializationError error = deserializeJson(doc, jsonFile);
    if (error) 
    {
      Serial.println("Failed to parse JSON");
      jsonFile.close();
      return false;
    }
    //寫新資料
    doc["user_ssid"] = user_ssid;
    doc["user_password"] = user_password;

    // 將文件指針移到文件的開頭
    jsonFile.seek(0);

    // 將JSON數據寫入文件
    if (serializeJson(doc, jsonFile) == 0) 
    {
      Serial.println("Failed to write JSON to file");
      jsonFile.close();
      return false;
    }
    else
    { 
      jsonFile.close();
      return true;
    }
  }


}
