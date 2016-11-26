//
//  blocking_test.c
//  HWtest
//
//  Created by Mattia Iodice on 26/11/16.
//  Copyright © 2016 Mattia Iodice. All rights reserved.
//

#include "blocking_test.h"

int init_suite2(void)
{
    buffer=buffer_init(1);
    return 0;
    
}

int clean_suite2(void)
{
    buffer_destroy(buffer);
    return 0;
}


void testThread(void)
{
    
    char* parola="hello";
    
    buffer_t* buffer= buffer_init(1);
    msg_t* msg= msg_init(parola);
    msg_t* msg_got1= NULL;
    msg_t* msg_got2= NULL;
    
    pthread_t p1,p2,p3,c1,c2;
    
    struct arg_struct ars;
    ars.buffer=buffer;
    ars.msg=msg;
    
    pthread_create(&c1, NULL, &do_get_bloccante, buffer);
    pthread_create(&p1, NULL, &do_put_bloccante, &ars);
    pthread_create(&c2, NULL, &do_get_bloccante, buffer);
    pthread_create(&p2, NULL, &do_put_bloccante, &ars);
    pthread_create(&p3, NULL, &do_put_bloccante, &ars);
    
    pthread_join(p1, NULL);
    pthread_join(p2, NULL);
    pthread_join(p3, NULL);
    pthread_join(c1, &msg_got1);
    pthread_join(c2, &msg_got2);
    
    CU_ASSERT_STRING_EQUAL("hello", msg_got1->content);
    CU_ASSERT_STRING_EQUAL("hello", msg_got2->content);
    CU_ASSERT( 1 == buffer->K);
    
}


void testMailBox(void)
{
    char* parola="hello";
    
    buffer=buffer_init(10);
    msg_t* msg= msg_init(parola);
    msg_t* msg1= msg_init(parola);
    
    pthread_t c1,p1;
    
    struct Ntimes ars;
    ars.buffer=buffer;
    ars.msg=msg;
    ars.i=5; //cambiare in 50
    
    pthread_create(&p1, NULL, &put_blocking_Ntimes, &ars);
    pthread_create(&c1, NULL, &get_blocking_Ntimes, &ars);
    pthread_join(p1, &msg);
    pthread_join(c1, &msg1);
    CU_ASSERT( 0 == buffer->K);
    CU_ASSERT_STRING_EQUAL("hello", msg->content);
    CU_ASSERT_STRING_EQUAL("hello", msg1->content);
    
    
}

