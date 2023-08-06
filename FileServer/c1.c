//client sents the file name to server,the client writes the contents to another file

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define SERVER_IP "127.0.0.1"
#define PORT 9000
#define BUFFER_SIZE 1024

int main() {
    int sockfd;
    struct sockaddr_in serverAddr;
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
    serverAddr.sin_addr.s_addr = inet_addr(SERVER_IP);
    
    // Connect to the server
    if (connect(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
        perror("Error connecting");
        exit(EXIT_FAILURE);
    }
    
    // Send file name to server
    printf("Enter the name of the file: ");
    scanf("%s", buffer);
    send(sockfd, buffer, strlen(buffer), 0);
    
    // Create a new file to write the received content
    FILE *outputFile = fopen("output.txt", "w");
    if (outputFile == NULL) {
        perror("Error creating output file");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
    
    // Receive and write file content from server
    ssize_t bytesRead;
    while ((bytesRead = recv(sockfd, buffer, sizeof(buffer), 0)) > 0) {
        buffer[bytesRead] = '\0'; // Null-terminate the received data
        fputs(buffer, outputFile);
    }
    
    printf("File content received and written to output.txt\n");
    
    // Close sockets and file
    fclose(outputFile);
    close(sockfd);
    
    return 0;
}