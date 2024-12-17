#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 8080
#define IP_ADDRESS "127.0.0.1" // local host
#define BUFFER_SIZE 1024

int main(void)
{
    int client_fd; 
    struct sockaddr_in serv_address = {
        .sin_family = AF_INET,
        .sin_port = htons(PORT)
    };
    
    client_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if ( client_fd < 0 ) {
        perror("Socket creation failed\n");
        exit(EXIT_FAILURE);
    }
    
    // converts a human-readable IP string (e.g., "127.0.0.1")
    // into binary form and stores it in serv_addr.sin_addr
    if ( inet_pton(AF_INET, IP_ADDRESS, &serv_address.sin_addr ) <= 0 ) {
        perror("Invalid address / Address not supported\n");
        exit(EXIT_FAILURE);
    }

    if ( connect(client_fd, (struct sockaddr*)&serv_address, sizeof(serv_address)) ) {
        perror("Failed to connect\n");
        close(client_fd);
        exit(EXIT_FAILURE);
    }
    // connecting to the server

    char *hello = "Hello from client\n";
    send(client_fd, hello, strlen(hello), 0);
    printf("Message sent to server: %s\n", hello);

    char buffer[BUFFER_SIZE] = { 0 };
    int valread = read(client_fd, buffer, sizeof(buffer) - 1);
    printf("message from server: %s\n", buffer);

    close(client_fd);
    printf("Connection closed\n");

    return 0;
}
