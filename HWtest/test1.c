#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>



#include "Basic.h"
#include "buffer_type.h"


/* Pointer to the file used by the tests. */
void* thread_function (void* arg)
{
    fprintf (stderr, "<><><><><>child thread pid is %d\n", (int) getpid ());
    /* Spin forever.  */
    while (1);
    return NULL;
}

/* The suite initialization function.
 * Opens the temporary file used by the tests.
 * Returns zero on success, non-zero otherwise.
 */
int init_suite1(void)
{
    return 0;

}

/* The suite cleanup function.
 * Closes the temporary file used by the tests.
 * Returns zero on success, non-zero otherwise.
 */
int clean_suite1(void)
{
    return 0;
}


/*This function returns a buffer of specified length*/

buffer_t* createBuffer(int size){
    return buffer_init(size);
}





/* Simple test of fprintf().
 * Writes test data to the temporary file and checks
 * whether the expected number of bytes were written.
 */
void testFPRINTF(void)
{
    buffer_t* buffer= createBuffer(1);
    if (NULL != buffer) {
        CU_ASSERT(1 == buffer->size);

    }
}


void testPut(void)
{
    char* msgg="hello";
    buffer_t* buffer= createBuffer(1);
    msg_t* msg= msg_init(msgg);

    put_bloccante(buffer, msg);
    CU_ASSERT(1 == buffer->full);

}

void testGet(void)
{
    char* msgg="hello";
    buffer_t* buffer= createBuffer(1);
    msg_t* msg= msg_init(msgg);
    
    put_bloccante(buffer, msg);
    msg=get_bloccante(buffer);
    CU_ASSERT_PTR_NOT_NULL(msg);
    
}

void testThread(void)
{
    char* parola="hello";
    
    buffer_t* buffer= createBuffer(1);
    msg_t* msg= msg_init(parola);
    
    pthread_t p1,p2;
    
    struct arg_struct ars;
    ars.buffer=buffer;
    ars.msg=msg;
    
    
    pthread_create(&p1, NULL, &do_put_bloccante, &ars);
    pthread_create(&p2, NULL, &do_get_bloccante, &buffer);
    
    pthread_join(p1, NULL);
    pthread_join(p2, NULL);
    


    
    CU_ASSERT(0 == buffer->full);
//    pthread_create(&p3, NULL, &do_put_bloccante, &ars);
//    pthread_join(p3, NULL);
    
}

void testProva(void){
    pthread_t thread;
    fprintf (stderr, "<><><><><>main thread pid is %d\n", (int) getpid ());
    pthread_create (&thread, NULL, &thread_function, NULL);
    /* Spin forever.  */
    while (1);
}



/* The main() function for setting up and running the tests.
 * Returns a CUE_SUCCESS on successful running, another
 * CUnit error code on failure.
 */
int main()
{
    CU_pSuite pSuite = NULL;
    CU_pSuite pSuite_blocking = NULL;

    
    /* initialize the CUnit test registry */
    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();
    
    /* add a suite to the registry */
    pSuite = CU_add_suite("Suite_1", init_suite1, clean_suite1);
    pSuite_blocking = CU_add_suite("blocking_test_suite", init_suite1, clean_suite1);

    if (NULL == pSuite) {
        CU_cleanup_registry();
        return CU_get_error();
    }
    
    if (NULL ==pSuite_blocking){
        CU_cleanup_registry();
        return CU_get_error();
    }
    
    /* add the tests to the suite */
    /* NOTE - ORDER IS IMPORTANT - MUST TEST fread() AFTER fprintf() */
    if ((NULL == CU_add_test(pSuite, "test of fprintf()", testFPRINTF)) ||
        (NULL == CU_add_test(pSuite, "test of put", testPut)) ||
        (NULL == CU_add_test(pSuite, "test of get", testGet)))
    {
        CU_cleanup_registry();
        return CU_get_error();
    }
    
    if ((NULL == CU_add_test(pSuite_blocking, "test of P()", testThread))||
        (NULL == CU_add_test(pSuite_blocking, "test of P()", testProva)))
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
