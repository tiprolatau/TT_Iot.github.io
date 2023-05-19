#include <WiFi.h>

const char* ssid = "Anh Thu COFFE";
const char* password = "xincamon";

// const char* ssid     = "your-ssid";
// const char* password = "your-password";

#define ServerPort 1999
#define ServerIP "192.168.1.2"

/*Creates a client that can connect 
to a specified internet IP address and port as defined in client.connect()*/
WiFiClient TcpClient;

//create a buffer to contain incoming bytes sent from Server
uint8_t receiveBuffer[255];

bool connectionState = false;
bool switchState;

void setup() {
  Serial.begin(115200);

  //connect to wifi
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("WiFi connected!");
  Serial.println(WiFi.localIP());

  Serial.println("TcpClient is connecting to server");
  //Connect to the IP address and port
  //Returns true if the connection succeeds, false if not
  while (!TcpClient.connect(ServerIP, ServerPort)) {
    Serial.println("Connection to server failed");
    delay(2000);
  }

  Serial.println("Connected to server successful!");
}

void loop() {
  //TcpClient.connected() returns true if the client is connected, false if not
  //while TcpClient is connected, perform read write operations
  if (TcpClient.connected()) {
    connectionState = true;
    bool switchState = true;

    // delay(2000);
    Serial.println("client send first");
    Serial.println("waiting to receive data from the keyboard");

    while (connectionState) {
      //Get the number of bytes (characters) available for reading from the serial port
      //Serial.available() returns the number of bytes available to read.
      if (switchState) {
        if (Serial.available() > 0) {
          //reads characters from the serial buffer into a String
          String incomingString = Serial.readStringUntil('\n');

          String checkClose = incomingString;

          incomingString = incomingString + "$\n";
          Serial.println(incomingString);

          //create a buffer to contain bytes that server will send to client
          uint8_t sendBuffer[incomingString.length()];

          //copy the string's characters to the sendBuffer.
          incomingString.getBytes(sendBuffer, incomingString.length());

          //send data in sendBuffer to server
          TcpClient.write(sendBuffer, sizeof(sendBuffer));

          //check whether or not the client is close
          if (checkClose == "close") {
            connectionState = false;
            TcpClient.stop();
            Serial.println("Client disconnected");
            break;
          }

          Serial.println(switchState);
          switchState = false;
          Serial.println("client is waiting for incoming data");
          // break; //exit while(switchState)
        }
      }
      //if there are incoming bytes available from the server, read them and print them
      //TcpClient.available() returns the number of bytes available for reading
      if (!switchState) {
        if (TcpClient.available() > 0) {

          //read the bytes incoming from the server and save to receiveBuffer
          int c = TcpClient.read(receiveBuffer, sizeof(receiveBuffer));

          //data processing
          /*When a new byte is written, it will overwrite the old byte that client has been written before
          handled by adding a special character($) at the end of the message 
          when client sends it to server*/

          //check special character($) and replace it with '\0', that means terminate strings
          for (size_t j = 0; j < strlen((char*)receiveBuffer); j++) {
            if (receiveBuffer[j] == '$') {
              receiveBuffer[j] = '\0';
              Serial.print("FromServer: ");
              Serial.println((char*)receiveBuffer);
              // Serial.println(strlen((char*)receiveBuffer));
              break;
            }
          }

          TcpClient.flush();

          Serial.println(switchState);
          switchState = true;
          Serial.println("waiting to receive data from the keyboard");
          // break; //exit while(switchState)
        }
      }
    }
  }

  //ckeck whether or not the client is connected
  if ((!TcpClient.connected()) && connectionState) {
    Serial.println();
    Serial.println("Client has disconnected");
    TcpClient.stop();
    connectionState = false;
  }
  delay(1000);
}