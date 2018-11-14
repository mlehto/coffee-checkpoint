void refreshLog() {
  sendData("");
  drinkLog = String(responseFromServer);
}

void sendData(String cardUid) {
  responseFromServer[0] = '\0';
  cmd = "AT+CIPSTART=\"TCP\",\"";
  cmd += DST_IP;
  cmd += "\",80";
  Serial1.println(cmd);
  if (Serial1.find("OK"));
  delay(1000);

  if (cardUid.length() == 0) {
    cmd = "GET /index.php?action=list&uid=" + cardUid + "&brewtype=1 HTTP/1.1\r\nHost: java.fotoni.org\r\n\r\n";
  }
  else {
    cmd = "GET /index.php?action=add&hash=ACCESSPAPERS&uid=" + cardUid + "&brewtype=1 HTTP/1.1\r\nHost: java.fotoni.org\r\n\r\n";
  }

  Serial1.print("AT+CIPSEND=");
  Serial1.println(cmd.length());
  if (!Serial1.find(">")) {
    Serial.println("Error starting send");
    Serial1.println("AT+CIPCLOSE");
    connectedToWlan = false;
  }
  flushSerial();

  Serial1.println(cmd);

  Serial1.find("{");
  long startTime = millis();
  responsePos = 0;
  while (startTime + 5000 > millis() && responsePos < 198) {
    if (Serial1.available() > 0) {
      char c = Serial1.read();
      if (c == '}') {
        startTime = 0;
      }
      else {
        responseFromServer[responsePos] = c;
        responseFromServer[responsePos+1] = '\0';
        responsePos++;
      }
    }
  }
  Serial.print("Read chars: ");
  Serial.println(responsePos);
  
  flushSerial();
  Serial1.println("AT+CIPCLOSE");
  Serial1.find("OK"); delay(100);  flushSerial();

  Serial.println("Response:");
  Serial.println(responseFromServer);
  processServerResponse();
  Serial.println("Processed:");
  Serial.println(responseFromServer);
}

void connectToNetwork() {
  drawLog("Connecting...");
  if (resetWifi()) {
    for (int i = 0; i < 5; i++)
    {
      if (connectWiFi())
      {
        drawLog("Connecting... OK");
        Serial.println("Connected to WLAN");
        connectedToWlan = true;
        Serial1.println("AT+CIPMUX=0");
        Serial1.find("OK");
        break;
      }
      else {
        delay(15000);
      }
    }
    if (!connectedToWlan) {
      delay(30000);    
    }
  }
}

boolean resetWifi() {
  connectedToWlan = false;
  flushSerial();
  Serial.print("Reseting WIFI... ");
  Serial1.println("AT+RST");

  if (Serial1.find("ready")) {
    Serial.println("OK");
    return true;
  }

  Serial.println("fail");
  return false;
}

boolean connectWiFi()
{
  Serial.println("Connecting to WLAN");
  flushSerial();

  Serial1.println("AT+CWMODE=1");
  if (!Serial1.find("OK")) return false;
  flushSerial();

  String cmd = "AT+CWJAP=\"";
  cmd += SSID;
  cmd += "\",\"";
  cmd += PASS;
  cmd += "\"";
  Serial1.println(cmd);

  if (Serial1.find("OK")) {
    Serial.println("Connected!");
    return true;
  }

  Serial.println("Failed connecting");
  return false;
}

void processServerResponse() {
  cmd = "";
  bool insideStrip = false;
  char c = ' ';
  for (int a = 0 ; a < 200 ; a++) {
    c = responseFromServer[a];
    if (c == '\0') break;
    if (c == '+') {
      insideStrip = true;
    }
    if (!insideStrip && c != '\n' && c != '\r') {
      cmd += c;
    }
    if (insideStrip && c == '\n') {
      insideStrip = false;
    }
  }
  cmd.toCharArray(responseFromServer, cmd.length()+1);
}

void refreshESPInfo() {
  //TODO
}

