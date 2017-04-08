#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include<string.h>
 
int main()
{
 
    char str[100];
    int listen_fd, comm_fd;
 
    struct sockaddr_in servaddr,cliaddr;
 
    listen_fd = socket(AF_INET, SOCK_STREAM, 0);
 
    bzero( &servaddr, sizeof(servaddr));
    bzero(&cliaddr, sizeof(cliaddr));
 
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htons(INADDR_ANY);
    servaddr.sin_port = htons(22000);
 
    bind(listen_fd, (struct sockaddr *) &servaddr, sizeof(servaddr));
 
    listen(listen_fd, 10);

	int len=sizeof(cliaddr);
    comm_fd = accept(listen_fd, (struct sockaddr*) &cliaddr, &len);
 
    //while(1)
    {
 
        bzero( str, 100);
 
        read(comm_fd,str,100);
 
        printf("Current Date and time: - %s",str);
 
        //write(comm_fd, str, strlen(str)+1);
 
    }
}
