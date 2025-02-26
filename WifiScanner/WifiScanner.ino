#include <WiFi.h>

#define LED_RED 19
#define LED_BLUE 18
#define MAX_ATTEMPTS 5

const char* SSID = "NMT-IoT";
const char* password = "Its!Working";

bool wifiConnection = false;

void setup() {
  Serial.begin(115200);

  pinMode(LED_RED, OUTPUT);
  pinMode(LED_BLUE, OUTPUT);

  digitalWrite(LED_RED, HIGH);
  digitalWrite(LED_BLUE, LOW);

  bool connected = wifiConnect();

  if (connected) {
    Serial.println("Connected successfully.");
    wifiConnection = true;
  } else {
    Serial.println("Connection failed.");
    wifiConnection = false;
  }
}

void loop() {
 while (wifiConnection){
  digitalWrite(LED_RED, LOW);
  digitalWrite(LED_BLUE, HIGH);
 }
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