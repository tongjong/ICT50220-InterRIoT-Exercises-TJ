#include <WiFi.h>

void setup() {
  Serial.begin(115200);
  WiFi.disconnect();
  delay(100);
  
  Serial.println("Setup Completed");
}

void loop() {
  Serial.println("Scanning...");

  int internetCount = WiFi.scanNetworks();

  if(internetCount == 0) {
    Serial.println("No network is found");
  } else {
    for (int count = 0; count < internetCount; ++count){
      Serial.print(count);
      Serial.print(":");
      Serial.print(WiFi.SSID(count));
      Serial.print(" [");
      Serial.print(WiFi.RSSI(count));
      Serial.print("] ");
      Serial.println(WiFi.encryptionType(count) == WIFI_AUTH_OPEN? "OPEM" : "*");
      delay(250);
    }
  }

  Serial.println("");
  delay(100000);
}

