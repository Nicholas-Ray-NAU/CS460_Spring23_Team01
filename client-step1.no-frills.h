#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <syslog.h>

/* Function prototypes */
void collatzConnect(char * server_IP, int client_socket);
void* handle_client(void* args);

/* Preprocessor directives */
#define SERVER_ADDR "104.168.202.152" // loopback ip address
#define PORT 23657              // port the server will listen on

#define FALSE 0
#define TRUE !FALSE

#define NUM_CONNECTIONS 5       // number of pending connections in the connection queue

