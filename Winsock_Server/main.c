#include <stdio.h>
#include <stdlib.h>
#include "connection.h"

const char* DEFAULT_PORT = "47698";

int main(int argc, char* argv[])
{
    system("color 0A");
    char port[6];
    memset(port, 0, 6);
    if(argc < 2) {
        puts("Usage: <server.exe [port]>");
        puts("Using default port: <47698>");
        memcpy(port, DEFAULT_PORT, 6);
    } else {
        memcpy(port, argv[1], 6);
    }
    if (!WSA_config(2, 2)) {
        return 1;
    }

    ADDRINFOA* result = NULL;
    if(!addrinfo_config(&result, port)) {
        return 1;
    }

    SOCKET server_socket;
    if(!listener_config(result, &server_socket)) {
        return 1;
    }
    freeaddrinfo(result);

    SOCKET client_socket;

    for (;;) {
        if (connect_client(server_socket, &client_socket)) {
            DWORD threadID;
            CreateThread(0, 0, echo_handler, (void*)client_socket, 0, &threadID);
        }
    }

    int error = shutdown(client_socket, SD_SEND);
    if (error == SOCKET_ERROR) {
        printf("Shutdown failed. Error: %d\n", WSAGetLastError());
        closesocket(client_socket);
        WSACleanup();
        return 1;
    }

    puts("Exit");
    closesocket(server_socket);
    closesocket(client_socket);
    WSACleanup();
    return 0;
}