#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main(void)
{
    int server_fd, client_fd;
    struct sockaddr_in address = {
        .sin_family = AF_INET, // IPv4
        .sin_addr.s_addr = INADDR_ANY, // the server`s ip address will be assigned automatically
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

    // accept collects data on who is the client(ip address and protocol)
    client_fd = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen);
    if ( client_fd == -1 ) {
        perror("Accept failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Connection accepted from client\n");

    char buffer[BUFFER_SIZE];
    // the loop where more complex communication happens
    while(1) {
        // receiving a message from the client
        memset(buffer, 0, sizeof(buffer));
        int bytes_read = recv(client_fd, buffer, sizeof(buffer) - 1, 0);
        if (bytes_read <= 0) {
            printf("Client disconnect or interrupt\n");
            break;
        }

        printf("Client: %s\n", buffer);

        // checks if the first word is `bye`. If positive, terminate the program
        if (strcmp(buffer, "bye") == 0) {
            printf("Client ended the chat\n");
            break;
        }

        printf("You: ");
        fgets(buffer, sizeof(buffer), stdin);
        buffer[strcspn(buffer, "\n")] = '\0';

        // strlen = sizeof in the case of strings/char buffers
        send(client_fd, buffer, strlen(buffer), 0);
        
        if(strcmp(buffer, "bye") == 0) {
            printf("You ended the connection\n");
            break;
        }

    }

    // closing the server
    close(server_fd);

    // closing the client
    close(client_fd);

    return 0;
}
