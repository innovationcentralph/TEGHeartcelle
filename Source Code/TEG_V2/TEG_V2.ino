#include <OneWire.h>
#include <DallasTemperature.h>

// Local Dependencies
#include "Wifi.h"
#include "ESP32_WiFi.h"
#include "SystemConfig.h"
#include "UserConfig.h"
#include "HTTPSClient.h"
#include "MQTTHandler.h"

// Instances
OneWire oneWire(oneWireBus);
DallasTemperature sensors(&oneWire);

Wifi Wifi("324dBm", "randomPASS");
HTTPSClient HTTPSClient;

MQTTHandler mqttHandler;
TaskHandle_t MQTTMonitor;


void setup() {

  Serial.begin(115200);
  delay(1000);

  // Connect to WiFi
  ESP32_WIFI WifiManager("TEG");

  // Set param to true to erase previously configured WiFi credentials
  WifiManager.resetMemory(false);

  // Setup configuration Portal
  if (WifiManager.init()) {

    // Fetch Saved Params
    Wifi.setSSID(WifiManager.getWifiSSID().c_str());
    Wifi.setPass(WifiManager.getWifiPass().c_str());

    // Connect to configured wifi
    Wifi.connect();
  } else {

    // Initial Configuration Failed, must restart ESP
    Serial.println(F("Failed to setup configuration portal"));
    Serial.println(F("Restarting ESP Now"));
    delay(100);

    ESP.restart();
  }

  // Initialize components
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, RELAY_ON);
  delay(1000);
  digitalWrite(RELAY_PIN, RELAY_OFF);
  delay(1000);
  digitalWrite(RELAY_PIN, RELAY_ON);

  // Initialize temperature sensor
  sensors.begin();

  // Initialize MQTT
  mqttHandler.init(mqttServer, mqttPort, mqttUser, mqttPassword, deviceESN);

  xTaskCreatePinnedToCore(MQTTMonitorRoutine, "MQTT Monitor", 5000, NULL, 1, &MQTTMonitor, 0);     // Default loop runs on Core 1
  delay(500);
  
}


float temperature = 0;
float battery = 0;
bool chargingState = 0;
int gasPPM = 0;

bool isHot = false;
bool isRemotelyOff = false;

uint32_t readingMillis = 0;
uint32_t sendingMillis = 0;
void loop() {

  // monitor MQTT Connectivity
  mqttHandler.checkConnectivity();

  if(millis() - readingMillis > 1000){
    // read temperature
    sensors.requestTemperatures();
    temperature = sensors.getTempCByIndex(0);
    gasPPM = analogRead(SMOKE_PIN);
  
    if(temperature > 45){
      isHot = true;
      digitalWrite(RELAY_PIN, RELAY_OFF);
    }else if(temperature < 38){
      isHot = false;
      if(isRemotelyOff == false){
        digitalWrite(RELAY_PIN, RELAY_ON);
      }
    }
  
    battery = 3.2;

    chargingState = digitalRead(RELAY_PIN);

    Serial.println("TEMP: " + String(temperature, 1));
    Serial.println("BATT: " + String(battery, 1));
    Serial.println("SMOKE: " + String(gasPPM));
    Serial.println("CHG: "  + String(chargingState));
    readingMillis = millis();
 
  }

  if(millis() - sendingMillis > 30000){
    // Send Data to Web
    if(Wifi.isConnected()){
      String URL = "https://innovationcentralph.com/TEG/resources/data/APIupdatemonitors.php?t=" + String(temperature, 2) + "&b=" + String(battery, 1) + "&g=" + String(gasPPM) + "&c=" + String(chargingState) + "&id=ABC123";
      Serial.println("URL: " + URL);
      
      String payload = HTTPSClient.SendGetRequest(URL, false);
      Serial.println("PAYLOAD: " + payload);
    }
    
    sendingMillis = millis();
  } 
}
