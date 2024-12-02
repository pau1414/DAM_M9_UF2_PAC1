#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    WSADATA wsaData;
    SOCKET sock;
    struct sockaddr_in serverAddr;
    char buffer[BUFFER_SIZE] = {0};
    const char* message = "Hola servidor!";

    // Inicialitzar Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "Error a WSAStartup" << std::endl;
        return -1;
    }

    // Crear socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) {
        std::cerr << "Error al crear el socket: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return -1;
    }

    // Configurar direcció del servidor
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Connectar al servidor
    if (connect(sock, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Error al connectar amb el servidor: " << WSAGetLastError() << std::endl;
        closesocket(sock);
        WSACleanup();
        return -1;
    }

    std::cout << "Connexió establerta amb el servidor." << std::endl;

    // Enviar missatge al servidor
    send(sock, message, strlen(message), 0);
    std::cout << "Missatge enviat: " << message << std::endl;

    // Rebre resposta del servidor
    recv(sock, buffer, BUFFER_SIZE, 0);
    std::cout << "Resposta del servidor: " << buffer << std::endl;

    // Tancar el socket
    closesocket(sock);
    WSACleanup();

    return 0;
}
