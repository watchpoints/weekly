//
// Created by liyunlong4 on 2020/7/22.
//

#include "SigalQuitTest.h"
#include "../Util.h"

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>

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
//        sigemptyset(&actions.sa_mask);
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

