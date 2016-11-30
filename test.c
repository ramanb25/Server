#include<stdio.h>
#include<string.h>    //strlen
#include<sys/socket.h>
#include<arpa/inet.h> //inet_addr
#include<omp.h>
#include<math.h>
//#include<bits/stdc++.h>
#define par true
#define num_threads 4
#define power 4
int main()
{       int i=0;
  
	int j=0;
	double aa,ab;
  
	aa=clock();
	//SERIAL REQUESTS
        for(i=0;i<pow(2,power);i++){
	    int socket_desc, c, s, r;
	    struct sockaddr_in server;
	    char *message, server_reply[2000];
	     
	    //Create socket
	    socket_desc = socket(AF_INET , SOCK_STREAM , 0);
	    if (socket_desc == -1)
	    {
		printf("Could not create socket");
	    }
		
		//PUT SERVER ADDRESS HERE Currently localhost:9999
	    server.sin_addr.s_addr = inet_addr("127.0.0.1");
	    server.sin_family = AF_INET;
	
	    server.sin_port = htons( 9999 );
	     
	    c=connect(socket_desc , (struct sockaddr *)&server , sizeof(server));
	    
	   message="GET /home.html HTTP/1.1\nHost: localhost:9997\nConnection: keep-alive\nPragma: no-cache\nCache-Control: no-cache\nUpgrade-Insecure-Requests: 1\nUser-Agent: Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/53.0.2785.143 Safari/537.36\nAccept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8\nAccept-Encoding: gzip, deflate, sdch\nAccept-Language: en-US,en;q=0.8";
	    write(socket_desc , message , strlen(message));
	    
	    if (c < 0)
	    {  
		puts("connect error");
		return 1;
	    }
	     
	  
		j++;
	     int i,a;
	   
	      read(socket_desc, server_reply , 2000);
	     
		ab=clock();
		printf("Serial Request=%d %f\n",j,ab-aa);
}
    





double serial=ab-aa;






//PARALLEL REQUESTS START HERE

 for(i=0;i<power;i++){
	fork();

        for(i=0;i<1;i++){
            aa=clock();
          //  printf("dvv\n");
    int socket_desc, c, s, r;
    struct sockaddr_in server;
    char *message, server_reply[2000];
     
    //Create socket
    socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    if (socket_desc == -1)
    {
        printf("Could not create socket");
    }


	//put server's address here currently 3 adrresses = 4*3 cores
        if(getpid()%2==0){ 
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_family = AF_INET;
    
    server.sin_port = htons( 9999 );}
else if(getpid()%3==1) {


    server.sin_addr.s_addr = inet_addr("192.168.43.190");
    server.sin_family = AF_INET;

 server.sin_port = htons( 9998 );}
 else if(getpid()%3==2) {


    server.sin_addr.s_addr = inet_addr("192.168.43.26");
    server.sin_family = AF_INET;

 server.sin_port = htons( 9999 );}



    
    c=connect(socket_desc , (struct sockaddr *)&server , sizeof(server));
    
   message="GET /home.html HTTP/1.1\nHost: localhost:9997\nConnection: keep-alive\nPragma: no-cache\nCache-Control: no-cache\nUpgrade-Insecure-Requests: 1\nUser-Agent: Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/53.0.2785.143 Safari/537.36\nAccept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8\nAccept-Encoding: gzip, deflate, sdch\nAccept-Language: en-US,en;q=0.8";
    write(socket_desc , message , strlen(message));
   
    if (c < 0)
    {  
        puts("connect error");
        return 1;
    }
     
   
    	j++;
    	 int i,a;
      read(socket_desc, server_reply , 2000);
 	ab=clock();
        printf("Parallel Request=%f\n",ab-aa);
        double parallel=ab-aa;


        printf("\nSpeed Up%f\n\n\n",serial/parallel);
        fflush(stdout);
}

    return 0;
}
