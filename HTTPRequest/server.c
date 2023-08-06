#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>

#include<arpa/inet.h>

int main() {
    int serverSocket, yes=1;
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if(serverSocket == -1) {
        perror("Socket Error");
        exit(1);
    }

    struct sockaddr_in serverAddress, clientAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(4501);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    if(setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(serverSocket))){
        perror("Socket Options Set error");
        exit(1);
    }

    int clientSize = sizeof(clientAddress);

    if(bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress))==-1){
        perror("Bind Error");
        exit(1);
    }

    printf(">> Server Running at %s\n", inet_ntoa(serverAddress.sin_addr));

    if(listen(serverSocket, 10)==-1){
        perror("Listen Error");
        exit(1);
    }

    printf(">> Listening for clients...\n");

    int clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddress, &clientSize);
    printf(">> Client Connection to server.\n");
    fflush(stdout);
    if(clientSocket==-1){
        perror("Accept Error");
        exit(1);
    }

    char fname[256];
    char file_buffer[1024];
    while(1){
        // receive message

        int fnameLen = recv(clientSocket, fname, 256, 0);
        if(fnameLen<=0){
            if(fnameLen<0){
                perror("Receive Error");
                exit(1);
            }
            else{
                printf("Client Disconnected.\n");
                fflush(stdout);
                exit(1);
            }
        }
        else {
            fname[fnameLen] = '\0';
            printf("%s\n",fname);
        }

        FILE *fptr;
        fptr = fopen(fname, "r");
        if(fptr == NULL){
            perror("Error Opening File");
            exit(1);
        }
        
        while(fgets(file_buffer, sizeof(file_buffer), fptr)!=NULL){
            // printf("%s", file_buffer);
            // fflush(stdout);
            send(clientSocket, file_buffer, strlen(file_buffer), 0);
            // printf("a\n");
        }

        send(clientSocket, "TRANSFER_END", strlen("TRANSFER_END"), 0);
        // send(clientSocket, "", 0, 0);
    }
    close(clientSocket);
    close(serverSocket);
    return 0;
}
