#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <iostream>
#include <exception>
#include <vector>
#include <stdexcept>
#include <stdio.h>
#include <cstdlib>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
using namespace std;

static int exit_flag = 0;
static int exit_flag1 = 0;
static void handle_quit(int sig)
{
    printf("Caught SIGTERM, exiting now\n");
    exit_flag1 = 1;
    // pthread_exit(0);
}

void *pthread_handle(void *obj)
{

    struct sigaction actions;
    actions.sa_flags = 0;
    actions.sa_handler = handle_quit;
    //终止信号	 SIGTERM
    sigaction(SIGTERM, &actions, NULL);

    while (!exit_flag)
    {

        if (exit_flag1 == 1)
        {
            break;
        }
        else
        {
            cout << "sleep 1111..=" << exit_flag << endl;
            sleep(30); //io
            cout << "sleep 22222..=" << exit_flag << endl;
        }
    }
}

//g++ -O3  exceptions_sig1.cpp  -lpthread
int main(int argc, char *argv[])
{

    pthread_t thread_id;
    pthread_create(&thread_id, NULL, &pthread_handle, NULL);
    sleep(3);
    cout << "send sig SIGTERM" << endl;
    pthread_kill(thread_id, SIGTERM); //只会导致在给定线程的上下文中处理信号；信号操作（终止或停止）作
    pthread_join(thread_id, NULL);

    return 0;
}