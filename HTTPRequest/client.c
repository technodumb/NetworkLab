#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>

#include<arpa/inet.h>

int main() {
    int clientSocket;
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if(clientSocket == -1) {
        perror("Socket Error");
        exit(1);
    }


    // struct sockaddr_in clientAddress;
    // clientAddress.sin_family = AF_INET;
    // clientAddress.sin_port = htons(4501);
    // clientAddress.sin_addr.s_addr = INADDR_ANY;

    
    struct sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(4501);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    // connect
    if(connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress))==-1){
        perror("Connect Error");
        exit(1);
    }

    char input[256];
    while(1){
        printf("Enter the HTTP Request: ");
        fflush(stdout);
        scanf("%s", input);

        FILE *output = fopen("output.txt", "w");

        send(clientSocket, input, strlen(input), 0);

        char input_buffer[1024];
        ssize_t inputLen;
        while((inputLen = recv(clientSocket, input_buffer, sizeof(input_buffer), 0))>0){
            input_buffer[inputLen] = 0;
            if(strcmp(input_buffer, "TRANSFER_END")==0){
                break;
            }
            fprintf(output, "%s", input_buffer);
            // printf("%s\n", input_buffer);
            // fflush(stdout);
            //  printf("a\n");
        }
        fclose(output);
    }

    close(clientSocket);

    // fd_set allSockets, selectedSockets;
    // FD_ZERO(&allSockets);
    // FD_SET()

    return 0;
}
