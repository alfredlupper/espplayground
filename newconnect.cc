
#define greenLEDpin D6   // LED zum Blinken. Bei ESP-07 Pin 2. Bei ESP-01 Pin 1
#define redLEDpin D8   // LED zum Blinken. Bei ESP-07 Pin 2. Bei ESP-01 Pin 1
#define Maxtimout 60
# define SleepTime 60

int timout = 0;

bool Ubidots::wifiConnection(char* ssid, char* pass) {

WiFi.begin(ssid, pass);

while (WiFi.status() != WL_CONNECTED) {
  delay(500);
  Serial.print(".");
  
  if  (timout > Maxtimeout) {
      Serial.println(F("WiFi failed to connect"));
      pinMode(redLEDpin, OUTPUT);
      digitalWrite(redLEDpin, 0);  // red LED ON für 2 Sekunden
      delay(2000);
      digitalWrite(redLEDpin, 1);    
      ESP.deepSleep(1000000 * SleepTime);
      delay(100);
  }    
  timout++;;
  WiFi.setAutoReconnect(true);
      Serial.println(F("WiFi connected"));
      Serial.println(F("IP address: "));
      Serial.println(WiFi.localIP());
      
      pinMode(greenLEDpin, OUTPUT);
      digitalWrite(greenLEDpin, 0);  // green LED ON für 2 Sekunden
      delay(2000);
      digitalWrite(greenLEDpin, 1);
  }
}     
