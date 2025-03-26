#include <WiFi.h>
#include <Adafruit_MQTT.h>
#include <Adafruit_MQTT_Client.h>

/**
 * Define Macros for constant values
 *
 * - MQTT Connection details
 * - Retry values
 * - Component Pins
 */
#define MQTT_USERNAME "Not_Needed"
#define MQTT_PASSWORD "Not_Needed"
#define MQTT_SERVER "10.25.35.123"
#define MQTT_SERVERPORT 1883

#define RETRY_PERIOD 1000
#define RETRY_ADJUSTMENT 500
#define MAX_ATTEMPTS 5

#define LED_RED_1 25
#define LED_GREEN_1 26
#define LED_BLUE_1 27
#define BUTTON 21

#define DEBUG true


const char* ssid = "NMT-IoT";
const char* password = "Its!Working";
const char* topic = "Button State Topic";

int buttonState = LOW;
int lastButtonState = LOW;

WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, MQTT_SERVER, MQTT_SERVERPORT, MQTT_USERNAME, MQTT_PASSWORD);
Adafruit_MQTT_Subscribe buttonStateSubscription = Adafruit_MQTT_Subscribe(&mqtt, topic);


void setup() {
  if (DEBUG) {
    Serial.begin(115200);
  }

  pinMode(LED_RED_1, OUTPUT);
  pinMode(LED_GREEN_1, OUTPUT);
  pinMode(LED_BLUE_1, OUTPUT);
  pinMode(BUTTON, INPUT);

  digitalWrite(LED_RED_1, HIGH);
  digitalWrite(LED_GREEN_1, LOW);
  digitalWrite(LED_BLUE_1, LOW);

  bool wiFiConnected = wiFiConnect();

  if (wiFiConnected) {
    if (DEBUG) {
      Serial.println("WiFi Connected.");
    }

    digitalWrite(LED_RED_1, LOW);
    digitalWrite(LED_GREEN_1, HIGH);
    digitalWrite(LED_BLUE_1, LOW);
    wiFiDetails();

    bool mqttConnected = mqttConnect();

    if (mqttConnected) {
      if (DEBUG) {
        Serial.println("MQTT Connected.");
      }

      digitalWrite(LED_BLUE_1, HIGH);
      delay(RETRY_PERIOD);
      digitalWrite(LED_BLUE_1, LOW);
    } else {
      if (DEBUG) {
        Serial.println("MQTT Connection Failed.");
      }

      digitalWrite(LED_RED_1, HIGH);
      digitalWrite(LED_GREEN_1, LOW);
      digitalWrite(LED_BLUE_1, LOW);
    }

  } else {
    if (DEBUG) {
      Serial.println("WiFi Connection Failed.");
    }
  }

  Serial.print("buttonStage: "); Serial.println(buttonState);
}

void loop() {
  
  buttonState = digitalRead(BUTTON); 

  if (buttonState != lastButtonState) { 

      if (buttonState == LOW) {
      digitalWrite(LED_BLUE_1, HIGH);
      Serial.println("pressed");
      mqtt.publish(topic, buttonState);
    } else {
      digitalWrite(LED_BLUE_1, LOW);
      mqtt.publish(topic, "0");
      Serial.println("not pressed");
      5
    }
    
    lastButtonState = buttonState;
  }
}

bool wiFiConnect() {
  int attempts = 0;

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  if (DEBUG) {
    Serial.println("Connecting to WiFi...");
  }

  while (WiFi.status() != WL_CONNECTED && attempts < MAX_ATTEMPTS) {

    if (DEBUG) {
      Serial.print(".");
    }

    delay(RETRY_PERIOD + RETRY_ADJUSTMENT * attempts);
    attempts++;
  }

  return attempts < MAX_ATTEMPTS;
}

bool mqttConnect() {

  uint8_t attempts = 0;
  int8_t mqttConnectionResult;

  if (DEBUG) {
    Serial.println();
    Serial.println("Connecting to MQTT Broker/Server...");
  }

  while ((mqttConnectionResult = mqtt.connect()) != 0 && attempts < MAX_ATTEMPTS) {

    if (DEBUG) {
      Serial.print(".");
    }

    delay(RETRY_PERIOD + RETRY_ADJUSTMENT * attempts);
    attempts++;

    if (mqttConnectionResult != 0) {
      mqtt.disconnect();
    }
  }

  return attempts < MAX_ATTEMPTS;
}

void wiFiDetails() {
  if (DEBUG) {
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
}
