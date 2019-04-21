import socket

server_port = 12000
#server_socket is the welcoming socket for any new client
server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server_socket.bind(('localhost', server_port))
server_socket.listen(1)

print("The TCP server is ready to receive messages on port {port}".format(port=server_port))

while True:
  #connection_socket is the new socket dedicated to the client
  connection_socket, addr = server_socket.accept()
  message = connection_socket.recv(1024)
  print("Received a message from client address: {client}".format(client=addr))
  modified_message = message.upper()
  connection_socket.send(modified_message)
  connection_socket.close()