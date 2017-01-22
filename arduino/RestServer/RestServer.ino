/*

*/

#define DHT11PIN 8


#include <RCSwitch.h>
#include <dht11.h>

#include <Wire.h>
#include <ArduinoWiFi.h>


#define        COV_RATIO                       0.2            //ug/mmm / mv
#define        NO_DUST_VOLTAGE                 400            //mv
#define        SYS_VOLTAGE                     5000


/*
I/O define
*/
const int iled = 7;                                            //drive the led of sensor
const int vout = 0;                                            //analog input port
const String VAL = "#Value: ";
const String STAT ="Sensor status:";
/*
private function
*/
int Filter(int m)
{
  static int flag_first = 0, _buff[10], sum;
  const int _buff_max = 10;
  int i;

  if (flag_first == 0)
  {
    flag_first = 1;

    for (i = 0, sum = 0; i < _buff_max; i++)
    {
      _buff[i] = m;
      sum += _buff[i];
    }
    return m;
  }
  else
  {
    sum -= _buff[0];
    for (i = 0; i < (_buff_max - 1); i++)
    {
      _buff[i] = _buff[i + 1];
    }
    _buff[9] = m;
    sum += _buff[9];

    i = sum / 10.0;
    return i;
  }
}

RCSwitch mySwitch = RCSwitch();



void setup() {
  Serial.begin(9600);
  Serial.println(F("Monitor port text."));

  Wifi.begin();
  Wifi.println(F("REST Server is up"));

  mySwitch.setPulseLength(162); // Pulse length
  mySwitch.enableTransmit(10); // Pilot pin
  Wifi.println(F("RC module configured."));

  pinMode(iled, OUTPUT);
  digitalWrite(iled, LOW);                                     //iled default closed
  Wifi.print(F("Dust Sensor enabled \n"));
  //
}

void loop() {
  while (Wifi.available()) {
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
    measureCommand('t', client);
  }

  if (command == "digitalHumidity") {
    measureCommand('h', client);
  }

  if (command == "digitalDust") {
    dustMeasure(client);
  }

}

void pilotCommand(WifiData client) {
  int buttonNumber, buttonValue;

  buttonNumber = client.parseInt();


  if (buttonNumber != 1 && buttonNumber != 2 && buttonNumber != 3 && buttonNumber != 4) {
    sendAnswear(F("Wrong button number."), client);
  } else if (client.read() == '/') {
    buttonValue = client.parseInt();
    sendPilotSignal(buttonNumber, buttonValue, client);
  } else {
    sendAnswear(F("Mode not specified"), client);
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
  dtostrf(value, 3, 3, valueString);
  String answear;
  answear = STAT + sensorStatus + '\n' + VAL + valueString;
  sendAnswear(answear, client);
}

void dustMeasure(WifiData client) {


  /*
  variable
  */
  float density, voltage;
  int   adcvalue;

  /*
   get adcvalue
   */
  digitalWrite(iled, HIGH);
  delayMicroseconds(280);
  adcvalue = analogRead(vout);
  digitalWrite(iled, LOW);

  adcvalue = Filter(adcvalue);

  /*
  covert voltage (mv)
  */
  voltage = (SYS_VOLTAGE / 1024.0) * adcvalue * 11;

  /*
  voltage to density
  */
  if (voltage >= NO_DUST_VOLTAGE)
  {
    voltage -= NO_DUST_VOLTAGE;

    density = voltage * COV_RATIO;
  }
  else
    density = 0;

  /*
  display the result
  */

  static char valueString2[15];
  dtostrf(density, 3, 3, valueString2);
  
  String answear2 =  VAL + String(valueString2);
  String answear3 = F(" ug/m3\n");

  String answear = answear2 + answear3;

  sendAnswear(answear, client);

}



