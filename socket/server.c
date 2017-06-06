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
	printf("%s", "Hello\n");
	int server_fd, client_fd;
	struct sockaddr_in server_addr, client_addr;
	server_fd = socket(AF_INET,SOCK_STREAM,0);
	printf("%d%s", server_fd, "\n");
	
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(10000);
	server_addr.sin_addr.s_addr = INADDR_ANY;
	bzero(&(server_addr.sin_zero), 8);
	bind(server_fd, (struct sockaddr *)&server_addr, sizeof(struct sockaddr));	
	listen(server_fd, 10);
	
	printf("%s", "done\n");
	

	client_fd = accept(server_fd, NULL, NULL);
	char s[256];
	memset(s, 0, 256);
	recv(client_fd, s, 256, 0);
	printf("%s", s);
	close(client_fd);
	close(server_fd);
}