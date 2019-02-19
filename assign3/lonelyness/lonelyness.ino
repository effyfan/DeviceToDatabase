#include <SPI.h>
#ifdef ARDUINO_SAMD_MKR1000
#include <WiFi101.h>
#define WL_NO_MODULE WL_NO_SHIELD 
#else
#include <WiFiNINA.h>
#endif
#include <ArduinoMqttClient.h>

#include "config.h"

WiFiSSLClient net;
MqttClient mqtt(net);

// Temperature and Humidity Sensor
#include <DHT.h>
#define DHTTYPE DHT22
#define DHTPIN  7
DHT dht(DHTPIN, DHTTYPE);

// sensor pins
int photocellPin = 0;
int distancePin = 1;
int motionPin = 2;
int soundPin = 3;
int vibrationPin = 4;

// float disSensor;
// float distance;
float photocellReading;
float lastPhotocellReading;
float motionReading;
float lastMotionReading;
float vibrationReading;
float lastVibrationReading;
float soundReading;
float lastSoundReading;

String temperatureTopic = "itp/" + DEVICE_ID + "/r20_temperature";
String humidityTopic = "itp/" + DEVICE_ID + "/r20_humidity";
String photocellTopic = "itp/" + DEVICE_ID + "/r20_photocell";
String vibrationTopic = "itp/" + DEVICE_ID + "/r20_vibration";
String soundTopic = "itp/" + DEVICE_ID + "/r20_sound";
String motionTopic = "itp/" + DEVICE_ID + "/r20_motion";

// Publish every 2 minutes for the workshop. Real world apps need this data every 5 or 10 minutes.
unsigned long publishInterval = 10 * 12000;
unsigned long lastMillis = 0;

unsigned long soundInterval = 10 * 1000;
unsigned long lastSoundMillis = 0;

void setup() {
  Serial.begin(9600);

  // Uncomment next line to wait for a serial connection
  // while (!Serial) { }
 
  // initialize temperature sensor
  dht.begin();   
 
  Serial.println("Connecting WiFi");
  connectWiFi();
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    connectWiFi();
  }

  if (!mqtt.connected()) {
    connectMQTT();
  }
  
  // poll for new MQTT messages and send keep alives
  mqtt.poll();

  if (millis() - lastMillis > publishInterval) {
    lastMillis = millis();

    float temperature = dht.readTemperature(true);
    float humidity = dht.readHumidity();

    Serial.print(temperature);
    Serial.print("°F ");
    Serial.print(humidity);
    Serial.println("% RH ");
    
    mqtt.beginMessage(temperatureTopic);
    mqtt.print(temperature); 
    mqtt.endMessage();

    mqtt.beginMessage(humidityTopic);
    mqtt.print(humidity); 
    mqtt.endMessage();
  }  

   if (millis() - lastMillis > soundInterval) {
     lastSoundMillis = millis();
     soundReading = digitalRead(soundPin); 
     if (soundReading != lastSoundReading) {
        Serial.print(soundReading);
        Serial.println(" SOUND");  
        mqtt.beginMessage(soundTopic);
        mqtt.print(soundReading); 
        mqtt.endMessage();
        lastSoundReading = soundReading; 
        delay(10);
    }
   }

    // disSensor = analogRead(distancePin);
    // distance = (12787.0 / (disSensor - 3.0)) - 4.0;
    photocellReading = analogRead(photocellPin);
    if (abs(lastPhotocellReading - photocellReading) > 3) {
      Serial.print(photocellReading);
      Serial.println(" LIGHT");  
      mqtt.beginMessage(photocellTopic);
      mqtt.print(photocellReading); 
      mqtt.endMessage();
      lastPhotocellReading = photocellReading; 
    }

    vibrationReading = digitalRead(vibrationPin);
    if (vibrationReading != lastVibrationReading) {
      Serial.print(vibrationReading);
      Serial.println(" VIBR");  
      mqtt.beginMessage(vibrationTopic);
      mqtt.print(vibrationReading); 
      mqtt.endMessage();
      lastVibrationReading = vibrationReading; 
    }

   

    motionReading = digitalRead(motionPin);
    if (motionReading != lastMotionReading) {
      Serial.print(motionReading);
      Serial.println(" MOTION");
      mqtt.beginMessage(motionTopic);
      mqtt.print(motionReading); 
      mqtt.endMessage();
      lastMotionReading = motionReading;
      delay(10);
    }
}

void connectWiFi() {
  // Check for the WiFi module
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    // don't continue
    while (true);
  }

  Serial.print("WiFi firmware version ");
  Serial.println(WiFi.firmwareVersion());
  
  Serial.print("Attempting to connect to SSID: ");
  Serial.print(WIFI_SSID);
  Serial.print(" ");

  while (WiFi.begin(WIFI_SSID, WIFI_PASSWORD) != WL_CONNECTED) {
    // failed, retry
    Serial.print(".");
    delay(3000);
  }

  Serial.println("Connected to WiFi");
  printWiFiStatus();

}

void connectMQTT() {
  Serial.print("Connecting MQTT...");
  mqtt.setId(DEVICE_ID);
  mqtt.setUsernamePassword(MQTT_USER, MQTT_PASSWORD);

  while (!mqtt.connect(MQTT_BROKER, MQTT_PORT)) {
    Serial.print(".");
    delay(5000);
  }

  Serial.println("connected.");
}

void printWiFiStatus() {
  // print your WiFi IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
}
