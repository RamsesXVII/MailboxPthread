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
    
    struct arg_struct parameters;
    parameters.buffer=bufferUnitary;
    parameters.msg=msgToPut;
    
    pthread_create(&p1, NULL, &do_put_bloccante, &parameters);
    pthread_join(p1, &msgReturnedByPut);
    
    CU_ASSERT(1 == bufferUnitary->K);
    CU_ASSERT(isFull(bufferUnitary));
    CU_ASSERT_EQUAL(contentToInsert, msgReturnedByPut->content);
    CU_ASSERT_EQUAL(contentToInsert, bufferUnitary->cells[0].content);
    CU_ASSERT(0 == bufferUnitary->T);
    CU_ASSERT(0 == bufferUnitary->D);

    
}


void blocking_put_emptyBuffer_msgNull_b1(void){
    msg_t* msgReturnedByPut= NULL;
    
    pthread_t p1;
    
    struct arg_struct para;
    para.buffer=bufferUnitary;
    para.msg=NULL;
    
    pthread_create(&p1, NULL, &do_put_bloccante, &para);
    pthread_join(p1, &msgReturnedByPut);
    
    CU_ASSERT_STRING_EQUAL(msgReturnedByPut->content, "non è possibile inserire messaggi nulli");

    
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
    CU_ASSERT(0 == bufferUnitary->T);
    CU_ASSERT(0 == bufferUnitary->D);
}


void blocking_get_emptyBuffer_b1(void)
{
    int* contentToInsert=42;
    bufferUnitary=buffer_init(1);
    msg_t* msgReturnedByGet=NULL;
    
    pthread_t c1;
    
    CU_ASSERT(isEmpty(bufferUnitary));
    
    pthread_create(&c1, NULL, &do_get_bloccante, bufferUnitary);

    sleep(2);
    CU_ASSERT(pthread_kill(c1, 0)==0);
    
    pthread_mutex_lock(&bufferUnitary->bufferMutex);
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
    msg_t* msgReturnedByPut=NULL;
    
    pthread_t p1;
    
    struct arg_struct parameters;
    parameters.buffer=bufferUnitary;
    parameters.msg=msgToPut;
    
    bufferUnitary->cells[0].content=12;
    bufferUnitary->K++;
    
    CU_ASSERT(isFull(bufferUnitary));
    
    pthread_create(&p1, NULL, &do_put_bloccante, &parameters);
    
    sleep(2);
    
    CU_ASSERT(pthread_kill(p1, 0)==0);
    
    pthread_mutex_lock(&bufferUnitary->bufferMutex); //effettuo l'inserimento
    bufferUnitary->K--;
    pthread_cond_signal(&bufferUnitary->notFull);
    pthread_mutex_unlock(&bufferUnitary->bufferMutex);
    
    pthread_join(p1, &msgReturnedByPut);
    
    CU_ASSERT(isFull(bufferUnitary));
    CU_ASSERT_EQUAL(contentToInsert, msgReturnedByPut->content);
    CU_ASSERT_EQUAL(contentToInsert, bufferUnitary->cells[0].content);
}

