#include <iostream>
#include <vector>
#include <string>
#include <cerrno>
#include <cstring>
#ifdef _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
#else
    #include <unistd.h>
    #include <arpa/inet.h>
    #include <sys/socket.h>
#endif


#ifdef _WIN32
using SocketHandle = SOCKET;
#else
using SocketHandle = int;
#endif

void closeSocket(SocketHandle sock) {
#ifdef _WIN32
    closesocket(sock);
#else
    close(sock);
#endif
}

int main() {
    std::string ip;
    int port;
    int totalConnections;

    std::cout << "Enter destination IP: ";
    std::cin >> ip;

    std::cout << "Enter destination port: ";
    std::cin >> port;

    std::cout << "Enter number of connections: ";
    std::cin >> totalConnections;

    if (port < 1 || port > 65535 ||
        totalConnections < 1 || totalConnections > 500) {
        std::cerr << "Invalid input.\n";
        return 1;
    }

    std::vector<int> sockets;
    int successful = 0;

    sockaddr_in server{};
    server.sin_family = AF_INET;
    server.sin_port = htons(port);

    if (inet_pton(AF_INET, ip.c_str(), &server.sin_addr) != 1) {
        std::cerr << "Invalid IP address.\n";
        return 1;
    }

    for (int i = 0; i < totalConnections; i++) {

        int sock = socket(AF_INET, SOCK_STREAM, 0);

        if (sock < 0) {
            std::cerr << "Socket creation failed.\n";
            continue;
        }

        if (connect(sock, (sockaddr*)&server, sizeof(server)) == 0) {
            successful++;
            sockets.push_back(sock);

            std::cout << "Connection " << i + 1
                      << " successful\n";
        }
        else {
            std::cout << "Connection " << i + 1 
                      << " failed: " << strerror(errno) << std::endl;
            close(sock);
        }

        // Limit the number of simultaneously open connections
        if (sockets.size() >= 20) {
            for (int s : sockets) {
                close(s);
            }
            sockets.clear();
        }
    }

    for (int s : sockets) {
        close(s);
    }

    std::cout << "\nSuccessful connections: "
              << successful << std::endl;

    return 0;
}