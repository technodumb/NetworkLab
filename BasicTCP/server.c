#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    char buffer[1024];

    // Create a socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Prepare the server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(8080); // Change this port as needed

    // Bind the socket to the server address
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Binding failed");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(server_socket, 5) == -1) {
        perror("Listening failed");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d\n", ntohs(server_addr.sin_port));
    int x = 100;
    char server_message[256];
    while(1){
        // Accept incoming connections
        client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_addr_len);
        if (client_socket == -1) {
            perror("Accepting connection failed");
            close(server_socket);
            exit(EXIT_FAILURE);
        }
        sprintf(server_message, "The server has been called %d time.\n", x);
        x++;
        send(client_socket, server_message, sizeof(server_message), 0);
        // Print the client port number
        printf("Connected to client from port %d\n", ntohs(client_addr.sin_port));
        close(client_socket);
    }

    // Close the connection
    close(server_socket);

    return 0;
}
