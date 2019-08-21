#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "hacking.h"

#define PORT 7890 // the port users will connect to

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

    if (listen(sockfd, 5) == -1) {
        fatal("listening on socket");
    }
}