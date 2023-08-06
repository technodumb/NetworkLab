#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

#define SERVER_IP "127.0.0.1"
#define PORT 12345
#define BUFFER_SIZE 1024

int main() {
    int client_socket;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];

    // Create socket
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        perror("Error creating socket");
        exit(EXIT_FAILURE);
    }

    // Prepare server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    if (inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr) <= 0) {
        perror("Invalid address/Address not supported");
        exit(EXIT_FAILURE);
    }

    // Connect to the server
    if (connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Error connecting to server");
        exit(EXIT_FAILURE);
    }

    printf("Connected to server.\n");

    // Send data to the server
    const char *message = "Hello, server!";
    if (send(client_socket, message, strlen(message), 0) == -1) {
        perror("Error sending data");
        exit(EXIT_FAILURE);
    }

    // Receive acknowledgment from server
    int bytes_received = recv(client_socket, buffer, BUFFER_SIZE, 0);
    if (bytes_received == -1) {
        perror("Error receiving data");
        exit(EXIT_FAILURE);
    }

    buffer[bytes_received] = '\0'; // Null-terminate the received message

    printf("Received acknowledgment from server: %s\n", buffer);

    // Close socket
    close(client_socket);

    return 0;
}
