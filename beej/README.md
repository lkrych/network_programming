# Beej's guide to socket programming

## Table of Contents


### What is a socket?

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

Packets that are sent across networks have to be **encapsulated**. To route across the network they need to use IP, to move from computer to computer they need to be encapsulated in a Data Link packet, and they need to be physically transmitted across a wire or through glass. 

The nice thing about socket programming is that you don't really need to care about how all of this lower-level stuff is done because programs on lower level deal with it for you!
