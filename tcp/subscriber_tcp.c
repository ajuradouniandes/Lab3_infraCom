#include <stdio.h>          // Entrada y salida estándar
#include <stdlib.h>         // Funciones generales del sistema
#include <string.h>         // Funciones para manejo de cadenas
#include <unistd.h>         // Funciones POSIX (close, read, etc.)
#include <arpa/inet.h>      // Funciones para el manejo de sockets e IPs

#define PORT 8080           // Puerto del broker al que se conectará
#define BUFFER_SIZE 1024    // Tamaño del buffer de recepción

int main() {
    int sockfd;                      // Descriptor del socket del subscriber
    struct sockaddr_in servaddr;     // Estructura para dirección del broker
    char buffer[BUFFER_SIZE];        // Buffer para mensajes recibidos

    // Crea un socket TCP (AF_INET = IPv4, SOCK_STREAM = TCP)
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    // Configura los datos del broker
    servaddr.sin_family = AF_INET;           // IPv4
    servaddr.sin_port = htons(PORT);         // Puerto en formato de red
    servaddr.sin_addr.s_addr = INADDR_ANY;   // Dirección del broker (localhost o red local)

    // Conecta el subscriber al broker
    connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));

    printf("Subscriber connected. Waiting for messages...\n");

    // Ciclo infinito para recibir mensajes del broker
    while (1) {
        int n = recv(sockfd, buffer, BUFFER_SIZE, 0);  // Recibe datos del broker
        if (n > 0) {
            buffer[n] = '\0';                          // Termina la cadena
            printf("Received: %s\n", buffer);           // Muestra el mensaje recibido
        }
    }

    // Cierra el socket al terminar
    close(sockfd);
    return 0;
}


