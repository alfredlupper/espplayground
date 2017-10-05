#include <UbidotsMicroESP8266.h>
#include <SoftwareSerial.h> 

//#define SSID "Aystetten"
//#define PASS "Fred01w#"

#define SSID "Aystetten2"
#define PASS "Amelie04"

#define TOKEN "A1E-i8eODe5VEDhD9wt8T4gYDIFd2IrxEF"

#define greenLEDpin D6   // LED zum Blinken. Bei ESP-07 Pin 2. Bei ESP-01 Pin 1
#define redLEDpin D8   // LED zum Blinken. Bei ESP-07 Pin 2. Bei ESP-01 Pin 1
#define Sensor_PIN 5 // Pin der an Reedkontackt angeschlossen ist
#define Light_PIN A0

#define Maxtimeout 60

const int sleepSeconds = 60;
int timout = 0;

Ubidots client(TOKEN);

void setup() {
    
  //------------------------------------ Init Serial -----------------------------------------------------------------------

    Serial.begin(115200);
    Serial.setTimeout(2000);
    // Wait for serial to initialize.
     while(!Serial) { }
    Serial.println();
    Serial.println("I'm awake.");

  //------------------------------------ Im Station Modus mit Netzwerk verbinden -------------------------------------------
 
WiFi.begin(SSID, PASS);

while (WiFi.status() != WL_CONNECTED) {
  delay(500);
  Serial.print(".");
  
  if  (timout > Maxtimeout) {
      Serial.println(F("WiFi failed to connect"));
      pinMode(redLEDpin, OUTPUT);
      digitalWrite(redLEDpin, 0);  // red LED ON für 2 Sekunden
      delay(2000);
      digitalWrite(redLEDpin, 1);    
      ESP.deepSleep(1000000 * SleepTime); // schlafe für 60 Sekunden und versuche es dann nochmals
      delay(100);
  }    
  timout++;
 }
    
      WiFi.setAutoReconnect(true);
      Serial.println(F("WiFi connected"));
      Serial.println(F("IP address: "));
      Serial.println(WiFi.localIP());
      
      pinMode(greenLEDpin, OUTPUT);
      digitalWrite(greenLEDpin, 0);  // green LED ON für 2 Sekunden
      delay(2000);
      digitalWrite(greenLEDpin, 1);
        
  //------------------------------------ Sensoren lesen  -------------------------------------------

    client.setDataSourceName("Garage");
    client.setDataSourceLabel("GarageESP");
    
    pinMode(Sensor_PIN, INPUT);
    int offen = digitalRead(Sensor_PIN);
    Serial.println(offen);

    pinMode(Light_PIN, INPUT);
    float lux = analogRead(A0);
    Serial.println(lux);
    
    client.add("tor", offen);
    client.add("lux", lux);
    client.sendAll(true);
    
    Serial.println();
    Serial.println("Going into deep sleep");
    ESP.deepSleep(0, WAKE_RFCAL); //gehe schlafen für immer
    delay(100);
}

void loop() {
  yield();
}
