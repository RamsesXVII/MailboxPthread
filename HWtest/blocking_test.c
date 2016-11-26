//
//  blocking_test.c
//  HWtest
//
//  Created by Mattia Iodice on 26/11/16.
//  Copyright © 2016 Mattia Iodice. All rights reserved.
//


#include "blocking_test.h"

int init_suiteTestBlocking(void)
{
    bufferUnitary=buffer_init(1);
    bufferNotUnitary=buffer_init(5);
    
    if(bufferUnitary!=NULL && bufferNotUnitary!=NULL)
        return 0;
    
    else return 1;
    
    
}

int clean_suiteTestBlocking(void)
{
    buffer_destroy(bufferUnitary);
    buffer_destroy(bufferNotUnitary);

    return 0;
}


void blocking_put_emptyBuffer_b1(void)
{
    int* contentToInsert=42;
    msg_t* msgToPut= msg_init(contentToInsert);
    msg_t* msgReturnedByPut= NULL;

    pthread_t p1;
    
    struct arg_struct ars;
    ars.buffer=bufferUnitary;
    ars.msg=msgToPut;
    
    pthread_create(&p1, NULL, &do_put_bloccante, &ars);
    pthread_join(p1, &msgReturnedByPut);
    
    CU_ASSERT(1 == bufferUnitary->K);
    CU_ASSERT(isFull(bufferUnitary));
    CU_ASSERT_EQUAL(contentToInsert, msgReturnedByPut->content);
    CU_ASSERT_EQUAL(contentToInsert, bufferUnitary->cells[0].content);

    
}

void blocking_get_not_emptyBuffer_b1(void){
    msg_t* msgReturnedByGet= NULL;

    pthread_t c1;
    
    CU_ASSERT(1 == bufferUnitary->K);
    CU_ASSERT(isFull(bufferUnitary));
    
    pthread_create(&c1, NULL, &do_get_bloccante, bufferUnitary);
    pthread_join(c1, &msgReturnedByGet);
    
    CU_ASSERT(isEmpty(bufferUnitary));
    CU_ASSERT_EQUAL(42,(int) msgReturnedByGet->content);



}


void blocking_get_emptyBuffer_b1(void)
{
    int* contentToInsert=42;
    bufferUnitary=buffer_init(1);
    msg_t* msgReturnedByGet;

    pthread_t c1;

    CU_ASSERT(isEmpty(bufferUnitary));
    
    pthread_create(&c1, NULL, &do_get_bloccante, bufferUnitary);
 
    sleep(2);
    
    CU_ASSERT(pthread_kill(c1, 0)==0); //il processo dorme? chiedere se effettivamente è così

    pthread_mutex_lock(&bufferUnitary->bufferMutex); //effettuo l'inserimento
    bufferUnitary->cells[0].content=contentToInsert;
    bufferUnitary->K++;
    pthread_cond_signal(&bufferUnitary->notEmpty);
    pthread_mutex_unlock(&bufferUnitary->bufferMutex);
    
    pthread_join(c1, &msgReturnedByGet);
    
    CU_ASSERT(isEmpty(bufferUnitary));
    CU_ASSERT_EQUAL(contentToInsert, msgReturnedByGet->content);    
}

void blocking_put_fullBuffer_b1(void)
{
  
    int* contentToInsert=42;
    msg_t* msgToPut= msg_init(contentToInsert);
    bufferUnitary=buffer_init(1);
    msg_t* msgReturnedByPut;
    
    pthread_t p1;
    
    struct arg_struct ars;
    ars.buffer=bufferUnitary;
    ars.msg=msgToPut;
    
    bufferUnitary->cells[0].content=12;
    bufferUnitary->K++;
    
    CU_ASSERT(isFull(bufferUnitary));
    
    pthread_create(&p1, NULL, &do_put_bloccante, &ars);
    
    sleep(2);
    
    CU_ASSERT(pthread_kill(p1, 0)==0); //il processo dorme? chiedere se effettivamente è così
    
    pthread_mutex_lock(&bufferUnitary->bufferMutex); //effettuo l'inserimento
    bufferUnitary->K--;
    pthread_cond_signal(&bufferUnitary->notFull);
    pthread_mutex_unlock(&bufferUnitary->bufferMutex);
    
    pthread_join(p1, &msgReturnedByPut);
    
    CU_ASSERT(isFull(bufferUnitary));
    CU_ASSERT_EQUAL(contentToInsert, msgReturnedByPut->content);
    CU_ASSERT_EQUAL(contentToInsert, bufferUnitary->cells[0].content);
}

void concurrent_putandget_b1(void){
    
    int* contentToInsert=42;
    
    bufferUnitary=buffer_init(1);
    msg_t* msgToPut= msg_init(contentToInsert);
    
    pthread_t c1,c2,p1,p2;
    
    struct Ntimes ars;
    ars.buffer=bufferUnitary;
    ars.msg=msgToPut;
    ars.i=5; //ciascun produttore produce 5 volte e ciascun consumatore consuma 5 volte
    
   
    pthread_create(&p1, NULL, &put_blocking_Ntimes, &ars);
    pthread_create(&p2, NULL, &put_blocking_Ntimes, &ars);
    pthread_create(&c1, NULL, &get_blocking_Ntimes, &ars);
    pthread_create(&c2, NULL, &get_blocking_Ntimes, &ars);

    pthread_join(p1, NULL);
    pthread_join(p2, NULL);
    pthread_join(c1, NULL);
    pthread_join(c2, NULL);

    CU_ASSERT(isEmpty(bufferUnitary));
    
    /* se per concorrente si intende senza loop va bene questo
     int* contentToInsert=42;
     
     bufferUnitary=buffer_init(1);
     msg_t* msgToPut= msg_init(contentToInsert);
     
     pthread_t c1,c2,c3,c4,p1,p2,p3,p4;
     
     struct arg_struct ars;
     ars.buffer=bufferUnitary;
     ars.msg=msgToPut;
     
     
     pthread_create(&c1, NULL, &do_get_bloccante, bufferUnitary);
     pthread_create(&c2, NULL, &do_get_bloccante, bufferUnitary);
     pthread_create(&c3, NULL, &do_get_bloccante, bufferUnitary);
     pthread_create(&c4, NULL, &do_get_bloccante, bufferUnitary);
     
     pthread_create(&p1, NULL, &do_put_bloccante, &ars);
     pthread_create(&p2, NULL, &do_put_bloccante, &ars);
     pthread_create(&p3, NULL, &do_put_bloccante, &ars);
     pthread_create(&p4, NULL, &do_put_bloccante, &ars);
     
     
     pthread_join(p1, NULL);
     pthread_join(p2, NULL);
     pthread_join(p3, NULL);
     pthread_join(p4, NULL);
     pthread_join(c1, NULL);
     pthread_join(c2, NULL);
     pthread_join(c3, NULL);
     pthread_join(c4, NULL);
     
     CU_ASSERT(isEmpty(bufferUnitary)); */

    
}



