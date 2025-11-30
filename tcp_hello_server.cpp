// tcp_hello_server_simple.cpp
// Compile: cl /EHsc /W4 tcp_hello_server_simple.cpp ws2_32.lib

#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>

#pragma comment(lib, "ws2_32.lib")

#define DEFAULT_PORT 54000
#define BUFSIZE 512

int main() {
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2,2), &wsa) != 0) {
        std::cerr << "WSAStartup failed\n";
        return 1;
    }

    SOCKET listenSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (listenSock == INVALID_SOCKET) {
        std::cerr << "socket failed: " << WSAGetLastError() << "\n";
        WSACleanup();
        return 1;
    }

    sockaddr_in servAddr{};
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = INADDR_ANY; // 0.0.0.0
    servAddr.sin_port = htons(DEFAULT_PORT);

    if (bind(listenSock, (sockaddr*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR) {
        std::cerr << "bind failed: " << WSAGetLastError() << "\n";
        closesocket(listenSock);
        WSACleanup();
        return 1;
    }

    if (listen(listenSock, SOMAXCONN) == SOCKET_ERROR) {
        std::cerr << "listen failed: " << WSAGetLastError() << "\n";
        closesocket(listenSock);
        WSACleanup();
        return 1;
    }

    std::cout << "Server listening on port " << DEFAULT_PORT << " ...\n";

    sockaddr_in clientAddr{};
    int clientAddrLen = sizeof(clientAddr);
    SOCKET clientSock = accept(listenSock, (sockaddr*)&clientAddr, &clientAddrLen);
    if (clientSock == INVALID_SOCKET) {
        std::cerr << "accept failed: " << WSAGetLastError() << "\n";
        closesocket(listenSock);
        WSACleanup();
        return 1;
    }

    char buf[BUFSIZE];
    int bytesReceived = recv(clientSock, buf, BUFSIZE - 1, 0);
    if (bytesReceived > 0) {
        buf[bytesReceived] = '\0';
        std::cout << "Received: " << buf << "\n";

        const char* reply = "Hello from TCP server";
        int sent = send(clientSock, reply, (int)strlen(reply), 0);
        if (sent == SOCKET_ERROR) {
            std::cerr << "send failed: " << WSAGetLastError() << "\n";
        }
    } else if (bytesReceived == 0) {
        std::cout << "Client closed connection\n";
    } else {
        std::cerr << "recv failed: " << WSAGetLastError() << "\n";
    }

    closesocket(clientSock);
    closesocket(listenSock);
    WSACleanup();
    return 0;
}
