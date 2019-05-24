#include "socket19.h"

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
    char usr_name[20];
    do {
        if (username == 0) {
            username = 1;
            printf("For help use --help:\n");
            printf("Please select a username!> ");
            gets(usr_name);
        }
        printf("%s> ", usr_name);
        gets(user_input);
        // Make sure the user typed in something
        if (strlen(user_input) > 0) {
            char send_buf[DEFAULT_BUFLEN];
            strcpy(send_buf, user_input);
            char delim[] = " ";
            char *ptr = strtok(user_input, delim);
            if (stricmp(ptr, "--username") == 0) {
                ptr = strtok(NULL, delim);
                memset(usr_name, 0, strlen(usr_name));
                for (int i = 0; i < strlen(ptr); i++) {
                    usr_name[i] = ptr[i];
                }
                printf("Your new username is: %s\n", usr_name);
            } else if (stricmp(user_input, "--help") == 0) {
                system("@cls||clear");
                printf("Please find below all the available commands:\n");
                printf("\t--help -check all commands\n");
                printf("\t--exit -disconnect from the server\n");
                printf("\t--username <new_username> -change username\n");
                printf("\t--lidar start - start the motor\n");
                printf("\t--lidar stop - stops the motor\n");
                printf("\t--lidar health - check lidar health (0-good, 1-warning, 2-error)\n");
                printf("\t--lidar reset - resets core\n");
                printf("\t--lidar read_single <angle1> <angle2> (value can be 0-360)\n");
                printf("\t--lidar read_continous <angle1> <angle2> (value can be 0-360)\n");
                printf("\t--motor forward <speed> <type>\n");
                printf("\t--motor backward <speed> <type>\n");
                printf("\t--servo turn_left <value1> <value2> (value can be 0-100, 0 0-> reset)\n");
                printf("\t--servo turn_right <value1> <value2> (value can be 0-100, 0 0-> reset)\n");
            } else {
                // Send message to srv
                // Wait for response
                if (send(socket, send_buf, strlen(send_buf) + 1, 0) != SOCKET_ERROR) {
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
        }
        if (stricmp(user_input, "--exit") == 0) {
            return 0;
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

void clear_up()
{
    system("@cls||clear");
}

void print_commands()
{
    clear_up();
    printf("Please find below all the available commands:\n");
    printf("\t--help -check all commands\n");
    printf("\t--exit -disconnect from the server\n");
    printf("\t--clear -clearas window\n");
    printf("\t--username <new_username> -change username\n");
    printf("\t--lidar start - start the motor\n");
    printf("\t--lidar stop - stops the motor\n");
    printf("\t--lidar health - check lidar health (0-good, 1-warning, 2-error)\n");
    printf("\t--lidar reset - resets core\n");
    printf("\t--lidar read_single <angle1> <angle2> (value can be 0-360)\n");
    printf("\t--lidar read_continous <angle1> <angle2> (value can be 0-360)\n");
    printf("\t--motor forward <speed> <type>\n");
    printf("\t--motor backward <speed> <type>\n");
    printf("\t--servo turn_left <value1> <value2> (value can be 0-100, 0 0-> reset)\n");
    printf("\t--servo turn_right <value1> <value2> (value can be 0-100, 0 0-> reset)\n");
}