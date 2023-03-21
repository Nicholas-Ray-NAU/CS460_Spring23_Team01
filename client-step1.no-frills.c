#include "client-step1.no-frills.h"

/************************************************************************
 * MAIN
 ************************************************************************/
int main() {
    int client_socket;                        // client side socket

    // create an unnamed socket, and then name it
    client_socket = socket(AF_INET, SOCK_STREAM, 0);

    // connect to resolved host time server
    collatzConnect(SERVER_ADDR, client_socket);

    return EXIT_SUCCESS;
}

void collatzConnect(char * server_IP, int client_socket)
    {
    struct sockaddr_in client_address;  // client socket naming struct
    int int_buffer;                     // read char from server

    // create addr struct
    client_address.sin_family = AF_INET;
    client_address.sin_addr.s_addr = inet_addr(SERVER_ADDR);
    client_address.sin_port = htons(PORT);

    // connect to server socket
    if (connect(client_socket, (struct sockaddr *)&client_address, sizeof(client_address)) == -1) {
        exit(EXIT_FAILURE);
    }

    // Get int to send to server
    printf("Input Number: ");
    scanf("%d", &int_buffer);

    //communicate to server
    write(client_socket, &int_buffer, sizeof(int));
    read(client_socket, &int_buffer, sizeof(int));

    // Print out result
    printf("Number of moves to get to 1: %d", int_buffer);
    close(client_socket);

    printf("\nDone!\n");

    }
