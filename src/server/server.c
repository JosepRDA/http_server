#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>

#define PORT 8080
#define BUFFER_SIZE 1024

//todo: add forks to the listen function, making the server concurrent and not iterative

int main(void)
{
    int server_fd, client_fd;
    struct sockaddr_in address = {
        .sin_family = AF_INET, // IPv4
        .sin_addr.s_addr = INADDR_ANY, // bind to all available network interfaces
        .sin_port = htons(PORT) // setting the port to 8080
    };
    int addrlen = sizeof(address);

    // creating the socket
    server_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if ( server_fd  == -1 ) {
        perror("Socket failed.");
        exit(EXIT_FAILURE);
    }

    if ( bind(server_fd, (struct sockaddr*)&address, addrlen) == -1 ) {
        perror("bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // this is an iterative server
    if ( listen(server_fd, 5) == -1 ) {
        perror("Listen failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("The server is listening on port 8080...\n");

    client_fd = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen);
    if ( client_fd == -1 ) {
        perror("Accept failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Connection accepted from client\n");

    while(1) {
        //char buffer[BUFFER_SIZE] = { 0 };
        //recv(server_fd, buffer, sizeof(buffer), 0);
        break; 
    }

    // closing the server
    close(server_fd);

    // closing the client
    close(client_fd);

    return 0;
}
