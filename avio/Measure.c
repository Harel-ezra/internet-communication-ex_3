/*
    TCP/IP-server
*/

#include<stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <time.h>
#include <netinet/tcp.h> 
#include <arpa/inet.h>

#define SERVER_PORT 5060  //The port that the server listens
#define SIZE 1024
  
  int i;
int main()
{
    char buf[256];
    socklen_t len;
    int msg_counter=0;
    double time =0;
    int message_check=0;
    double total_time=0;
    int clock_Flag=0;
    struct timeval begin, end;
    signal(SIGPIPE, SIG_IGN); // on linux to prevent crash on closing socket

    // Open the listening (server) socket
    int listeningSocket = -1;  
	 
    if((listeningSocket = socket(AF_INET , SOCK_STREAM , 0 )) == -1)
    {
        printf("Could not create listening socket : %d" ,errno);
    }
      printf("Socket created succesfuly \n");

	// Reuse the address if the server socket on was closed
	// and remains for 45 seconds in TIME-WAIT state till the final removal.
	//
    int enableReuse = 1;
    if (setsockopt(listeningSocket, SOL_SOCKET, SO_REUSEADDR, &enableReuse,sizeof(int)) < 0)
    {
         printf("setsockopt() failed with error code : %d" , errno);
    }

    // "sockaddr_in" is the "derived" from sockaddr structure
    // used for IPv4 communication. For IPv6, use sockaddr_in6
    //
    struct sockaddr_in serverAddress;
    memset(&serverAddress, 0, sizeof(serverAddress));

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(SERVER_PORT);  //network order
      
    // Bind the socket to the port with any IP at this port
    if (bind(listeningSocket, (struct sockaddr *)&serverAddress , sizeof(serverAddress)) == -1)
    {
        printf("Bind failed with error code : %d" ,errno);

	// TODO: close the socket
        return -1;
    }
      
    printf("Bind() success\n");
  
    // Make the socket listening; actually mother of all client sockets.
    if (listen(listeningSocket, 500) == -1) //500 is a Maximum size of queue connection requests
											//number of concurrent connections 
    {
	printf("listen() failed with error code : %d",errno);

	// TODO: close the socket
        return -1;
    }
      
    //Accept and incoming connection
    printf("Waiting for incoming TCP-connections...\n");
      
    struct sockaddr_in clientAddress;  
    socklen_t clientAddressLen = sizeof(clientAddress);

    while (1)
    {
    	memset(&clientAddress, 0, sizeof(clientAddress));
        clientAddressLen = sizeof(clientAddress);
        int clientSocket = accept(listeningSocket, (struct sockaddr *)&clientAddress, &clientAddressLen);
    	if (clientSocket == -1)
    	{
           printf("listen failed with error code : %d",errno);

	   // TODO: close the sockets
           return -1;
    	}
    	printf("A new client connection accepted\n");
        len = sizeof(buf); 
        if (getsockopt(listeningSocket, IPPROTO_TCP, TCP_CONGESTION, buf, &len) != 0) { 
        perror("getsockopt");
        return -1;
        } 

        printf("Current: %s\n", buf); 


  
//   for(int j=0;j<5;j++)
//   {
        // FILE *fp;
        char buffer[SIZE];
        int n;
        //fp = fopen(filename, "w");
        
        while (1)
        {
         if(clock_Flag==0)
         {
            // start=clock();
            gettimeofday(&begin, 0);
            clock_Flag=-1;
         }
        //    printf("%d\n",i);
        //    i++;
           n = recv(clientSocket, buffer, SIZE, 0);
        //    printf("size:%d\n",n);
          message_check=message_check+n;
          if(message_check>=1024*1024)
          {
              msg_counter++;
            //   end=clock();
              clock_Flag=0;
            //   time=(double)(end-start)/CLOCKS_PER_SEC;
              gettimeofday(&end, 0);
              long seconds = end.tv_sec - begin.tv_sec;
              long microseconds = end.tv_usec - begin.tv_usec;
              time = seconds + microseconds*1e-6;
                printf("Server got the message time taken in seconds is : %lf\n",time);
              total_time=total_time+time;
              message_check=message_check-1024*1024;
          }

          
         if (msg_counter==5)
          {
             break;
             return; 
             clock_Flag=0;
         }   
         //fprintf(fp, "%s", buffer);
         bzero(buffer, SIZE);
        }
         printf("avg time in seconds  is %lf\n",total_time/5.0);
         total_time=0;
         strcpy(buf, "reno"); 
        len = strlen(buf);
        if (setsockopt(listeningSocket, IPPROTO_TCP, TCP_CONGESTION, buf, len) != 0) 
        {
        perror("setsockopt"); 
        return -1;
        }
        len = sizeof(buf); 
        if (getsockopt(listeningSocket, IPPROTO_TCP, TCP_CONGESTION, buf, &len) != 0) {
        perror("getsockopt"); 
        return -1; 
        } 
        printf("New: %s\n", buf); 
        while (1)
        {
            if(clock_Flag==0)
         {
            // start=clock();
            clock_Flag=-1;
             gettimeofday(&begin, 0);
         }
        //    printf("%d\n",i);
        //    i++;
           n = recv(clientSocket, buffer, SIZE, 0);
        //    printf("size:%d\n",n);
          message_check=message_check+n;
          if(message_check>=1024*1024)
          {
              msg_counter++;
            //   end=clock();
              clock_Flag=0;
            //   time=(double)(end-start)/CLOCKS_PER_SEC;
              gettimeofday(&end, 0);
              long seconds = end.tv_sec - begin.tv_sec;
              long microseconds = end.tv_usec - begin.tv_usec;
              time = seconds + microseconds*1e-6;
                printf("Server got the message time taken in seconds is : %lf\n",time);
              total_time=total_time+time;
             
              message_check=message_check-1024*1024;
          }

         bzero(buffer, SIZE);
          if (n<=0)
          {
             break;
             return; 
          }   
        }
         printf("avg time in seconds  is %lf\n",total_time/5.0);
        break;
    }
        
        

        close(listeningSocket);
        return 0;
    }

  
    // TODO: All open clientSocket descriptors should be kept
    // in some container and closed as well.

    //return 0;

