#ifndef blocking_test_h
#define blocking_test_h

#include <stdio.h>
#include "myUtility.h"
#include "Basic.h"



static buffer_t* bufferUnitary;
static buffer_t* bufferNotUnitary;


int init_suiteTestBlocking(void);
int clean_suiteTestBlocking(void);

void blocking_put_emptyBuffer_b1(void);
void blocking_get_not_emptyBuffer_b1(void);
void blocking_get_emptyBuffer_b1(void);
void blocking_put_fullBuffer_b1(void);
void blocking_concurrent_putandget_b1(void);
void blocking_sequential_putandget_bN(void);
void blocking_concurrent_putandget_bN(void);
void blocking_put_emptyBuffer_msgNull_b1(void);


void blocking_put_fullBuffer_b1_TestSupport(void);



#endif /* blocking_test_h */
