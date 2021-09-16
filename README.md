# SocketChatApp
 A simplified client-server chat application developed using socket programming with the c programming language.

## Features
1. Written with the c programming language.
2. Uses Unix based socket libraries
3. Concurrent server allows for multiple clients

## Requirements
1. A Unix based OS e.g. Ubuntu, Kali Linux etc
2. Two open terminals with one running the server program and the other the client program.
3. Hostname or IP address of the server must be provided for the client to connect to it.

## How to use
1. Open the Linux Terminal
2. [OPTIONAL] Run <code>sudo apt-get update</code> to run an update.
3. compile the 'chatserver.c' to object code using the command <code>gcc chatserver.c -o [servername]</code>
4. compile the 'chatclient.c' to object code using the command <code>gcc chatclient.c -o [clientname]</code>
5. Run the 'chatserver' first using the command <code>./servername</code>
6. Run the 'chatclient' using the command <code>./clientname [hostname] [chatname]</code>
7. Server program must start the chatting process.

## Limitations
1. Server must initiate the chat before client can respond
2. It uses a form of half-duplex communication.

## References
1. Unix Network Programming vol 1. 3rd Ed. by W. Richard Stevens, Bill Fenner, Andrew M. Rudoff
2. Beej's Guide to Network Programming Using Internet Sockets by Brian Beej Jorgensen
