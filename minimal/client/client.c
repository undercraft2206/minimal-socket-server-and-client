#include <arpa/inet.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define PORT 7471

#define TCP SOCK_STREAM 
#define UDP SOCK_DGRAM

#define IPV4 AF_INET
#define IPV6 AF_INET6

#define ANY_INTERFACE INADDR_ANY

// exemplain:

// SOL_SOCKET = socket-level option level used with getsockopt and setsockopt to configure socket-wide settings like broadcast, keep-alive, or exclusive address use, exemple:
//   - SO_BROADCAST : Allows sending of broadcast messages
//   - SO_KEEPALIVE : Enables keep-alive messages to maintain the connection
//   - SO_EXCLUSIVEADDRUSE: Prevents other sockets from binding to the same address and port

// SO_REUSEADDR = bind to a port already in use by another socket, which can help avoid "address already in use" errors during server restarts

// SO_REUSEPORT = socket option that allows multiple sockets to bind to the same address and port, enabling load balancing among them. This option is particularly useful for improving the performance of server applications by allowing them to handle more connections simultaneously

// valread = store the number of bytes read from a socket or file using the read() function

int main(int argc, char const* argv[]){

    // clear terminal
    system("clear");
    
    // make var and struct and buf
    int status, valread, client_fd;

    char buffer[1024] = { 0 };
    char ip[15];
    char* hello = "test this is a test, from client";
    
    struct sockaddr_in serv_addr;

    // ask ip addresse of server
    printf("ip:\n>");
    scanf("%s", ip);

    // initial client socket 
    client_fd = socket(IPV4, TCP, 0);

    // look if client socket have been initialed
    if(client_fd < 0){
        perror("socket failed on start");
        return -1;
    }

    // Use IPV4 and port
    serv_addr.sin_family = IPV4;
    serv_addr.sin_port = htons(PORT);

    // change ip (like 192.168.1.4) to binarie
    if(inet_pton(IPV4, ip, &serv_addr.sin_addr) <= 0){
        perror("Invalide addresse / addresse not supported");
        return -1;
    }

    // check status if client connected
    if ((status = connect(client_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr))) < 0) {
        printf("\nConnection Failed \n");
        return -1;
    }

    // send message to the server
    send(client_fd, hello, strlen(hello), 0);
    printf("Hello message sent from client\n\n");
    
    // read buffer
    valread = read(client_fd, buffer, 1024 - 1); 
    printf("%s\n", buffer);

    close(client_fd);
    return 0;
}