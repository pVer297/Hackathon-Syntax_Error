#include "win_socket2019.h"

int main()
{
    // Declare and initialize variables
    // Initialize Winsock
    if(!start_wsa()) {
        return -1;
    }
    // Create a SOCKET for connecting to server
    SOCKET connect_socket = socket(AF_INET, SOCK_STREAM, 0);
    if(!create_socket(connect_socket)) {
        return -1;
    }
    // The sockaddr_in structure specifies the address family,
    // IP address, and port of the server to be connected to.
    struct sockaddr_in client_service;
    client_service.sin_family = AF_INET;
    client_service.sin_addr.s_addr = inet_addr(IP_ADDRESS);
    client_service.sin_port = htons(PORT);
    // Connect to server
    if(!connect_sck(connect_socket, client_service)) {
        return -1;
    }
    // Send and receive data
    if(!send_and_receive_data(connect_socket)) {
        return -1;
    }
    // Close down everything
    if(!close_sck(connect_socket)) {
        return -1;
    }
    WSACleanup();
    return 0;
}