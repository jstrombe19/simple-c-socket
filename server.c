#include <netinet/in.h>
#include <sys/types.h>
#include <unistd.h>      // read() write() close()
#include "tcp.h"

// #define MAX 80
// #define PORT 8080
#define SA struct sockaddr


// function to handle chat between client and server
void func(int connfd) {
    char buff[MAX];
    int n;

    // infinite loop for chat
    for(;;) {
        bzero(buff, MAX);

        // read the messages from the client and copy it in the buffer
        read(connfd, buff, sizeof(buff));
        // print buffer contents
        printf("From client: %s\t To client : ", buff);
        bzero(buff, MAX);
        n = 0;

        // copy server messages in the buffer
        while ((buff[n++] = getchar()) != '\n') 
        ;

        // send the buffer to the client
        write(connfd, buff, sizeof(buff));

        // check for exit chat keyword
        if (strncmp("exit", buff, 4) == 0) {
            printf("Server Exit...\n");
            break;
        }
    }
}


// driver function
int main() {
    int sockfd, connfd, len;
    struct sockaddr_in servaddr, cli;

    // create and verify socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("Failed to create socket\n");
        exit(0);
    } else {
        printf("Socket created and verified\n");
    }
    bzero(&servaddr, sizeof(servaddr));

    // assign IP and PORT
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);

    // bind newly created socket
    if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) {
        printf("Failed to bind socket\n");
        exit(0);
    } else {
        printf("Socket successfully bound\n");
    }

    // server listening and verification
    if (listen(sockfd, 5) != 0) {
        printf("Listen failed\n");
        exit(0);
    } else {
        printf("Server listening on %d:%d", servaddr.sin_addr.s_addr, servaddr.sin_port);
    }
    len = sizeof(cli);

    // accept and verify packets from client
    connfd = accept(sockfd, (SA*)&cli, &len);
    if (connfd < 0) {
        printf("Server accept failed\n");
        exit(0);
    } else {
        printf("Client connected\n");
    }

    // support chat between client and server until exit keyword is received
    func(connfd);

    // exit -> close socket connection
    close(sockfd);
}


