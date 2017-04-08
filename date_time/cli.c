#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include<string.h>
#include<time.h> 
int main(int argc,char **argv)
{
    int sockfd,n,i;
    time_t t;
    time(&t);
    char sendline[100];
    char recvline[100];
    struct sockaddr_in servaddr;
 
    sockfd=socket(AF_INET,SOCK_STREAM,0);
    bzero(&servaddr,sizeof servaddr);
 
    servaddr.sin_family=AF_INET;
    servaddr.sin_port=htons(22000);
 
    inet_pton(AF_INET,"localhost",&(servaddr.sin_addr));

    connect(sockfd,(struct sockaddr *)&servaddr,sizeof(servaddr));
 
    //while(1)
    {
        bzero( sendline, 100);
        bzero( recvline, 100);
		//ctime_r(&t,sendline);
		scanf("%s",&sendline);
	 
        write(sockfd,(void*)sendline,100);
        //read(sockfd,recvline,100);
        printf("%s",recvline);
    }
 
}
