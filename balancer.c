
#include <stdio.h>
#include <errno.h>
#include <sys/socket.h>
#include <resolv.h>
#include <arpa/inet.h>
#include <errno.h>
#include<stdlib.h>
#include<string.h>
#define println() printf("\n");
#define MY_PORT		9999
#define MAXBUF		1024

static int dbg=0;
void debug(){
	dbg++;
	printf("Debug:%d\n\n",dbg);
	fflush(stdin);
}

int elastic[1000];
static int rr=0;
int n;
int round_robin(){
		while(1){
			if(elastic[rr%n]==1)
				{printf("rr%d",(rr)%n);return (rr++)%n;}
			rr++;
		}
}

int ishttp(char *url){
	if(url[0]=='h' && url[1]=='t' && url[2]=='t' && url[3]=='p' && url[4]==':' && url[5]=='/' && url[6]=='/')
		return 7;
	if(url[0]=='h' && url[1]=='t' && url[2]=='t' && url[3]=='p' && url[4]=='s' && url[5]==':' && url[6]=='/' && url[7]=='/')
		return 8;
	return 0;
}
int find(char *url){
	int len=strlen(url);
	int i=0;
	for(;i<len;i++)
		if(url[i]=='/')
			return i+1;
	return len;
}

void mParseUrl(char *mUrl, char** serverName, char** filepath, char** filename)
{	
	//debug();
    int len=strlen(mUrl);
    char *url = mUrl;

    int skip=ishttp(url);
//debug();
    len=len-skip;
    while(skip--)
	url++;
//	debug();
    n = find(url);
//debug();
    if (n != len)
    {	debug();
        //*serverName = url.substr(0,n);

	//char subbuff[5];
printf("%d",n);
println();
	printf("%s",url);
	printf("%d",n);
	memcpy( *serverName, url, n );
	debug();
	*serverName[n] = '\0';
	debug();
       // *filepath = url.substr(n);
	//debug();
	strcpy( *filepath, &url[n-1]);
	debug();
        n = find(*filepath);
        //*filename = filepath.substr(n+1);
	strcpy( *filename, &filepath[n]);
	debug();
	
	println();
	printf("%s",*filename);
	println();
    }

    else
    {
        *serverName = url;
        *filepath = "/";
        *filename = "";
    }
}

int Parsefile(char* req, char* file){
	debug();
	printf("req= %s\n\n\n\n",req);
	char* gethost = strstr(req, "/");
	if(gethost!=NULL){
		
		debug();
		printf("gethost=%s\n\n\n\n",gethost);
		int i=0;
		int len=strlen(gethost);
		for(i=0;i<len;i++){
			//printf("char= %c\n\n\n",gethost[i]);
			if(gethost[i]==' ')
				break;
		}
		memcpy( file, gethost, i+1 );
		return 1;	
	}else return -1;

}
int main(){
	//cout<<"Number of Servers"<<endl;
	scanf("%d",&n);
	
	char ports[n][1000];
	
//	cout<<"Enter Ports"<<endl;

	int i;
	for(i=0;i<n;i++)
		elastic[i]=1;

	for(i=0;i<n;i++)
		scanf("%s",ports[i]);

	char *head = "HTTP/1.1 301 Moved Permanently\nServer: Apache/2.2.3\nLocation: http://";
	char *tail="\nContent-Length: 1000\nConnection: close\nContent-Type:  text/html; charset=UTF-8";
	char reply[1000];

	

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

	
		
		/*---Echo back anything sent---*/
		recv(clientfd, buffer, MAXBUF, 0);
		char *filename;
		char filepath[1000];
		char* servername;
		int len=strlen(reply);
		Parsefile(buffer,&filepath);
		debug();
		printf("filepath= %s\n",filepath);
		sprintf(reply,"%s%s%s%s",head,ports[round_robin()],filepath,tail);
		debug();
		printf("reply= %s\n",reply);
		send(clientfd, reply, len, 0);

		/*---Close data connection---*/
		close(clientfd);
	}

	/*---Clean up (should never get here!)---*/
	close(sockfd);
	


}
