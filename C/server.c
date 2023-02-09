#include "server.h"

/* ************************************************************************* */
/* MAIN                                                                      */
/* ************************************************************************* */

int main(int argc, char** argv) {
    int server_socket;                 // descriptor of server socket
    struct sockaddr_in server_address; // for naming the server's listening socket

    // sent when client disconnected
    signal(SIGPIPE, SIG_IGN);
    
    // create unnamed network socket for server to listen on
    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Error creating socket");
        exit(EXIT_FAILURE);
    }
    
    // name the socket (making sure the correct network byte ordering is observed)
    server_address.sin_family      = AF_INET;                  // accept IP addresses
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);        // accept clients on any interface
    server_address.sin_port        = htons(LISTEN_PORT);       // port to listen on
    
    // binding unnamed socket to a particular port
    if (bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)) != 0) {
        perror("Error binding socket");
        exit(EXIT_FAILURE);
    }
    
    // listen for client connections (pending connections get put into a queue)
    if (listen(server_socket, NUM_CONNECTIONS) != 0) {
        perror("Error listening on socket");
        exit(EXIT_FAILURE);
    }
    
    // server loop
    while (TRUE) {
        
        // accept connection to client
        int client_socket = accept(server_socket, NULL, NULL);
        printf("\nServer with PID %d: accepted client\n", getpid());

        // create thread to handle the client's request
        // note that this is a naive approach, i.e. there are race conditions
        // for now this is okay, assuming low load
        pthread_t thread;
        if (pthread_create(&thread, NULL, (void*)handle_client, (void*)&client_socket) != 0) {
            perror("Error creating thread");
            exit(EXIT_FAILURE);
        }
        
        // detach the thread so that we don't have to wait (join) with it to reclaim memory.
        // memory will be reclaimed when the thread finishes.
        if (pthread_detach(thread) != 0) {
            perror("Error detaching thread");
            exit(EXIT_FAILURE);
        }
    }
}


/* ************************************************************************* */
/* handle client                                                             */
/* ************************************************************************* */

void* handle_client(void* arg) { // xxxxx
    int client_socket = *((int*)arg);         // the socket connected to the client
	char *time_server_name = "time.nist.gov"; // hostname
    int time_socket;                          // client side socket
    struct sockaddr_in time_address;          // client socket naming struct
    struct hostent *host_struct;              // resolved hostname struct
    struct in_addr **addr_list;               // Resolved IP address(es)
    char server_IP[100];                      // ip address buffer
    char in_char = '\0';                      // input buffer
    char output_time[100];                    // buffer for output
    int index;
    
    // attempt to resolve hostname
    if( (host_struct = gethostbyname(time_server_name)) == NULL)
        {
        printf("failed to resolve hostname\n");
        exit(EXIT_FAILURE);
        }
    
    //attempt to resolve IP address(es)
    if(host_struct->h_addrtype == AF_INET)
        {
        addr_list = (struct in_addr **)host_struct->h_addr_list;
        for(index=0; addr_list[index] != NULL; index++)
            {
            strcpy(server_IP, inet_ntoa(*addr_list[index]));    
            }
        }
    else
        {
        printf("failed to resolve host IP\n");
        exit(EXIT_FAILURE);
        }

    // create an unnamed socket, and then name it
    time_socket = socket(AF_INET, SOCK_STREAM, 0);
    // create addr struct
    time_address.sin_family = AF_INET;
    time_address.sin_addr.s_addr = inet_addr(server_IP);
    time_address.sin_port = htons(TIME_PORT);
    
    // connect to time server socket
    if (connect(time_socket, (struct sockaddr *)&time_address, sizeof(time_address)) == -1) {
        perror("Error connecting to server!\n");
        exit(EXIT_FAILURE);
    }
    
    //read data from time server
    index = 0;
    while( read(time_socket, &in_char, sizeof(in_char)) != '\0' )
        {
        output_time[index] = in_char;
        index++;
        output_time[index] = '\0';
        }
    close(time_socket);
    
    // relay data to client
    write(client_socket, &output_time, sizeof(char) * strlen(output_time));
    
    // cleanup
    if (close(client_socket) == -1) {
        perror("Error closing socket");
        exit(EXIT_FAILURE);
    } else {
        printf("Closed socket to client, exit");
    }
    
    pthread_exit(NULL);
}

