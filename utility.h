//
//  utility.h
//  HWtest
//
//  Created by Mattia Iodice on 25/11/16.
//  Copyright © 2016 Mattia Iodice. All rights reserved.
//

#ifndef utility_h
#define utility_h

#include "buffer_type.h"


/*Per passare più parametri ad una funzione di un thread*/
struct arg_struct{
    buffer_t *buffer;
    msg_t* msg;
};

struct Ntimes{
    buffer_t *buffer;
    msg_t* msg;
    int i; //mica ci va un puntatore?
};

/*importante*/
void printThreadId();
void printPutCompleted();
void printGetCompleted();

void printK(int i);
void printStars();


void get_blocking_Ntimes(void* arguments);
void put_blocking_Ntimes(void* arguments);

void get_notBlocking_Ntimes(void* arguments);
void put_notBlocking_Ntimes(void* arguments);





#endif /* utility_h */
