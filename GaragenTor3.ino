#include <UbidotsMicroESP8266.h>
#include <SoftwareSerial.h> 

//#define SSID "Aystetten"
//#define PASS "Fred01w#"

#define SSID "Aystetten2"
#define PASS "Amelie04"

#define TOKEN "A1E-i8eODe5VEDhD9wt8T4gYDIFd2IrxEF"

const int sleepSeconds = 60;
int timout = 0;

#define Sensor_PIN 5 // Pin der an Reedkontackt angeschlossen ist

Ubidots client(TOKEN);

void setup() {
    Serial.begin(115200);
    Serial.setTimeout(2000);
    // Wait for serial to initialize.
     while(!Serial) { }
    Serial.println("I'm awake.");

  //------------------------------------ Im Station Modus mit Netzwerk verbinden -------------------------------------------
  
    client.wifiConnection(SSID, PASS);
    delay(100);

  while (WiFi.status() != WL_CONNECTED)     // Solange nicht verbunden versuche 6S zu verbinden
  {
    //----------------------------------------------- Timeout abgelaufen ---------------------------------------
    if  (timout > 60)         // Wenn Anmeldung nicht möglich
    {
      Serial.println("Timeout");
      ESP.deepSleep(sleepSeconds * 1000000); // sleepbfor 60 seconds
      delay(100);
    }                           // End timeout
    timout++;
  }

    pinMode(Sensor_PIN, INPUT);
    int offen = digitalRead(Sensor_PIN);
    Serial.println(offen);
    client.add("tor", offen);

    pinMode(A0, INPUT);
    float lux = analogRead(A0);
    Serial.println(lux);
    client.add("Lux", lux);
    client.sendAll(true);
    Serial.println();
    Serial.println("Going into deep sleep");
    ESP.deepSleep(0, WAKE_RFCAL); //gehe schlafen
    delay(100);
}

void loop() {
  yield();
}
