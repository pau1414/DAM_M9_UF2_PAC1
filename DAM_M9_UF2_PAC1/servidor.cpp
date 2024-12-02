#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    WSADATA wsaData;
    SOCKET serverSocket, clientSocket;
    struct sockaddr_in serverAddr, clientAddr;
    int addrlen = sizeof(clientAddr);
    char buffer[BUFFER_SIZE] = {0};
    const char* response = "Hola des del servidor!";

    // Inicialitzar Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "Error a WSAStartup" << std::endl;
        return -1;
    }

    // Crear socket
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == INVALID_SOCKET) {
        std::cerr << "Error al crear el socket: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return -1;
    }

    // Configurar direcció i port
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(PORT);

    // Associar socket a la direcció i port
    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Error al fer bind: " << WSAGetLastError() << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return -1;
    }

    // Escoltar connexions
    if (listen(serverSocket, 3) == SOCKET_ERROR) {
        std::cerr << "Error al escoltar: " << WSAGetLastError() << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return -1;
    }

    std::cout << "Esperant connexions..." << std::endl;

    // Acceptar connexió
    clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &addrlen);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "Error al acceptar connexió: " << WSAGetLastError() << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return -1;
    }

    // Llegir missatge del client
    recv(clientSocket, buffer, BUFFER_SIZE, 0);
    std::cout << "Missatge rebut del client: " << buffer << std::endl;

    // Enviar resposta al client
    send(clientSocket, response, strlen(response), 0);
    std::cout << "Resposta enviada al client." << std::endl;

    // Tancar sockets
    closesocket(clientSocket);
    closesocket(serverSocket);
    WSACleanup();

    return 0;
}
