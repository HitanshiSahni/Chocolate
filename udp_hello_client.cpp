// udp_hello_client_simple.cpp
// Compile: cl /EHsc /W4 udp_hello_client_simple.cpp ws2_32.lib

#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>

#pragma comment(lib, "ws2_32.lib")

#define SERVER_IP "127.0.0.1"
#define DEFAULT_PORT 54001
#define BUFSIZE 512

int main() {
    WSADATA wsa; if (WSAStartup(MAKEWORD(2,2), &wsa) != 0) return 1;
    SOCKET sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock == INVALID_SOCKET) { std::cerr<<"socket error\n"; WSACleanup(); return 1; }

    sockaddr_in serv{}; serv.sin_family = AF_INET; serv.sin_port = htons(DEFAULT_PORT);
    serv.sin_addr.s_addr = inet_addr(SERVER_IP);

    const char* msg = "Hello from UDP client";
    sendto(sock, msg, (int)strlen(msg), 0, (sockaddr*)&serv, sizeof(serv));

    char buf[BUFSIZE]; sockaddr_in from{}; int fromlen = sizeof(from);
    int r = recvfrom(sock, buf, BUFSIZE-1, 0, (sockaddr*)&from, &fromlen);
    if (r > 0) { buf[r]=0; std::cout<<"Server replied: "<<buf<<"\n"; }

    closesocket(sock); WSACleanup();
    return 0;
}
