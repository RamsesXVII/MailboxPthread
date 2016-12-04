#include "dataStructures_test.h"


/* This suite tests a put and a get into the buffer
 * It's a single-thread excecution
 */
int init_suiteBasic(void)
{
    bufferUnitary=buffer_init(1);
    bufferNotUnitary=buffer_init(5);
    
    if(bufferUnitary!=NULL && bufferNotUnitary!=NULL)
        return 0;
    
    else return 1;
    
}

int clean_suiteBasic(void)
{
    buffer_destroy(bufferUnitary);
    buffer_destroy(bufferNotUnitary);
    return 0;
}

void testBufferInit(void)
{
    CU_ASSERT(1 == bufferUnitary->size);
    CU_ASSERT(0 == bufferUnitary->K);
    CU_ASSERT(0 == bufferUnitary->T);
    CU_ASSERT(0 == bufferUnitary->D);

    CU_ASSERT(5 == bufferNotUnitary->size);
    CU_ASSERT(0 == bufferNotUnitary->K);
    CU_ASSERT(0 == bufferNotUnitary->T);
    CU_ASSERT(0 == bufferNotUnitary->D);
   
    
}

void testPut(void)
{
    
    char* message_content="hello";
    msg_t* msg= msg_init(message_content);
    
    msg=put_bloccante(bufferUnitary, msg);
    CU_ASSERT(1 == bufferUnitary->K);
    CU_ASSERT_STRING_EQUAL("hello", bufferUnitary->cells[0].content);
    CU_ASSERT_STRING_EQUAL("hello", msg->content);

}

void testGet(void)
{
    msg_t* msg= get_bloccante(bufferUnitary);
    CU_ASSERT(0 == bufferUnitary->K);
    CU_ASSERT_STRING_EQUAL("hello", bufferUnitary->cells[0].content); //il messaggio non deve essere cancellato ma sovrascritto
    CU_ASSERT_STRING_EQUAL("hello", msg->content);
}



