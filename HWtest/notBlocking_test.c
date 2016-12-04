//
//  notBlocking_test.c
//  HWtest
//
//  Created by Mattia Iodice on 26/11/16.
//  Copyright © 2016 Mattia Iodice. All rights reserved.
//

#include "notBlocking_test.h"

int init_suiteTestNotBlocking(void)
{
    bufferUnitary=buffer_init(1);
    bufferNotUnitary=buffer_init(5);
    if(bufferUnitary!=NULL && bufferNotUnitary!=NULL)
        return 0;
    
    else return 1;
}

int clean_suiteTestNotBlocking(void)
{
    buffer_destroy(bufferUnitary);
    buffer_destroy(bufferNotUnitary);
    return 0;
}


void notBlocking_put_emptyBuffer_b1(void)
{
    int* contentToInsert=42;
    
    msg_t* msgToPut= msg_init(contentToInsert);
    msg_t* msgReturnedByPut=NULL;
    
    pthread_t p1;

    struct arg_struct parameters;
    parameters.buffer=bufferUnitary;
    parameters.msg=msgToPut;
    
    CU_ASSERT( isEmpty(bufferUnitary));
    pthread_create(&p1, NULL, &do_put_non_bloccante, &parameters);
    pthread_join(p1, &msgReturnedByPut);
    CU_ASSERT_EQUAL(msgReturnedByPut->content, contentToInsert);
    CU_ASSERT( 1 == bufferUnitary->K);
    CU_ASSERT( 0 == bufferUnitary->D);
}

void notBlocking_get_not_emptyBuffer_b1(void){
    msg_t* msgReturnedByGet= NULL;
    
    pthread_t c1;
    
    CU_ASSERT(1 == bufferUnitary->K);
    CU_ASSERT(isFull(bufferUnitary));
    
    pthread_create(&c1, NULL, &do_get_non_bloccante, bufferUnitary);
    pthread_join(c1, &msgReturnedByGet);
    
    CU_ASSERT(isEmpty(bufferUnitary));
    CU_ASSERT_EQUAL(42,(int) msgReturnedByGet->content);
    CU_ASSERT(0 == bufferUnitary->T);
    CU_ASSERT(0 == bufferUnitary->D);
}

void notBlocking_get_emptyBuffer_b1(void){
    char* notNullcontent="not null";
    msg_t* msgReturnedByGet= msg_init(notNullcontent); // mi assicuro che il messaggio su cui faro il test non è null
   
    bufferUnitary=buffer_init(1);
    
    pthread_t c1;
    
    CU_ASSERT(isEmpty(bufferUnitary));
    
    pthread_create(&c1, NULL, &do_get_non_bloccante, bufferUnitary);
    pthread_join(c1, &msgReturnedByGet);
    
    CU_ASSERT(isEmpty(bufferUnitary));
    CU_ASSERT_EQUAL(msgReturnedByGet,BUFFER_ERROR);
    CU_ASSERT(0 == bufferUnitary->T);
    CU_ASSERT(0 == bufferUnitary->D);
    
}

void notBlocking_put_fullBuffer_b1(void){
    int* contentToInsert=42;
    msg_t* msgToPut= msg_init(contentToInsert);
    bufferUnitary=buffer_init(1);
    
    msg_t* msgReturnedByPut=msg_init_string("not null");
    
    pthread_t p1;
    
    struct arg_struct parameters;
    parameters.buffer=bufferUnitary;
    parameters.msg=msgToPut;
    
    bufferUnitary->cells[0].content=12;
    bufferUnitary->K++;
    
    CU_ASSERT(isFull(bufferUnitary));
    
    pthread_create(&p1, NULL, &do_put_non_bloccante, &parameters);
    pthread_join(p1, &msgReturnedByPut);
    
    CU_ASSERT(isFull(bufferUnitary));
    CU_ASSERT_EQUAL(BUFFER_ERROR, msgReturnedByPut);
    
}

