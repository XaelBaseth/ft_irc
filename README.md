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

## What is IRC

IRC (Internet Relay Chat) is  an application-level Protocol for exchanging messages in real-time. While, primarly designated for group communication, it also allows you to communicate via private messages and exchange data via the TCP protocol, and cryptographic TSL (optional).

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

### Handling multiples connections

As the functions are blocking, if we run the mini_server, the system will wait for a connection for a connection request that will never come.

This is not a bad thing, but it can cause issues for a server that attempts to handle several clients at the same time. If we are waiting to receive data from a client, it should not stop us from accepting a new connection or another message from another client.

Thankfully we can use **multiplexing** methods that can make our sockets non-blocking. When we invoke the socket() system call to get a file descriptor for our socket, the operating system’s kernel automatically creates it as “blocking”. If we so desire, we can transform it into a non-blocking socket with the file descriptor manipulation function _fcntl()_.

In order to do so, we can use the **poll()** function. It is used to monitor several file descriptors to see if any of them are ready for I/O operations. IT returns the numbers of file descriptors that have events pending. Naturally, the _poll()_ function returns -1 on failure, and indicate the error in errno.

### CheckLists

[ X ] Creates a server that only allows client-server communication.

[ X ] Server must be able to handle mutiple clients at the same time and never hang. (I/O operation must be non-blocking.)

[ X ] Server use only 1 **poll()** to handle read, write, listen etc. operations

[ X ] Client must be able to connect to server without any errors.

[ X ] Communication has to be done via TCP/IP(v4 or v6).

[ ] Using the client, you must be able to authenticate, set a nickname, a username, join a channel, send and receive private message.

[ ] All the message sent from one client have to be forwarded to every other client in the channel.

[ ] You must have operators and regulars users. The operator have to have specific channel operators commands:

1. KICK - Eject a client from the channel.
2. INVITE - Invite a client to a channel.
3. TOPIC - Change or view the channel topic
4. MODE - Change the channel mode:

- i: Set/Remove invite-only channel.
- t: Set/Remove the restrictions of the TOPIC command to channel operators.
- k: Set/Remove the channel key (password)
- o: Give/take channel operator privilege
- l: Set/remove the user limit to channel

[ X ] **BONUS** Handle file transfert (not sure if done properly, need to be checked.)

[ ] **BONUS** a bot (raultbot ?)

## Sources

[Subject](https://cdn.intra.42.fr/pdf/pdf/120067/en.subject.pdf)

[Codequoi](https://www.codequoi.com/en/sockets-and-network-programming-in-c/)

[Geeksforgeeks](https://www.geeksforgeeks.org/socket-programming-in-cc-handling-multiple-clients-on-server-without-multi-threading/)

[vildac](http://vidalc.chez.com/lf/socket.html)

[IRC protocol](https://modern.ircdocs.horse/)

[Socket guide pdf](https://beej.us/guide/bgnet/pdf/bgnet_a4_c_1.pdf)

[CHIRC](http://chi.cs.uchicago.edu/chirc/irc.html)