import socket

server_port = 12000
server_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
server_socket.bind(('localhost', server_port))

print("The UDP server is ready to receive messages on port {port}".format(port=server_port))
while True:
  message, client_address = server_socket.recvfrom(2048)
  print("Received a message from client address: {client}".format(client=client_address))
  modified_message = message.upper()
  server_socket.sendto(modified_message, client_address)