void notblocking_sequential_putandget_bN(void){
    bufferNotUnitary=buffer_init(3);
    int* contentToInsert=42;
    msg_t* msgToPut= msg_init(contentToInsert);
    msg_t* msgReturned=msg_init_string("not null");
    
    pthread_t c1,c2,c3,p1,p2,p3,p4,p5,p6;
    
    struct arg_struct parameters;
    parameters.buffer=bufferNotUnitary;
    parameters.msg=msgToPut;
    
    CU_ASSERT(isEmpty(bufferNotUnitary));
    
    pthread_create(&p1, NULL, &do_put_non_bloccante, &parameters);
    pthread_join(p1, NULL);
    CU_ASSERT(bufferNotUnitary->K==1);
    
    pthread_create(&p2, NULL, &do_put_non_bloccante, &parameters);
    pthread_join(p2, NULL);
    CU_ASSERT(bufferNotUnitary->K==2);
    
    pthread_create(&p3, NULL, &do_put_non_bloccante, &parameters);
    pthread_join(p3, NULL);
    CU_ASSERT(bufferNotUnitary->K==3);
    CU_ASSERT(isFull(bufferNotUnitary))
    
    pthread_create(&p4, NULL, &do_put_non_bloccante, &parameters);
    pthread_join(p4, &msgReturned);

    CU_ASSERT_EQUAL(msgReturned, BUFFER_ERROR);
    CU_ASSERT(0 == bufferUnitary->T);
    CU_ASSERT(0 == bufferUnitary->D);
    
    
    pthread_create(&c1, NULL, &do_get_bloccante, bufferNotUnitary);
    pthread_create(&c2, NULL, &do_get_bloccante, bufferNotUnitary);
    pthread_create(&c3, NULL, &do_get_bloccante, bufferNotUnitary);
    pthread_join(c1, NULL);
    pthread_join(c2, NULL);
    pthread_join(c3, NULL);
    
    CU_ASSERT(isEmpty(bufferNotUnitary));
    
}

void notblocking_concurrent_put_bN(void){
    bufferNotUnitary=buffer_init(3);
    int* contentToInsert=42;
    msg_t* msgToPut= msg_init(contentToInsert);
    msg_t* msgReturned=msgToPut; //not null
    pthread_t p1,p2,p3,p4;
    
    
    struct arg_struct parameters;
    parameters.buffer=bufferNotUnitary;
    parameters.msg=msgToPut;
    
    CU_ASSERT(isEmpty(bufferNotUnitary));
    
    pthread_create(&p1, NULL, &do_put_non_bloccante, &parameters);
    pthread_create(&p2, NULL, &do_put_non_bloccante, &parameters);
    pthread_create(&p3, NULL, &do_put_non_bloccante, &parameters);
    pthread_join(p1, NULL);
    pthread_join(p2, NULL);
    pthread_join(p3, NULL);

    CU_ASSERT(bufferNotUnitary->K==3);
    CU_ASSERT(isFull(bufferNotUnitary))
    
    pthread_create(&p4, NULL, &do_put_non_bloccante, &parameters);
    pthread_join(p4, &msgReturned);
    
    CU_ASSERT_EQUAL(msgReturned, BUFFER_ERROR);
    CU_ASSERT(0 == bufferUnitary->T);
    CU_ASSERT(0 == bufferUnitary->D);
    
}

/*
void test_notBlocking_MailBox(void)
{
    char* parola="hello";
    
    bufferUnitary=buffer_init(5);
    msg_t* msg= msg_init(parola);
    msg_t* msg1= NULL;
    
    pthread_t c1,p1;
    
    struct Ntimes ars;
    ars.buffer=bufferUnitary;
    ars.msg=msg;
    ars.i=20; //cambiare in 50
    
    pthread_create(&p1, NULL, &put_notBlocking_Ntimes, &ars);
    pthread_create(&c1, NULL, &get_notBlocking_Ntimes, &ars);
    pthread_join(p1, &msg1);
    pthread_join(c1, &msg1);
/* su questa esecuzione di test non si possono fare assunzioni, ma è interessante stampare il numero di messaggi 
 *del buffer e vedere la loro evoluzione*/


//}