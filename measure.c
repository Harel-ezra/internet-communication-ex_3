#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define SERVER_PORT 5765

int main(int argc, char **argv)
{
    int sock;
    int e;
    struct sockaddr_in serverAddress;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1)
    {
        printf("cant creat a socket");
        perror("[-]Error in socket");
        exit(1);
    }
    printf("[+]Server socket created successfully.\n");

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(port);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    inet_pton(AF_INET, *ip, &(serverAddress.sin_addr));

    int listeningSocket = socket(AF_INET, SOCK_STREAM, 0);

    e = bind(sock, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (e == -1)
    {
        printf("cant bind");
        perror("[-]Error in bind");
        exit(1);
    }

    

    return 0;
}