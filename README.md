# SocketChatApp
 A simplified client-server chat application developed using socket programming with the c programming language.

## Features
1. Written with the c programming language.
2. Uses Unix based socket libraries

## How to use
1. Open the Linux Terminal
2. [OPTIONAL] Run <code>sudo apt-get update</code> to run an update.
3. compile the 'chatserver.c' to object code using the command <code>gcc chatserver.c -o [servername]</code>
4. compile the 'chatclient.c' to object code using the command <code>gcc chatclient.c -o [clientname]</code>
5. Run the 'chatserver' first using the command <code>./servername</code>
6. Run the 'chatclient' using the command <code>./clientname [hostname] [chatname]</code>
 
 ## References
 1. Unix Network Programming vol 1. 3rd Ed. by W. Richard Stevens, Bill Fenner, Andrew M. Rudoff
 2. Beej's Guide to Network Programming Using Internet Sockets by Brian Beej Jorgensen
