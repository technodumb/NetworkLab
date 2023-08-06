#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    int client_socket;
    struct sockaddr_in server_addr;
    char buffer[1024];

    // Create a socket
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Prepare the server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY; // Replace with the server IP address
    server_addr.sin_port = htons(8080); // Change this port to the server's port

    // Connect to the server
    if (connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Connection failed");
        close(client_socket);
        exit(EXIT_FAILURE);
    }

    // Send data (optional)
    char message[256];
    recv(client_socket, &message, sizeof(message), 0);
    printf("Recieved Message: %s\n", message);
    // send(client_socket, message, strlen(message), 0);

    // Close the connection
    close(client_socket);

    return 0;
}
