#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>

#include<arpa/inet.h>

#define PORT 4501  
#define BUFFER_SIZE 1024   

int main(){

    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    
    if(client_socket == -1){
        perror("Binding Error");
        exit(1);
    }
    printf(">> Client socket created.\n");
    fflush(stdout);

    struct sockaddr_in client_address;
    client_address.sin_family = AF_INET;
    client_address.sin_port = htons(PORT);
    client_address.sin_addr.s_addr = INADDR_ANY;

    int connection_status = connect(client_socket, (struct sockaddr*)&client_address, sizeof(client_address));

    if(connection_status==-1){
        perror("Connect Error");
        exit(1);
    }
    printf(">> Connected to the server\n");
    fflush(stdout);

    char *fname;
    printf("Enter the file name: ");
    fflush(stdout);
    fgets(fname, 1024, stdin);

    send(client_socket, fname, sizeof(fname), 0);

    printf("Message sent.\n");
    fflush(stdout);
    close(client_socket);

    return 0;
}