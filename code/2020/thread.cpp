#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/syscall.h>
#include<stdio.h>
#include<pthread.h>
#include <unistd.h>
#include<iostream>
#include <signal.h>
#include <string.h>
#include <errno.h>
using namespace std;

#define gettid()    syscall(__NR_gettid)

#define log_info(fmt, args...)  do {                                    \
	printf("[%ld] %ld info : " fmt, gettid(), time(NULL), ## args); \
} while(0)
#define log_error(fmt, args...) do {                                    \
	printf("[%ld] %ld error: " fmt, gettid(), time(NULL), ## args); \
} while(0)


#define LOGI(fmt, args...)  do {                                    \
	printf("[%ld] %ld info : " fmt, gettid(), time(NULL), ## args); printf("\n"")\
} while(0)
	


class SigalQuitTest {

public:
//    SigalQuitTest();
//    ~SigalQuitTest();

    static void* pthread_handle(void* obj);
    void chreat_pthread();

private:
     pthread_t pid1;
    pthread_t pid2;
//    sigaction actions;
};


void handle_quit(int signo)
{
     LOGI("SigalQuitTest  [handle_quit signal:%d]", signo);
//    pthread_exit(NULL);

}

//SigalQuitTest::SigalQuitTest() {
//
//}
//
//SigalQuitTest::~SigalQuitTest() {
//
//}

void* SigalQuitTest::pthread_handle(void* obj) {

//    signal(SIGQUIT,handle_quit );
//    memset(&actions, 0, sizeof(actions));
    struct sigaction actions;
//        sigemptyset(&actions.sa_mask);
    actions.sa_flags = 0;
    actions.sa_handler = handle_quit;
    sigaction(SIGUSR1,&actions,NULL);

    LOGI("SigalQuitTest  [pthread_handle] [in pthread test] %lu", gettid());
    sleep(25);

}

void SigalQuitTest::chreat_pthread() {
    LOGI("SigalQuitTest  [chreat_pthread] [begin] pid=%d", pid1);
    LOGI("SigalQuitTest  [chreat_pthread] [begin] pid=%d", pid2);

    struct sigaction actions;
    actions.sa_flags = 0;
    actions.sa_handler = handle_quit;
    sigaction(SIGUSR1,&actions,NULL);


    pthread_create(&pid1, NULL , &pthread_handle, NULL);
    pthread_create(&pid2, NULL , &pthread_handle, NULL);
    sleep(3);

    LOGI("SigalQuitTest  [chreat_pthread] [thread %lu exists!]", pid1);
    LOGI("SigalQuitTest  [chreat_pthread] [thread %lu exists!]", pid2);
        int sturt = pthread_kill(pid1, SIGUSR1);
    if(sturt == ESRCH)
    {
        LOGI("SigalQuitTest  [pthread_kill] [ESRCH]");
    }else if(sturt == EINVAL)
    {
        LOGI("SigalQuitTest  [pthread_kill] [EINVAL]");
    }
    LOGI("SigalQuitTest  [pthread_kill] [sturt %d!]", sturt);
    pthread_kill(pid2, SIGUSR1);
        LOGI("SigalQuitTest  [chreat_pthread] [after kill]");


    sleep(30);
    LOGI("SigalQuitTest  [chreat_pthread] [exit in chreat_pthread]");
}

//g++ thread.cpp -pthread
int main(void)
{    
        
        SigalQuitTest * test =new SigalQuitTest();
		test->chreat_pthread();
        return 0;
}
