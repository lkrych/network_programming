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

Once you have a socket you might have to associate that socket with a port on your local machine.