#include <WiFi.h>

const char* ssid     = "IoT Lab";
const char* password = "IoT@123456";

#include <WiFiUdp.h>
WiFiUDP UDP;

#define UDP_PORT 23001
#define SERVER_PORT 2002
#define ServerIP "192.168.79.199"

int first = 1;

char packet[255];
uint8_t dataToSend[] = "hello server from esp32";

void setup()
{
    Serial.begin(115200);
    delay(10);

    // We start by connecting to a WiFi network
    // To debug, please enable Core Debug Level to Verbose

    Serial.println();
    Serial.print("[WiFi] Connecting to ");
    Serial.println(ssid);

    WiFi.begin(ssid, password);

    while(WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.println("Connect...");
    }

    Serial.println("ip address is: ");
    Serial.println(WiFi.localIP());

    UDP.begin(UDP_PORT);
    Serial.print("listen on UDP port");
    Serial.println(UDP_PORT);
    //gui lan dau 
  if (first) {
    UDP.beginPacket(ServerIP, SERVER_PORT);
    UDP.write(dataToSend, sizeof(dataToSend));
    UDP.endPacket();
    first--;
  }
}

void loop()
{
  int packetSize = UDP.parsePacket();
  if (packetSize) {
    Serial.print("received packet, size: ");
    Serial.println(packetSize);
    int len = UDP.read(packet, 255);
    if (len > 0) {
      packet[len] = '\0';
    }
    Serial.print("nhan packet: ");
    Serial.println(packet);
  }

  if (Serial.available() > 0) {
    // read the incoming string:
    String incomingString = Serial.readString();
    if (incomingString == "close") {
      UDP.stop();
    }
    uint8_t packet2[incomingString.length()];
    incomingString.getBytes(packet2,(incomingString).length());

    UDP.beginPacket(UDP.remoteIP(), UDP.remotePort());
    UDP.write(packet2, sizeof(packet2));
    UDP.endPacket();
  }
  delay(2000);
}