void blocking_concurrent_putandget_b1(void){
 
     int* contentToInsert=42;
     
     bufferUnitary=buffer_init(1);
     msg_t* msgToPut= msg_init(contentToInsert);
     
     pthread_t c1,c2,p1,p2;
     
     struct Ntimes parameters;
     parameters.buffer=bufferUnitary;
     parameters.msg=msgToPut;
     parameters.i=5; //ciascun produttore produce 5 volte e ciascun consumatore consuma 5 volte
     
     
     pthread_create(&p1, NULL, &put_blocking_Ntimes, &parameters);
     pthread_create(&p2, NULL, &put_blocking_Ntimes, &parameters);
     pthread_create(&c1, NULL, &get_blocking_Ntimes, &parameters);
     pthread_create(&c2, NULL, &get_blocking_Ntimes, &parameters);
     
     pthread_join(p1, NULL);
     pthread_join(p2, NULL);
     pthread_join(c1, NULL);
     pthread_join(c2, NULL);
     
    
     /* è probabile che con questa eseczione le sequenze di interleaving siano quelle che ci si aspetta con maggiore prrobabilità
    int* contentToInsert=42;
    
    bufferUnitary=buffer_init(1);
    msg_t* msgToPut= msg_init(contentToInsert);
    
    pthread_t c1,c2,c3,c4,p1,p2,p3,p4;
    
    struct arg_struct parameters;
    parameters.buffer=bufferUnitary;
    parameters.msg=msgToPut;
    
    CU_ASSERT(isEmpty(bufferUnitary));
    
    pthread_create(&c1, NULL, &do_get_bloccante, bufferUnitary);
    pthread_create(&c2, NULL, &do_get_bloccante, bufferUnitary);
    pthread_create(&c3, NULL, &do_get_bloccante, bufferUnitary);
    pthread_create(&c4, NULL, &do_get_bloccante, bufferUnitary);
    
    pthread_create(&p1, NULL, &do_put_bloccante, &parameters);
    pthread_create(&p2, NULL, &do_put_bloccante, &parameters);
    pthread_create(&p3, NULL, &do_put_bloccante, &parameters);
    pthread_create(&p4, NULL, &do_put_bloccante, &parameters);
    
    pthread_join(p1, NULL);
    pthread_join(p2, NULL);
    pthread_join(p3, NULL);
    pthread_join(p4, NULL);
    pthread_join(c1, NULL);
    pthread_join(c2, NULL);
    pthread_join(c3, NULL);
    pthread_join(c4, NULL); */
    
    CU_ASSERT(isEmpty(bufferUnitary));
}

void blocking_sequential_putandget_bN(void){
    int* contentToInsert=42;
    msg_t* msgToPut= msg_init(contentToInsert);
    pthread_t c1,c2,c3,p1,p2,p3,p4,p5,p6;
    
    struct arg_struct parameters;
    parameters.buffer=bufferNotUnitary;
    parameters.msg=msgToPut;
    
    CU_ASSERT(isEmpty(bufferNotUnitary));

    pthread_create(&p1, NULL, &do_put_bloccante, &parameters);
    pthread_join(p1, NULL);
    CU_ASSERT(bufferNotUnitary->K==1);
    
    pthread_create(&p2, NULL, &do_put_bloccante, &parameters);
    pthread_join(p2, NULL);
    CU_ASSERT(bufferNotUnitary->K==2);

    pthread_create(&p3, NULL, &do_put_bloccante, &parameters);
    pthread_join(p3, NULL);
    CU_ASSERT(bufferNotUnitary->K==3);

    pthread_create(&p4, NULL, &do_put_bloccante, &parameters);
    pthread_join(p4, NULL);
    CU_ASSERT(bufferNotUnitary->K==4);

    pthread_create(&p5, NULL, &do_put_bloccante, &parameters);
    pthread_join(p5, NULL);
    CU_ASSERT(bufferNotUnitary->K==5);
    CU_ASSERT(isFull(bufferNotUnitary));

    CU_ASSERT(0 == bufferUnitary->T);
    CU_ASSERT(0 == bufferUnitary->D);

    pthread_create(&p6, NULL, &do_put_bloccante, &parameters);
    CU_ASSERT(pthread_kill(p1, 0)==0); //il processo dorme? chiedere se effettivamente è così

    pthread_create(&c1, NULL, &do_get_bloccante, bufferNotUnitary);
    pthread_create(&c2, NULL, &do_get_bloccante, bufferNotUnitary);
    pthread_create(&c3, NULL, &do_get_bloccante, bufferNotUnitary);
    pthread_join(c1, NULL);
    pthread_join(c2, NULL);
    pthread_join(c3, NULL);
    
    pthread_join(p6, NULL);
    
    CU_ASSERT(3 == bufferNotUnitary->T);
    CU_ASSERT(1 == bufferNotUnitary->D);
    CU_ASSERT(3 == bufferNotUnitary->K);

    
}

