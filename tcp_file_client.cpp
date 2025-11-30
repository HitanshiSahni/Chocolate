#include <iostream>
#include <fstream>
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")

#define PORT 9090

int main() {
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in serv{};
    serv.sin_family = AF_INET;
    serv.sin_port = htons(PORT);
    serv.sin_addr.s_addr = inet_addr("127.0.0.1");

    connect(sock, (sockaddr*)&serv, sizeof(serv));

    std::string filename;
    std::cout << "Enter filename to download: ";
    std::cin >> filename;

    // Send filename request
    send(sock, filename.c_str(), filename.size(), 0);

    // Receive file size
    long fileSize;
    recv(sock, (char*)&fileSize, sizeof(long), 0);

    if (fileSize <= 0) {
        std::cout << "❌ Server reported file not found.\n";
        return 1;
    }

    std::cout << "📥 Downloading " << filename << " (" << fileSize << " bytes)...\n";

    std::ofstream outfile("downloaded_" + filename, std::ios::binary);

    char buffer[1024];
    long totalReceived = 0;
    int bytes;

    while (totalReceived < fileSize) {
        bytes = recv(sock, buffer, sizeof(buffer), 0);
        outfile.write(buffer, bytes);
        totalReceived += bytes;

        int progress = (int)((totalReceived * 100) / fileSize);
        std::cout << "\rProgress: " << progress << "%";
    }

    std::cout << "\n✅ File downloaded successfully!\n";

    outfile.close();
    closesocket(sock);
    WSACleanup();
    return 0;
}
// Compile Server:
// g++ file_server_any.cpp -o file_server_any -lws2_32

// Compile Client:
// g++ file_client_any.cpp -o file_client_any -lws2_32

// Run server first:
// .\file_server_any.exe

// Run client in another terminal:
// .\file_client_any.exe
