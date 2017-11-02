#include <UbidotsMicroESP8266.h>
#include <SoftwareSerial.h> 

//#define SSID "Aystetten"
//#define PASS "Fred01w#"

#define SSID "Aystetten2"
#define PASS "Amelie04"

#define TOKEN "A1E-i8eODe5VEDhD9wt8T4gYDIFd2IrxEF"

#define greenLEDpin D6   // LED zum Blinken. Bei ESP-07 Pin 2. Bei ESP-01 Pin 1
#define redLEDpin D8   // LED zum Blinken. Bei ESP-07 Pin 2. Bei ESP-01 Pin 1
#define Sensor_PIN D1 // Pin der an Reedkontackt angeschlossen ist
#define Light_PIN A0

#define Maxtimeout 20

#define flashtime 1000

# define microsec 1000000

#define _debug 1

int sleepSeconds = 3600;
int timeout = 0;

Ubidots client(TOKEN);

void setup() {
    
  //------------------------------------ Init Serial -----------------------------------------------------------------------
  
  if (_debug){
      Serial.begin(115200);
      Serial.setTimeout(2000);
      // Wait for serial to initialize.
      while(!Serial) { }
      if (_debug){
        Serial.println();
        Serial.println("I'm awake.");
      }
   }

  //------------------------------------ Im Station Modus mit Netzwerk verbinden -------------------------------------------


   // WiFi fix: https://github.com/esp8266/Arduino/issues/2186
  WiFi.persistent(false);
  WiFi.mode(WIFI_OFF);
  WiFi.mode(WIFI_STA);
  WiFi.begin(SSID, PASS);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    if (_debug){  Serial.print("."); }    
    timeout++;
   }

  if  (timeout > Maxtimeout) {
      if (_debug){
        Serial.println();
        Serial.println(F("WiFi failed to connect"));
      }  
      pinMode(redLEDpin, OUTPUT);
      digitalWrite(redLEDpin, 1);  // red LED ON für 2 Sekunden
      delay(flashtime);
      digitalWrite(redLEDpin, 0);
    }
    else {
        WiFi.setAutoReconnect(true);
        if (_debug){
          Serial.println(F("WiFi connected"));
          Serial.println(F("IP address: "));
          Serial.println(WiFi.localIP());
        }
        pinMode(greenLEDpin, OUTPUT);
        digitalWrite(greenLEDpin, 1);  // green LED ON für 2 Sekunden
        delay(flashtime);
        digitalWrite(greenLEDpin, 0);
          
        //-------------------- Sensoren lesen
  
        client.setDataSourceName("garage");
        client.setDataSourceLabel("garage");
        
        pinMode(Sensor_PIN, INPUT);
        int offen = digitalRead(Sensor_PIN);
        if (_debug){ Serial.println(offen); }
    
        pinMode(Light_PIN, INPUT);
        float lux = analogRead(Light_PIN);
        if (_debug){ Serial.println(lux); }
        
        client.add("tor", offen);
        client.add("lux", lux);
        client.sendAll(true);
      
        sleepSeconds =0;
    }  
    if (_debug){
        Serial.println();
        Serial.println("Going into deep sleep");
    }
    ESP.deepSleep(microsec * sleepSeconds, WAKE_RFCAL); //gehe schlafen für immer
    delay(100);
  }

void loop() {
  yield();
}

