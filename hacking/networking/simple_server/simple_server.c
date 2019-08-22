#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "hacking.h"

#define PORT 7890 // the port users will connect to

void fatal(char *); //a function for fatal errors see below

int main(void) {
    int sockfd, new_sockfd; // listen on sock_fd, new connection on new_fd
    struct sockaddr_in host_addr, client_addr; // Address information
    socklen_t sin_size;
    int recv_length=1, yes=1;
    char buffer[1024];

    // set up a streaming (TCP/IP) socket for IPv4 (PF_INET)
    // socket returns a socket file descriptor which is saved in sockfd
    if ((sockfd = socket(PF_INET, SOCK_STREAM, 0)) == -1) {
        fatal("in socket");
    }

    // sets socket options
    // sets SO_REUSEADDR to true, which will allow it to reuse a given address for binding
    // w/o this option set, when the program tries to bind a given port
    // it will fail if that port is already in use
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
        fatal("setting socket option SO_REUSEADDR");
    }


    // set up the host_Addr struct for use in the bind call
    // the port must be converted to network byte order
    // the address is set to 0, which means it will automatically be filled with
    // the hosts current IP address
    host_addr.sin_family = AF_INET; // Host byte order
    host_addr.sin_port = htons(PORT); // Short, network byte order
    host_addr.sin_addr.s_addr = 0; // automatically fill with my IP
    memset(&(host_addr.sin_zero), '\0', 8); // Zero the rest of the struct

    // bind the socket to the current IP address and port 7890
    if (bind(sockfd, (struct sockaddr *)&host_addr, sizeof(struct sockaddr)) == -1) {
        fatal("binding to socket");
    }

    // tells the socket to listen for incoming connections
    // places all incoming connections into a backlog queue until an
    // accept call accepts the connections
    // 5 is the max size of the backlog queue
    if (listen(sockfd, 5) == -1) {
        fatal("listening on socket");
    }

    while(1) { // Accept loop
        sin_size = sizeof(struct sockaddr_in);
        // the accept function returns a new socket file descriptor for the
        // accepted connection. This way, the original socket file descriptor
        // can be used for accepting new connections
        new_sockfd = accept(sockfd, (struct sockaddr *)&client_addr, &sin_size);
        if (new_sockfd == -1) {
            fatal("accepting connection");
        }
        printf("Server: got connection from %s port %d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
        send(new_sockfd, "Hello, world!\n", 13, 0);
        recv_length = recv(new_sockfd, &buffer, 1024, 0);
        while(recv_length > 0) {
            printf("RECV: %d bytes\n", recv_length);
            dump(buffer, recv_length);
            recv_length = recv(new_sockfd, &buffer, 1024, 0);
        }
        close(new_sockfd);
    }
    return 0;
}

// a function to display an error message and then exit
void fatal(char *message) {
    char error_message[100];

    strcpy(error_message, "[!!] Fatal Error ");
    strncat(error_message, message, 83);
    perror(error_message);
    exit(-1);
}