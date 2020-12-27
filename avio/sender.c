/*
	TCP/IP client
*/ 


#include <stdio.h>
#include <stdlib.h> 
#include <errno.h> 
#include <string.h> 
#include <sys/types.h> 
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/tcp.h> 



#define SERVER_PORT 5060
#define SERVER_IP_ADDRESS "192.168.100.2"
#define SIZE 1024

int main()
{
    char buf[256];
    socklen_t len;
     int sock = socket(AF_INET, SOCK_STREAM, 0);

     if(sock == -1)
    {
        printf("Could not create socket : %d" );
    }

    // "sockaddr_in" is the "derived" from sockaddr structure
    // used for IPv4 communication. For IPv6, use sockaddr_in6
    //
    struct sockaddr_in serverAddress;
    memset(&serverAddress, 0, sizeof(serverAddress));

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(SERVER_PORT);
	int rval = inet_pton(AF_INET, (const char*)SERVER_IP_ADDRESS, &serverAddress.sin_addr);
	if (rval <= 0)
	{
		printf("inet_pton() failed");
		return -1;
	}

     // Make a connection to the server with socket SendingSocket.

     if (connect(sock, (struct sockaddr *) &serverAddress, sizeof(serverAddress)) == -1)
     {
	   printf("connect() failed with error code : %d" ,errno);
     }

     printf("connected to server\n");
     sleep(1);
    for(int i=0;i<5;i++)
    {
    FILE *fp;
    char *filename = "1mb.txt";
    fp = fopen(filename, "r");
        if (fp == NULL)
        {
            perror("[-]Error in reading file.");
            exit(1);
        }
        int n;
        char data[SIZE] = {0};
        while(fgets(data, SIZE, fp) != NULL)
        {   
            n=send(sock, data, sizeof(data), 0);
            if (n==-1)
            {
                printf("cant send file");
                perror("Error in sending file.");
                exit(1);
            }
            bzero(data, SIZE);
        }
        printf("File data sent successfully. \n");
    }
    strcpy(buf, "reno"); 
    len = strlen(buf);
    if (setsockopt(sock, IPPROTO_TCP, TCP_CONGESTION, buf, len) != 0) 
    {
    perror("setsockopt"); 
    return -1;
    }
    for(int i=0;i<5;i++)
    {
    FILE *fp;
    char *filename = "1mb.txt";
    fp = fopen(filename, "r");
        if (fp == NULL)
        {
            perror("[-]Error in reading file.");
            exit(1);
        }
        int n;
        char data[SIZE] = {0};
        while(fgets(data, SIZE, fp) != NULL)
        {   
            n=send(sock, data, sizeof(data), 0);
            if (n==-1)
            {
                perror("Error in sending file.");
                exit(1);
            }
            bzero(data, SIZE);
        }
        printf("File data sent successfully. \n");
       
    }

     // Sends some data to server
    //  char message[] = "Good morning, Vietnam\n";
    //  int messageLen = strlen(message) + 1;

    //  int bytesSent = send(sock, message, messageLen, 0);

    //  if (-1 == bytesSent)
    //  {
	//     printf("send() failed with error code : %d",errno);
    //  }
    //  else if (0 == bytesSent)
    //  {
	//     printf("peer has closed the TCP connection prior to send().\n");
    //  }
    //  else if (messageLen > bytesSent)
    //  {
	//     printf("sent only %d bytes from the required %d.\n", messageLen, bytesSent);
    //  }
    //  else 
    //  {
	//     printf("message was successfully sent .\n");
    //  }

// TODO: All open clientSocket descriptors should be kept
    // in some container and closed as well.
    // sleep(3000);
    //    char buffer[SIZE];
    //     while (1) {
    //        n = read(sock, buffer, SIZE, 0);
    //        if(n>=0)
    //         break; 
    //      //fprintf(fp, "%s", buffer);
    //      bzero(buffer, SIZE);
    //     }
         //fprintf(fp, "%s", buffer);  
    
    close(sock);
     return 0;
}
