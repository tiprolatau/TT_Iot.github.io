#include <WiFi.h>

// const char* ssid     = "Anh Thu COFFE";
// const char* password = "xincamon";

const char* ssid     = "your-ssid";
const char* password = "your-password";

//create static IP for server
IPAddress local_IP(192, 168, 1, 2);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);
IPAddress primaryDNS(8,8,8,8);
IPAddress secondaryDNS(8,8,4,4);

#define SERVER_PORT 1999

//Creates a server that listens for incoming connections on the specified port.
WiFiServer TcpServer(SERVER_PORT); 

//create a buffer to contain incoming bytes sent from client
uint8_t receiveBuffer[255];

bool switchState;

void setup() 
{
  Serial.begin(115200);

  //set static 
  if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS)){
    Serial.println("STA failed to configure");
  }

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

  //Tells the server to begin listening for incoming connections.
  TcpServer.begin();
  Serial.println("TCP Server is listening");
}

void loop() 
{
  //Gets a client that is connected to the server and has data available for reading
  //Returns a Client object
  WiFiClient TcpClient = TcpServer.available();

  if (TcpClient) {

    if (TcpClient.connected()) {    
      Serial.println("TcpClient connect");
      switchState = true;
    }
    //TcpClient.connected() returns true if the client is connected, false if not
    //while TcpClient is connected, perform read write operations
    while (TcpClient.connected()) {

      //if there are incoming bytes available read them and print them
      //TcpClient.available() returns the number of bytes available for reading
      if (switchState) {
        if (TcpClient.available() > 0) {

          //read the bytes incoming from the client and save to receiveBuffer
          int c = TcpClient.read(receiveBuffer, sizeof(receiveBuffer));

          //data processing
          /*When a new byte is written, it will overwrite the old byte that client has been written before
          handled by adding a special character($) at the end of the message 
          when client sends it to server*/

          //check special character($) and replace it with '\0', that means terminate strings
          for (size_t j = 0; j < strlen((char*)receiveBuffer); j++) {
            if (receiveBuffer[j] == '$') {
              receiveBuffer[j] = '\0';
              Serial.print("FromClient: ");
              Serial.println((char*)receiveBuffer);
              break;
            }
          }

          //checkclose
          if ((char*)receiveBuffer == "close") {
            Serial.println("Client has disconnected 1");
            TcpClient.stop();
            // break;
          }

          /*a real check to ensure transmission has been completed
          Discard any bytes that have been written but not yet read
          else TcpClient.available() always returns the number of 
          bytes available for reading > 0*/
          TcpClient.flush();
          switchState = !switchState;
          Serial.println("waiting to receive data from the keyboard");
          // break;
        }
      }

      //Get the number of bytes (characters) available for reading from the serial port
      //Serial.available() returns the number of bytes available to read.
      if (!switchState) {
        if (Serial.available() > 0) {
          //reads characters from the serial buffer into a String

          // 2 cach doc string tra ve 2 ket qua giong nhau nhung do dai chuoi khac nhau
          // String incomingString = Serial.readString; //cach nay se co ky tu /n o cuoi chuoi
          String incomingString = Serial.readStringUntil('\n');
          
          incomingString = incomingString + "$\0";
          Serial.println(incomingString);

          //create a buffer to contain bytes that server will send to client
          uint8_t sendBuffer[incomingString.length()];

          //copy the string's characters to the sendBuffer.
          incomingString.getBytes(sendBuffer,incomingString.length());

          //send data in sendBuffer to client
          TcpClient.write(sendBuffer, sizeof(sendBuffer));

          switchState = !switchState;
          Serial.println("server is waiting for incoming data");
          // break;
        }

        // delay(1000);
      }
    }

    //ckeck whether or not the client is connected
    if (!TcpClient.connected()) {
      Serial.println();
      Serial.println("Client has disconnected");
      TcpClient.stop();
    }
    delay(1000);
  }
}
