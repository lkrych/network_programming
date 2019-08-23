#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <netdb.h> //for dns stuff

// #include <hacking.h>

int main(int argc, char *argv[]) {
    struct hostent *host_info;
    struct in_addr *address;

    if (argc < 2) {
        printf("Usage: %s <hostname>\n", argv[0]);
        exit(1);
    }
    // gethostbyname returns a pointer to a hostent struct,
    // which contains thue IP address in element h_addr
    // on line 28 a pointer of the struct is typecast into an in_addr pointer
    // which is later dereferenced for the call to inet_toa
    host_info = gethostbyname(argv[1]);
    if (host_info == NULL) {
        printf("Couldn't lookup %s\n", argv[1]);
    } else {
        address = (struct in_addr *) (host_info->h_addr);
        printf("%s has address %s\n", argv[1], inet_ntoa(*address));
    }
}