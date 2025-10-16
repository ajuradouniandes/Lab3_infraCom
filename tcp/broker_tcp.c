#include <stdio.h>          // Entrada y salida estándar (printf, perror, etc.)
#include <stdlib.h>         // Funciones generales (exit, malloc, etc.)
#include <string.h>         // Manejo de cadenas (strlen, strcpy, etc.)
#include <unistd.h>         // Funciones POSIX (close, read, write)
#include <arpa/inet.h>      // Funciones para sockets e internet (inet_pton, htons)
#include <sys/select.h>     // Para el uso de 'select' y manejo de múltiples sockets

#define PORT 8080           // Puerto donde escuchará el broker
#define MAX_CLIENTS 10      // Máximo número de clientes simultáneos
#define BUFFER_SIZE 1024    // Tamaño del buffer de lectura/escritura

int main() {
    int server_fd, new_socket, client_sockets[MAX_CLIENTS]; // Descriptores del servidor y clientes
    struct sockaddr_in address;                             // Estructura que guarda dirección y puerto
    fd_set readfds;                                          // Conjunto de descriptores de archivos para 'select'
    char buffer[BUFFER_SIZE];                                // Buffer para recibir/enviar mensajes
    int max_sd, sd, activity, valread;                       // Variables auxiliares

    // Inicializa todos los sockets de clientes a 0 (vacíos)
    for (int i = 0; i < MAX_CLIENTS; i++) client_sockets[i] = 0;

    // Crea el socket del servidor (AF_INET = IPv4, SOCK_STREAM = TCP)
    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    // Configura la dirección del servidor
    address.sin_family = AF_INET;           // IPv4
    address.sin_addr.s_addr = INADDR_ANY;   // Acepta conexiones desde cualquier IP
    address.sin_port = htons(PORT);         // Puerto convertido a formato de red

    // Enlaza (asocia) el socket con la dirección y puerto
    bind(server_fd, (struct sockaddr *)&address, sizeof(address));

    // Pone el socket en modo escucha con una cola de hasta 3 conexiones pendientes
    listen(server_fd, 3);

    printf("TCP Broker running on port %d...\n", PORT);

    // Bucle infinito para atender clientes permanentemente
    while (1) {
        FD_ZERO(&readfds);                  // Limpia el conjunto de descriptores
        FD_SET(server_fd, &readfds);        // Añade el socket del servidor al conjunto
        max_sd = server_fd;                 // Inicializa el descriptor máximo

        // Añade todos los sockets de clientes al conjunto de lectura
        for (int i = 0; i < MAX_CLIENTS; i++) {
            sd = client_sockets[i];
            if (sd > 0) FD_SET(sd, &readfds);   // Si está activo, lo añade
            if (sd > max_sd) max_sd = sd;       // Actualiza el descriptor máximo
        }

        // Espera actividad en alguno de los sockets
        activity = select(max_sd + 1, &readfds, NULL, NULL, NULL);

        // Si hay una nueva conexión entrante
        if (FD_ISSET(server_fd, &readfds)) {
            int addrlen = sizeof(address);
            new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
            printf("New connection accepted\n");

            // Agrega el nuevo socket de cliente a la lista
            for (int i = 0; i < MAX_CLIENTS; i++) {
                if (client_sockets[i] == 0) {
                    client_sockets[i] = new_socket;
                    break;
                }
            }
        }

        // Recorre todos los clientes para ver si alguno envió datos
        for (int i = 0; i < MAX_CLIENTS; i++) {
            sd = client_sockets[i];

            // Si hay datos listos para leer
            if (FD_ISSET(sd, &readfds)) {
                // Lee los datos del cliente
                if ((valread = read(sd, buffer, BUFFER_SIZE)) == 0) {
                    // Si read devuelve 0 → el cliente cerró la conexión
                    close(sd);
                    client_sockets[i] = 0;  // Marca el espacio como libre
                } else {
                    // Termina la cadena leída y muestra el mensaje recibido
                    buffer[valread] = '\0';
                    printf("Broker received: %s\n", buffer);

                    // Reenvía el mensaje a todos los demás clientes conectados
                    for (int j = 0; j < MAX_CLIENTS; j++) {
                        if (client_sockets[j] != 0 && client_sockets[j] != sd) {
                            send(client_sockets[j], buffer, strlen(buffer), 0);
                        }
                    }
                }
            }
        }
    }
}
