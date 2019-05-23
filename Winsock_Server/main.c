#include <stdio.h>
#include <stdlib.h>
#include "connection.h"

int main()
{
    WSADATA wsa_data;
    int error;

    if (!WSA_config(2, 2, &wsa_data)) {
        return 1;
    }

    ADDRINFOA* result = NULL;
    if(!addrinfo_config(&result)) {
        return 1;
    }

    SOCKET server_socket;
    if(!socket_config(result, &server_socket)) {
        return 1;
    }
    freeaddrinfo(result);

    SOCKET client_socket;
    if(!connect_client(&server_socket, &client_socket)) {
        return 1;
    }

    char recieve_buffer[BUFF_LEN];
    int recieved_len = BUFF_LEN;
    int byte_rec = 0;
    do {
        byte_rec = recv(client_socket, recieve_buffer, recieved_len, 0);
        if (byte_rec > 0) {
            printf("Bytes recieved: %d\n", byte_rec);

            int send_result = send(client_socket, recieve_buffer, byte_rec, 0);
            if (send_result == SOCKET_ERROR) {
                printf("Send failed. Error: %d\n", WSAGetLastError());
                closesocket(server_socket);
                closesocket(client_socket);
                WSACleanup();
                return 1;
            }
        } else if (byte_rec == 0) {
            printf("Connection closing...\n");
        } else {
            printf("Recieve failed. Error: %d\n", WSAGetLastError());
            closesocket(server_socket);
            closesocket(client_socket);
            WSACleanup();
            return 1;
        }

    } while (byte_rec > 0);

    error = shutdown(client_socket, SD_SEND);
    if (error == SOCKET_ERROR) {
        printf("Shutdown failed. Error: %d\n", WSAGetLastError());
        closesocket(client_socket);
        WSACleanup();
        return 1;
    }

    puts("Exit");
    closesocket(client_socket);
    WSACleanup();
    return 0;
}