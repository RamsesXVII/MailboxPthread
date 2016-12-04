#ifndef utility_h
#define utility_h

#include "buffer_type.h"


//passaggio di parametri a pthread_create
struct arg_struct{
    buffer_t *buffer;
    msg_t* msg;
};

//passaggio di parametri a get/put_blocking_Ntimes
struct Ntimes{
    buffer_t *buffer;
    msg_t* msg;
    int i;
};

//passaggio di parametri a get/put_bloccante_TestSupport
struct arg_structTest{
    buffer_t *buffer;
    msg_t* msg;
    pthread_cond_t*sleepingMutex;
    pthread_cond_t* isAwake;
    pthread_cond_t* isSleeping;
    int* sleepState;
};


void get_blocking_Ntimes(void* arguments);
void put_blocking_Ntimes(void* arguments);

void get_notBlocking_Ntimes(void* arguments);
void put_notBlocking_Ntimes(void* arguments);


msg_t* put_bloccante_TestSupport(buffer_t* buffer, msg_t* msg,
                                 pthread_cond_t*sleepingMutex, pthread_cond_t* isAwake, pthread_cond_t* isSleeping, int* sleepState);
void do_put_bloccante_TestSupport(void* arguments);

void iamgoingtosleep(pthread_mutex_t* sleepingMutex,pthread_cond_t* isSleeping,int* sleepState);
void iamAwake(pthread_mutex_t* sleepingMutex,pthread_cond_t* isSleeping,int* sleepState);






#endif /* utility_h */
