
      
      
bool Ubidots::wifiConnection(char* ssid, char* pass) {

WiFi.begin(ssid, pass);

while (WiFi.status() != WL_CONNECTED) {
  delay(500);
  Serial.print(".");
}
      
IF (WiFi.status() = WL_CONNECTED) {
      WiFi.setAutoReconnect(true);
      Serial.println(F("WiFi connected"));
      Serial.println(F("IP address: "));
      Serial.println(WiFi.localIP());
      
      pinMode(greenLEDpin, OUTPUT);
      digitalWrite(greenLEDpin, 0);  // green LED ON für 2 Sekunden
      delay(2000);
      digitalWrite(greenLEDpin, 1);
}      
else {
      Serial.println(F("WiFi failed to connect"));
      pinMode(redLEDpin, OUTPUT);
      digitalWrite(redLEDpin, 0);  // red LED ON für 2 Sekunden
      delay(2000);
      digitalWrite(redLEDpin, 1);
      
}

}
