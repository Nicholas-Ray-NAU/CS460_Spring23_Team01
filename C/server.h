/************************************************************************
 * libraries
 ************************************************************************/
// should always be there ...
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

// threading
#include <pthread.h>

// socket/bind/listen/accept
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <signal.h>

// hostname utils
#include <netdb.h>

// read/write/close
#include <sys/uio.h>
#include <sys/types.h>
#include <unistd.h>

/************************************************************************
 * function prototype declarations
 ************************************************************************/
void *handle_client(void *arg);
void dayimeConnect(char * server_IP, int client_socket);

/************************************************************************
 * preprocessor directives
 ************************************************************************/
#define SERVER_ADDR "127.0.0.1"        // loopback ip address
#define HOSTWIND_ADDR "142.11.212.226" // Hostwinds IP address
#define LISTEN_PORT 13                 // port the server will listen on
#define TIME_PORT 13                   // Standard time server port

// I cannot let go of the old-fashioned way :) - for readability ...
#define FALSE false
#define TRUE !false

// number of pending connections in the connection queue
#define NUM_CONNECTIONS 1
