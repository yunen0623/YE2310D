#include "WIFI_Func.h"
#include "WIFI_Driver.h"
namespace WIFI_FUNC
{
  WIFI_function::WIFI_function(): timeClient(udp)
  {
    
  }

  WIFI_function::~WIFI_function()
  {
      
  }

  void WIFI_function::updateTime() 
  {
    timeClient.update();
  }
  
  void WIFI_function::initialize()
  {
      timeClient.begin();
      timeClient.setTimeOffset(28800);
  }
  
  String WIFI_function::getTime()
  {
    updateTime();
    Timestr = timeClient.getFormattedTime();
    hours = Timestr.substring(0, 2).toInt();    
    minutes = Timestr.substring(3, 5).toInt();
    seconds = Timestr.substring(6, 8).toInt();
    return timeClient.getFormattedTime();
  }

  int WIFI_function::getTimehours() {return hours;}
  int WIFI_function::getTimeminutes() {return minutes;}
  int WIFI_function::getTimeseconds() {return seconds;}

  
  
}
