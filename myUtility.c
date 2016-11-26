//
//  utility.c
//  HWtest
//
//  Created by Mattia Iodice on 25/11/16.
//  Copyright © 2016 Mattia Iodice. All rights reserved.
//


#include "myUtility.h"


void printThreadId(){
    pthread_t self_id;
    self_id=pthread_self();
    printf("Hello from %d thread.\n",self_id);
}

void printPutCompleted(){
    printThreadId();
    printf("Ho inserito\n");
    
}
void printGetCompleted(){
    printThreadId();
    printf("Ho prelevato\n");
}

void printStars(){
    printf("*****************\n");
    
}

void printK(int i){
    printf("%d%",i);
}


/*se non metto gli sleep uno scrive tutto quello che può poi l'altro scrive tutto quello che può*/
 
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
        //se qui si facesse una stampa della dimensione si potrebbero apprezzare fenomeni di interferenza
    }
    pthread_exit(msg);
}