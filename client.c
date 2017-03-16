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

int main(int argc, char** argv) {
    struct sockaddr_in stSockAddr;
    int i32Res;
    int i32SocketFD = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    
    if(i32SocketFD == -1) {
        printf("Can not create socket");
        return(EXIT_FAILURE);        
    }
    
    char buff[BUFFER_LENGTH];
    int numberOfWroteChars = -1;
    
    memset(&stSockAddr, 0, sizeof(stSockAddr));
    
    stSockAddr.sin_family = AF_INET;
    stSockAddr.sin_port = htons(PORT);

    i32Res = inet_pton(AF_INET, "127.0.0.1", &stSockAddr.sin_addr);
    
    if(i32Res < 0) {
        printf("Can not create socket 1");
        
        close(i32SocketFD);
        return(EXIT_FAILURE);        
    } else if(!i32Res) {
        printf("Can not create socket 2");
        
        close(i32SocketFD);
        return(EXIT_FAILURE);        
    }
    
    if(connect(i32SocketFD, (struct sockaddr*) &stSockAddr, sizeof(stSockAddr)) == -1) {
        printf("Can connect");
        
        close(i32SocketFD);
        return(EXIT_FAILURE);        
    }
    
    strcpy(buff, "Hello Server\n");
    numberOfWroteChars = write(i32SocketFD, buff, strlen(buff));
    
    if(numberOfWroteChars < 0) {
        printf("wrote error");
    } else {
        printf("numberOfWroteChars %d\n", numberOfWroteChars);
    }
    
    if(shutdown(i32SocketFD, SHUT_RDWR) == -1) {
        printf("shutdown error");
        
        close(i32SocketFD);
        return(EXIT_FAILURE);        
    }
    
    close(i32SocketFD);
    
    return 0;
}

