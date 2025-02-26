#define RETRY_PERIOD 1000
#define RETRY_ADJUSTMENT 500
#define MAX_ATTEMPTS 5

#define LED_RED_ON_PERIOD 100
#define LED_BLUE_ON_PERIOD 300
#define LED_RED 19
#define LED_BLUE 18

void setup() {
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_BLUE, OUTPUT);
  Serial.begin(115200);

  xTaskCreatePinnedToCore(
    redFlash,    //function to implement the task
    "redFlash",  //name of the task
    1000,     // stack size in bytes
    NULL,     // task input param
    0,        //priority of the task
    NULL,     //task handle
    0         // core where the task should run
  );

  xTaskCreatePinnedToCore(
    blueFlash,    //function to implement the task
    "blueFlash",  //name of the task
    1000,     // stack size in bytes
    NULL,     // task input param
    0,        //priority of the task
    NULL,     //task handle
    0         // core where the task should run
  );
}

void loop() {
  digitalWrite(LED_RED, HIGH);
  delay(LED_RED_ON_PERIOD);
  digitalWrite(LED_RED, LOW);
  delay(LED_RED_ON_PERIOD);
}

void redFlash(void* pvParameters) {
  while (1) {
    Serial.print("Hello");
    delay(500);
    Serial.println(" World");
    delay(500);
  }
}

void blueFlash(void* pvParameters) {
  while (1) {
    digitalWrite(LED_BLUE, HIGH);
    delay(LED_BLUE_ON_PERIOD);
    digitalWrite(LED_BLUE, LOW);
    delay(LED_BLUE_ON_PERIOD);
  }
}
