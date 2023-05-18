import socket

#dia chi IP cua server la dia chi cua may tinh
# Vi DU: ServerIP = "192.168.1.8"
ServerIP = "A.B.C.D"
ServerPort = 1999
bufferSize = 1024

TCPServerSocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)\

TCPServerSocket.bind((ServerIP, ServerPort))

TCPServerSocket.listen(5)

print("TCP server up and listening")

clientSocket, address = TCPServerSocket.accept()

print("TCP server connected to server and ready to perform read write operations")

print(f"Connected by client {address}")

receivedData = ""
bufferSize = 1024

print("server is wating for incoming data")

switchState = 1

while True:
    if switchState:
        try:
            receivedData = clientSocket.recv(bufferSize).decode('utf-8')
        except:
            print("something's wrong, server can't receive data from client")

        if not receivedData: 
            break
        else:
            print("[FromClient]: " + receivedData)
            if "close" in receivedData:
                clientSocket.close()
                print("client has disconnected")
                print("close socket server")
                clientSocket.close()
                TCPServerSocket.close()
                break

        switchState = not switchState
        print("waiting to receive data from the keyboard")



    if not switchState:
        msg = input()

        if len(msg) > 0:
            dataToSend = (msg + '$').encode('utf-8')
            # print(dataToSend)

            try:
                clientSocket.sendall(dataToSend)
                
            except:
                print("something's wrong, server can't send data to client")
                break
            
            if msg == "closeSV":
                clientSocket.close()
                TCPServerSocket.close()
                print("close server")
                break
        
        switchState = not switchState
        print("server is waiting for incoming data")


    
 
        
