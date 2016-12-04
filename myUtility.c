#include "myUtility.h"

void get_blocking_Ntimes(void* arguments){
    int i;
    int k;
    msg_t* msg=NULL;
    struct Ntimes *args=arguments;
    
    for (i=0; i<args->i; i++) {
        k=rand() % 2;
        msg=get_bloccante(args->buffer);
        if (k==0)
            sleep(2);
    }
    pthread_exit(msg);
}

void put_blocking_Ntimes(void* arguments){
    int i;
    msg_t* msg=NULL;
    int k;
    struct Ntimes *args=arguments;
    
    for (i=0; i<args->i; i++) {
        k=rand() % 2;
        msg=put_bloccante(args->buffer,args->msg);
        if (k==0)
            sleep(2);
    }
    pthread_exit(msg);
}

void get_notBlocking_Ntimes(void* arguments){
    int i;
    int k;
    msg_t* msg=NULL;
    struct Ntimes *args=arguments;
    
    for (i=0; i<args->i; i++) {
        k=rand() % 2;
        msg=get_non_bloccante(args->buffer);
        if (k==0)
            sleep(2);
    }
    pthread_exit(msg);

    
}
void put_notBlocking_Ntimes(void* arguments){
    int i;
    msg_t* msg=NULL;
    int k;
    struct Ntimes *args=arguments;
    
    for (i=0; i<args->i; i++) {
        k=rand() % 2;
        msg=put_non_bloccante(args->buffer,args->msg);
        if (k==0)
            sleep(2);
    }
    pthread_exit(msg);

}


msg_t* put_bloccante_TestSupport(buffer_t* buffer, msg_t* msg,
                                 pthread_cond_t*sleepingMutex, pthread_cond_t* isAwake, pthread_cond_t* isSleeping, int* sleepState){
    
    pthread_mutex_lock(&buffer->bufferMutex);
    while(isFull(buffer)){
        iamgoingtosleep(sleepingMutex,isSleeping,sleepState);
        pthread_cond_wait(&buffer->notFull, &buffer->bufferMutex);
    }
    iamAwake(sleepingMutex,isAwake,sleepState);
    int d_pos = buffer->D;
    
    msg_t* new_msg=msg;
    buffer->cells[d_pos]=*new_msg;
    buffer->D=(d_pos+1)%((buffer->size));
    buffer->K++;
    
    pthread_cond_signal(&buffer->notEmpty);
    pthread_mutex_unlock(&buffer->bufferMutex);
    
    return new_msg;
}


void do_put_bloccante_TestSupport(void* arguments){
    struct arg_structTest *args=arguments;
    msg_t* msg=put_bloccante_TestSupport(args->buffer, args->msg,args->sleepingMutex,args->isAwake,args->isSleeping,args->sleepState);
    pthread_exit(msg);
}

void iamgoingtosleep(pthread_mutex_t* sleepingMutex,pthread_cond_t* isSleeping,int* sleepState){
    pthread_mutex_lock(sleepingMutex);
    *sleepState=1;
    pthread_cond_signal(isSleeping);
    pthread_mutex_unlock(sleepingMutex);
}

void iamAwake(pthread_mutex_t* sleepingMutex,pthread_cond_t* isAwake,int* sleepState){
    pthread_mutex_lock(sleepingMutex);
    *sleepState=0;
    pthread_cond_signal(isAwake);
    pthread_mutex_unlock(sleepingMutex);
}


