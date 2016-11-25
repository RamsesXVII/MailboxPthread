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
#define BUFFER_ERROR  (msg_t *) NULL




/*al posto di usare full ed empty usasare soltanto k*/




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
    

    buffer->Full_mutex = (pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER; // mutex che gestisce corse critiche su var.cond.
 
    buffer->notEmpty= (pthread_cond_t)PTHREAD_COND_INITIALIZER;     //var cond che serve a sincroizzarsi
    buffer->notFull= (pthread_cond_t)PTHREAD_COND_INITIALIZER;

    return buffer;
}




msg_t* put_non_bloccante(buffer_t* buffer, msg_t* msg){
    pthread_mutex_lock(&buffer->Full_mutex);     //voglio accedere alla variabile condizione senza interferenze
    if (buffer->empty == 0)
        return BUFFER_ERROR;
    else{
        int d_pos = buffer->D;
        
        msg_t* new_msg=msg;
        buffer->cells[d_pos]=*new_msg;
        buffer->D=(d_pos+1)%((buffer->size));
        buffer->empty--;
        buffer->full++;
        
        pthread_cond_signal(&buffer->notEmpty);
        pthread_mutex_unlock(&buffer->Full_mutex);

        return  new_msg;
        
    }
}

    



msg_t* get_non_bloccante(buffer_t* buffer){
    pthread_mutex_lock(&buffer->Full_mutex);     //voglio accedere alla variabile condizione senza interferenze
    if(buffer->full == 0)
        return BUFFER_ERROR;
    else{
        msg_t* msg=NULL;
        int t_pos=buffer->T;
        msg=&buffer->cells[t_pos];
        buffer->T = (t_pos+1)%((buffer->size));
        buffer->empty++;
        buffer->full--;
        
        
        pthread_cond_signal(&buffer->notFull);
        pthread_mutex_unlock(&buffer->Full_mutex);
        
        //   printf("preso ");
        //    printf("Ho finito di prelevare %d",self_id,buffer->full);
        return msg;

    
    
    }

}



msg_t* put_bloccante(buffer_t* buffer, msg_t* msg){
    
    pthread_t self_id;
    self_id=pthread_self();
 //   printf("\nHello from thread, i'm going to put.  My id is  %u and now there are %d elements into the buufer\n",self_id,buffer->full);
    
    pthread_mutex_lock(&buffer->Full_mutex);     //voglio accedere alla variabile condizione senza interferenze
    while(buffer->empty == 0){ //una variabile per verigi
        // gli passo anche il riferimento al mutex per far entrare in regione critica anche altri f.d.e
        pthread_cond_wait(&buffer->notFull, &buffer->Full_mutex);
    }

    
    //è necessaria la mutua esclusione su D? alla fine soltanto uno può essere qui dentro
    // es. faccio un mutex su empty e vedo che ci sono 8 caselle libere, nessun altro può entrare qui dentro!
    
    int d_pos = buffer->D;

    msg_t* new_msg=msg;
    buffer->cells[d_pos]=*new_msg;
    buffer->D=(d_pos+1)%((buffer->size));
    buffer->empty--;
    buffer->full++;
    
    pthread_cond_signal(&buffer->notEmpty);
    pthread_mutex_unlock(&buffer->Full_mutex);

//    printf("%d: inserito",getpid());
//    printf("Ho finito di inserire %d",self_id,buffer->full);

    return new_msg;
}

msg_t* get_bloccante(buffer_t* buffer){
    
 //   fprintf (stderr, "child thread pid is %d\n", (int) getpid ());
    
    pthread_t self_id;
    self_id=pthread_self();
//    printf("\nHello from thread, i'm going to get.  My id is  %u\n and now there are %d elements into the buufer\n",self_id,buffer->full);
    
    pthread_mutex_lock(&buffer->Full_mutex);
    
    while(buffer->full == 0){
//        printf("******************************");

        pthread_cond_wait(&buffer->notEmpty, &buffer->Full_mutex);
 //       printf("******************************");

    }
    
    msg_t* msg=NULL;
    int t_pos=buffer->T;
    msg=&buffer->cells[t_pos];
    buffer->T = (t_pos+1)%((buffer->size));
    buffer->empty++;
    buffer->full--;

    
    pthread_cond_signal(&buffer->notFull);
    pthread_mutex_unlock(&buffer->Full_mutex);
    
 //   printf("preso ");
//    printf("Ho finito di prelevare %d",self_id,buffer->full);
    return msg;

}

msg_t* do_put_bloccante(void* arguments){

    struct arg_struct *args=arguments;
    return put_bloccante(args->buffer, args->msg);
}

msg_t* do_put_non_bloccante(void* arguments){
    
    struct arg_struct *args=arguments;
    return put_non_bloccante(args->buffer, args->msg);
}


msg_t* do_get_bloccante(void* arguments){
    struct arg_struct *args=arguments;
    return get_bloccante(args->buffer);

}

msg_t* do_get_non_bloccante(void* arguments){
    struct arg_struct *args=arguments;
    return get_non_bloccante(args->buffer);
    
}


void buffer_destroy(buffer_t* buffer){
    free(buffer->cells);
    free(buffer);
}


