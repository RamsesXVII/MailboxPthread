//
//  notBlocking_test.c
//  HWtest
//
//  Created by Mattia Iodice on 26/11/16.
//  Copyright © 2016 Mattia Iodice. All rights reserved.
//

#include "notBlocking_test.h"

int init_suite3(void)
{
    buffer=buffer_init(1);
    return 0;
    
}

int clean_suite3(void)
{
    buffer_destroy(buffer);
    return 0;
}



void testNotBlocking(void)
{
    
    char* parola="hello";
    
    msg_t* msg= msg_init(parola);
    msg_t* msg1= msg_init(parola);
    
    pthread_t c1,p1;
    
    struct arg_struct ars;
    ars.buffer=buffer;
    ars.msg=msg;
    
    CU_ASSERT( 0 == buffer->K);
    pthread_create(&c1, NULL, &do_get_non_bloccante, buffer);
    pthread_join(c1, &msg1);
    CU_ASSERT_PTR_NULL(msg1);
    CU_ASSERT( 0 == buffer->K);
    CU_ASSERT( 0 == buffer->D);
    
    pthread_create(&p1, NULL, &do_put_non_bloccante, &ars);
    pthread_join(p1, NULL);
    CU_ASSERT( 1 == buffer->K);
    
    
}