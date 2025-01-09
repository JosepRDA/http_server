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
    if (client_fd < 0) {
        perror("Socket creation failed\n");
        exit(EXIT_FAILURE);
    }

    // converts a human-readable IP string (e.g., "127.0.0.1")
    // into binary form and stores it in serv_addr.sin_addr
    if (inet_pton(AF_INET, IP_ADDRESS, &serv_address.sin_addr ) <= 0) {
        perror("Invalid address / Address not supported\n");
        exit(EXIT_FAILURE);
    }

    if ( 
        connect(client_fd, (struct sockaddr*)&serv_address, sizeof(serv_address))
    ) {
        perror("Failed to connect\n");
        close(client_fd);
        exit(EXIT_FAILURE);
    }

    // communication
    char buffer[BUFFER_SIZE];
    while(1) {
        printf("You: ");
        fgets(buffer, sizeof(buffer), stdin);
        buffer[strcspn(buffer, "\n")] = '\0';

        send(client_fd, buffer, strlen(buffer), 0);

        if (strcmp(buffer, "bye") == 0) {
            printf("You ended the chat\n");
            break;
        }

        memset(buffer, 0, sizeof(buffer));
        int bytes_read = recv(client_fd, buffer, sizeof(buffer) - 1, 0);
        if (bytes_read <= 0) {
            printf("Server connection failed/was interrupted\n");
            break;
        }

        printf("Server: %s\n", buffer);
        if(strcmp(buffer, "bye") == 0) {
            printf("Server ended the chat.\n");
            break;
        }
    }
    close(client_fd);
    printf("Connection closed\n");

    return 0;
}
