#include <ESP8266WiFi.h>

const char* ssid     = "kokwei";
const char* password = "xopalx217";
const char* host = "192.168.43.222";
int button = 5;
int previous = HIGH;
int current;

void setup() {
  Serial.begin(9600);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
  }

  pinMode(button, INPUT_PULLUP);
  Serial.println("ready");
}

void loop() {
  // This is empty because in order for the sleep cycle to work all code is in setup()
  POST(80, '3');
}

void POST(int httpPort, char postnumber) {
  current = digitalRead(button);

  WiFiClient client;

  if (!client.connect(host, httpPort)) Serial.println("connection failed");
  if (current == LOW && previous == HIGH) client.println(postnumber);

  previous = current;
}
