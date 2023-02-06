#include "server.h"

/************************************************************************
 * MAIN
 ************************************************************************/
int main() {
    char input[100];                    // buffer for user input
    int client_socket;                  // client side socket
    struct sockaddr_in client_address;  // client socket naming struct
    char c;

    printf("Getting time information...\n");

    // create an unnamed socket, and then name it
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    // create addr struct
    client_address.sin_family = AF_INET;
    client_address.sin_addr.s_addr = inet_addr(SERVER_ADDR);
    client_address.sin_port = htons(PORT);

    // connect to server socket
    if (connect(client_socket, (struct sockaddr *)&client_address, sizeof(client_address)) == -1) {
        perror("Error connecting to server!\n");
        exit(EXIT_FAILURE);
    }

    while (TRUE) {

        int i = 0;
        read(client_socket, &c, sizeof(char));

        printf("%c", c);
        if(c == '*') {
            close(client_socket);
            printf("\n\n");
            exit(EXIT_SUCCESS);
        }
    }

    return EXIT_SUCCESS;
}

