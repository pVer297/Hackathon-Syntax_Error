#ifndef CLIENT_TCP_SOCKET19_H
#define CLIENT_TCP_SOCKET19_H

#include <stdio.h>
#include <ws2tcpip.h>
#include <winsock2.h>
#include <string.h>
#include <stdlib.h>

#pragma comment(lib, "ws2_32.lib");

#define DEFAULT_BUFLEN 256
#define IP_ADDRESS "10.27.6.113"
#define PORT 47698

int create_socket(SOCKET socket);
int start_wsa();
int connect_sck(SOCKET socket, struct sockaddr_in client);
int send_and_receive_data(SOCKET socket);
int close_sck(SOCKET socket);

void clear_up();
void print_commands();

#endif //CLIENT_TCP_SOCKET19_H
