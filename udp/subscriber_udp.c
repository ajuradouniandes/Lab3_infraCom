#include <stdio.h>      // Para printf, perror
#include <stdlib.h>     // Para exit()
#include <string.h>     // Para memset, strlen
#include <unistd.h>     // Para close()
#include <arpa/inet.h>  // Para funciones y estructuras de red

#define PORT 8080            // Puerto donde está el broker
#define BUFFER_SIZE 1024     // Tamaño máximo del mensaje recibido

int main() {
    int sockfd;  // Descriptor del socket UDP
    struct sockaddr_in servaddr;  // Dirección del broker
    char buffer[BUFFER_SIZE];     // Buffer para recibir mensajes

    // Crear socket UDP
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    // Configurar la dirección del broker
    servaddr.sin_family = AF_INET;           // IPv4
    servaddr.sin_port = htons(PORT);         // Puerto
    servaddr.sin_addr.s_addr = INADDR_ANY;   // Dirección (localhost o cualquier interfaz)

    // Enviar mensaje de suscripción al broker
    sendto(sockfd, "subscribe", 9, 0,
           (const struct sockaddr *)&servaddr, sizeof(servaddr));

    printf("Subscriber registered. Waiting for messages...\n");

    // Espera mensajes continuamente desde el broker
    while (1) {
        int n = recvfrom(sockfd, buffer, BUFFER_SIZE, 0, NULL, NULL);
        buffer[n] = '\0';  // Termina la cadena recibida
        printf("Received: %s\n", buffer);
    }

    close(sockfd);  // Cierra el socket (aunque no se alcanza por el while infinito)
    return 0;
}
