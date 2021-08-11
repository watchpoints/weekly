//
// Created by liyunlong4 on 2020/7/22.
//

#ifndef POKEU_SIGALQUITTEST_H
#define POKEU_SIGALQUITTEST_H

#include <pthread.h>
#include <stdio.h>
#include <signal.h>



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


#endif //POKEU_SIGALQUITTEST_H
