#include <stdio.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "Ws2_32.lib")

char* error_to_string(int error_code);


int main()
{
    WORD wversion_req = MAKEWORD(2U, 2U);
    WSADATA wsa_data;

    int error = WSAStartup(wversion_req, &wsa_data);
    if(error) {
        printf("WSAStartup failed with error code %d:\n\"%s\"\n", error, error_to_string(error));
    } else {
        printf("WSAStartup called successfully!\n");
    }

    printf("Version: %d.%d", LOBYTE(wsa_data.wVersion), HIBYTE(wsa_data.wVersion));
    WSACleanup();
    return 0;
}

char* error_to_string(int error_code)
{
    switch(error_code) {
        case WSASYSNOTREADY:
            return "The underlying network subsystem is not ready for network communication!";
        case WSAVERNOTSUPPORTED:
            return "The version of Windows Sockets support requested is not provided by this particular Windows Sockets implementation!";
        case WSAEINPROGRESS:
            return "A blocking Windows Sockets 1.1 operation is in progress!";
        case WSAEPROCLIM:
            return "A limit on the number of tasks supported by the Windows Sockets implementation has been reached!";
        case WSAEFAULT:
            return "The lpWSAData parameter is not a valid pointer!";
        default:
            return "Unknown error!";
    }
}