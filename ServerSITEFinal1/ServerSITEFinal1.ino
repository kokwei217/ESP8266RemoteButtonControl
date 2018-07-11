#include <ESP8266WiFi.h>

int set_button = 16;
int set = HIGH;
int set_previous = HIGH;

int redled = 15;
int yellowled = 5;
int blueled = 4;

int response[3];
int rescounter;

int sequence[5][3] = {
  {2, 3, 1},
  {3, 1, 2},
  {1, 2, 3},
  {3, 2, 1},
  {2, 1, 3}
};
int seqcounter = 0;

const char* ssid = "JinYeePhone";
const char* password = "dragon97";

WiFiServer server(80);

void setup() {
  pinMode(set_button, INPUT_PULLUP);

  Serial.begin(9600);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) delay(100);

  server.begin();
  resetResponse();
}

void loop() {
  WiFiClient client = server.available();

  if (client) {

    while (client.connected()) {
      
      set = digitalRead(set_button);
      if (set == LOW && set_previous == HIGH) {
        resetResponse();
        seqcounter++;
        if (seqcounter == 5) seqcounter = 0;
        showSequence();
      }
      set_previous = set;

      if (client.available()) {

        char c = client.read();
        client.flush();

        switch (c) {
          case '1':
            response[rescounter] = 1;
            digitalWrite(redled, HIGH);
            rescounter++;
            break;
          case '2':
            response[rescounter] = 2;
            digitalWrite(yellowled, HIGH);
            rescounter++;
            break;
          case '3':
            response[rescounter] = 3;
            digitalWrite(blueled, HIGH);
            rescounter++;
            break;
        }

        //check sequence here
        for (int u = 0; u < 3; u++) {
          if (sequence[seqcounter][u] != response[u] && response[u] != 0 ) {
            digitalWrite(redled, LOW);
            digitalWrite(yellowled, LOW);
            digitalWrite(blueled, LOW);
            resetResponse();
          }
        }
      }
    }
  }
}

void resetResponse() {
  for (int j = 0; j < 3; j++) response[j] = 0;
  rescounter = 0;
}

void showSequence() {
  for (int i = 0; i < 3; i++) {
    switch (sequence[seqcounter][i]) {
        case 1: digitalWrite(redled, HIGH); delay(800); digitalWrite(redled, LOW); break;
        case 2: digitalWrite(yellowled, HIGH); delay(800); digitalWrite(yellowled, LOW); break;
        case 3: digitalWrite(blueled, HIGH); delay(800); digitalWrite(blueled, HIGH); break;
    }
  }
}
