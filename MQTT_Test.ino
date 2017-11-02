//--------------------------------------------------------------------------------
// MQTT to AIMOS MQTT Brocker
// Fred Lupper, 16.10.2017
// Last modified: 16.10.2017
//--------------------------------------------------------------------------------

#include "UbidotsESPMQTT.h"
#include <SoftwareSerial.h> 

//#define SSID "iPhone von Alfred"
//#define PASS "Fred01i#"

#define SSID "Aystetten2"
#define PASS "Amelie04"

#define TOKEN "A1E-i8eODe5VEDhD9wt8T4gYDIFd2IrxEF"

//mosquitto_sub -h 192.168.0.6 -u fred -t zway/aimos/#

#define greenLEDpin D6   // LED zum Blinken. Bei ESP-07 Pin 2. Bei ESP-01 Pin 1
#define redLEDpin D8   // LED zum Blinken. Bei ESP-07 Pin 2. Bei ESP-01 Pin 1
#define Light_PIN A0

#define Maxtimeout 60

const int SleepTime = 60;
int timeout = 0;

Ubidots client("fred");

/****************************************
 * Auxiliar Functions
 ****************************************/

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i=0;i<length;i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

void setup() {
    
  //------------------------------------ Init Serial -----------------------------------------------------------------------

    Serial.begin(115200);
    Serial.setTimeout(2000);
    // Wait for serial to initialize.
    while(!Serial) { }
    Serial.println();
    Serial.println("I'm awake.");

  //------------------------------------ Im Station Modus mit Netzwerk verbinden -------------------------------------------
  
  client.ubidotsSetBroker("fredlupper.ddns.net");
 
  client.setDebug(true); // Pass a true or false bool value to activate debug messages
  
  WiFi.begin(SSID, PASS);
  client.setClientName(); // Modified by Fred
  client.begin(callback);

while (WiFi.status() != WL_CONNECTED) {
  delay(500);
  Serial.print(".");
  
  if  (timeout > Maxtimeout) {
      Serial.println(F("WiFi failed to connect"));
      pinMode(redLEDpin, OUTPUT);
      digitalWrite(redLEDpin, 0);  // red LED ON für 2 Sekunden
      delay(1000);
      digitalWrite(redLEDpin, 1);    
      ESP.deepSleep(1000000 * SleepTime); // schlafe für 60 Sekunden und versuche es dann nochmals
      delay(100);
  }    
  timeout++;
 }
    
      WiFi.setAutoReconnect(true);
      Serial.println(F("WiFi connected"));
      Serial.println(F("IP address: "));
      Serial.println(WiFi.localIP());

      if(!client.connected()){
      client.reconnect();
      }
      
      pinMode(greenLEDpin, OUTPUT);
      digitalWrite(greenLEDpin, 0);  // green LED ON für 2 Sekunden
      delay(1000);
      digitalWrite(greenLEDpin, 1);
        
  //------------------------------------ Sensoren lesen  -------------------------------------------

    pinMode(Light_PIN, INPUT);
    float lux = analogRead(Light_PIN);
    Serial.print("lux: ");    Serial.println(lux);
    
    client.add("lux", lux); //Insert your variable Labels and the value to be sent
    client.ubidotsPublish("aimos");
    client.loop();
    
 //   delay(10000);
    Serial.println();
    Serial.println("Going into deep sleep");
    ESP.deepSleep(1000000 * SleepTime); // schlafe für 60 Sekunden
    delay(100);
}

void loop() {
  yield();
}
