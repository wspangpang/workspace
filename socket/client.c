#include<stdio.h>
#include<unistd.h> 
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include<sys/wait.h>
void main()
{
	int server_fd;
	struct sockaddr_in server_addr;
	server_fd=socket(AF_INET,SOCK_STREAM,0);
	printf("%d%s",server_fd,"\n");
	
	server_addr.sin_family=AF_INET;
	server_addr.sin_port=htons(10000);
	inet_aton("172.26.190.202",&server_addr.sin_addr);
	int error = connect(server_fd,(struct sockaddr *)&server_addr,sizeof(struct sockaddr));
	printf("%d",error);	
	
	send(server_fd,"你好！",256,0);
	close(server_fd);
}
