#include <iostream>
#include <fstream>
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")

#define PORT 9090

int main() {
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    SOCKET server_fd = socket(AF_INET, SOCK_STREAM, 0);
    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, (char*)&opt, sizeof(opt));

    sockaddr_in address{};
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    bind(server_fd, (sockaddr*)&address, sizeof(address));
    listen(server_fd, 3);

    std::cout << "\n📂 Server ready. Waiting for client on port " << PORT << "...\n";

    int addrlen = sizeof(address);
    SOCKET new_socket = accept(server_fd, (sockaddr*)&address, &addrlen);
    std::cout << "Client connected!\n";

    char buffer[1024];
    int bytes = recv(new_socket, buffer, sizeof(buffer), 0);
    buffer[bytes] = '\0';

    std::string filename = buffer;
    std::cout << "Client requested file: " << filename << "\n";

    std::ifstream file(filename, std::ios::binary);

    if (!file) {
        const char* msg = "ERROR: File Not Found";
        send(new_socket, msg, strlen(msg), 0);
        std::cout << "❌ File not found\n";
        return 1;
    }

    // Send file size first (important)
    file.seekg(0, std::ios::end);
    long size = file.tellg();
    file.seekg(0, std::ios::beg);

    send(new_socket, (char*)&size, sizeof(long), 0);

    std::cout << "➡ Sending file (" << size << " bytes)...\n";

    long sentBytes = 0;
    while (!file.eof()) {
        file.read(buffer, sizeof(buffer));
        std::streamsize count = file.gcount();
        send(new_socket, buffer, (int)count, 0);

        sentBytes += count;

        // Progress bar
        int progress = (int)((sentBytes * 100) / size);
        std::cout << "\rProgress: " << progress << "%";
    }

    std::cout << "\n✅ File sent successfully!" << std::endl;

    file.close();
    closesocket(new_socket);
    closesocket(server_fd);
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
