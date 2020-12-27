#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define SERVER_PORT 5765
#define SERVER_IP_ADDRESS "172.17.17.23"
#define SIZE 1024

void send_file(FILE *fp, int sockfd)
{
    int n;
    char data[SIZE] = {0};

    while (fgets(data, SIZE, fp) != NULL)
    {
        if (send(sockfd, data, sizeof(data), 0) == -1)
        {
            perror("[-]Error in sending file.");
            exit(1);
        }
        bzero(data, SIZE);
    }
}

int main()
{

    int sock;
    struct sockaddr_in serverAddress;
    int e;
    FILE *fp;
    char *fileName = "text.txt";

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1)
    {
        printf("Could not create socket\n");
        perror("[-]Error in socket");
        exit(1);
    }
    printf("[+]Server socket created successfully.\n");

    memset(&serverAddress, 0, sizeof(serverAddress));

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(SERVER_PORT);
    int rval = inet_pton(AF_INET, (const char *)SERVER_IP_ADDRESS, &serverAddress.sin_addr);
    if (rval <= 0)
    {
        printf("inet_pton() failed\n");
        return 1;
    }

    e = connect(sock, (struct sockaddr *)&serverAddress, sizeof(serverAddress));
    if (e == -1)
    {
        printf("server connection is failed\n");
        perror("[-]Error in socket");
        exit(1);
    }

    printf("[+]Connected to Server.\n");

    fp = fopen(fileName, "r");
    if (fp == NULL)
    {
        printf("fail to read the file\n.");
        perror("[-]Error in reading file.");
        exit(1);
    }
    for (int i = 0; i < 5; i++)
    {
        send_file(fp, sock);
    }
    printf("[+]File data sent successfully -part A.\n");

    close(sock);
    return 0;
}