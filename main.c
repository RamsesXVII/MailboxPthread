//
//  main.c
//  HWtest
//
//  Created by Mattia Iodice on 23/11/16.
//  Copyright © 2016 Mattia Iodice. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "buffer_type.h"
#include "message_type.h"

void inserisci(buffer_t* buffer);
void preleva(buffer_t* buffer);

int main(){
    buffer_t* buffer=buffer_init(5);

    
    pthread_t prod1,prod2,prod3,prod4,cons1,cons2,cons3,cons4;

    pthread_create(&prod1, NULL, &inserisci, buffer);
    pthread_create(&prod2, NULL, &inserisci, buffer);
    pthread_create(&prod3, NULL, &inserisci, buffer);
    pthread_create(&prod4, NULL, &inserisci, buffer);
    pthread_create(&cons1, NULL, &preleva, buffer);
    pthread_create(&cons2, NULL, &preleva, buffer);
    pthread_create(&cons3, NULL, &preleva, buffer);
    pthread_create(&cons4, NULL, &preleva, buffer);
}


void inserisci(buffer_t* buffer){
    msg_t* ms=msg_init("ciao");
    put_bloccante(buffer, ms);
}

void preleva(buffer_t* buffer){
    msg_t* ms=NULL;
    get_bloccante(buffer);
}