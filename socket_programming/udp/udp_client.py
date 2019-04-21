import socket

#setup the constants for the client
server_name = 'localhost'
server_port = 12000
client_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

#ask for input from user and send to server
message = input("Input lowercase sentence:")
client_socket.sendto(message.encode(), (server_name, server_port))

#receive input from server and output to user
modified_message, server_address = client_socket.recvfrom(2048)
print(modified_message.decode())
client_socket.close()