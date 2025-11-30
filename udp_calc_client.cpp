#include <iostream>
#include <winsock2.h>
using namespace std;

#define PORT 9090
#define BUF_SIZE 1024

int main() {
    WSADATA wsa;
    SOCKET sockfd;
    sockaddr_in servaddr{};
    int len = sizeof(servaddr);
    char buffer[BUF_SIZE];

    if (WSAStartup(MAKEWORD(2,2), &wsa) != 0) {
        cout << "WSAStartup failed\n";
        return 1;
    }

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd == INVALID_SOCKET) {
        cout << "Socket creation failed\n";
        return 1;
    }

    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    char op[10];
    double angle;
    cout << "Enter operation (sin/cos/tan): ";
    cin >> op;
    cout << "Enter angle in degrees: ";
    cin >> angle;

    snprintf(buffer, BUF_SIZE, "%s %lf", op, angle);
    sendto(sockfd, buffer, strlen(buffer), 0, (sockaddr*)&servaddr, len);

    recvfrom(sockfd, buffer, BUF_SIZE, 0, (sockaddr*)&servaddr, &len);
    buffer[BUF_SIZE - 1] = '\0'; 
    cout << "Server: " << buffer << endl;

    closesocket(sockfd);
    WSACleanup();
    return 0;
}
// g++ udp_trig_server_win.cpp -o trig_server -lws2_32
// g++ udp_trig_client_win.cpp -o trig_client -lws2_32

// .\trig_server
// .\trig_client
