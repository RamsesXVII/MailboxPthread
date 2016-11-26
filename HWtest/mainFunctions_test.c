//
//  mainFunctions_test.c
//  HWtest
//
//  Created by Mattia Iodice on 26/11/16.
//  Copyright © 2016 Mattia Iodice. All rights reserved.
//

#include "mainFunctions_test.h"


/* This suite tests an insertion and a getting into the buffer
 * It's a single-thread excecution
 */
int init_suite1(void)
{
    buffer=buffer_init(1);
    return 0;
    
}

int clean_suite1(void)
{
    buffer_destroy(buffer);
    return 0;
}

void testBufferInit(void)
{
    if (NULL != buffer)
        CU_ASSERT(1 == buffer->size);
    
}

void testPut(void)
{
    
    char* message_content="hello";
    msg_t* msg= msg_init(message_content);
    
    msg=put_bloccante(buffer, msg);
    CU_ASSERT(1 == buffer->K);
    CU_ASSERT_STRING_EQUAL("hello", buffer->cells[0].content);
    CU_ASSERT_STRING_EQUAL("hello", msg->content);
}

void testGet(void)
{
    
    msg_t* msg= get_bloccante(buffer);
    CU_ASSERT(0 == buffer->K);
    CU_ASSERT_STRING_EQUAL("hello", msg->content);
}



