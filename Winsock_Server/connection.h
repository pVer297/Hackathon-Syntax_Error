#ifndef WINSOCK_SERVER_CONNECTION_H
#define WINSOCK_SERVER_CONNECTION_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "Ws2_32.lib")

#define BUFF_LEN 2048

uint8_t WSA_config(uint8_t major_ver, uint8_t minor_ver);
uint8_t addrinfo_config(ADDRINFOA** result, const char* port);
uint8_t listener_config(ADDRINFOA* addrinfo, SOCKET* socket_data);
uint8_t connect_client(SOCKET server, SOCKET* client);
uint8_t echo(SOCKET client);
DWORD WINAPI echo_handler(void* sock);

#endif //WINSOCK_SERVER_CONNECTION_H
