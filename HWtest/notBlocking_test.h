#ifndef notBlocking_test_h
#define notBlocking_test_h

 #include "blocking_test.h"

int init_suiteTestNotBlocking(void);
int clean_suiteTestNotBlocking(void);

void notBlocking_put_emptyBuffer_b1(void);
void notBlocking_get_not_emptyBuffer_b1(void);
void notBlocking_get_emptyBuffer_b1(void);
void notBlocking_put_fullBuffer_b1(void);

void notblocking_sequential_putandget_bN(void);
void notblocking_concurrent_put_bN(void);



#endif /* notBlocking_test_h */
