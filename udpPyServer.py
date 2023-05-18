import socket
import threading
from time import sleep

clientlst = []
ServerIP = "192.168.1.65"
ServerPort = 1990
bufferSize = 1024

msgFromServer = "Hello UDP client"
bytesToSend = msgFromServer.encode('utf-8')

UDPServerSocket = socket.socket(family=socket.AF_INET, type=socket.SOCK_DGRAM)

UDPServerSocket.bind((ServerIP, ServerPort))

print("UDP server up and listening")

def ReceiveThread():
    while True:
        try:
            bytesAddressPair = UDPServerSocket.recvfrom(bufferSize)
            message = bytesAddressPair[0].decode('utf-8')
            global clientAddress 
            clientAddress = bytesAddressPair[1]

            print(clientAddress)
            print(message)

            if clientAddress not in clientlst:
                clientlst.append(clientAddress)
            
            for addr in clientlst:
                UDPServerSocket.sendto(bytesToSend, addr)
        except:
            print("client close")
            break

thread = threading.Thread(target=ReceiveThread, args=())
thread.start()

while True:
    msg = input()
    bytesToSend_2 = msg.encode('utf-8')
    try:
        UDPServerSocket.sendto(bytesToSend_2, clientAddress)
    except: 
        print("no connection")