#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <time.h>

#define PORT 8484
#define MAX_BUFFER_SIZE 1024

void handleClient(int clientSocket) {
    char buffer[MAX_BUFFER_SIZE];
    char response[MAX_BUFFER_SIZE];

    while (1) {
        int receivedBytes = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (receivedBytes <= 0)
            break;

        buffer[receivedBytes] = '\0';

        time_t now;
        time(&now);
        struct tm *localTime = localtime(&now);
        strftime(response, sizeof(response), "%d %B %Y, %H:%M", localTime);

        strcat(response, " - ");
        strcat(response, buffer);

        send(clientSocket, response, strlen(response), 0);
    }

    printf("Connection closed\n");
    close(clientSocket);
}

int main() {
    int serverSocket, clientSocket;
    struct sockaddr_in serverAddress, clientAddress;
    socklen_t clientAddressLength = sizeof(clientAddress);

    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        perror("Failed to create socket");
        exit(EXIT_FAILURE);
    }
   serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(PORT);

    if (bind(serverSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(serverSocket, 5) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Server started, listening on port %d\n", PORT);

    while (1) {
        clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddress, &clientAddressLength);
        if (clientSocket < 0) {
            perror("Accept failed");
            exit(EXIT_FAILURE);
        }

        printf("Accepted connection from %s:%d\n", inet_ntoa(clientAddress.sin_addr), ntohs(clientAddr>

        if (fork() == 0) {
            // Child process
            close(serverSocket);
            handleClient(clientSocket);
            exit(EXIT_SUCCESS);
        } else {
            // Parent process
            close(clientSocket);
        }
    }

    close(serverSocket);
    return 0;
}

