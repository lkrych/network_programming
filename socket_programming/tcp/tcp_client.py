import socket

server_name = 'localhost'
server_port = 12000
client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

#initialize the tcp connection between the client and server
client_socket.connect((server_name, server_port))

message = input("Input lowercase sentence:")
client_socket.send(message.encode())
modified_message = client_socket.recv(1024)

print("Message from server: {message}".format(message=modified_message.decode()))
client_socket.close()

