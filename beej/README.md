# Beej's guide to socket programming

## Table of Contents
* [What is a socket](#what-is-a-socket?)
    * [How do I use a socket](#how-do-i-use-a-socket?)
    * [Types of Internet Sockets](
        #types-of-internet-sockets)
* [IP Addresses and Structs](#ip-addresses-structs-and-data-organization)
    * [IPv4 and IPv6 address review](#ipv4-and-ipv6-addresses)
    * [Subnets](#subnets)
    * [Port Numbers](#port-numbers)
    * [Byte Order](#byte-order)
    * [Structs](#structs)
        * [addrinfo](#addrinfo)
        * [sockaddr](#sockaddr)
        * [sockaddr_in](#sockaddr_in)
    * [IP address helper functions](#ip-addresses-and-helper-functions)
* [System Calls](#system-calls)
    * [getaddrinfo()](#getaddrinfo())
    * [socket()](#socket())
    * [bind()](#bind())
    * [connect()](#connect())
    * [listen()](#listen())
    * [accept()](#accept())
    * [send() and recv()](#send()-and-recv())
    * [sendto() and recvfrom()](#sendto()-and-recvfrom())
    * [close() and shutdown()](#close()-and-shutdown())
## What is a socket?

A **socket** is a way to speak to other programs using standard *Unix file descriptors*. 

When Unix programs do any sort of I/O, they do it by reading or writing to a file descriptor. A **file descriptor** is simply an integer associated with an open file. The catch here is that everything is a file, a network connection, a pipe, a terminal, a real on-the-disk file. They're all files!

### How do I use a socket?

**To retrieve a file descriptor for network communication**, you make a call to the `socket()` system routine.

You can communicate through it using the specialized `send()` and `recv()` socket calls. 

Why not use `read()` and `write()` calls if it is a file descriptor? Because `send()` and `recv()` will offer you greater control over data transmission. 

### Types of Internet Sockets

Raw sockets are very powerful, but we won't be discussing them. That's future work on my part. 

The two types of Internet sockets are:
1. **Stream Sockets** (SOCK_STREAM) - reliable two-way connected communication streams. Packets will arrive in order and error-free at the opposite end. They achieve this by using **TCP**.
2. **Datagram Sockets** (SOCK_DGRAM) - Connectionless, packets may arrive out-of-order and unreliable. These sockets use **UDP**. Nice for speed.

Both socket types use IP for routing.

### Going Deeper 

<img src="resources/encapsulation.png">

Packets that are sent across networks have to be **encapsulated**. To route across the network they need to use IP, to move from computer to computer they need to be encapsulated in a Data Link packet, and they need to be physically transmitted across a wire or through glass. 

The nice thing about socket programming is that you don't really need to care about how all of this lower-level stuff is done because programs on lower level deal with it for you!

## IP Addresses, Structs and Data organization

### IPv4 and IPv6 addresses

IPv4 Addresses are made up of four bytes and are commonly written in dots and numbers form likes so: `192.0.2.111`

IPv6 addresses uses hexadecimal representation, with each two-byte chunk separated by a colon like this: `200:0db8:c9d2:aee5:73e3:934a:a5ae:9551`.

Lot's of times you'll have an IP address with lots of zeros in it. You can compress an IPv6 address segment with all zeros to just two colons. You can also leave off leading zeros for each byte pair. 

```bash
#compressing zeros
2001:0db8:c9d2:0012:0000:0000:0000:0051
2001:db8:c9d2:12::51

## removing leading zeros
2001:0db8:ab00:0000:0000:0000:0000:0000
2001:db8:ab00::

#localhost
0000:0000:0000:0000:0000:0000:0000:0001
::1
```

The address `::1` is the **loopback address**. It always means "this machine I'm running on now". In IPv4 the loopback address is `127.0.0.1`.

### Subnets

For organizations reasons, it's sometimes convenient to declare that "this first part of the IP address up through this bit is the **network portion** of the IP address, and the remainder is the **host portion**".

For instance, with IPv4, you might have `192.0.2.12`, and we could say that the first three bytes are the network and the last byte was the host. To put it another way, we're talking about host 12 on network `192.0.2.0`.

The network portion of the IP address is described by something called the **netmask**, which you can **bitwise AND with the IP address** to get the network number out of it. The netmask is allowed to be an arbitrary number of bits. The netmask is always a bunch of 1-bits followed by a bunch of 0-bits. 

It's a bit unwieldy to to use a big string of numbers as a netmask. You just put a slash after the IP address, and then follow that by the number of network bits in decimal : `192.0.2.12/30`.

### Port Numbers

Aside from an IP address, there is another address that is used by TCP and UDP sockets. It is the **port number**. It's a 16-bit number that's like the local address for the connection.

You can think of the IP address as the street address for a hotel, and the port number as the room number.

### Byte Order

Most people generally agree that if you want to represent the two-byte hex number `b34f`, you'll store it in two sequential bytes: `b3` and `4f`. This number, stored with the big end first, is called **Big-Endian**. 

Unfortunately, some computers, namely anything with an Intel or Intel-compatible processor, stores the bytes reversed, so `b34f` would be stored in memory as `4f` followed by `b3`. This is called **Little-Endian**. 

**Big-Endian** is also called **Network Byte Order**. Because that's what networks like. 

A lot of the times when you're building packets or filling out data structures, you'll need to make sure your numbers are in Network Byte Order. 

There are two types of numbers you can convert, **short** (2-byte) and **long** (4-byte). To convert a short from Host Byte Order to Network Byte Order, you use the `htons()` function.
1. `htons()` - host to network short
2. `htonl()` - host to network long
3. `ntohs()` - network to host short
4. `ntohl()` - network to host short

### Structs

Yay, it's time to start talking about programming. 

Socket descriptors are of type `int`. Things get weird after this. Thanks for the simple descriptor interface though!

#### addrinfo

The first struct we will talk about is `struct addrinfo`. This struct is used to prep the socket address structures for subsequent use. It's also used for host name lookups and service name lookups. 

This is one of the first things you'll call when making a connection.

```c
struct addrinfo {
    int                 ai_flags; 
    int                 ai_family;
    int                 ai_socktype;
    int                 ai_protocol;    // use 0 for "any"
    size_t              ai_addrlen;     // size of ai_addr in bytes
    struct sockaddr     *ai_addr;       // struct sockaddr_in
    char                *ai_canonname;  // full canonical hostname

    struct addrinfo     *ai_next;       // linked list, next node
}
```
You will load this struct up a bit, and then call `getaddrinfo()`. It'll return a pointer to anew linked list of these structures filled out with everything you need.

The ai_family field is used to specify IPv4 or IPv6. 

#### sockaddr

The `struct sockaddr` holds socket address information for many types of sockets. 

```c
struct sockaddr {
    unsigned short          sa_family;          // address family, AF_xxx
    char                    sa_data[14];        // 14 bytes of protocol address
}
```

* `sa_family` can be a variety of things, but it will be `AF_INET` (IPv4) or `AF_INET` (IPV6) in this guide.
* `sa_data` contains a destination address and port number for the socket. This is rather unwieldy since you don't want to pack the address by hand. To deal with `struct sockaddr`, programmers created a parallel structure: struct `sockaddr_in` ("in" for Internet) to be used with IPv4. 

A pointer to `struct sockaddr_in` can be cast to a pointer to a `struct sockaddr` and vice-versa. This means you can use a `struct sockaddr_in` in the call to `connect()`.

#### sockaddr_in

```c
// This is for IPv4 only, see struct sockaddr_in6 for IPv6

struct sockaddr_in {
    short int                   sin_family;         // Address family, AF_INET
    unsigned short int          sin_port;           // Port number
    struct in_addr              sin_addr;           // Internet address
    unsigned char               sin_zero[8];         Sam size as struct sock_addr
}
```

This structure makes it easy to reference elements of the socket address. sin_Zero is included to to pad the structure to the length of struct sockaddr. It should be set to all zeros with the function `memset()`. `sin_port` must be in Network Byte Order!

```c
struct in_addr {
    uint32_t s_addr; // a 32-bit int (4 bytes)
}
```

Last, but not least, there is a simple structure, `struct sockaddr_storage` that is designed to be large enough to hold both IPv4 and IPv6 structures.

### IP Addresses and Helper Functions

Let's say you have a `struct sockaddr_in`, let's call it `ina`. We have an IP addresses `10.12.110.57` and `2001:db8:63b3:1::3490` and we want to store into the struct. 

The function `inet_pton()` (presentation to network) converts an IP address in numbers-and-dots notation into either a `struct in_addr` or `struct in_6addr` depending on the family.

```c
struct sockaddr_in sa;   //IPv4
struct sockaddr_in6 sa6; //IPv6

inet_pton(AF_INET, "10.12.110.57", &(sa.sin_addr));
inet_pton(AF_INET6, "2001:db8:63b3:1::3490", &(sa6.sin6_addr));
```

Okay, now we can convert string IP addresses into their binary representations, what about the other way around? The function we want to use is `inet_ntop()` (network to presentation).

```c
//IPv4:

char ip4[INET_ADDRSTRLEN]; // space to hold the IPv4 string
struct sockaddr_in sa;

inet_ntop(AF_INET, &(sa.sin_addr), ip4, INET_ADDRSTRLEN);

printf("The IPv4 address is: %s\n" ip4);

// IPv6:
char ip6[INET6_ADDRSTRLEN]; // space to hold the IPv6 string
struct sockaddr_in6 sa6; // pretend this is loaded with something
inet_ntop(AF_INET6, &(sa6.sin6_addr), ip6, INET6_ADDRSTRLEN); printf("The address is: %s\n", ip6);
```

A quick word about **private networks**. The details of which private network numbers are available to you are outlined in RFC1918, but some common ones you'll see for IPv4 are `10.x.x.x` and `192.168.x.x`. Less commonly you will see `172.y.x.x` where y goes between 16 and 31.

IPv6 networks have private nteworks too, they'll start with `fdxx:`

## System Calls

In this section we talk about system calls (and other library calls) that allow you to access the network functionality of a Unix box, or any other box that supports the sockets API. 

When you call one of these functions, the kernel takes over and does all the work for you automagically. 

The place where most people get stuck is around what order to call these things in. 

<img src='resources/server-client-system-calls'>

### getaddrinfo()

This is a workhorse of a function. *It helps to set up structs you need later on.*
It does all sorts of ice things for you including DNS and service name lookups and fills out the structs you need. 

```c
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

int getaddrinfo(const char *node        //"www.example.com" or IP
                const char *service     // "http" or port number
                const struct addrinfo *hints,
                struct addrinfo **res);
```

You gives this function three input parameters and it gives you a pointer to a linked list, `res`, of results.
1. node - is the host name to connect to, or an IP address.
2. service - can be a port number, or the name of a particular service like 'http' or 'ftp'. 
3. hints - points to a struct addrinfo that you've already filled out with relevant information.

`freeaddrinfo()` is used to free up the getaddrinfo linked list allocated for us. 

```c
int status;
struct addrinfo hints;
struct addrinfo *servinfo;  // will point to the results
memset(&hints, 0, sizeof hints); // make sure the struct is empty hints.ai_family = AF_UNSPEC; // don't care IPv4 or IPv6 hints.ai_socktype = SOCK_STREAM; // TCP stream sockets
// get ready to connect
status = getaddrinfo("www.example.net", "3490", &hints, &servinfo);
// servinfo now points to a linked list of 1 or more struct addrinfos // etc.
```

### socket()

The socket system call returns the file descriptor. 

```c
#include <sys/types.h>
#include <sys/socket.h>

int socket(int domain, int type, int protocol);
```

1. domain - what kind of socket you want (IPv4 of IPv6).
2. type - stream or datagram.
3. protocol - UDP or TCP.

It returns a socket descriptor that you can use in later system calls, or -1 on error.

```c
int s;
struct addrinfo hints, *res;
// do the lookup
// [pretend we already filled out the "hints" struct] getaddrinfo("www.example.com", "http", &hints, &res);

// [again, you should do error-checking on getaddrinfo(), and walk // the "res" linked list looking for valid entries instead of just // assuming the first one is good (like many of these examples do.) // See the section on client/server for real examples.]
s = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
```

### bind()

Once you have a socket you might have to associate that socket with a port on your local machine. This is commonly done if you're going to `listen()` for incoming connections on a specific port.

The port number is used by the kernel to match an incoming packet to a certain process's socket descriptor. 

```c
#include <sys/types.h>
#include <sys/socket.h>

int bind(int sockfd, struct sockaddr *my_addr, int addrlen);
```
1. sockfd - the file descriptor returned by `socket()`.
2. myaddr - a pointer to the struct that contains information about your address, namely, port and IP address.
3. addrlen - the length in bytes of the address

```c
struct addrinfo hints, *res;
int sockfd;
// first, load up address structs with getaddrinfo():
memset(&hints, 0, sizeof hints);
hints.ai_family = AF_UNSPEC; // use IPv4 or IPv6, whichever hints.ai_socktype = SOCK_STREAM;
hints.ai_flags = AI_PASSIVE; // fill in my IP for me
getaddrinfo(NULL, "3490", &hints, &res);
// make a socket:
sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol); // bind it to the port we passed in to getaddrinfo():
bind(sockfd, res->ai_addr, res->ai_addrlen);
```

By using the `AI_PASSIVE` flag, we're telling the program to bind to the IP of the host it's running on. If you want to bind to a specific IP address, drop the flag and put an IP address into the first argument of `getaddrinfo()`.

### connect()

The connect function helps you connect to a remote host.

```c
#include <sys/types.h>
#include <sys/socket.h>

int connexct(int sockfd, struct sockaddr *serv_addr, int addrlen);
```
1. sockfd - the file descriptor returned by the `socket()` call.
2. serv_addr - a pointer to the sockaddr struct that contains the destination port and IP address of the remote host.
3. addrlen - the length in bytes of the server address structure

All of this information can be grabbed from the results of `getaddrinfo()`.

Here is some example code of making a socket connection to "www.example.com" on port 3490.

```c
struct addrinfo hints, *res;
int sockfd;
// first, load up address structs with getaddrinfo():
memset(&hints, 0, sizeof hints); hints.ai_family = AF_UNSPEC; hints.ai_socktype = SOCK_STREAM;
getaddrinfo("www.example.com", "3490", &hints, &res);
// make a socket:
sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
// connect!
connect(sockfd, res->ai_addr, res->ai_addrlen);
```

Be sure to check the return value from `connect()`, it'll return -1 on error and set the variable `errno`. Also note that we didn't call `bind()`. Basically we don't care about our local port number, only where we are going. *The kernel will choose a port for us.* 

### listen()

The listen call allows the code to sit and wait for incoming connections on a socket. There is a two-step process, you need to `listen()` and `accept()`.

```c
#include <sys/types.h>
#include <sys/socket.h>

int listen(int sockfd, int backlog);
```
1. sockfd - the usual file descriptor
2. backlog - the number of connections allowed on the incoming queue. Incoming connections are going to wait in this queue until you `accept()` them. Most systems silently limit this number to 20.

### accept()

Alright, it's going to get a little weird here. In the `accept()` call someone from far away is trying to `connect()` to your machine on a part that you are `listen()`ing ong. Their connection will be queued up waiting to be `accept()`ed.

`accept()` **will return to you a brand new socket file descriptor** for this single connection. That's right, suddenly we have two socket file descriptors. The original one is still listening for more new connections, and the newly created one is finally ready to `send()` and `recv()`.

```c
#include <sys/types.h>
#include <sys/socket.h>

int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
```
1. sockfd - the `listen()`ing socket descriptor.
2. addr - a pointer to a local struct where the information about the incoming connection will go.
3. addrlen - a local integer variable that should be set to the `sizeof(struct sockaddr_storage)` before its address is passed to `accept()`. `accept()` will not put more than that many bytes into addr.

```c
#include <string.h> #include <sys/types.h> #include <sys/socket.h> #include <netinet/in.h>
#define MYPORT "3490"  // the port users will be connecting to
#define BACKLOG 10     // how many pending connections queue will hold
int main(void)
{
struct sockaddr_storage their_addr; socklen_t addr_size;
struct addrinfo hints, *res;
int sockfd, new_fd;
    // !! don't forget your error checking for these calls !!
    // first, load up address structs with getaddrinfo():
memset(&hints, 0, sizeof hints);
hints.ai_family = AF_UNSPEC; // use IPv4 or IPv6, whichever hints.ai_socktype = SOCK_STREAM;
hints.ai_flags = AI_PASSIVE; // fill in my IP for me
    getaddrinfo(NULL, MYPORT, &hints, &res);
    // make a socket, bind it, and listen on it:
sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol); bind(sockfd, res->ai_addr, res->ai_addrlen);
listen(sockfd, BACKLOG);
    // now accept an incoming connection:
addr_size = sizeof their_addr;
new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &addr_size);
// ready to communicate on socket descriptor new_fd! .
```

## send() and recv()

These two functions are for communicating over stream sockets or connected datagram sockets. If you want to use regular unconnected datagram sockets, you'll need to use `sendto()` and `recvfrom()`.

```c
int send(int sockfd, const void *msg, int len, int flags);
```
1. sockfd - the file descriptor you want to send data to.
2. msg - a pointer to the data you want to send.
3. len - the length of the data you want to send in bytes.
4. flags - just set this to zero :p.

```c
char *msg = "Beej was here!"; int len, bytes_sent;

len = strlen(msg);
bytes_sent = send(sockfd, msg, len, 0);
```
`send()`returns the number of bytes actually sent out. Remember, if the value returned by `send()` doesn't match the value in `len`, it's up to you to send the rest of the string.

```c
int recv(int sockfd, void *buf, int len, int flags);
```
1. sockfd - the file descriptor you want to read data from.
2. buf - the buffer to read the information into.
3. len - the maximum length of the buffer.
4. flags - just set this to zero :p.

`recv()` returns the number of bytes read into the buffer. `recv()` can also **return 0**, this means that the remote side has closed the connection on you. 

### sendto() and recvfrom()

Since datagram sockets aren't connected to a remote host, you need to add the destination address to call the function.

```c
int sendto(int sockfd, const void *msg, int len, unsigned int flags, const struct sockaddr *to, socklen_t tolen);
```
1. sockfd - the file descriptor you want to send data to.
2. msg - a pointer to the data you want to send.
3. len - the length of the data you want to send in bytes.
4. flags - just set this to zero :p.
5. to - a pointer to a struct which contains the IP address and port you want to send to.
6. tolen - the sizeof(*to).

Just like `send()`, `sendto()` returns the amount of bytes actually sent. 

```c
 int recvfrom(int sockfd, void *buf, int len, unsigned int flags, struct sockaddr *from, int *fromlen);
```
1. sockfd - the file descriptor you want to send data to.
2. buf - the buffer to read the information into.
3. len - the maximum length of the buffer.
4. flags - just set this to zero :p.
5. from - a pointer to a local struct sockaddr_storage that will be filled with the IP address and port of the originating machine.
6. fromlen - the sizeof(*from).

`recvfrom()` returns the number of bytes received or -1 on error.

# close() and shutdown()

To close the connection you use the regular Unix file descriptor `close()`.

```c
close(int sockfd);
```
1. sockfd - the file descriptor you want to close.

This will prevent any more reads and writes to the scoket. Anyone attempting to read or write to the socket on the remote end will receive an error.

Just in case you want **a little more control over how the socket closes**, you can use the `shutdown()` function. It allows you to cut off communication in a certain direction, or both ways (just like close).

```c
int shutdown(int sockfd, int how);
```
1. sockfd - the socket file descriptor you want to shutdown.
2. how - options. 
    * 0 - Further receives are disallowed
    * 1 - Further sends are disallowed
    * 2 - Further sends and receives are disallowed

It's important to note that `shutdown` doesn't actually close the file descriptor, you still need to use `close()` to do that.