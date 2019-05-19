# Socket Programming

### Client-Server implementations

*implemented in tcp/udp folders*

Simple client-server implementations done in conjunction with reading Computer Networking from the top down (Kurose and Ross)

All implementations were modified to use Python 3.7

1. The client will read a line of characters from the keyboard and send the data to the server
2. The server will receive the data and convert the characters to uppercase
3. the server sends the modified data back to the client
4. The client receives the modified data and display the lines on its screen


### Web Server implementation

*implemented in web_server folder*

1. Implementation creates a connection socket when contacted by a client (browser)
2. server receives the HTTP request from this connection
3. server parses the request to determine which file is being requested
4. server gets the requested file from the system
5. creates an HTTP message consisting of the requested file preceded by headers
6. sends the response over the TCP connection to the browser

If the file requested cannot be found, the server should send a 404 Not Found message

### UDP Pinger

*implemented in udp_ping folder*

Client will send a simple ping message to a server, receiving a corresponding pong message back from the server, and determine the delay between when the client sent the ping message and received the pong message. This delay is called the Round Trip Time (RTT). The functionality provided by the client and server is similar to the functionality provided by the standard ping program availiable in most operating systems. However standard ping programs use the Internet Control Message Protocol (ICMP), instead of UDP.

1. Client sends 10 ping messages to the target server over UDP
2. For each message, the client determines and prints the RTT when the corresponding Pong message is returned. 
3. The client will wait for one second, if no reply is received, the client should assume the packet is lost and print a message.

### Mail Client

*implemented in mail_client folder* (are you catching the pattern yet?)

The goal of this programming assignment is to create a simple mail client that sends email to any recipient.

The client will need to:

1. Establish a TCP connection with a mail server.
2. Dialogue with the mail server using the SMTP protocol.
3. Send an email message to a recipient via the mail server
4. Close the mail server

Specifically this code allows for a message to be sent to GMAILs smtp server

### Mail Server

*implemented in the mail_server folder*

The goal of this programming assignment is to show how to use the python standard library to create a mail server that recieves an email from a mail client. The mail server will print out the dialog between the two programs.

### Web Proxy

*implemented in the web_proxy folder*

The goal of this assignment is to develop a Web proxy that intercepts traffic and redirects it to the proper 
server. The proxy should also redirect the response from the server back to the client. This proxy should be 
multi-threaded so that it will handle multiple requests at the same time. 
