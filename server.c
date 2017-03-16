#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <stdio.h>
#include <stdlib.h>

#include <string.h>
#include <unistd.h>

#define PORT 1100
#define BUFFER_LENGTH 256
#define MAXIMUM_LENGTH_QUEUE_OF_PENDING_CONNECTIONS 10

int main(int argc, char** argv) {
    struct sockaddr_in stSockAddr;
    int i32SocketFD = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    
    if(i32SocketFD == -1) {
        printf("Can not create socket");
        return(EXIT_FAILURE);
    }
    
    memset(&stSockAddr, 0, sizeof(stSockAddr));
    
    stSockAddr.sin_family = AF_INET;
    stSockAddr.sin_port = htons(PORT);
    stSockAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    
    if(bind(i32SocketFD, (struct sockaddr*) &stSockAddr, sizeof(stSockAddr)) == -1) {
        printf("Connection error");
        
        close(i32SocketFD);
        return(EXIT_FAILURE);
    }
    
    if(listen(i32SocketFD, MAXIMUM_LENGTH_QUEUE_OF_PENDING_CONNECTIONS) == -1) {
        printf("Listen error");
        
        close(i32SocketFD);
        return(EXIT_FAILURE);
    }
    
    char buff[BUFFER_LENGTH];
    int numberOfReadChars = -1;
    
    for(;;) {
        int i32ConnectFD = accept(i32SocketFD, 0, 0);
        
        if(i32ConnectFD < 0) {
            printf("Recieve error");

            close(i32SocketFD);
            return(EXIT_FAILURE);
        }
        
        bzero(buff, BUFFER_LENGTH);
        
        numberOfReadChars = read(i32ConnectFD, buff, BUFFER_LENGTH);
        if(numberOfReadChars < 0) {
            printf("read error");
            
            close(i32ConnectFD);
            close(i32SocketFD);
            return(EXIT_FAILURE);
        }
        printf("Buff: %s", buff);
        
        if(shutdown(i32ConnectFD, SHUT_RDWR) == -1) {
            printf("shutdown error");

            close(i32ConnectFD);
            close(i32SocketFD);
            return(EXIT_FAILURE);
        }
        close(i32ConnectFD);
    }
    
    close(i32SocketFD);
    
    return (EXIT_SUCCESS);
}

