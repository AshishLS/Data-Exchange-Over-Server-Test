#include <iostream>
#include <string>
#include <Winsock2.h>
#include <WS2tcpip.h> // Include for InetPton

#pragma comment(lib, "Ws2_32.lib")

bool BindAndListen(SOCKET serverSocket, sockaddr_in& serverAddr) {
    // Bind socket
    if (bind(serverSocket, reinterpret_cast<sockaddr*>(&serverAddr), sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Bind failed: " << WSAGetLastError() << std::endl;
        return false;
    }

    // Listen for incoming connections
    if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR) {
        std::cerr << "Listen failed: " << WSAGetLastError() << std::endl;
        return false;
    }

    return true;
}

int main() {
    // Starting server.
    std::cout << "Starting the server.." << std::endl;

    // Initialize Winsock
    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0) {
        std::cerr << "WSAStartup failed: " << result << std::endl;
        return 1;
    }

    // Server details
    std::wstring server_ip = L"127.0.0.1"; // Server IP address as wide string
    int server_port = 8000; // Server port

    // Create socket
    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (serverSocket == INVALID_SOCKET) {
        std::cerr << "Socket creation failed: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return 1;
    }

    // Server address structure
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(server_port);

    // Convert IP address from string to binary format
    if (InetPton(AF_INET, server_ip.c_str(), &serverAddr.sin_addr) != 1) {
        std::cerr << "Invalid IP address: " << server_ip.c_str() << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    // Attempt to bind and listen to the port
    if (!BindAndListen(serverSocket, serverAddr)) {
        // If binding and listening failed, exit the program
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }
    // Starting server.
    std::cout << "Waiting For connections.." << std::endl;

    // Accept incoming connections
    sockaddr_in clientAddr;
    int clientAddrSize = sizeof(clientAddr);
    SOCKET clientSocket = accept(serverSocket, reinterpret_cast<sockaddr*>(&clientAddr), &clientAddrSize);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "Accept failed: " << WSAGetLastError() << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    // Receive data from the client
    //int timeout_ms = 5000; // 5 seconds
    //setsockopt(clientSocket, SOL_SOCKET, SO_RCVTIMEO, (const char*)&timeout_ms, sizeof(timeout_ms));
    const int bufferSize = 1024;
    char buffer[bufferSize];
    do {
        int bytesReceived = recv(clientSocket, buffer, bufferSize, 0);

        if (bytesReceived == SOCKET_ERROR) {
            std::cerr << "Receive failed: " << WSAGetLastError() << std::endl;
            closesocket(clientSocket);
            closesocket(serverSocket);
            WSACleanup();
            return 1;
        }
        // Null-terminate the received data
        buffer[bytesReceived] = '\0';
        // Print the received data
        std::cout << "Received data from the client: " << buffer << std::endl;
    } while (strcmp("C", buffer) != 0);

    // Close sockets
    closesocket(clientSocket);
    closesocket(serverSocket);

    // Cleanup Winsock
    WSACleanup();

    return 0;
}
