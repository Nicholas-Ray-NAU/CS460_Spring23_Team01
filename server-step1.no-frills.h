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
#include <signal.h>

/* Function prototypes */
void* handle_client(void* args);

/* Preprocessor directives */
#define PORT 23657              // port the server will listen on

#define FALSE 0
#define TRUE !FALSE

#define NUM_CONNECTIONS 5       // number of pending connections in the connection queue

