#include "ESP32_WiFi.h"


ESP32_WIFI::ESP32_WIFI(const char *accessPointName) {
  
  APName = accessPointName;

}

bool ESP32_WIFI::init(byte configPortalTimeout, byte connectTimeout, byte connectRetry, byte saveConnectTimeout){
  
  wm.setConfigPortalTimeout(configPortalTimeout);
  wm.setConnectTimeout(connectTimeout);  
  wm.setConnectRetries(connectRetry); 
  wm.setSaveConnectTimeout(saveConnectTimeout);

  return wm.autoConnect(APName);

}

void ESP32_WIFI::resetMemory(bool b){
  if(b){
    wm.resetSettings();
  }
}

bool ESP32_WIFI::isConnectedToWifi(){

  if(WiFi.status() == WL_CONNECTED){
    return true;
  }
  if(WiFi.status() != WL_CONNECTED){
    return false;
  }
  
}

String ESP32_WIFI::getWifiSSID(){
  return wm.getWiFiSSID();
}

String ESP32_WIFI::getWifiPass(){
  return wm.getWiFiPass();
}
