#include <stdio.h>
#include <winsock2.h>
#include <ws2tcpip.h>

// Link with ws2_32.lib
#pragma comment(lib, "ws2_32.lib");

#define PORT 54000

int main()
{
    // Initialize winsock
    WSADATA ws_data;
    WORD ws_ver = MAKEWORD(2, 2);

    int ws_result = WSAStartup(ws_ver, &ws_data);
    if (ws_result != NO_ERROR) {
        printf("WSAStartup failed with error: %d\n", ws_result);
        return 1;
    } else {
        printf("WSAStartup done!\n");
    }

    // Create socket
    SOCKET listening = socket(AF_INET, SOCK_STREAM, 0);
    if (listening == INVALID_SOCKET) {
        printf("Socket failed with error: %d \n", WSAGetLastError());
        WSACleanup();
        return 1;
    } else {
        printf("Socket created!\n");
    }

    // Bind the ip address and port to a socket
    struct sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_addr.s_addr = htonl(INADDR_ANY);
    hint.sin_port = htons(PORT);

    bind(listening, (SOCKADDR*)&hint, sizeof(hint));

    // Tell Winsock the socket is for listening
    listen(listening, SOMAXCONN);

    // Wait for a connection
    struct sockaddr_in client;
    int client_size = sizeof(client);

    SOCKET client_socket = accept(listening, (SOCKADDR*)&client, &client_size);

    char host[NI_MAXHOST]; // client's remote name
    char service[NI_MAXHOST]; // Service (i.e. port) the client is connect on
    memset(host, 0, NI_MAXHOST);
    memset(service, 0, NI_MAXHOST);

    if (getnameinfo((SOCKADDR*)&client, sizeof(client), host, NI_MAXHOST, service, NI_MAXSERV,  0) == 0) {
        printf("%s connected on port %s\n", host, service);
    } else {
        printf("client connected on whatever port\n");
    }

    // Close listening socket
    closesocket(listening);
    // While loop: accept and echo message back to client
    char buf[4096];

    while (1) {
        memset(buf, 0, 4096);
        // wait for client to send data
        int bytes_received = recv(client_socket, buf, 4096, 0);
        if (bytes_received == SOCKET_ERROR) {
            printf("Error in recv(). Quitting\n");
            break;
        }
        if (bytes_received == 0)
        {
            printf("Client disconnected!\n");
            break;
        }
        // echo message back to client
        send(client_socket, buf, bytes_received + 1, 0);
    }
    // Close the socket
    closesocket(client_socket);
    // Shutdown winsock
    WSACleanup();
    return 0;
}