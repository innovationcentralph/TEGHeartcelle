#ifndef ESP32_WIFI_H
#define ESP32_WIFI_H

#include <Arduino.h>
#include <WiFiManager.h> 

class ESP32_WIFI{

  WiFiManager wm;
  
  private:
    const char * APName;
    
    
  public:
    ESP32_WIFI(const char *accessPointName);
    bool init(byte configPortalTimeout = 60, byte connectTimeout = 15, byte connectRetry = 1, byte saveConnectTimeout = 10);
    void resetMemory(bool b);
    bool isConnectedToWifi();
    String getWifiSSID();
    String getWifiPass();
    
    
};

#endif
