#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

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

int main(void){
    
    // clear terminal
    system("clear");

    // intialise socket at server_fd
    int server_fd = socket(IPV4, TCP, 0);

    // creating var and buf with socket accept
    char buffer[1024] = { 0 };
    int opt = 1;
    int new_socket;
    char* test = "\n\nthis is a test ! (from server)\n\n";

    ssize_t valread;

    // make struct address
    struct sockaddr_in address;

    // look if any failure at socket start
    if((server_fd) < 0){
        perror("socket failed the start :(");
        exit(EXIT_FAILURE);
    }

    // force attache port (optionnal)
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    // give IPV4 on any interface on port who is defined on the top
    address.sin_family = IPV4;
    address.sin_addr.s_addr = ANY_INTERFACE;
    address.sin_port = htons(PORT);

    // bind to port
    if(bind(server_fd, (struct sockaddr*)&address, sizeof(address))){
        perror("failed to bind :(");
        exit(EXIT_FAILURE);
    }

    // listen
    if(listen(server_fd, 3) < 0){
        perror("listen failure :(");
        exit(EXIT_FAILURE);
    }

    // accept user incomming
    if((new_socket = accept(server_fd, (struct sockaddr*)&address, &address)) < 0){
        perror("Failed to accept");
        exit(EXIT_FAILURE);
    }

    valread = read(new_socket, buffer, 1024-1);

    printf("\n%s", buffer);
    send(new_socket, test, strlen(test), 0);
    printf("\n\ntest message send");
    
    close(new_socket);
    close(server_fd);

    return 0;
}