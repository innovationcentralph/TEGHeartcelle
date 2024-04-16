#define RELAY_PIN 26
#define SMOKE_PIN 34

#define oneWireBus 33

#define RELAY_ON  HIGH
#define RELAY_OFF LOW

#define DEBUG_MQTT


//MQTT Parameters

const char* mqttServer   = "driver.cloudmqtt.com";
const int   mqttPort     = 18933;
const char* mqttUser     = "gnigrukf";
const char* mqttPassword = "4YkZBFFRAgIy";

const char* deviceESN = "ABC001";
const char* topicToSubscribe = "TEG/ChargerStatus";
