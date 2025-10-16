#include <stdio.h>      // Para printf, fgets
#include <stdlib.h>     // Para exit()
#include <string.h>     // Para memset, strlen
#include <unistd.h>     // Para close()
#include <arpa/inet.h>  // Para funciones y estructuras de red

#define PORT 8080            // Puerto donde está el broker
#define BUFFER_SIZE 1024     // Tamaño máximo del mensaje a enviar

int main() {
    int sockfd;  // Descriptor del socket UDP
    struct sockaddr_in servaddr;  // Dirección del broker
    char buffer[BUFFER_SIZE];     // Buffer donde se almacenan los mensajes a enviar

    // Crear socket UDP
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    // Configurar la dirección del broker
    servaddr.sin_family = AF_INET;           // IPv4
    servaddr.sin_port = htons(PORT);         // Puerto del broker
    servaddr.sin_addr.s_addr = INADDR_ANY;   // Dirección (localhost o cualquier IP local)

    printf("Publisher ready. Type messages:\n");

    // Leer mensajes del teclado y enviarlos al broker
    while (fgets(buffer, BUFFER_SIZE, stdin)) {
        sendto(sockfd, buffer, strlen(buffer), 0,
               (const struct sockaddr *)&servaddr, sizeof(servaddr));
    }

    close(sockfd);  // Cierra el socket
    return 0;
}
