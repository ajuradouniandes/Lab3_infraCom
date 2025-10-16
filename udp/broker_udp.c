#include <stdio.h>      // Para printf, perror
#include <stdlib.h>     // Para exit, malloc, free, etc.
#include <string.h>     // Para memset, strcmp, strlen
#include <unistd.h>     // Para close()
#include <arpa/inet.h>  // Para estructuras y funciones de sockets (sockaddr_in, htons, etc.)

#define PORT 8080            // Puerto donde el broker escucha
#define MAX_SUBS 10          // Máximo número de suscriptores que el broker puede manejar
#define BUFFER_SIZE 1024     // Tamaño máximo del mensaje

int main() {
    int sockfd;  // Descriptor del socket del broker (identificador del socket)
    struct sockaddr_in servaddr, cliaddr;  // Estructuras para la dirección del servidor (broker) y del cliente (publisher/subscriber)
    socklen_t len = sizeof(cliaddr);       // Tamaño de la dirección del cliente
    char buffer[BUFFER_SIZE];              // Buffer donde se almacenan los mensajes recibidos

    // Arreglo para guardar las direcciones de los suscriptores
    struct sockaddr_in subscribers[MAX_SUBS];
    int sub_count = 0;  // Contador de suscriptores actuales

    // 🔹 Crear el socket UDP
    // AF_INET -> familia IPv4
    // SOCK_DGRAM -> tipo de socket para UDP
    // 0 -> usa el protocolo por defecto (UDP)
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    // Inicializa la estructura del servidor con ceros
    memset(&servaddr, 0, sizeof(servaddr));

    // Configurar los parámetros del socket del servidor
    servaddr.sin_family = AF_INET;           // Familia de direcciones IPv4
    servaddr.sin_addr.s_addr = INADDR_ANY;   // Escucha en cualquier interfaz de red disponible
    servaddr.sin_port = htons(PORT);         // Puerto del servidor (convertido a formato de red)

    // Asocia el socket con la dirección y puerto especificados
    bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr));

    printf("UDP Broker running on port %d...\n", PORT);

    // Bucle infinito: el broker siempre está activo
    while (1) {
        // Espera recibir un mensaje (bloqueante)
        int n = recvfrom(sockfd, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&cliaddr, &len);

        buffer[n] = '\0';  // Agrega terminador de cadena
        printf("Broker received: %s\n", buffer);

        // Si el mensaje es "subscribe", se agrega al listado de suscriptores
        if (strcmp(buffer, "subscribe") == 0) {
            if (sub_count < MAX_SUBS) {
                subscribers[sub_count++] = cliaddr;  // Guarda la dirección del nuevo suscriptor
                printf("New subscriber added\n");
            }
        } 
        // Si el mensaje no es "subscribe", se considera un mensaje de un publisher
        else {
            // Reenvía el mensaje a todos los suscriptores registrados
            for (int i = 0; i < sub_count; i++) {
                sendto(sockfd, buffer, strlen(buffer), 0,
                       (const struct sockaddr *)&subscribers[i], sizeof(subscribers[i]));
            }
        }
    }
}
