#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "Ws2_32.lib")

#define SERVER_PORT "27015"
#define BUFF_LEN 1024

void program_exit();

int main()
{
    WORD wversion_req = MAKEWORD(2U, 2U);
    WSADATA wsa_data;

    int error = WSAStartup(wversion_req, &wsa_data);
    if (error) {
        printf("WSAStartup failed with error code '%d'\n", error);
        WSACleanup();
        return 1;
    } else {
        printf("WinSock initialized successfully with version %d.%d\n",
               LOBYTE(wsa_data.wVersion),
               HIBYTE(wsa_data.wVersion));
    }

    ADDRINFOA hints;
    ADDRINFOA* result = NULL;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    error = getaddrinfo(NULL, SERVER_PORT, &hints, &result);
    if (error) {
        printf("Address info resolving failed. Error: %d\n", error);
        WSACleanup();
        return 1;
    }

    SOCKET server_socket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (server_socket == INVALID_SOCKET) {
        printf("Socket creation failed. Error: %d\n", WSAGetLastError());
        freeaddrinfo(result);
        WSACleanup();
        return 1;
    }

    error = bind(server_socket, result->ai_addr, (int) result->ai_addrlen);
    freeaddrinfo(result);
    if (error == SOCKET_ERROR) {
        printf("Socket binding failed. Error: %d\n", WSAGetLastError());
        closesocket(server_socket);
        WSACleanup();
        return 1;
    }
    if (listen(server_socket, SOMAXCONN) == SOCKET_ERROR) {
        printf("Listen failed with error: %d\n", WSAGetLastError());
        closesocket(server_socket);
        WSACleanup();
        return 1;
    }

    SOCKET client_socket;

    client_socket = accept(server_socket, NULL, NULL);
    if (client_socket == INVALID_SOCKET) {
        printf("Client socket rejected. Error: %d\n", WSAGetLastError());
        closesocket(server_socket);
        WSACleanup();
        return 1;
    }

    char recieve_buffer[BUFF_LEN];
    int recieved_len = 0;
    int byte_rec = 0;
    do {
        byte_rec = recv(client_socket, recieve_buffer, recieved_len, 0);
        if (byte_rec > 0) {
            printf("Bytes recieved: %d\n", byte_rec);

            int send_result = send(client_socket, recieve_buffer, byte_rec, 0);
            if (send_result == SOCKET_ERROR) {
                printf("Send failed. Error: %d\n", WSAGetLastError());
                closesocket(client_socket);
                WSACleanup();
                return 1;
            }
        } else if (byte_rec == 0) {
            printf("Connection closing...\n");
        } else {
            printf("Recieve failed. Error: %d\n", WSAGetLastError());
            closesocket(client_socket);
            WSACleanup();
            return 1;
        }

    } while (byte_rec > 0);

    error = shutdown(client_socket, SD_SEND);
    if(error == SOCKET_ERROR) {
        printf("Shutdown failed. Error: %d\n", WSAGetLastError());
        closesocket(client_socket);
        WSACleanup();
        return 1;
    }

    closesocket(client_socket);
    WSACleanup();
    return 0;
}