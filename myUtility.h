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



/*  NON CANCELLARE
 char* parola="hello";
 
 bufferUnitary=buffer_init(10);
 msg_t* msg= msg_init(parola);
 msg_t* msg1= msg_init(parola);
 
 pthread_t c1,p1;
 
 struct Ntimes ars;
 ars.buffer=bufferUnitary;
 ars.msg=msg;
 ars.i=5; //cambiare in 50
 
 pthread_create(&p1, NULL, &put_blocking_Ntimes, &ars);
 pthread_create(&c1, NULL, &get_blocking_Ntimes, &ars);
 pthread_join(p1, &msg);
 pthread_join(c1, &msg1);
 CU_ASSERT( 0 == bufferUnitary->K);
 CU_ASSERT_STRING_EQUAL("hello", msg->content);
 CU_ASSERT_STRING_EQUAL("hello", msg1->content); */





#endif /* utility_h */
