#include "mainFunctions_test.h"

int main()
{
    CU_pSuite pSuite_basicFunction = NULL;
    CU_pSuite pSuite_blocking = NULL;
    CU_pSuite pSuite_not_blocking = NULL;
    
    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();
    
    pSuite_basicFunction = CU_add_suite("Single_thread_data_structure_test_suite", init_suiteBasic, clean_suiteBasic);
    pSuite_blocking = CU_add_suite("Blocking_methods_test_suite", init_suiteTestBlocking, clean_suiteTestBlocking);
    pSuite_not_blocking=CU_add_suite("not_blocking_methods_test_suite", init_suiteTestNotBlocking, clean_suiteTestNotBlocking);
    
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

    if ((NULL == CU_add_test(pSuite_basicFunction, "test of buffer init function", testBufferInit)) ||
        (NULL == CU_add_test(pSuite_basicFunction, "test of put", testPut)) ||
        (NULL == CU_add_test(pSuite_basicFunction, "test of get", testGet)))
    {
        CU_cleanup_registry();
        return CU_get_error();
    }
    
    
    if ((NULL == CU_add_test(pSuite_blocking, "test of a blocking put into an empty buffer. Unitary buffer. ",blocking_put_emptyBuffer_b1))||
        (NULL == CU_add_test(pSuite_blocking, "test of a blocking get from a not empty buffer. Unitary buffer.",blocking_get_not_emptyBuffer_b1))||
        (NULL == CU_add_test(pSuite_blocking, "test of a blocking get from an empty buffer. Unitary buffer.",blocking_get_emptyBuffer_b1))||
        (NULL == CU_add_test(pSuite_blocking, "test of a blocking put into a full buffer. Unitary buffer.",blocking_put_fullBuffer_b1))||
        (NULL == CU_add_test(pSuite_blocking, "test of concurrent puts and gets from a buffer. Unitary buffer.",concurrent_putandget_b1))||
        (NULL == CU_add_test(pSuite_blocking, "test of sequential puts and gets from a buffer that becomes full.Not unitary buffer.",sequential_putandget_bN)))
    {
        CU_cleanup_registry();
        return CU_get_error();
    }
    
    if ((NULL == CU_add_test(pSuite_not_blocking, "test of not blocking functions", testNotBlocking))||
        (NULL == CU_add_test(pSuite_not_blocking, "test of esecution of mailbox problem", test_notBlocking_MailBox)))
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();
    return CU_get_error();
}
