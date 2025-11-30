// tcp_hello_client_simple.cpp
// Compile: cl /EHsc /W4 tcp_hello_client_simple.cpp ws2_32.lib

#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>

#pragma comment(lib, "ws2_32.lib")

#define SERVER_IP "127.0.0.1"
#define DEFAULT_PORT 54000
#define BUFSIZE 512

int main() {
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2,2), &wsa) != 0) {
        std::cerr << "WSAStartup failed\n";
        return 1;
    }

    SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock == INVALID_SOCKET) {
        std::cerr << "socket failed: " << WSAGetLastError() << "\n";
        WSACleanup();
        return 1;
    }

    sockaddr_in servAddr{};
    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(DEFAULT_PORT);
    servAddr.sin_addr.s_addr = inet_addr(SERVER_IP); // simple; returns INADDR_NONE on error

    if (servAddr.sin_addr.s_addr == INADDR_NONE) {
        std::cerr << "Invalid server IP\n";
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    if (connect(sock, (sockaddr*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR) {
        std::cerr << "connect failed: " << WSAGetLastError() << "\n";
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    const char* msg = "Hello from TCP client";
    int sent = send(sock, msg, (int)strlen(msg), 0);
    if (sent == SOCKET_ERROR) {
        std::cerr << "send failed: " << WSAGetLastError() << "\n";
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    char buf[BUFSIZE];
    int bytesReceived = recv(sock, buf, BUFSIZE - 1, 0);
    if (bytesReceived > 0) {
        buf[bytesReceived] = '\0';
        std::cout << "Server replied: " << buf << "\n";
    } else if (bytesReceived == 0) {
        std::cout << "Server closed connection\n";
    } else {
        std::cerr << "recv failed: " << WSAGetLastError() << "\n";
    }

    closesocket(sock);
    WSACleanup();
    return 0;
}
