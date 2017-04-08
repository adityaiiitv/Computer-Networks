#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include<string.h>

struct dns
{
	char name[20];
	char ip[20];
}
inputs[20];

int main()
{
    char ip_got[100];
    int i;
    for(i=0;i<20;i++)
    {
	if(i==8)
	{
		strcpy(inputs[i].name,"www.google.com");
                strcpy(inputs[i].ip,"981.043.543.800");
	}
	else
	{
		strcpy(inputs[i].name,"www.blah.com");
		strcpy(inputs[i].ip,"0.0.0.0");

	}
    }

    char str[100];
    int listen_fd, comm_fd;

    struct sockaddr_in servaddr;

    listen_fd = socket(AF_INET, SOCK_STREAM, 0);

    bzero( &servaddr, sizeof(servaddr));

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htons(INADDR_ANY);
    servaddr.sin_port = htons(22004);

    bind(listen_fd, (struct sockaddr *) &servaddr, sizeof(servaddr));

    listen(listen_fd, 10);

    comm_fd = accept(listen_fd, (struct sockaddr*) NULL, NULL);

    while(1)
    {

        bzero( str, 100);

        read(comm_fd,str,100);
	int flag=0,i=0;
	while(flag==0 || i<20)
	{
		if(strcmp(inputs[i].name,str)==0)
		{
		        write(comm_fd, inputs[i].ip, strlen(inputs[i].ip)+1);
			flag++;
		}
		i++;
	}

/*        printf("Echoing back - %s",str);
*/

    }
}
