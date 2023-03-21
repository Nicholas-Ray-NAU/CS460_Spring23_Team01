#include "client-step1.byte-order.h"

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
    unsigned long network_long_buff, result_long_buff;

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

    // Switch endianness of input integer
    network_long_buff = htonl(int_buffer);

    //communicate to server
    write(client_socket, &network_long_buff, sizeof(unsigned long));
    read(client_socket, &result_long_buff, sizeof(unsigned long));

    // Switch endianness of resulting integer
    result_long_buff = ntohl(result_long_buff);

    // Print out result
    printf("Number of moves to get to 1: %lu", result_long_buff);
    close(client_socket);

    printf("\nDone!\n");

    }
