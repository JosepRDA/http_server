#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <time.h> // used for cooldowns between calls

#define PORT 8080
#define BUFFER_SIZE 1024

// todo: refactor this code in different functions
// 

int main(void)
{
    // initializing the variables
    int server_fd, client_fd;
    struct sockaddr_in address = {
        .sin_family = AF_INET, // IPv4
        .sin_addr.s_addr = INADDR_ANY, // bind to all available network interfaces
        .sin_port = htons(PORT) // setting the port to 8080
    };
    int addrlen = sizeof(address);

    // creating the socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0); // Using TCP
    if ( server_fd  == -1 ) {
        perror("Socket failed.");
        exit(EXIT_FAILURE);
    }

    printf("Socket created succesfully! file descriptor: %d\n");

    if ( bind(server_fd, (struct sockaddr*)&address, addrlen) == -1 ) {
        perror("bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Socket succesfully bound to port 8080\n");

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

    // closing the server file descriptor
    // it will still be used for listening
    close(server_fd);

    // communication with the client can now occur through client_fd
    
    // now, functions like send() and recv() can be used to communicate
    // between client and server

    // closing the cliend file descriptor
    close(client_fd);

    return 0;
}
