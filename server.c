/* simple-server.c
 *
 * Copyright (c) 2000 Sean Walton and Macmillan Publishers.  Use may be in
 * whole or in part in accordance to the General Public License (GPL).
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
*/

/*****************************************************************************/
/*** simple-server.c                                                       ***/
/***                                                                       ***/
/*****************************************************************************/

/**************************************************************************
*	This is a simple echo server.  This demonstrates the steps to set up
*	a streaming server.
**************************************************************************/
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
#define MAXBUF		1024

static int dbg=0;
void debug(){
	dbg++;
	printf("Debug:%d\n\n",dbg);
	fflush(stdin);
}


char buffer[MAXBUF];
int my_port;
char *filename;
char filepath[1000];
char filetype[1000];
size_t length;
char reply[1000];
char *head = "HTTP/1.0 200 OK\r\nServer:Web Server\r\nContent-Type: ";
char *tail="; charset=UTF-8\r\n\n";
char *error="Error";
char *home="/index.html";
int sockfd;
char* buffer1[1000];
struct sockaddr_in self;

void init(){
	memset(filetype, 0, 1000);
	memset(filepath, 0, 1000);
	memset(reply, 0, 1000);
	memset(buffer,0,MAXBUF);
	memset(buffer1,0,MAXBUF);
}

int get_filepath(char* req, char* file){
	debug();
	printf("req= %s\n\n\n\n",req);
	char* gethost = strstr(req, "/");
	if(gethost!=NULL){
		
		//debug();
		//printf("gethost=%s\n\n\n\n",gethost);
		int i=0;
		int len=strlen(gethost);
		for(i=0;i<len;i++){
			//printf("char= %c\n\n\n",gethost[i]);
			if(gethost[i]==' ')
				break;
		}
		if(i!=1)
			memcpy( file, gethost, i );
		else{
			memcpy( file, home, strlen(home) );
		}
		

		//debug();
		//printf("filepath %s\n",filepath);
		return 1;	
	}else{ 	
		return -1;
	}

}

int get_filetype(char* filename, char* filetype){
	if (strstr(filename, ".html"))
	    strcpy(filetype, "text/html");
    	else if (strstr(filename, ".gif"))
    		strcpy(filetype, "image/gif");
   	else if (strstr(filename, ".jpg"))
   	 	strcpy(filetype, "image/jpeg");
   	else
		strcpy(filetype, "text/plain");
	
	//debug();
	//printf("filetype %s\n",filetype);
}

/* Read the contents of FILENAME into a newly-allocated buffer.  The
   size of the buffer is stored in *LENGTH.  Returns the buffer, which
   the caller must free.  If FILENAME doesn't correspond to a regular
   file, returns NULL.  */


char* read_file (char* filename, size_t* length)
{
  int fd;
  struct stat file_info;
  

  /* Open the file.  */
  fd = open (++filename, O_RDONLY);

	//debug();
	//printf("%s\n",filename);

  /* Get information about the file.  */
  fstat (fd, &file_info);
  *length = file_info.st_size;
  /* Make sure the file is an ordinary file.  */

	//debug();
	//printf("%d\n",*length);
 

  /* Allocate a buffer large enough to hold the file's contents.  */
  //buffer1 = (char*) malloc (*length);
	//debug();
  //printf("File=%s\n",buffer1);

  /* Read the file into the buffer.  */
  read (fd, buffer1, *length);

  //debug();
  //printf("File=%s\n",buffer1);

  /* Finish up.  */
  close (fd);
  return buffer1;
}

void send_message(int fd,char *reply){
	//debug();	
	//printf("Reply=%s\n",reply);	
	send(fd, reply,strlen(reply),0);
}



isHTTP(){
	if(strstr(buffer,"HTTP")!=NULL)
		return 1;
	return 0;
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
		FILE *fp;


		

		
		/*---Echo back anything sent---*/
		recv(clientfd, buffer, MAXBUF, 0);


		if(isHTTP()){
		//parse filepath
		get_filepath(buffer,filepath);		

		//parse filetype
		get_filetype(filepath,filetype);
		
		//reply
		char* file_read=read_file(filepath,&length);
		sprintf(reply,"%s%s%s%s",head,filetype,tail,file_read);
		}else{
			sprintf(reply,"%s",error);
		}
		send_message(clientfd, reply);
		init();



		/*---Close data connection---*/
		close(clientfd);
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

