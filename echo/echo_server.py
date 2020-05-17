import socket

HOST = '127.0.0.1'  # Standard loopback interface address (any traffic that is addressed to the loopback IP address is addressed to the same computer)
PORT = 65432        # Port to listen on (non-privileged ports are > 1023)

# because we are using the context manager type of socket, we don't have to call close()
# AF_INET specifies IPv4 address family, SOCK_STREAM is the socket type for TCP.
# bind() is used to associate the socket with a specific network interface
with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    s.bind((HOST, PORT))
    s.listen()
    conn, addr = s.accept()
    with conn:
        print("Connected by", addr)
        while True:
            data = conn.recv(1024)
            if not data:
                break
            conn.sendall(data) #send the data back