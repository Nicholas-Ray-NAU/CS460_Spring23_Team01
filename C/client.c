#include "server.h"

/************************************************************************
 * MAIN
 ************************************************************************/
int main() {
    char *time_server_name = "time.nist.gov"; // hostname
    int client_socket;                        // client side socket
    struct hostent *host_struct;              // resolved hostname struct
    struct in_addr **addr_list;               // resolved host IP address(es)
    char time_server_IP[100];                 // buffer for ip address
    int index;

    // create an unnamed socket, and then name it
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
        
    // connect to student daytime server
    printf("Student Time Server: ");
    dayimeConnect(HOSTWIND_ADDR, client_socket);
    
    // attempt to resolve hostname
    if( (host_struct = gethostbyname(time_server_name)) == NULL )
        {
        printf("failed to resolve hostname\n");
        exit(EXIT_FAILURE);
        }
    
    // attempt to resolve host IP
    if(host_struct->h_addrtype == AF_INET)
        {
        addr_list = (struct in_addr **)host_struct->h_addr_list;
        for(index=0; addr_list[index] != NULL; index++)
            {
            strcpy(time_server_IP, inet_ntoa(*addr_list[index]));    
            }
        }
    else
        {
        printf("failed to resolve host IP\n");
        exit(EXIT_FAILURE);
        }
    
    // create an unnamed socket, and then name it
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    
    // connect to resolved host time server
    printf("NIST Time Server: ");
    dayimeConnect(time_server_IP, client_socket);
    
    return EXIT_SUCCESS;
}

void dayimeConnect(char * server_IP, int client_socket)
    {
    struct sockaddr_in client_address;        // client socket naming struct
    char output_time[100];                    // buffer for output
    char in_char = '\0';                      // read char from server
    int index = 0;
    
    // create addr struct
    client_address.sin_family = AF_INET;
    client_address.sin_addr.s_addr = inet_addr(server_IP);
    client_address.sin_port = htons(TIME_PORT);
    
    // connect to server socket
    if (connect(client_socket, (struct sockaddr *)&client_address, sizeof(client_address)) == -1) {
        perror("Error connecting to server!\n");
        exit(EXIT_FAILURE);
    }
    
    while( read(client_socket, &in_char, sizeof(in_char)) != '\0' )
        {
        
        output_time[index] = in_char;
        index++;
        output_time[index] = '\0';
        
        }
    
    printf("%s", output_time);
    close(client_socket);
    printf("\nDone!\n");
        
    }
