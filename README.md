# FT_IRC

The goal of this exercice is to create an IRC server in C++, fully compatible with an official client. To do so, we will need to understand socket programming.

## Authorized functions
| | FUNCTIONS | |
| --- | --- | --- |
| Everything in C++ 98 | [socket](http://manpagesfr.free.fr/man/man2/socket.2.html) | [man](http://manpagesfr.free.fr/man/man2/close.2.html) |
| [setsockopt](https://linux.die.net/man/2/setsockopt) | [getsockname](http://manpagesfr.free.fr/man/man2/getsockopt.2.html) | [getprotobyname](https://linux.die.net/man/3/getprotobyname) |
| [gethostbyname](https://man7.org/linux/man-pages/man3/gethostbyname.3.html) | [getaddrinfo](http://manpagesfr.free.fr/man/man3/getaddrinfo.3.html) | [freeaddrinfo](https://man7.org/linux/man-pages/man3/freeaddrinfo.3p.html) |
| [bind](http://manpagesfr.free.fr/man/man2/bind.2.html) | [connect](http://manpagesfr.free.fr/man/man2/connect.2.html) | [listen](http://manpagesfr.free.fr/man/man2/listen.2.html) |
| [accept](http://manpagesfr.free.fr/man/man2/accept.2.html) | [htons](https://linux.die.net/man/3/htons) | [htonl](https://linux.die.net/man/3/htonl) |
| [ntohs](https://fr.manpages.org/ntohs/3) | [ntohl](https://linux.die.net/man/3/ntohl) | [inet_addr](https://linux.die.net/man/3/inet_addr) |
| [inet_ntoa](https://linux.die.net/man/3/inet_ntoa) | [send](manpagesfr.free.fr/man/man2/send.2.html) | [recv](http://manpagesfr.free.fr/man/man2/recv.2.html) |
| [signal](http://manpagesfr.free.fr/man/man2/signal.2.html) | [sigaction](manpagesfr.free.fr/man/man2/sigaction.2.html) | [lseek](http://manpagesfr.free.fr/man/man2/lseek.2.html) |
| [fstat](https://linux.die.net/man/2/fstat) | [fcntl](http://manpagesfr.free.fr/man/man2/fcntl.2.html) | [poll](http://manpagesfr.free.fr/man/man2/poll.2.html) |


## What is socket programming

### What are socket

A socket is a file descriptor that enable remote communication. The type of Internet socket that interest us is

- **Stream Socket** which use TCP in order to communicate. This protocol enforces a reliable and connect transmission of data, at the cost of a slightly reduce perfomance.

### The importance of Byte order

Whenever we wish to send and receive data from one computer to another, we must keep in mind that systems can represent their data in two distinct and opposite ways.
For example, take the hexadecimal integer 2F0A. This must be stored over two bytes: 2F and 0A.

It would be logical to assume this integer will always be stored in this order: 2F, followed by 0A. This is the most common ordering, known as **big endian** since the big end of the number, the most significant byte is stored first. But this is not always...

In some systems, particularly those with an Intel prefer storing the bytes of our integer in the opposite order with the small en first: 0A followed by 2F. We call this ordering **little endian**.

This potentially incompatible diffrence between host systems can of course cause some issues during data transfer.

The Network Byte Order is always big endian. But the Host Byte Order can be either big endian, or little endian, depending on its architecture.

### Converting the Network Byte Order

Thankfully we can simply assume that the host system is not storing its byte in the right order compared to the network. All we need to do is to systematically reorder these bytes when we transfer them between the network and host systems. For this we can use

1. htonl
2. htons
3. ntohl
4. ntohs

As we can see, these functions come in two variants: the ones that convert a _short_ (16 bits), and those that convert a _long_ (32 bits).

In order to convert a four byte (32 bit) integer from the Host Byte Order to the Network Byte Order, we’ll want to call the htonl() function (“htonl” stands for “Host to Network Long”). For the opposite operation, we’d use ntohl() (“Network to Host Long”).

_see microserver.c_

### Sending data via a socket

While the read() and write() system calls could be used to send and receive data, since sockets are just file descriptors, other functions exists. Functions that gives us more control over the way our data is sent and received.

- the _send()_ function allow us to send data through a TCP connection. It returns the number of bytes that were successfully sent. This means we can check the value returned with the lenght of the message we wish to send.

- the _recv()_ function allow us to receive data throught a socket (either TCP or UDP) connection. Just like _send()_, _recv()_ returns the number of bytes managed to store in the buffer. 

### Closing a socket connection

Once we are done sending and receiving data, we will be able to close our socket. Just as any other file descriptor, a socket can be closed with a simple call to _close()_. This destroys the file descriptor and prevents any further communication with the socket.

## Sources

[Codequoi](https://www.codequoi.com/en/sockets-and-network-programming-in-c/)

[Geeksforgeeks](https://www.geeksforgeeks.org/socket-programming-in-cc-handling-multiple-clients-on-server-without-multi-threading/)

[vildac](http://vidalc.chez.com/lf/socket.html)
