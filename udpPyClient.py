import socket
import threading

msgFromClient = "hello udp server"
bytesToSend = msgFromClient.encode('utf-8')

serverAddressPort = ("192.168.1.199", 2002)
bufferSize = 1024

#C1
# clientIP = "192.168.1.79"
# clientPort = 20015


UDPClientSocket = socket.socket(family=socket.AF_INET, type=socket.SOCK_DGRAM)

# UDPClientSocket.bind((clientIP, clientPort))

#C2
UDPClientSocket.sendto(bytesToSend, serverAddressPort)
print("udp client up and listening")

def ReceiveThread():
    while True:
        try:
            bytesAddressPair = UDPClientSocket.recvfrom(bufferSize)
            message = bytesAddressPair[0].decode('utf-8')
            address = bytesAddressPair[1]
            
            print(address)
            print(message)
        except:
            print("client close")
            break

thread = threading.Thread(target=ReceiveThread, args=())
thread.start()

while True:
    msg = input()
    bytesToSend = msg.encode('utf-8')
    try:
        UDPClientSocket.sendto(bytesToSend, serverAddressPort)
    except:
        print("client close")
        break
    if (msg == "close"):
        UDPClientSocket.close()