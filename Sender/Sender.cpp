#include <iostream>
#include <string>
#include <Winsock2.h>
#include <WS2tcpip.h> // Include for InetPton

#pragma comment(lib, "Ws2_32.lib")

int main() {
    // Initialize Winsock
    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0) {
        std::cerr << "WSAStartup failed: " << result << std::endl;
        return 1;
    }

    // Server details
    std::wstring server_ip = L"127.0.0.1"; // Server IP address as wide string
    int server_port = 8080; // Server port

    // Create socket
    SOCKET sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sockfd == INVALID_SOCKET) {
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
        closesocket(sockfd);
        WSACleanup();
        return 1;
    }

    // Connect to the server
    if (connect(sockfd, reinterpret_cast<sockaddr*>(&serverAddr), sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Connection failed: " << WSAGetLastError() << std::endl;
        closesocket(sockfd);
        WSACleanup();
        return 1;
    }

    // Send data to the server
    // Loop to ask user if they want to send data
    std::string choice = "Hello Server!!";
    const int bufferSize = 1024;
    char buffer[bufferSize];
    do {
        
        int count = 0;

        {

            int bytesSent = send(sockfd, choice.c_str(), choice.size(), 0);
            if (bytesSent == SOCKET_ERROR) {
                std::cerr << "Send failed: " << WSAGetLastError() << std::endl;
                closesocket(sockfd);
                WSACleanup();
                return 1;
            }
            std::cout << "Data sent successfully\n";

            std::cout << "Enter the data string - or send C to close: ";
            std::cin >> choice;
        }

    } while (std::strcmp("C", choice.c_str()) != 0);
    
    std::cout << "Data sent successfully\n";

    // Close socket
    closesocket(sockfd);

    // Cleanup Winsock
    WSACleanup();

    std::cin.get();

    return 0;
}
