import socket

HOST = '127.0.0.1'  # the server's hostname or IP address
PORT = 65432        # the port used by the server to accept connections (typically 80 or 443 for HTTP and HTTPS respectively)

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    s.connect((HOST, PORT))
    s.sendall(b'Echo! Echo! Echo!')
    data = s.recv(1024)

print('Received', repr(data))