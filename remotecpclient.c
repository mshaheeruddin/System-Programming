#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <fcntl.h> 
#include <unistd.h> 
#include<pthread.h>



#define NUM_THREADS 8


typedef struct args_holder{
        //thread id
	int id;
	char *filename; 
}args_holder;



// Thread Function for Multithreaded Clients
void * client_thread(void *arg)
{


// Initialise my struct and bind to function args
  args_holder *s = (args_holder *)arg; 

  printf("Executing Thread Number #%d\n", s->id);    
 
 
  // Client Socket 
  int clientSocket; 
  struct sockaddr_in serverAddr; 
// Structure to store client ip and correspoding server address
  socklen_t addr_size;

  // Create the socket. 
  clientSocket = socket(PF_INET, SOCK_STREAM, 0);

  //Configure settings of the server address
 // Address family is Internet 
  serverAddr.sin_family = AF_INET;

  //Set port number, using htons function 
  serverAddr.sin_port = htons(8888);

 //Set IP address to localhost
  serverAddr.sin_addr.s_addr = inet_addr("10.2.70.78");
  memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);

    //Connect the socket to the server using the address
    addr_size = sizeof serverAddr;
    connect(clientSocket, (struct sockaddr *) &serverAddr, addr_size);

//real code here


      // Open Source File 
      FILE *fp;
      
      fp = fopen(s->filename, "r");
      // Calculate File Size
      fseek(fp, 0L, SEEK_END);
      int filesize = ftell(fp);
      printf("File Size = %d\n", filesize); 

      //Buffer to Store Contents

      char buffer[filesize]; 


    // Calculate Start and End Readig Positions     
      int start_pos;
      int end_pos;
     
      if (s->id == 1) {

          start_pos = 0;
          end_pos = ((s->id)*(filesize/NUM_THREADS));

     }


     else if(s->id == NUM_THREADS) {

       start_pos = ((s->id - 1) * (filesize/NUM_THREADS));
       end_pos = ((s->id) * ((filesize/NUM_THREADS) + (filesize%NUM_THREADS)));

     }

      else  {

          start_pos = (s->id - 1) * (filesize/NUM_THREADS);
          end_pos = ((s->id)*(filesize/NUM_THREADS)) ;


     }

    printf("Start = %d, End = %d\n",start_pos, end_pos );


    // Seek and Send Data
    fseek(fp,start_pos, SEEK_SET);
    fread(buffer, sizeof(buffer), (end_pos-start_pos), fp);
    printf("Data Sent = %s\n", buffer);

   if( send(clientSocket , buffer , strlen(buffer) , 0) < 0)
    {
            printf("Send failed\n");
    }

    bzero(buffer,sizeof(buffer)); // Clear Buffer
    printf("Data Tranferred \n");
    printf("Closing Thread Number #%d \n\n",s->id); 
    close(clientSocket);
    
    pthread_exit(NULL);
}


// --------- Main Driver Finction ------------ //

int main(int argc,char **argv){


 


  if(argc != 2) {

	printf("usage : %s <filepath> \n ",argv[0]);
	return -1;

}

   printf("Threads are hardcoded to 8 \n");
	

  // For Loop to Create Clients Threads based on NUM_THREADS
  for(int i = 1; i <= NUM_THREADS; i++) {

     // Initilaise p_thread objects
    args_holder *my_args_holder = (args_holder *)malloc(sizeof(args_holder)); 
	  my_args_holder->id = i; 
	  my_args_holder->filename = argv[1];
  
    // create and join thread
    pthread_t thread;
    pthread_create(&thread, NULL, client_thread, (void *)my_args_holder);
    pthread_join(thread, NULL);
	
    
}


	
pthread_exit(0);

return 0;
}
