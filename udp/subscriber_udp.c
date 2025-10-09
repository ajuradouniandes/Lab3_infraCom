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

    // Send subscribe message
    sendto(sockfd, "subscribe", 9, 0,
           (const struct sockaddr *)&servaddr, sizeof(servaddr));

    printf("Subscriber registered. Waiting for messages...\n");
    while (1) {
        int n = recvfrom(sockfd, buffer, BUFFER_SIZE, 0, NULL, NULL);
        buffer[n] = '\0';
        printf("Received: %s\n", buffer);
    }
    close(sockfd);
    return 0;
}
