//server opens and reads the file and sends the contents to client

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#define PORT 9000
#define BUFFER_SIZE 1024

int main() {
    int sockfd, newsockfd;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t addr_size;
    char buffer[BUFFER_SIZE];
    
    // Create socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("Error creating socket");
        exit(EXIT_FAILURE);
    }
    
    // Set up server address
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    
    // Bind socket to the server address
    if (bind(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
        perror("Error binding socket");
        exit(EXIT_FAILURE);
    }
    
    // Listen for incoming connections
    if (listen(sockfd, 5) == -1) {
        perror("Error listening");
        exit(EXIT_FAILURE);
    }
    
    printf("Server listening on port %d...\n", PORT);
    
    // Accept incoming connection
    addr_size = sizeof(clientAddr);
    newsockfd = accept(sockfd, (struct sockaddr*)&clientAddr, &addr_size);
    if (newsockfd == -1) {
        perror("Error accepting connection");
        exit(EXIT_FAILURE);
    }
    
    // Receive file name from client
    ssize_t bytesRead = recv(newsockfd, buffer, sizeof(buffer), 0);
    if (bytesRead == -1) {
        perror("Error receiving data");
    } else if (bytesRead == 0) {
        printf("Client disconnected\n");
    } else {
        buffer[bytesRead] = '\0'; // Null-terminate the received file name
        printf("Received file name from client: %s\n", buffer);
        
        // Open the file and read its content
        FILE *file = fopen(buffer, "r");
        if (file == NULL) {
            perror("Error opening file");
        } else {
            // Read and send file content
            while (fgets(buffer, sizeof(buffer), file) != NULL) {
                send(newsockfd, buffer, strlen(buffer), 0);
            }
            fclose(file);
            printf("File content sent to client\n");
        }
    }
    
    // Close sockets
    close(newsockfd);
    close(sockfd);
    
    return 0;
}