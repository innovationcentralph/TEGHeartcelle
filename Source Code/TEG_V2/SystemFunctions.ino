void MQTTMonitorRoutine( void * pvParameters ){

  Serial.print("MQTT monitoring running on core ");
  Serial.println(xPortGetCoreID());

  for(;;){

    // Check if there is an incoming message
    if (mqttHandler.messageAvailable()) {
        // Get the topic and payload of the incoming message
        String incomingTopic = mqttHandler.getMessageTopic();
        String incomingPayload = mqttHandler.getMessagePayload();

        #ifdef DEBUG_MQTT
          // Process the incoming message
          Serial.print("Received message on topic: ");
          Serial.println(incomingTopic);
          Serial.print("Message payload: ");
          Serial.println(incomingPayload);
        #endif

        if(incomingTopic == "TEG/ChargerStatus"){
          if(incomingPayload == "[0]"){
            isRemotelyOff = true;
            digitalWrite(RELAY_PIN, RELAY_OFF);
          }else if(incomingPayload == "[1]"){
            isRemotelyOff = false;
            if(!isHot){
              digitalWrite(RELAY_PIN, RELAY_ON);
            }
          }
        }
        
        // Clear the flag after processing the message
        mqttHandler.clearMessageFlag();
    }
  
    delay(1);
  } 
  
}