void blocking_concurrent_putandget_bN(void){
    bufferNotUnitary=buffer_init(3);
    int* contentToInsert=42;
    msg_t* msgToPut= msg_init(contentToInsert);
    pthread_t c1,c2,c3,p1,p2,p3,p4,p5,p6;
    
    struct arg_struct parameters;
    parameters.buffer=bufferNotUnitary;
    parameters.msg=msgToPut;
    
    CU_ASSERT(isEmpty(bufferNotUnitary));
    
    pthread_create(&c1, NULL, &do_get_bloccante, bufferNotUnitary);
    pthread_create(&p1, NULL, &do_put_bloccante, &parameters);
    pthread_join(p1, NULL);
    pthread_join(c1, NULL);
    
    CU_ASSERT(bufferNotUnitary->K==0);

    pthread_create(&p2, NULL, &do_put_bloccante, &parameters);
    pthread_create(&p3, NULL, &do_put_bloccante, &parameters);
    pthread_create(&p4, NULL, &do_put_bloccante, &parameters);
    pthread_create(&p5, NULL, &do_put_bloccante, &parameters);
    pthread_create(&p6, NULL, &do_put_bloccante, &parameters);

    pthread_create(&c2, NULL, &do_get_bloccante, bufferNotUnitary);
    pthread_create(&c3, NULL, &do_get_bloccante, bufferNotUnitary);

    pthread_join(p2, NULL);
    pthread_join(p3, NULL);
    pthread_join(p4, NULL);
    pthread_join(p5, NULL);
    pthread_join(p6, NULL);
    pthread_join(c2, NULL);
    pthread_join(c3, NULL);
    
    CU_ASSERT(bufferNotUnitary->K==3);
    CU_ASSERT(isFull(bufferNotUnitary));
    CU_ASSERT(bufferNotUnitary->T==0);
    CU_ASSERT(bufferNotUnitary->D==0);
    
}


void blocking_put_fullBuffer_b1_TestSupport(void)
{
    bufferUnitary=buffer_init(1);
    
    pthread_mutex_t sleepingMutex;
    pthread_cond_t isSleeping;
    pthread_cond_t isAwake;
    int* sleepState;
    
    pthread_mutex_init(&sleepingMutex, NULL);
    pthread_cond_init(&isSleeping, NULL);
    pthread_cond_init(&isAwake, NULL);
    
    *sleepState=0; //non sta dormendo
    
    int* contentToInsert=42;
    msg_t* msgToPut= msg_init(contentToInsert);
    msg_t* msgReturnedByPut=NULL;
    
    pthread_t p1;
    
    struct arg_structTest parameters;
    parameters.buffer=bufferUnitary;
    parameters.msg=msgToPut;
    parameters.sleepingMutex=&sleepingMutex;
    parameters.sleepState=sleepState;
    parameters.isSleeping=&isSleeping;
    parameters.isAwake=&isAwake;
    
    bufferUnitary->cells[0].content=12; //riempio il buffer
    bufferUnitary->K++;
    
    CU_ASSERT(isFull(bufferUnitary)); //il buffer è pieno
    
    pthread_create(&p1, NULL, &do_put_bloccante_TestSupport, &parameters);
    
    pthread_mutex_lock(&sleepingMutex);
    while(*sleepState!=1)
        pthread_cond_wait(&isSleeping, &sleepingMutex); //attendo che il thread effettui la wait e si addormenti
    pthread_mutex_unlock(&sleepingMutex);
    
    sleep(2);
    
    CU_ASSERT(*sleepState==1)
    
    pthread_mutex_lock(&bufferUnitary->bufferMutex); //effettuo il prelievo
    bufferUnitary->K--;
    pthread_cond_signal(&bufferUnitary->notFull);
    pthread_mutex_unlock(&bufferUnitary->bufferMutex);
    
    pthread_join(p1, &msgReturnedByPut);
    
    CU_ASSERT(*sleepState==0); //il thread non sta più dormendo
    CU_ASSERT(isFull(bufferUnitary));
    CU_ASSERT_EQUAL(contentToInsert, msgReturnedByPut->content);
    CU_ASSERT_EQUAL(contentToInsert, bufferUnitary->cells[0].content);
}


