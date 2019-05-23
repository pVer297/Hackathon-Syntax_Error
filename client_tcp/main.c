#include <stdio.h>
#include <ws2tcpip.h>
#include <winsock2.h>
#include <string.h>

// Link with ws2_32.lib
#pragma comment(lib, "ws2_32.lib");

#define DEFAULT_BUFLEN 256
#define IP_ADDRESS "10.27.6.113"
#define PORT 27015

int main()
{
    // Declare and initialize variables
    int ws_result;
    WSADATA ws_data;
    WORD ws_ver =  MAKEWORD(2, 2);

    // Initialize Winsock
    ws_result = WSAStartup(ws_ver, &ws_data);
    if (ws_result != NO_ERROR) {
        printf("WSAStartup failed with error: %d\n", ws_result);
        return 1;
    } else {
        printf("WSAStartup done!\n");
    }

    // Create a SOCKET for connecting to server
    SOCKET connect_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (connect_socket == INVALID_SOCKET) {
        printf("Socket failed with error: %d \n", WSAGetLastError());
        WSACleanup();
        return 1;
    } else {
        printf("Socket created!\n");
    }

    // The sockaddr_in structure specifies the address family,
    // IP address, and port of the server to be connected to.
    struct sockaddr_in client_service;
    client_service.sin_family = AF_INET;
    client_service.sin_addr.s_addr = inet_addr(IP_ADDRESS);
    client_service.sin_port = htons(PORT);

    // Connect to server
    int connect_result = connect(connect_socket, (SOCKADDR*)&client_service, sizeof(client_service));
    if (connect_result == SOCKET_ERROR) {
        printf("Connect failed with error: %d\n", WSAGetLastError());
        closesocket(connect_socket);
        WSACleanup();
        return 1;
    } else {
        printf("Connected to the server!\n");
    }

    char buf[DEFAULT_BUFLEN * 16];
    char user_input[DEFAULT_BUFLEN];

    // Send and receive data
    do {
        printf("CLIENT> ");
        gets(user_input);
        // Make sure the user typed in something
        if (strlen(user_input) > 0) {
            // Send message to srv
            int send_result = send(connect_socket, user_input, strlen(user_input) + 1, 0);
            // Wait for response
            if (send_result != SOCKET_ERROR) {
                memset(&buf, 0, sizeof(buf));
                int bytes_received = recv(connect_socket, buf,4096, 0);
                // Echo to console
                if (bytes_received > 0) {
                    printf("SERVER> ");
                    for (int i = 0; i < bytes_received; i++) {
                        printf("%c", buf[i]);
                    }
                    printf(" <\n");
                }
            }
        }
    } while (strlen(user_input) > 0);

    // Close down everything
    int close_result = closesocket(connect_socket);
    if (close_result == SOCKET_ERROR) {
        printf("Close failed with error: %d\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }

    WSACleanup();
    return 0;
}