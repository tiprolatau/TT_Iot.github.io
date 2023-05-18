#include <WiFi.h>


const char* ssid     = "Anh Thu COFFE";
const char* password = "xincamon";


#include <WiFiUdp.h>
WiFiUDP UDP;

#define SERVER_PORT 2002

IPAddress local_IP(192, 168, 1, 199);
IPAddress gateway(192, 168, 1, 1);

IPAddress subnet(255, 255, 255, 0);
IPAddress primaryDNS(8,8,8,8);
IPAddress secondaryDNS(8,8,4,4);
char packet[255];

// uint8_t la kieu bytes
uint8_t dataToSend[] = "hello client from esp32 server";

void setup()
{
    Serial.begin(115200);
    delay(10);
    //set static ip 
    if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS)){
      Serial.println("STA failed to configure");
    }
    

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

    UDP.begin(SERVER_PORT);
    Serial.print("listen on UDP port");
    Serial.println(SERVER_PORT);

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

    UDP.beginPacket(UDP.remoteIP(), UDP.remotePort());
    UDP.write(dataToSend, sizeof(dataToSend));
    UDP.endPacket();
  }

 if (Serial.available() > 0) {
    // read the incoming string:
    String incomingString = Serial.readString();
    uint8_t packet2[incomingString.length()];
    incomingString.getBytes(packet2,(incomingString).length());

    UDP.beginPacket(UDP.remoteIP(), UDP.remotePort());
    UDP.write(packet2, sizeof(packet2));
    UDP.endPacket();
  }
  delay(2000);
}
