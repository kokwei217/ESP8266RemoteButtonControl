//Serial.println is for debug purpose
//It should all be changed to digital.write()
//in order to light up the led.

#include <ESP8266WiFi.h>

int set_button = 16;
int set = HIGH;
int set_previous = HIGH;

int response[3];
int rescounter;

//Since we have 3 buttons, so we have 6 combinations, here we choose 5
int sequence[5][3] = {
  {2, 3, 1},
  {3, 1, 2},
  {1, 2, 3},
  {3, 2, 1},
  {2, 1, 3}
};
int seqcounter = 0;

//All the beacons need a wifi connection in order to set up local area network
//SSID = wifi name
//PASSWORD = wifi password
//Not recommended using University hotspot.
const char* ssid = "JinYeePhone";
const char* password = "dragon97";

//Creates a server that listens for incoming connections on the specified port.
WiFiServer server(80);

void setup() {
  pinMode(set_button, INPUT_PULLUP);

  Serial.begin(9600);

  //Initializes the WiFi library's network settings and provides the current status.
  //Return Parameter:    WL_CONNECTED when connected to a network
  //                     WL_IDLE_STATUS when not connected to a network, but powered on
  WiFi.begin(ssid, password);

  //WL_CONNECTED: assigned when connected to a WiFi network;
  //WL_NO_SHIELD: assigned when no WiFi shield is present;
  //WL_NO_SSID_AVAIL: assigned when no SSID are available;
  //WL_SCAN_COMPLETED: assigned when the scan networks is completed;
  //WL_CONNECT_FAILED: assigned when the connection fails for all the attempts;
  //WL_CONNECTION_LOST: assigned when the connection is lost;
  //WL_DISCONNECTED: assigned when disconnected from a network;
  //If wifi is not connected, delay and wait for it to connect to wifi.
  while (WiFi.status() != WL_CONNECTED) delay(100);

  //Once wifi is connected, program exits while loop, and start the server.
  server.begin();
  resetResponse();
  Serial.println("ready");
}

void loop() {

  //available() method belongs to Server class
  //if client is connected, it returns a client object
  //IF NO CLIENT has data available for reading this object will evaluate to FALSE in an if-statement
  //Throw this client to the in statement below to control the program flow
  WiFiClient client = server.available();

  //If client is not false
  if (client) {

    while (client.connected()) {
      set = digitalRead(set_button);
      if (set == LOW && set_previous == HIGH) {
        resetResponse();
        seqcounter++;
        if (seqcounter == 5) seqcounter = 0;
        showSequence();
        Serial.println("a");
      }
      set_previous = set;

      //server.available is difference from client.available
      //client.available return the data available from the client for server to read
      if (client.available()) {

        char c = client.read();
        client.flush();

        switch (c) {
          case '1':
            response[rescounter] = 1;
            Serial.println("1");
            rescounter++;
            break;
          case '2':
            response[rescounter] = 2;
            Serial.println("2");
            rescounter++;
            break;
          case '3':
            response[rescounter] = 3;
            Serial.println("3");
            rescounter++;
            break;
        }

        //check sequence here
        //If the response is not empty (response[u] != 0, very important!, because if one of them is 0
        //means either one out of the three button haven't been pressed by user, the response is not completed
        //hence is not comparable with a set sequence.)
        //and (second condition to reset all response) if the sequence is not same with the response.
        for (int u = 0; u < 3; u++) {
          if (sequence[seqcounter][u] != response[u] && response[u] != 0 ) {
            Serial.println("4");
            Serial.println("5");
            Serial.println("6");
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
      case 1: Serial.println('1'); delay(800); Serial.println('4'); break;
      case 2: Serial.println('2'); delay(800); Serial.println('5'); break;
      case 3: Serial.println('3'); delay(800); Serial.println('6'); break;
    }
  }
}

