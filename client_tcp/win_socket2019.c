#include "win_socket2019.h"

int start_wsa()
{
    // Declare and initialize variables
    int ws_result;
    WSADATA ws_data;
    WORD ws_ver =  MAKEWORD(2, 2);

    // Initialize Winsock
    ws_result = WSAStartup(ws_ver, &ws_data);
    if (ws_result != NO_ERROR) {
        printf("WSAStartup failed with error: %d\n", ws_result);
        return 0;
    } else {
        printf("WSAStartup done!\n");
        return 1;
    }
}

int create_socket(SOCKET socket)
{

    if (socket == INVALID_SOCKET) {
        printf("Socket failed with error: %d \n", WSAGetLastError());
        WSACleanup();
        return 0;
    } else {
        printf("Socket created!\n");
        return 1;
    }
}
int connect_sck(SOCKET socket, struct sockaddr_in client)
{
    if (connect(socket, (SOCKADDR*)&client, sizeof(client)) == SOCKET_ERROR) {
        printf("Connect failed with error: %d\n", WSAGetLastError());
        closesocket(socket);
        WSACleanup();
        return 0;
    } else {
        printf("Connected to the server!\n");
        return 1;
    }
}

int send_and_receive_data(SOCKET socket)
{
    char buf[DEFAULT_BUFLEN * 16];
    char user_input[DEFAULT_BUFLEN];
    int username = 0;
    char usr_name[15];

    do {
        if (username == 0) {
            username = 1;
            printf("Please give a username!> ");
            gets(usr_name);
        }
        printf("%s> ", usr_name);
        gets(user_input);
        // Make sure the user typed in something
        if (strlen(user_input) > 0) {
            // Send message to srv
            // Wait for response
            if (send(socket, user_input, strlen(user_input) + 1, 0) != SOCKET_ERROR) {
                memset(&buf, 0, sizeof(buf));
                int bytes_received = recv(socket, buf, 4096, 0);
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
}

int close_sck(SOCKET socket)
{
    if (closesocket(socket) == SOCKET_ERROR) {
        printf("Close failed with error: %d\n", WSAGetLastError());
        WSACleanup();
        return 0;
    } else {
        return 1;
    }
}