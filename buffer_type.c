//
//  buffer_type.c
//  HWtest
//
//  Created by Mattia Iodice on 22/11/16.
//  Copyright © 2016 Mattia Iodice. All rights reserved.
//
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include "buffer_type.h"


buffer_t* buffer_init(unsigned int maxsize){
    buffer_t *buffer = (buffer_t*)malloc(sizeof(buffer_t));
    
    buffer->D=0;
    buffer->T=0;
    buffer->empty=maxsize; //usare soltanto k limita il parallelismo
    buffer->full=0;
    buffer->size=maxsize;
    buffer->cells=(msg_t*)calloc(maxsize,sizeof(msg_t));

    
    buffer->buffer_init = buffer_init;
    buffer->buffer_destroy=buffer_destroy;
    
    buffer->T_mutex = (pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER;  // necessari?
    buffer->D_mutex = (pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER;  // necessari?
    buffer->Full_mutex = (pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER;     // mutex che gestisce corse critiche su var.cond.
    buffer->Empty_mutex = (pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER;    // mutex che gestisce corse critiche su var.cond.
 
    buffer->notEmpty= (pthread_cond_t)PTHREAD_COND_INITIALIZER;           //var cond che serve a sincroizzarsi
    buffer->notFull= (pthread_cond_t)PTHREAD_COND_INITIALIZER;

    return buffer;
}



msg_t* put_bloccante(buffer_t* buffer, msg_t* msg){
    
    int pid;
    printf("%d:sto per inserire",getpid());
    pthread_mutex_lock(&buffer->Empty_mutex);     //voglio accedere alla variabile condizione senza interferenze
    while(buffer->empty == 0){ //una variabile per verigi
        // gli passo anche il riferimento al mutex per far entrare in regione critica anche altri f.d.e
        pthread_cond_wait(&buffer->notFull, &buffer->Empty_mutex);
    }

    
    //è necessaria la mutua esclusione su D? alla fine soltanto uno può essere qui dentro
    // es. faccio un mutex su empty e vedo che ci sono 8 caselle libere, nessun altro può entrare qui dentro!
    
    int d_pos = buffer->D;

    msg_t* new_msg=msg;
    buffer->cells[d_pos]=*new_msg;
    buffer->D=(d_pos+1)%((buffer->size));
    buffer->empty=buffer->empty-1;
    buffer->full=buffer->full+1;
    
    pthread_mutex_unlock(&buffer->Empty_mutex);

    
    pthread_mutex_lock(&buffer->Full_mutex);     //voglio accedere alla variabile condizione senza interferenze
    pthread_cond_signal(&buffer->notEmpty);
    pthread_mutex_unlock(&buffer->Full_mutex);

    printf("%d: inserito",getpid());
    return new_msg;
}

msg_t* get_bloccante(buffer_t* buffer){
    
 //   fprintf (stderr, "child thread pid is %d\n", (int) getpid ());
    printf("%d: sto per prelevare",getpid());

    pthread_mutex_lock(&buffer->Full_mutex);
    while(buffer->full == 0){
        pthread_cond_wait(&buffer->notEmpty, &buffer->Full_mutex);
    }
    
    msg_t* msg=NULL;
    int t_pos=buffer->T;
    msg=&buffer->cells[t_pos];
    buffer->T = (t_pos+1)%((buffer->size));
    buffer->empty=buffer->empty+1;
    buffer->full=buffer->full-1;
    pthread_mutex_unlock(&buffer->Empty_mutex);

    pthread_mutex_lock(&buffer->Empty_mutex);     //voglio accedere alla variabile condizione senza interferenze
    pthread_cond_signal(&buffer->notFull);
    pthread_mutex_unlock(&buffer->Empty_mutex);
    
    printf("preso ");
    return msg;

}

msg_t* do_put_bloccante(void* arguments){

    struct arg_struct *args=arguments;
    return put_bloccante(args->buffer, args->msg);
}

msg_t* do_get_bloccante(void* arguments){
    buffer_t *buffer=arguments;
    return get_bloccante(buffer);
}


void buffer_destroy(buffer_t* buffer){
//    int size=buffer->size;
//    int i;
 //   for (i=0; i<size; i++)
 //       buffer->cells[i].msg_destroy;
 //   free(buffer);
    buffer=NULL;
}


