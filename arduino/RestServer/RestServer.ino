/*
 
*/

#define DHT11PIN 8

#include <RCSwitch.h>
#include <dht11.h>
 
#include <Wire.h>
#include <ArduinoWiFi.h>

RCSwitch mySwitch = RCSwitch();



void setup() {
    Wifi.begin();
    Wifi.println("REST Server is up");
    
    mySwitch.setPulseLength(162); // Pulse length
    mySwitch.enableTransmit(10); // Pilot pin
    Wifi.println("RC module configured.");
}

void loop() {
    while(Wifi.available()){
      process(Wifi);
    }
  delay(50);
  
}

void process(WifiData client) {
  // read the command
  String command = client.readStringUntil('/');

  if (command == "digitalPilot") {
  pilotCommand(client);
  } 
  
  if (command == "digitalTemperature") {
  measureCommand('t',client);
  } 
  
  if (command == "digitalHumidity") {
  measureCommand('h',client);
  }

}

void pilotCommand(WifiData client){
  int buttonNumber, buttonValue;
  
  buttonNumber = client.parseInt();
  
  
  if (buttonNumber != 1 && buttonNumber != 2 && buttonNumber != 3 && buttonNumber != 4){
    sendAnswear(F("Wrong button number."),client);
  } else if (client.read() == '/'){
    buttonValue = client.parseInt();
    sendPilotSignal(buttonNumber,buttonValue,client);
  } else {
    sendAnswear(F("Mode not specified"),client);
  }
}

void measureCommand(char what, WifiData client) {
  dht11 DHT11;
  int chk = DHT11.read(DHT11PIN);   
  String sensorStatus;  
  
  switch (chk)
  {
    case DHTLIB_OK: 
        sensorStatus = F("OK\t"); 
        break;
    case DHTLIB_ERROR_CHECKSUM: 
        sensorStatus = F("Checksum Error."); 
        break;
    case DHTLIB_ERROR_TIMEOUT: 
        sensorStatus = F("Timeout."); 
        break;
    default: 
        sensorStatus = F("Unknown Error."); 
        break;
  }
  
  float value;
  if (what == 't') {
    value = DHT11.temperature;
  } else if (what == 'h') {
    value = DHT11.humidity;
  } else {
   sensorStatus = F("Application ERROR :( !!!"); 
  }
  
  static char valueString[10];
  dtostrf(value,3,3,valueString);
  String answear;
  answear = "Sensor status:" + sensorStatus + '\n' + "#Value:" + valueString;
  sendAnswear(answear,client);
}

