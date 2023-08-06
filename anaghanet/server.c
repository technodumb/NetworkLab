#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

#define PORT 12345
#define BUFFER_SIZE 1024

int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    char buffer[BUFFER_SIZE];

    // Create socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("Error creating socket");
        exit(EXIT_FAILURE);
    }

    // Prepare server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // Bind the socket to the server address
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Error binding");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(server_socket, 1) == -1) {
        perror("Error listening");
        exit(EXIT_FAILURE);
    }

    printf("Server is listening on port %d...\n", PORT);

    // Accept incoming connection
    int addr_size = sizeof(client_addr);
    client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &addr_size);
    if (client_socket == -1) {
        perror("Error accepting connection");
        exit(EXIT_FAILURE);
    }

    printf("Client connected!\n");

    // Receive data from client
    int bytes_received = recv(client_socket, buffer, BUFFER_SIZE, 0);
    if (bytes_received == -1) {
        perror("Error receiving data");
        exit(EXIT_FAILURE);
    }

    buffer[bytes_received] = '\0'; // Null-terminate the received message

    printf("Received message from client: %s\n", buffer);

    // Send acknowledgment back to client
    const char *response = "Server received your message.";
    if (send(client_socket, response, strlen(response), 0) == -1) {
        perror("Error sending response");
        exit(EXIT_FAILURE);
    }

    // Close sockets
    close(client_socket);
    close(server_socket);

    return 0;
}