
void sendPilotSignal(int buttonNr, int buttonValue, WifiData client) {  
  String WrongMode = F("Incorrect buttonMode");

  if (buttonNr == 1) {
    if  (buttonValue == 0) {
      mySwitch.send("010100000000000000000000");
      sendAnswear(F("1 OFF sent."),client);
    } else if (buttonValue == 1) {
      mySwitch.send("010100000100010101010011");
      sendAnswear(F("1 ON sent."),client);
    } else {
     sendAnswear(WrongMode, client);
    }
    
  } else if (buttonNr == 2){
    if  (buttonValue == 0) {
      mySwitch.send("010100000101000101011100");
      sendAnswear(F("2 OFF sent."),client);
    } else if (buttonValue == 1) {
      mySwitch.send("010100000101000101010011");
      sendAnswear(F("2 ON sent."),client);
    } else {
     sendAnswear(WrongMode, client);
    }
    
  } else if (buttonNr == 3){
    if  (buttonValue == 0) {
      mySwitch.send("010100000101010001011100");
      sendAnswear(F("3 OFF sent."),client);
    } else if (buttonValue == 1) {
      mySwitch.send("010100000101010001010011");
      sendAnswear(F("3 ON sent."),client);
    } else {
     sendAnswear(WrongMode, client);
    }
    
  } else if (buttonNr == 4) {
    if  (buttonValue == 0) {
      mySwitch.send("010100000101010100011100");
      sendAnswear(F("4 OFF sent."),client);
    } else if (buttonValue == 1) {
      mySwitch.send("010100000101010100010011");
      sendAnswear(F("4 ON sent."),client);
    } else {
     sendAnswear(WrongMode, client);
    }
    
  } else {
   sendAnswear(F("Wrong nr."),client); 
  }
}



