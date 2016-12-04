#ifndef buffer_type_h
#define buffer_type_h

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include "message_type.h"

#define BUFFER_ERROR (msg_t*) NULL

typedef struct buffer{
    int T;
    int D;
    int K;
    int size;
    msg_t* cells;
    
    pthread_mutex_t bufferMutex;

    pthread_cond_t notEmpty;
    pthread_cond_t notFull;
    
     struct buffer* (*buffer_init)(unsigned int);
     void (*buffer_destroy)(struct buffer*);

}buffer_t;


buffer_t* buffer_init(unsigned int maxsize);
void buffer_destroy(buffer_t* buffer);

int isEmpty(buffer_t* buffer);
int isFull(buffer_t* buffer);


msg_t* put_bloccante(buffer_t* buffer, msg_t* msg);
msg_t* put_non_bloccante(buffer_t* buffer, msg_t* msg);

msg_t* get_bloccante(buffer_t* buffer);
msg_t* get_non_bloccante(buffer_t* buffer);

void do_put_bloccante(void* arguments);
void do_put_non_bloccante(void* arguments);

void do_get_non_bloccante(void* arguments);
void do_get_bloccante(void* arguments);




#endif /* buffer_type_h */

