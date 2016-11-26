//
//  blocking_test.h
//  HWtest
//
//  Created by Mattia Iodice on 26/11/16.
//  Copyright © 2016 Mattia Iodice. All rights reserved.
//

#ifndef blocking_test_h
#define blocking_test_h

#include <stdio.h>
#include "myUtility.h"
#include "Basic.h"



static buffer_t* buffer;


int init_suite2(void);
int clean_suite2(void);
void testThread(void);
void testMailBox(void);



#endif /* blocking_test_h */
