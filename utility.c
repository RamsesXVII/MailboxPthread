//
//  utility.c
//  HWtest
//
//  Created by Mattia Iodice on 25/11/16.
//  Copyright © 2016 Mattia Iodice. All rights reserved.
//


#include "utility.h"


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

void get_notBlocking_Ntimes(void* args){
    /*   int i;
     for (i=0; i<n; i++)
     get_non_bloccante(buffer);
     pthread_exit(NULL); */
}
void put_notBlocking_Ntimes(void* args){
    /*    int i;
     for (i=0; i<n; i++)
     put_non_bloccante(buffer, msg);
     pthread_exit(msg); */
}