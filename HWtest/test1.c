#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

#include "Basic.h"
#include "buffer_type.h"
#include "utility.h"

static buffer_t* buffer;

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


int main()
{
    CU_pSuite pSuite_basicFunction = NULL;
    CU_pSuite pSuite_blocking = NULL;
    CU_pSuite pSuite_not_blocking = NULL;
    
    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();
    
    pSuite_basicFunction = CU_add_suite("Single_thread_data_structure_test_suite", init_suite1, clean_suite1);
    pSuite_blocking = CU_add_suite("Blocking_methods_test_suite", init_suite2, clean_suite2);
    pSuite_not_blocking=CU_add_suite("not_blocking_methods_test_suite", init_suite3, clean_suite3);
    
    if (NULL == pSuite_basicFunction) {
        CU_cleanup_registry();
        return CU_get_error();
    }
    
    if (NULL ==pSuite_blocking){
        CU_cleanup_registry();
        return CU_get_error();
    }
    
    if (NULL ==pSuite_not_blocking){
        CU_cleanup_registry();
        return CU_get_error();
    }
    
    /* add the tests to the suite */
    /* NOTE - ORDER IS IMPORTANT - MUST TEST fread() AFTER fprintf() */
    if ((NULL == CU_add_test(pSuite_basicFunction, "test of buffer init", testBufferInit)) ||
        (NULL == CU_add_test(pSuite_basicFunction, "test of put", testPut)) ||
        (NULL == CU_add_test(pSuite_basicFunction, "test of get", testGet)))
    {
        CU_cleanup_registry();
        return CU_get_error();
    }
    
    if ((NULL == CU_add_test(pSuite_blocking, "test of put and get with N=1", testThread)))
    {
        CU_cleanup_registry();
        return CU_get_error();
    }
    
    if ((NULL == CU_add_test(pSuite_not_blocking, "test of not blocking functions", testNotBlocking))||
        (NULL == CU_add_test(pSuite_not_blocking, "test of get", testMailBox)))
    {
        CU_cleanup_registry();
        return CU_get_error();
    }
    
    /* Run all tests using the CUnit Basic interface */
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();
    return CU_get_error();
}
