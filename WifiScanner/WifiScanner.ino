#include <WiFi.h>
#include <Adafruit_MQTT.h>
#include <Adafruit_MQTT_Client.h>

#define MQTT_USERNAME "Not_Needed"
#define MQTT_PASSWORD "Not_Needed"
#define MQTT_SERVER "10.25.35.123"
#define MQTT_SERVERPORT 1883

#define LED_RED 25
#define LED_BLUE 27
#define LED_GREEN 26
#define PUSH_BUTTON 18

#define MAX_ATTEMPTS 5
#define RETRY_PERIOD 1000
#define RETRY_ADJUSTMENT 500

const char* SSID = "NMT-IoT";
const char* password = "Its!Working";

bool buttonState = LOW; 

WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, MQTT_SERVER, MQTT_SERVERPORT, MQTT_USERNAME, MQTT_PASSWORD);

void setup() {
  Serial.begin(115200);

  pinMode(LED_RED, OUTPUT);
  pinMode(LED_BLUE, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(PUSH_BUTTON, INPUT);

  digitalWrite(LED_RED, HIGH);
  digitalWrite(LED_BLUE, LOW);
  digitalWrite(LED_GREEN, LOW);

  bool wifiConnected = wifiConnect();

  if (wifiConnected) {
    Serial.println("Wifi connected successfully.");
    digitalWrite (LED_RED, LOW);
    digitalWrite (LED_GREEN, HIGH);
    wiFiDetails();
  } else {
    Serial.println("Wifi connection failed.");
  }

  bool mqttConnected = mqttConnect();
  if (mqttConnected) {
    digitalWrite(LED_RED, HIGH);
    Serial.println("MQTT connected successfully.");
    delay(RETRY_PERIOD);
    digitalWrite(LED_RED, LOW);
  } else {
    digitalWrite(LED_RED, HIGH);
    digitalWrite(LED_GREEN, LOW);
    Serial.println("MQTT connection failed.");
  }
}

void loop() {
  buttonState = digitalRead(PUSH_BUTTON);
  if (buttonState == HIGH) {
    Serial.print("1");
    digitalWrite(LED_RED, HIGH);
    digitalWrite(LED_GREEN, LOW);
  } else {
    Serial.print("0");
    digitalWrite(LED_GREEN, HIGH);
    digitalWrite(LED_RED, LOW);
  }
}

bool mqttConnect() {
  uint8_t attempts = 0;
  int8_t mqttConnectionResult;
  
  Serial.println();
  Serial.println("Connecting to MQTT broker...");

  while((mqttConnectionResult = mqtt.connect()) != 0 && attempts < MAX_ATTEMPTS) {
    Serial.print(".");
    delay(RETRY_PERIOD + RETRY_ADJUSTMENT * attempts);
    attempts++;

    if (mqttConnectionResult != 0) {
      mqtt.disconnect();
    }
  }

  return attempts < MAX_ATTEMPTS;
}

bool wifiConnect() {
  int attempts = 0;

  WiFi.mode(WIFI_STA);
  WiFi.begin(SSID, password);

  Serial.println("Connecting to wifi...");

  while (WiFi.status() != WL_CONNECTED && attempts < MAX_ATTEMPTS) {
    Serial.print(".");
    delay(1000);
    attempts++;
  }

  return attempts < MAX_ATTEMPTS;
}

void wiFiDetails() {
  Serial.println();
  Serial.print("Local IP:        ");
  Serial.println(WiFi.localIP());
  Serial.print("Gateway IP:      ");
  Serial.println(WiFi.gatewayIP());
  Serial.print("Signal Strength: ");
  Serial.println(WiFi.RSSI());
  Serial.print("Hostname:        ");
  Serial.println(WiFi.getHostname());
}