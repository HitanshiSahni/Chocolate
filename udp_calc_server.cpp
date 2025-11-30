#include <iostream>
#include <winsock2.h>
#include <cmath>
using namespace std;

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#define PORT 9090
#define BUF_SIZE 1024

int main() {
    WSADATA wsa;
    SOCKET sockfd;
    sockaddr_in servaddr{}, cliaddr{};
    int len = sizeof(cliaddr);
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
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(PORT);

    if (bind(sockfd, (sockaddr*)&servaddr, sizeof(servaddr)) == SOCKET_ERROR) {
        cout << "Bind failed\n";
        return 1;
    }

    cout << "UDP Trigonometry Server running...\n";

    int bytes = recvfrom(sockfd, buffer, BUF_SIZE, 0, (sockaddr*)&cliaddr, &len);
    buffer[bytes] = '\0';   //  FIXED

    cout << "Request: " << buffer << endl;

    char op[10];
    double angle, result = 0.0;
    sscanf(buffer, "%s %lf", op, &angle);

    // Convert degrees to radians
    angle = angle * M_PI / 180.0;

    if (strcmp(op, "sin") == 0) result = sin(angle);
    else if (strcmp(op, "cos") == 0) result = cos(angle);
    else if (strcmp(op, "tan") == 0) result = tan(angle);
    else {
        snprintf(buffer, BUF_SIZE, "Invalid operation");
        sendto(sockfd, buffer, strlen(buffer), 0, (sockaddr*)&cliaddr, len);
        return 0;
    }

    snprintf(buffer, BUF_SIZE, "Result: %.4f", result);
    sendto(sockfd, buffer, strlen(buffer), 0, (sockaddr*)&cliaddr, len);

    closesocket(sockfd);
    WSACleanup();
    return 0;
}
