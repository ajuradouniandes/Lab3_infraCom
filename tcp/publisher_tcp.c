#include <stdio.h>          // Entrada y salida estándar
#include <stdlib.h>         // Funciones del sistema (exit, etc.)
#include <string.h>         // Manejo de cadenas
#include <unistd.h>         // Funciones POSIX (close, read)
#include <arpa/inet.h>      // Funciones para sockets e internet

#define PORT 8080           // Puerto del broker
#define BUFFER_SIZE 1024    // Tamaño del buffer de envío

int main() {
    int sockfd;                      // Descriptor del socket del publisher
    struct sockaddr_in servaddr;     // Dirección del broker
    char buffer[BUFFER_SIZE];        // Buffer de mensaje

    // Crea el socket TCP
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    // Configura dirección y puerto del broker
    servaddr.sin_family = AF_INET;           // IPv4
    servaddr.sin_port = htons(PORT);         // Puerto
    servaddr.sin_addr.s_addr = INADDR_ANY;   // Conecta al localhost o red local

    // Conecta el publisher al broker
    connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));

    printf("Publisher connected. Type messages:\n");

    // Ciclo para leer mensajes desde la consola y enviarlos al broker
    while (fgets(buffer, BUFFER_SIZE, stdin)) {
        send(sockfd, buffer, strlen(buffer), 0);    // Envía mensaje al broker
    }

    // Cierra el socket al terminar
    close(sockfd);
    return 0;
}
