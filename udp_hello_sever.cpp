// udp_hello_server_simple.cpp
// Compile: cl /EHsc /W4 udp_hello_server_simple.cpp ws2_32.lib

#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>

#pragma comment(lib, "ws2_32.lib")

#define DEFAULT_PORT 54001
#define BUFSIZE 512

int main() {
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2,2), &wsa) != 0) return 1;

    SOCKET sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock == INVALID_SOCKET) { std::cerr<<"socket error\n"; WSACleanup(); return 1; }

    sockaddr_in serv{};
    serv.sin_family = AF_INET;
    serv.sin_addr.s_addr = INADDR_ANY;
    serv.sin_port = htons(DEFAULT_PORT);

    if (bind(sock, (sockaddr*)&serv, sizeof(serv)) == SOCKET_ERROR) {
        std::cerr<<"bind failed: "<<WSAGetLastError()<<"\n"; closesocket(sock); WSACleanup(); return 1;
    }

    std::cout<<"UDP Hello server listening on port "<<DEFAULT_PORT<<"\n";
    char buf[BUFSIZE];
    sockaddr_in client{};
    int clientLen = sizeof(client);
    int r = recvfrom(sock, buf, BUFSIZE-1, 0, (sockaddr*)&client, &clientLen);
    if (r > 0) {
        buf[r]=0; std::cout<<"Received: "<<buf<<"\n";
        const char* reply = "Hello from UDP server";
        sendto(sock, reply, (int)strlen(reply), 0, (sockaddr*)&client, clientLen);
    }

    closesocket(sock);
    WSACleanup();
    return 0;
}
