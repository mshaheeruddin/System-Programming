#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

//Global variable
pthread_mutex_t mtx;
int G = 1000;


void* credit(void* ptr) {
   int i =0;
   pthread_mutex_lock(&mtx);
   for (i = 0; i<1000000; i++) 
   //<Critical Section>
   G++; 
   pthread_mutex_unlock(&mtx);
}

void* debit(void* ptr) {

pthread_mutex_lock(&mtx);
int i;
for (i = 0; i<1000000; i++) 
    G--;
pthread_mutex_unlock(&mtx);
pthread_exit(0);

}

int main(int argc,char** argv) {
  pthread_t t0, t1;
  pthread_mutex_init(&mtx,0);
   pthread_create(&t0,0,debit,(void*)0);
   pthread_create(&t1,0,credit,(void*)1);
   pthread_join(t0,0);
   pthread_join(t1,0);
   printf("%d\n",G);
  pthread_mutex_destroy(&mtx);

return 0;


}
