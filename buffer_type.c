//
//  buffer_type.c
//  HWtest
//
//  Created by Mattia Iodice on 22/11/16.
//  Copyright © 2016 Mattia Iodice. All rights reserved.
//

#include "buffer_type.h"
#include "myUtility.h"

buffer_t* buffer_init(unsigned int maxsize){
    buffer_t *buffer = (buffer_t*)malloc(sizeof(buffer_t));
    
    buffer->D=0;
    buffer->T=0;
    buffer->K=0;
    buffer->size=maxsize;
    
    buffer->cells=(msg_t*)calloc(maxsize,sizeof(msg_t));
    
    buffer->buffer_init = buffer_init;
    buffer->buffer_destroy=buffer_destroy;
    
    pthread_mutex_init(&(buffer->bufferMutex), NULL); // mutex che gestisce corse critiche su var.cond.
    pthread_cond_init(&(buffer->notFull), NULL);		/* Initialize consumer condition variable */
    pthread_cond_init(&(buffer->notEmpty), NULL);		/* Initialize consumer condition variable */
    
    return buffer;
}

int isEmpty(buffer_t* buffer){
    if (buffer->K==0)
        return 1;
    else
        return 0;
}

int isFull(buffer_t* buffer){
    if (buffer->K==buffer->size)
        return 1;
    else
        return 0;
}

void buffer_destroy(buffer_t* buffer){
    pthread_mutex_destroy(&buffer->bufferMutex);	/* Free up the_mutex */
    pthread_cond_destroy(&buffer->notFull);		    /* Free up consumer condition variable */
    pthread_cond_destroy(&buffer->notEmpty);		/* Free up producer condition variable */
    free(buffer->cells);
    free(buffer);
}

msg_t* put_non_bloccante(buffer_t* buffer, msg_t* msg){
    msg_t* new_msg=msg;

    pthread_mutex_lock(&buffer->bufferMutex);
    if (isFull(buffer))
        new_msg=BUFFER_ERROR;
    
    else{
        int d_pos = buffer->D;
        
        buffer->cells[d_pos]=*new_msg;
        buffer->D=(d_pos+1)%((buffer->size));
        buffer->K++;
    }
    
    pthread_cond_signal(&buffer->notEmpty);
    pthread_mutex_unlock(&buffer->bufferMutex);
    
    return  new_msg; 
}

msg_t* get_non_bloccante(buffer_t* buffer){
    msg_t* msg=NULL;
    
    pthread_mutex_lock(&buffer->bufferMutex);
    if(isEmpty(buffer)){
        msg=BUFFER_ERROR;}
    
    else{
        int t_pos=buffer->T;
        msg=&buffer->cells[t_pos];
        buffer->T = (t_pos+1)%((buffer->size));
        buffer->K--;
        
    }
    
    pthread_cond_signal(&buffer->notFull);
    pthread_mutex_unlock(&buffer->bufferMutex);
    
    return msg;
}

msg_t* put_bloccante(buffer_t* buffer, msg_t* msg){
    
    pthread_mutex_lock(&buffer->bufferMutex);
    while(isFull(buffer))
        pthread_cond_wait(&buffer->notFull, &buffer->bufferMutex);
    
    int d_pos = buffer->D;
    
    msg_t* new_msg=msg;
    buffer->cells[d_pos]=*new_msg;
    buffer->D=(d_pos+1)%((buffer->size));
    buffer->K++;
    
    pthread_cond_signal(&buffer->notEmpty);
    pthread_mutex_unlock(&buffer->bufferMutex);
    
    return new_msg;
}

msg_t* get_bloccante(buffer_t* buffer){
    msg_t* msg=NULL;
    
    pthread_mutex_lock(&buffer->bufferMutex);
    while(isEmpty(buffer))
        pthread_cond_wait(&buffer->notEmpty, &buffer->bufferMutex);
    
    int t_pos=buffer->T;
    msg=&buffer->cells[t_pos];
    buffer->T = (t_pos+1)%((buffer->size));
    buffer->K--;
    
    pthread_cond_signal(&buffer->notFull);
    pthread_mutex_unlock(&buffer->bufferMutex);

    return msg;
}

void do_put_bloccante(void* arguments){
    struct arg_struct *args=arguments;
    msg_t* msg=put_bloccante(args->buffer, args->msg);
    pthread_exit(msg);
}

void do_put_non_bloccante(void* arguments){
    struct arg_struct *args=arguments;
    msg_t* msg=put_non_bloccante(args->buffer, args->msg);
    pthread_exit(msg);}

void do_get_bloccante(void* arguments){
    buffer_t* buffer=arguments;
    msg_t* msg=get_bloccante(buffer);
    pthread_exit(msg);
}

void do_get_non_bloccante(void* arguments){
    buffer_t* buffer=arguments;
    msg_t* msg=get_non_bloccante(buffer);
    pthread_exit(msg);
}



