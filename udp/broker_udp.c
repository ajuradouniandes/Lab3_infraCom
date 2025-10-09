#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define MAX_SUBS 10
#define BUFFER_SIZE 1024

int main() {
    int sockfd;
    struct sockaddr_in servaddr, cliaddr;
    socklen_t len = sizeof(cliaddr);
    char buffer[BUFFER_SIZE];

    struct sockaddr_in subscribers[MAX_SUBS];
    int sub_count = 0;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(PORT);

    bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr));

    printf("UDP Broker running on port %d...\n", PORT);

    while (1) {
        int n = recvfrom(sockfd, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&cliaddr, &len);
        buffer[n] = '\0';
        printf("Broker received: %s\n", buffer);

        if (strcmp(buffer, "subscribe") == 0) {
            if (sub_count < MAX_SUBS) {
                subscribers[sub_count++] = cliaddr;
                printf("New subscriber added\n");
            }
        } else {
            for (int i = 0; i < sub_count; i++) {
                sendto(sockfd, buffer, strlen(buffer), 0,
                       (const struct sockaddr *)&subscribers[i], sizeof(subscribers[i]));
            }
        }
    }
}
