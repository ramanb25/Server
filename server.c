#include <stdio.h>
#include <errno.h>
#include <sys/socket.h>
#include <resolv.h>
#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define MY_PORT		9999
#define MAXBUF		200000

static int dbg=0;
void debug(){
	dbg++;
	printf("Debug:%d\n\n",dbg);
	fflush(stdin);
}


char buffer[MAXBUF];
int my_port;
char *filename;

size_t length;
static char *http_200 = "HTTP/1.1 200 OK\r\nServer:The Server\r\nContent-Type: ";
char *tail="; charset=UTF-8\r\n\n";
static char *error="Error";
static char *home="/index.html";
static char* error_404 = 
  "HTTP/1.0 404 Not Found\n"
  "Content-type: text/html\n"
  "\n"
  "<html>\n"
  " <body>\n"
  "  <h1>Not Found</h1>\n"
  "  <p>The requested URL was not found on this server.</p>\n"
  " </body>\n"
  "</html>\n";

static char* not_HTTP = 
  "HTTP/1.0 404 Not Found\n"
  "Content-type: text/html\n"
  "\n"
  "<html>\n"
  " <body>\n"
  "  <h1>Only HTTP req</h1>\n"
  "  <p>The requested URL was not found on this server.</p>\n"
  " </body>\n"
  "</html>\n";
int sockfd;
struct sockaddr_in self;


void init(){
	
}

int get_filepath(char* req, char* file){
	//debug();
	//printf("req= %s\n\n\n\n",req);
	//char* gethost = strstr(req, "/");
	if(req!=NULL){
		
		int i=0;
		int len=strlen(req);
		for(i=0;i<len;i++){
			if(req[i]=='?')
				break;
		}
		if(i!=1)
			memcpy( file, req, i );
		else{
			memcpy( file, home, strlen(home) );
		}
		return 1;	
	}else{ 	
		return -1;
	}

}


int get_filetype(char* filename, char* filetype){
	if (strstr(filename, ".html"))
	    strcpy(filetype, "text/html\r\n");
    	else if (strstr(filename, ".gif"))
    		strcpy(filetype, "image/gif");
   	else if (strstr(filename, ".jpg")){
   	 	strcpy(filetype, "image/jpeg\r\n");
		return 1;}
	else if (strstr(filename, ".png")){
   	 	strcpy(filetype, "image/png\r\n");
		return 1;}
   	else
		strcpy(filetype, "");
return 0;
}

/* Read the contents of FILENAME into a newly-allocated buffer.  The
   size of the buffer is stored in *LENGTH.  Returns the buffer, which
   the caller must free.  If FILENAME doesn't correspond to a regular
   file, returns NULL.  */
char* buffer1[MAXBUF] ;
 
int read_file (char* filename, size_t* length,char* file_read)
{
  int fd;
  struct stat file_info;
  

  /* Open the file.  */
  fd = open (++filename, O_RDONLY);

	debug();
	printf("FILENAME=%s\n",filename);
	
  if(fd==-1){
	return -1;	
  }

  /* Get information about the file.  */
  fstat (fd, &file_info);
  *length = file_info.st_size;
  /* Make sure the file is an ordinary file.  */

	debug();
	printf("%d\n",*length);
 

  /* Allocate a buffer large enough to hold the file's contents.  */
 //char* buffer1[MAXBUF] ;
	debug();
  printf("File=%s\n",buffer1);

  /* Read the file into the buffer.  */
  read (fd, buffer1, *length);

  debug();
  printf("File=%s\n\n\n",buffer1);
	strcpy(file_read,buffer1);

  /* Finish up.  */
 // close (fd);
debug();
  return 1;
}

void send_message(int fd,char *reply){	
	send(fd, reply,strlen(reply),0);
}

isHTTP(char* req){
	if(strstr(req,"HTTP")!=NULL)
		return 1;
	return 0;
}

isGET(char* req){
	if(strstr(req,"GET")!=NULL)
		return 1;
	return 0;
}


void handle_connection(int clientfd){

		
		char filepath[1000];
		char filetype[1000];
		char reply[200000];
		char req[1000];
		
		recv(clientfd, req, MAXBUF, 0);
		
		char method[100],protocal[100],url[100];
		sscanf(req,"%s %s %s",method,url,protocal);
		
		printf("req=\n%s\n\n\n",req);
		//printf("DATA=%s %s %s",method,url,protocal);

		if(isHTTP(protocal)){
			debug();
			if(isGET(method)){
			debug();
			printf("egjhugie\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
				if(get_filepath(url,filepath)!=-1){
				debug();
				printf("euoe");	
					
					int type=get_filetype(filepath,filetype);

					char file_read[200000];
				
					if(read_file(filepath,&length,file_read)!=-1){
						printf("file=%s\n",buffer1);
						if(type)
						sprintf(reply,"%s%s%s%d%s%s",http_200,filetype,"Content-Length:",length,"\r\n",buffer1+4);

						else
						sprintf(reply,"%s%s%s%s",http_200,filetype,tail,buffer1);

					}



					else{	printf("file=\n");
						sprintf(reply,"%s",error_404);}

					debug();
					printf("yo%s\n", reply);
				}
				else{
					debug();
				printf("euoes	");
					sprintf(reply,"%s",error_404);}

				printf("euoes	");
			}else{
				sprintf(reply,"%s",error);
			}
			
		}else{
			sprintf(reply,"%s",not_HTTP);
		}
		int j=0;
		printf("euoe\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
		//for(;j<10000000;j++){
		//	int a=2*3;		
		//}
		printf("%s\n\n\n\n",reply);
		send_message(clientfd, reply);
		
		memset(filetype, 0, 1000);
		memset(filepath, 0, 1000);
		memset(reply, 0, 1000);
		memset(req,0,1000);

		/*---Close data connection---*/
		close(clientfd);

}
void server(){

/*---Forever... ---*/
	while (1)
	{	int clientfd;
		struct sockaddr_in client_addr;
		int addrlen=sizeof(client_addr);

		/*---accept a connection (creating a data pipe)---*/
		clientfd = accept(sockfd, (struct sockaddr*)&client_addr, &addrlen);

		printf("%s:%d connected\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
	//	printf("fork()");
		int child_pid = fork ();
		    if (child_pid == 0) {
		      /* This is the child process.  It shouldn't use stdin or stdout,
			 so close them.  */
		      close (STDIN_FILENO);
		      //close (STDOUT_FILENO);
		      /* Also this child process shouldn't do anything with the
			 listening socket.  */
		      close (sockfd);
		      /* Handle a request from the connection.  We have our own copy
			 of the connected socket descriptor.  */
			//printf("Handle");
		      handle_connection (clientfd);
		      /* All done; close the connection socket, and end the child
			 process.  */
		      close (clientfd);
			//printf("EXIT");
		      exit (0);
			//printf("EXIT");
		    }
		    else if (child_pid > 0) {
		      /* This is the parent process.  The child process handles the
			 connection, so we don't need our copy of the connected socket
			 descriptor.  Close it.  Then continue with the loop and
			 accept another connection.  */
		      close (clientfd);
			//exit(0);
		    }	
		
	}

}


int main(int Count, char *Strings[])
{   
	scanf("%d",&my_port);

	/*---Create streaming socket---*/
    if ( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0 )
	{
		perror("Socket");
		exit(errno);
	}
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

	server();

	/*---Clean up (should never get here!)---*/
	close(sockfd);
	return 0;
}

