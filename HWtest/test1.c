#include "mainFunctions_test.h"

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

    if ((NULL == CU_add_test(pSuite_basicFunction, "test of buffer init", testBufferInit)) ||
        (NULL == CU_add_test(pSuite_basicFunction, "test of put", testPut)) ||
        (NULL == CU_add_test(pSuite_basicFunction, "test of get", testGet)))
    {
        CU_cleanup_registry();
        return CU_get_error();
    }
    
    if ((NULL == CU_add_test(pSuite_blocking, "test of put and get with N=1", testThread))||
        (NULL == CU_add_test(pSuite_blocking, "test of esecution of mailbox problem", testMailBox)))
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
