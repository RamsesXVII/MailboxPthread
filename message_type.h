
#ifndef message_type_h
#define message_type_h

#include <stdlib.h>
#include <stdio.h>
#include <stdio.h>


typedef struct mssg {
    void *content;
    struct mssg* (*msg_init)(void*);
    void (* msg_destroy)(struct mssg *);
    struct mssg * (*msg_copy)(struct mssg *);
} msg_t;

msg_t *msg_init(void *content);

void msg_destroy(msg_t *msg);

msg_t *msg_copy(msg_t* msg);

msg_t* msg_copy_string(msg_t* msg);
msg_t* msg_init_string(void* content);
void msg_destroy_string(msg_t* msg);

#endif /* message_type_h */
