#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

#define MQTT_USERNAME "Not_Needed"
#define MQTT_PASSWORD "Not_Needed"
#define MQTT_SERVER "10.25.35.123"
#define MQTT_SERVERPORT 1883

#define DELAY_PERIOD 500

#define LED_RED 25
#define LED_GREEN 26
#define LED_BLUE 27

const char *client_id = "esp32-08-aa";
const char *ssid = "NMT-IoT";
const char *password = "Its!Working";
const char *topic = "testtopic";

String client_name;
String colour;
int state;

WiFiClient espClient;
PubSubClient client(espClient);
DynamicJsonDocument doc(JSON_OBJECT_SIZE(3));

void setup() {
  Serial.begin(115200);
  delay(DELAY_PERIOD);

  registerLED();

  WiFi.mode(WIFI_STA);
  
  WiFi.onEvent(connectedToWiFi, ARDUINO_EVENT_WIFI_STA_CONNECTED);
  WiFi.onEvent(disconnectedToWiFi, ARDUINO_EVENT_WIFI_STA_DISCONNECTED);

  WiFi.begin(ssid, password);
  Serial.println("Connecting to WiFi...");
  delay(DELAY_PERIOD);

  setUpMQTT();

  client.subscribe(topic);
}

void loop() {
  if (client_name != "esp32-08-aa"){
    return;
  }

  Serial.println(client_name);
  Serial.println(colour);
  Serial.println(state);

  if (colour == "red"){
    digitalWrite(LED_RED, state);
  } else if (colour == "blue") {
    digitalWrite(LED_BLUE, state);
  } else {
    digitalWrite(LED_GREEN, state);
  }
}

void registerLED(){
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_BLUE, OUTPUT);
}

void setUpMQTT(){
  client.setServer(MQTT_SERVER, MQTT_SERVERPORT);
  client.setCallback(callback);

  if (client.connect(client_id, ssid, password)) {
    Serial.println("MQTT connected.");
  } else {
    Serial.print("failed with state ");
    Serial.print(client.state());
    delay(2000);
  }
}

void connectedToWiFi(WiFiEvent_t wifi_event, WiFiEventInfo_t wifi_info){
  Serial.println('Connected to Wifi.');
  Serial.println(WiFi.status());
  digitalWrite(LED_BLUE, HIGH);
  delay(1000);
  digitalWrite(LED_BLUE, LOW);

}

void disconnectedToWiFi(WiFiEvent_t wifi_event, WiFiEventInfo_t wifi_info){
  Serial.println('Disconnected to Wifi.');
  WiFi.begin(ssid, password);
}

void callback(char *topic, byte *message, unsigned int length) {
    DeserializationError err = deserializeJson(doc, message);

    if (err) {
      Serial.print(F("deserializeJson() failed with code "));
      Serial.println(err.f_str());
    } else {
      client_name = String(doc["client"]);
      colour = String(doc["colour"]);
      state = String(doc["state"]) == "on" ? HIGH : LOW;
    }
}









