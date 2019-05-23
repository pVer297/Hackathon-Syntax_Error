#include "connection.h"

uint8_t WSA_config(uint8_t major_ver, uint8_t minor_ver)
{
    WSADATA wsa_data;
    WORD wversion_req = MAKEWORD(major_ver, minor_ver);
    int error = WSAStartup(wversion_req, &wsa_data);
    if (error) {
        printf("WSAStartup failed with error code '%d'\n", error);
        WSACleanup();
        return 0;
    } else {
        printf("WinSock initialized successfully with version %d.%d\n",
               LOBYTE(wsa_data.wVersion),
               HIBYTE(wsa_data.wVersion));
        return 1;
    }
}

uint8_t addrinfo_config(ADDRINFOA** result, const char* port)
{
    ADDRINFOA hints;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    int error = getaddrinfo(NULL, port, &hints, result);
    if (error) {
        printf("Address info resolving failed. Error: %d\n", error);
        WSACleanup();
        return 0;
    } else {
        puts("Address info resolved!");
        return 1;
    }
}

uint8_t listener_config(ADDRINFOA* addrinfo, SOCKET* socket_data)
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

uint8_t connect_client(SOCKET server, SOCKET* client)
{
    SOCKADDR_IN client_connect;
    int addrSize = sizeof(client_connect);
    *client = accept(server, (SOCKADDR*)&client_connect, &addrSize);
    if (*client == INVALID_SOCKET) {
        printf("Client socket rejected. Error: %d\n", WSAGetLastError());
        closesocket(server);
        WSACleanup();
        return 0;
    } else {
        printf("Client connected: %s\n", inet_ntoa(client_connect.sin_addr));
        return 1;
    }
}

DWORD WINAPI echo_handler(void* _sock)
{
    SOCKET sock = (SOCKET)_sock;
    echo(sock);
    shutdown(sock, SD_SEND);
    return 0;
}

uint8_t echo(SOCKET client)
{
    char receive_buffer[BUFF_LEN];
    int received_len = BUFF_LEN;
    int byte_rec = 0;
    do {
        byte_rec = recv(client, receive_buffer, received_len, 0);
        if (byte_rec > 0) {
            printf("Bytes received: %d\n", byte_rec);
            printf("\t\"%s\"\n", receive_buffer);

            int send_result = send(client, receive_buffer, byte_rec, 0);
            if (send_result == SOCKET_ERROR) {
                printf("Send failed. Error: %d\n", WSAGetLastError());
                closesocket(client);
                return 0;
            }
        } else if (byte_rec == 0) {
            printf("Connection closing...\n");
        } else {
            closesocket(client);
            return 1;
        }
    } while (byte_rec > 0);
    return 1;
}