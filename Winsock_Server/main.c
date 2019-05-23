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
    uint8_t stop_flag = 0;

    for (;;) {
        if (!connect_client(&server_socket, &client_socket)) {
            return 1;
        }

        char receive_buffer[BUFF_LEN];
        int received_len = BUFF_LEN;
        int byte_rec = 0;
        do {
            byte_rec = recv(client_socket, receive_buffer, received_len, 0);
            if (byte_rec > 0) {
                printf("Bytes received: %d\n", byte_rec);
                printf("\t\"%s\"\n", receive_buffer);
                if(strcmp(receive_buffer, "stop") == 0) {
                    stop_flag = 1;
                    break;
                }

                int send_result = send(client_socket, receive_buffer, byte_rec, 0);
                if (send_result == SOCKET_ERROR) {
                    printf("Send failed. Error: %d\n", WSAGetLastError());
                    closesocket(client_socket);
                    break;
                }
            } else if (byte_rec == 0) {
                printf("Connection closing...\n");
            } else {
                closesocket(client_socket);
                break;
            }

        } while (byte_rec > 0);
        if(stop_flag == 1) {
            break;
        }
    }

    error = shutdown(client_socket, SD_SEND);
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