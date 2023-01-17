#include <arpa/inet.h>
#include <strings.h>
#include <unistd.h>
#include "tcp.h"

// #define MAX 80
// #define PORT 8080
#define SA struct sockaddr

void func(int sockfd) {
    char buff[MAX];
    int n;
    for (;;) {
        bzero(buff, sizeof(buff));
        printf("Message for the server: ");
        n = 0;
        while ((buff[n++] = getchar()) != '\n')
        ;
        write(sockfd, buff, sizeof(buff));
        bzero(buff, sizeof(buff));
        read(sockfd, buff, sizeof(buff));
        printf("From server: %s\n", buff);
        if ((strncmp(buff, "exit", 4)) == 0) {
            printf("Client Exit\n");
            break;
        }
    }
}

int main() {
    int sockfd, connfd;
    struct sockaddr_in servaddr, cli;

    // socket creation and verification
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("Socket creation failed\n");
        exit(0);
    } else {
        printf("Socket created\n");
    }
    bzero(&servaddr, sizeof(servaddr));

    // assign IP, PORT
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_port = htons(PORT);

    // connect to the server socket
    if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0) {
        printf("Failed to connect to server socket\n");
        exit(0);
    } else {
        printf("Successfuly connected to socket at %d:%d\n", servaddr.sin_addr.s_addr, servaddr.sin_port);
    }

    // chat on the socket
    func(sockfd);

    // close the chat; disconnect from socket
    close(sockfd);
}


