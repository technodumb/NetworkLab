#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<time.h>

#include<sys/socket.h>
#include<sys/types.h>

#include<arpa/inet.h>

#define BUFFER_LEN 1024
#define PORT 4501

int main(){
    int server_socket;
    server_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if(server_socket==-1){
        perror("Socket creation error");
        exit(1);
    }

    struct sockaddr_in server_address, client_address;
    socklen_t client_address_size = sizeof(client_address);
    memset(&client_address, 0, sizeof(client_address));

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);
    server_address.sin_addr.s_addr = INADDR_ANY;

    int bind_status = bind(server_socket, (struct sockaddr*)&server_address, sizeof(server_address));
    if(bind_status==-1){
        perror("Binding Error");
        exit(1);
    }

    time_t cur_time;
    char send_buffer[BUFFER_LEN], recv_buffer[BUFFER_LEN];
    int recv_len;    
    printf("UDP Time server started on %s:%d\n\n", inet_ntoa(server_address.sin_addr), ntohs(server_address.sin_port));
    while(1){
        recv_len = recvfrom(server_socket, recv_buffer, BUFFER_LEN, 0, (struct sockaddr*)&client_address, &client_address_size);
        printf("Client connected from port: %d\n", ntohs(client_address.sin_port));
        fflush(stdout);
        recv_buffer[recv_len] = 0;
        if(strcmp(recv_buffer, "getTime")==0){
            // valid request
            cur_time = time(NULL);
            sprintf(send_buffer, "%s", ctime(&cur_time));
            sendto(server_socket, send_buffer, strlen(send_buffer), 0, (struct sockaddr*)&client_address, client_address_size);
        }
        else{
            printf("Invalid request!");
            fflush(stdout);
            exit(1);
        }
    }
    close(server_socket);
    return 0;
}