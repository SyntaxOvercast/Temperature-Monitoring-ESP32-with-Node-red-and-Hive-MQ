#include "DHTesp.h"
#include <LiquidCrystal_I2C.h>
#include <PubSubClient.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>

const char* ssid = "Wokwi-GUEST";
const char* password = "";

#define I2C_ADDR    0x27
#define LCD_COLUMNS 16
#define LCD_LINES   2

const char* MQTT_SERVER = "d004eb1356264ae999a6524da6cbd5da.s1.eu.hivemq.cloud";
const int   MQTT_PORT   = 8883;
const char* MQTT_TOPIC_DATA = "team1/weather";
const char* MQTT_TOPIC_WIFI = "team1/wifi";

const char* MQTT_USER = "sultannify";
const char* MQTT_PASS = "12345Sultan";

WiFiClientSecure espClient;
PubSubClient mqttClient(espClient);

const int DHT_PIN = 19;
const int R_PIN = 12;
const int G_PIN = 14;
const int B_PIN = 27;
const int BUZZ_PIN = 18;

unsigned long prevMillis = 0;
unsigned long buzzMillis = 0;
const long interval = 5000;
const long buzzInterval = 1500;

DHTesp DHT;
LiquidCrystal_I2C lcd(I2C_ADDR, LCD_COLUMNS, LCD_LINES);


void wifiCheck() {
  if (WiFi.status() != WL_CONNECTED) {
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
      Serial.print(".");
    }
    Serial.println("Connected!");
  }
}

void connectToMQTT() {
  espClient.setInsecure();
  mqttClient.setServer(MQTT_SERVER, MQTT_PORT);

  while (!mqttClient.connected()) {
    wifiCheck();
    Serial.print("Connecting to MQTT...");
    
    if (mqttClient.connect("ESP32Client123", MQTT_USER, MQTT_PASS)) {
      Serial.println("Connected!");
      mqttClient.publish(MQTT_TOPIC_WIFI, "WiFi Connected");
      mqttClient.subscribe("team1/weather/status");

    } else {
      Serial.print("Failed, rc=");
      Serial.println(mqttClient.state());
      delay(1000);
    }
  }
}

bool checkInterval(long duration) {
  unsigned long now = millis();
  if (now - prevMillis >= duration) {
    prevMillis = now;
    return true;
  }
  return false;
}

// void changeColor(float temp) {
//   if (temp > 30) {          // Panas
//     analogWrite(R_PIN, 255);
//     analogWrite(G_PIN, 20);
//     analogWrite(B_PIN, 20);
//   } 
//   else if (temp >= 25 && temp <= 29) {   // Normal
//     analogWrite(R_PIN, 20);
//     analogWrite(G_PIN, 255);
//     analogWrite(B_PIN, 20);
//   }
//   else {                   // Dingin
//     analogWrite(R_PIN, 20);
//     analogWrite(G_PIN, 20);
//     analogWrite(B_PIN, 255);
//   }
// }
void mqttCallback(char* topic, byte* payload, unsigned int length) {
  String msg = "";

  for (int i = 0; i < length; i++) {
    msg += (char)payload[i];
  }

  Serial.print("MQTT Message: ");
  Serial.println(msg);

  if (msg == "DINGIN") {
    ledcWrite(0, 20);
    ledcWrite(1, 20);
    ledcWrite(2, 255);
  } 
  else if (msg == "NORMAL") {
    ledcWrite(0, 20);
    ledcWrite(1, 255);
    ledcWrite(2, 20);
  }
  else if (msg == "PANAS") {
    ledcWrite(0, 255);
    ledcWrite(1, 20);
    ledcWrite(2, 20);
  }

}

void setup() {
  Serial.begin(115200);

  ledcSetup(0, 5000, 8);  // channel, freq, 8-bit resolution
  ledcSetup(1, 5000, 8);  
  ledcSetup(2, 5000, 8);  

  ledcAttachPin(R_PIN, 0);
  ledcAttachPin(G_PIN, 1);
  ledcAttachPin(B_PIN, 2);

  WiFi.begin(ssid, password);
  wifiCheck();

  connectToMQTT();

  DHT.setup(DHT_PIN, DHTesp::DHT22);
  mqttClient.setCallback(mqttCallback);

  lcd.init();
  lcd.backlight();
}

void loop() {
  wifiCheck();
  if (!mqttClient.connected()) connectToMQTT();
  mqttClient.loop();

  if (checkInterval(interval)) {

    TempAndHumidity data = DHT.getTempAndHumidity();

    // LCD
    lcd.setCursor(0, 0);
    lcd.print("Temp:" + String(data.temperature, 1) + "\xDF" + "C   ");

    lcd.setCursor(0, 1);
    lcd.print("Hum :" + String(data.humidity, 1) + "%     ");

    // LED logic
    // changeColor(data.temperature);
      String payload =
      "{\"temperature\": " + String(data.temperature, 1) +
      ", \"humidity\": " + String(data.humidity, 1) + "}";

    mqttClient.publish(MQTT_TOPIC_DATA, payload.c_str());
  }
}


