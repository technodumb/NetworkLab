// basic
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>

// networking
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>

#define PORT 4501
#define BUFFER_LEN 1024

int main(){
    // socket creation
    int client_socket;
    if((client_socket = socket(AF_INET, SOCK_DGRAM, 0))==-1){
        perror("Socket error");
        exit(1);
    }

    // address initialization
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);
    server_address.sin_addr.s_addr = INADDR_ANY;

    // sending request message
    char message[20] = "getTime";
    sendto(client_socket, message, strlen(message), 0, (struct sockaddr *)&server_address, sizeof(server_address));
    printf("Request sent to server.\n");
    fflush(stdout);


    // receiving response message
    char recv_message[BUFFER_LEN];
    int recv_length;
    socklen_t address_size = sizeof(server_address);
    recv_length = recvfrom(client_socket, recv_message, BUFFER_LEN, 0, (struct sockaddr *)&server_address, &address_size);
    recv_message[recv_length] = 0;

    // displaying
    printf("The current time is: %s\n\n", recv_message);
    fflush(stdout);

    close(client_socket);
    return 0;
}