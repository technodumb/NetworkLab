#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

#include<arpa/inet.h>

#define PORT 4501
#define BUFFER_SIZE 1024

int main(){
    int server_socket, client_socket;
    
    server_socket = socket(AF_INET, SOCK_STREAM, 0);

    if(server_socket<0){
        perror("Socket Error");
        exit(1);
    }
    printf(">> Server Socket Created\n");
    fflush(stdout);

    struct sockaddr_in server_address, client_address;
    int clientSize = sizeof(client_address);
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);
    server_address.sin_addr.s_addr = INADDR_ANY;

    if(bind(server_socket, (struct sockaddr*) &server_address, sizeof(server_address))<0) {
        perror("Binding Error");
        exit(1);
    }

    if(listen(server_socket, 10)<0){
        perror("Listen Error");
        exit(1);
    }
    printf(">> Server is listening...\n");
    fflush(stdout);

    // accept the connection
    if((client_socket = accept(server_socket, (struct sockaddr*)&client_address, &clientSize))==-1){
        perror("Accept Error");
        exit(1);
    }
    printf(">> Connection accepted from address %s.\n", inet_ntoa(client_address.sin_addr));
    fflush(stdout);

    char message[BUFFER_SIZE];
    int msglen;
    msglen = recv(client_socket, message, BUFFER_SIZE, 0);

    if(msglen<0){
        perror("Message recieve error.");
        exit(1);
    }
    message[msglen] = 0;

    printf("File name: %s\n", message);
    close(server_socket);
    close(client_socket);
    return 0;
}
