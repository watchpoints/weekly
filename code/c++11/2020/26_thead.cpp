#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
using namespace std;

void* PrintHello(void* data){
  pthread_t tid = (pthread_t)data;
  int rc;

  rc = pthread_join(tid, NULL);
  if(rc){
    exit(1);
  } else{
    printf("Hello from new thread %d - got %d\n", pthread_self(), data);
    sleep(180);
    pthread_exit(NULL);
  }
}
//g++  26_thead.cpp  -pthread
int main(int argc, char* argv[]){
  int rc;
  pthread_t thread_id;
  pthread_t  tid;

  tid = pthread_self();
  printf("\nmain thread(%d) ", tid); 

  rc = pthread_create(&thread_id, NULL, PrintHello, (void*)tid);
  if(rc){
    printf("\n ERROR: return code from pthread_create is %d \n", rc);
    exit(1);
  }
  sleep(1);
  printf("\n Created new thread (%d) ... \n", thread_id);
  pthread_exit(NULL);
}

/**
ps auwx |  grep a.out
[a.out] <defunct>
ls /proc/22487/task/
cat /proc/22487/maps 
cat /proc/11765/status
cat /proc/22488/maps 
 ls /proc/11765/task/
**/