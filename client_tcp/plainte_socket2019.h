#ifndef CLIENT_TCP_PLAINTE_SOCKET2019_H
#define CLIENT_TCP_PLAINTE_SOCKET2019_H

#include <stdio.h>
#include <ws2tcpip.h>
#include <winsock2.h>
#include <string.h>
#include <stdlib.h>

#pragma comment(lib, "ws2_32.lib");

#define DEFAULT_BUFLEN 256
#define IP_ADDRESS "10.27.6.113"
#define PORT 47698

int create_client();
int create_socket(SOCKET socket);
int start_wsa();
int connect_sck(SOCKET socket, struct sockaddr_in client);
int send_and_receive_data(SOCKET socket);
int close_sck(SOCKET socket);
int exit_command(int* username, char* user_input, char* usr_name);

#endif //CLIENT_TCP_PLAINTE_SOCKET2019_H
