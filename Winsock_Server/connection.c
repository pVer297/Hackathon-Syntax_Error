#include "connection.h"

uint8_t WSA_config(uint8_t major_ver, uint8_t minor_ver, WSADATA* wsa_data)
{
    WORD wversion_req = MAKEWORD(major_ver, minor_ver);
    int error = WSAStartup(wversion_req, wsa_data);
    if (error) {
        printf("WSAStartup failed with error code '%d'\n", error);
        WSACleanup();
        return 0;
    } else {
        printf("WinSock initialized successfully with version %d.%d\n",
               LOBYTE(wsa_data->wVersion),
               HIBYTE(wsa_data->wVersion));
        return 1;
    }
}

uint8_t addrinfo_config(ADDRINFOA** result)
{
    ADDRINFOA hints;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    int error = getaddrinfo(NULL, SERVER_PORT, &hints, result);
    if (error) {
        printf("Address info resolving failed. Error: %d\n", error);
        WSACleanup();
        return 0;
    } else {
        puts("Address info resolved!");
        return 1;
    }
}

uint8_t socket_config(ADDRINFOA* addrinfo, SOCKET* socket_data)
{
    *socket_data = socket(addrinfo->ai_family, addrinfo->ai_socktype, addrinfo->ai_protocol);
    if (*socket_data == INVALID_SOCKET) {
        printf("Socket creation failed. Error: %d\n", WSAGetLastError());
        freeaddrinfo(addrinfo);
        WSACleanup();
        return 0;
    }

    int error = bind(*socket_data, addrinfo->ai_addr, (int) addrinfo->ai_addrlen);

    if (error == SOCKET_ERROR) {
        printf("Socket binding failed. Error: %d\n", WSAGetLastError());
        freeaddrinfo(addrinfo);
        closesocket(*socket_data);
        WSACleanup();
        return 0;
    }

    if (listen(*socket_data, SOMAXCONN) == SOCKET_ERROR) {
        printf("Listen failed with error: %d\n", WSAGetLastError());
        closesocket(*socket_data);
        WSACleanup();
        return 0;
    } else {
        puts("Socket config done!");
        return 1;
    }
}

uint8_t connect_client(SOCKET* server, SOCKET* client)
{
    *client = accept(*server, NULL, NULL);
    if (*client == INVALID_SOCKET) {
        printf("Client socket rejected. Error: %d\n", WSAGetLastError());
        closesocket(*server);
        WSACleanup();
        return 0;
    } else {
        puts("Client connected!");
        return 1;
    }
}