#include <stdio.h>
#include <time.h>
#include <strings.h>
#include <ctype.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/socket.h>
#include <arpa/inet.h>




#define MAX 1024
#define NUM_THREADS 8


 

// Socket to hold customized data
typedef struct sock_thread{
	int cfd;
	struct sockaddr_in clie_addr;
	int num;
}sock_thread;
 
//error function
void print_error(const char *msg)
{
	perror(msg);
	exit(1);
}


//Merger function
void file_merge(char *source){


   FILE *from, *to;
   char *filename = "Merged.png";
   char c;


   from = fopen(source, "r");
    if (from == NULL)
    {
        printf("Cannot open file %s \n", source);
        exit(0);
    }

    to = fopen(filename, "a+");
    if (to == NULL)
    {
        printf("Cannot open file %s \n", filename);
        exit(0);
    }


    c = fgetc(from);
    while (c != EOF)
    {
        fputc(c, to);
        c = fgetc(from);
    }

    fclose(from);
    fclose(to);

}


// Thread that gets message from client 
 
void *server_thread(void *arg)
{

   

	sock_thread *s = (sock_thread *)arg;
	char ipbuf[BUFSIZ] = {};
	inet_ntop(AF_INET,&s->clie_addr.sin_addr.s_addr,ipbuf,sizeof(ipbuf));
	int n,i,port;
	port = ntohs(s->clie_addr.sin_port);
  s->num = s->num ;

	printf("new client join ip:%s port:%d thread_number:%d.\n",ipbuf,port,s->num );



// code is here



  FILE *fp;
  char *filename;
  filename = (char*) malloc(25*sizeof(char));
  sprintf(filename, "file_part_%03d.png", s->num);
  fp = fopen(filename, "w");

  char buffer[MAX];
  int x;

  while (1) {
    
    x = read(s->cfd,buffer,sizeof(buffer));

    if (x <= 0){
      break;
      
    }

    printf("Data Recieved = %s\n\n", buffer);
    fprintf(fp,"%s",buffer);
    bzero(buffer, MAX);
    
  }

  fclose(fp);

  


	close(s->cfd);
	free(s);
	return NULL;
}
 


//main function
int main(int argc,char **argv)
{	

	

if(argc != 1){


		printf("usage : %s \n",argv[0]);
		return -1;

}

	
  // Server side socket 

	int sfd = socket(AF_INET,SOCK_STREAM,0);

	if (sfd == -1)
	{
		print_error("socket");
	}

	struct sockaddr_in serv_addr,clie_addr;
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(8888);
	inet_pton(AF_INET,"127.0.0.1",&serv_addr.sin_addr.s_addr);
	int result = bind(sfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr));

	if (result == -1)
	{
		print_error("bind");
	}
 
	result = listen(sfd,128);

	if (result == -1)
	{
		print_error("listen");
	}




  

  // creation of sockets using for loop
	
	for(int i = 1; i <= NUM_THREADS; i = i + 1)
	{
		socklen_t clie_addr_len = sizeof(clie_addr);
		int cfd = accept(sfd,(struct sockaddr *)&clie_addr,&clie_addr_len);
		if (cfd == -1)
		{
			print_error("accept");
		}
		
		sock_thread *clie_sock = (sock_thread *)malloc(sizeof(sock_thread));
		clie_sock->cfd = cfd;
		clie_sock->clie_addr = clie_addr;
		clie_sock->num = i;
    
		pthread_t id;
		pthread_create(&id,NULL,(void *)server_thread,(void *)clie_sock);
		pthread_detach(id);
		
	} 

	


  // Merge Files

  char *file = (char*) malloc(25*sizeof(char));

  for (int d = 1; d <  NUM_THREADS + 1 ; d++) {
      sprintf(file, "file_part_%03d.png", d);
      file_merge(file);
}

 
printf("Files Merged");
return 0;
}
