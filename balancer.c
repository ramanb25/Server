
#include <stdio.h>
#include <errno.h>
#include <sys/socket.h>
#include <resolv.h>
#include <arpa/inet.h>
#include <errno.h>


#define MY_PORT		9999
#define MAXBUF		1024


int main(){
	int n;
	//cout<<"Number of Servers"<<endl;
	scanf("%d",&n);
	printf("%d",n);
	int ports[n];
	
//	cout<<"Enter Ports"<<endl;

int i=0;
	for(;i<n;i++)
		scanf("%d",&ports[i]);

	char* reply="HTTP/1.1 301 Moved Permanently\nServer: Apache/2.2.3\nLocation:localhost:9999\nContent-Length: 1000\nConnection:close\nContent-Type:text/html; charset=UTF-8";


//	cout<<"My Port"<<endl;
	 int sockfd;
	struct sockaddr_in self;
	char buffer[MAXBUF];

	/*---Create streaming socket---*/
    if ( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0 )
	{
		perror("Socket");
		exit(errno);
	}
	int my_port;
	scanf("%d",&my_port);
	/*---Initialize address/port structure---*/
	bzero(&self, sizeof(self));
	self.sin_family = AF_INET;
	self.sin_port = htons(my_port);
	self.sin_addr.s_addr = INADDR_ANY;

	/*---Assign a port number to the socket---*/
    if ( bind(sockfd, (struct sockaddr*)&self, sizeof(self)) != 0 )
	{
		perror("socket--bind");
		exit(errno);
	}

	/*---Make it a "listening socket"---*/
	if ( listen(sockfd, 20) != 0 )
	{
		perror("socket--listen");
		exit(errno);
	}

	/*---Forever... ---*/
	while (1)
	{	int clientfd;
		struct sockaddr_in client_addr;
		int addrlen=sizeof(client_addr);

		/*---accept a connection (creating a data pipe)---*/
		clientfd = accept(sockfd, (struct sockaddr*)&client_addr, &addrlen);
		printf("%s:%d connected\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
		FILE *fp;
		   char* buff,buf;

	
		
		/*---Echo back anything sent---*/
		recv(clientfd, buffer, MAXBUF, 0);
		printf("%s\n",reply);
		send(clientfd, reply, strlen(reply), 0);

		/*---Close data connection---*/
		close(clientfd);
	}

	/*---Clean up (should never get here!)---*/
	close(sockfd);
	


}
