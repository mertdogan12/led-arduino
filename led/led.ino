#include<ESP8266WiFi.h>
#include <WiFiUdp.h>

#include "./arduino_secrets.h"

const char* ssid = SECRET_SSID;
const char* password = SECRET_PASSWORD;
WiFiServer server(80);

WiFiUDP Udp;
unsigned int localUdpPort = 1337;
char incomingPacket[256];

void setup() {
  Serial.begin(115200);
  Serial.println();

  delay(10); 

  Serial.print("Connecting to ");
  Serial.println(ssid); 
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.print("WiFi connected, ip: "); 
  Serial.println(WiFi.localIP());

  Udp.begin(localUdpPort);
}

void loop() {
  int packetSize = Udp.parsePacket();
  if (packetSize) {
    int len = Udp.read(incomingPacket, 255);

    if (len > 0) {
      incomingPacket[len] = '\0';
    }

    Serial.printf("%s | %s\n", Udp.remoteIP().toString(), incomingPacket);
  }
}

