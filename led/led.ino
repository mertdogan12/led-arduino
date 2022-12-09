#include<ESP8266WiFi.h>
#include <WiFiUdp.h>

#include "./arduino_secrets.h"

const char* ssid = SECRET_SSID;
const char* password = SECRET_PASSWORD;
WiFiServer server(80);

IPAddress localIP(192, 168, 100, 2);
IPAddress gateway(192, 168, 100, 1);
IPAddress subnet(255, 255, 255, 0);

WiFiUDP Udp;
unsigned int localUdpPort = 1337;
byte incomingPacket[7];

const int redPin = 12;
const int greenPin = 14;
const int bluePin = 13;

const int resolution = 256;

int redColor = 1024;
int greenColor = 1024;
int blueColor = 1024;

union ArrayToInteger {
  byte array[2];
  uint16_t integer;
};

void setup() {
  Serial.begin(115200);
  Serial.println();

  delay(10); 

  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);

  analogWrite(redPin, 0);
  analogWrite(greenPin, 0);
  analogWrite(bluePin, 1024);

  if (!WiFi.config(localIP, gateway, subnet))
    Serial.println("STA Failed to configure");

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
  analogWrite(redPin, redColor);
  analogWrite(greenPin, greenColor);
  analogWrite(bluePin, blueColor);

  ArrayToInteger red, green, blue;

  int packetSize = Udp.parsePacket();
  if (packetSize) {
    int len = Udp.read(incomingPacket, 6);

    if (len > 0) {
      incomingPacket[len] = '\0';
    }

    for (int i = 0; i < 2; i++) {
      red.array[i] = incomingPacket[i];
      green.array[i] = incomingPacket[i + 2];
      blue.array[i] = incomingPacket[i + 2*2];
    }
 
    redColor = red.integer;
    greenColor = green.integer;
    blueColor = blue.integer;

    Serial.printf("%s | %d %d %d\n", Udp.remoteIP().toString().c_str(), redColor, greenColor, blueColor);
  }
}

