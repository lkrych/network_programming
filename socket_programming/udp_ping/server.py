import socket
import time

server_port = 12000
server_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
server_socket.bind(('localhost', server_port))

print("The UDP server is ready to receive ping messages on port {port}".format(port=server_port))
while True:
  message, client_address = server_socket.recvfrom(2048)

  if message.decode() == "ping":
    print("Received a ping message from client address: {client}".format(client=client_address))
    server_socket.sendto("pong".encode(), client_address)
  