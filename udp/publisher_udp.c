#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int sockfd;
    struct sockaddr_in servaddr;
    char buffer[BUFFER_SIZE];

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = INADDR_ANY;

    printf("Publisher ready. Type messages:\n");
    while (fgets(buffer, BUFFER_SIZE, stdin)) {
        sendto(sockfd, buffer, strlen(buffer), 0,
               (const struct sockaddr *)&servaddr, sizeof(servaddr));
    }
    close(sockfd);
    return 0;
}
