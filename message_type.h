//
//  message_type.h
//  HWtest
//
//  Created by Mattia Iodice on 22/11/16.
//  Copyright © 2016 Mattia Iodice. All rights reserved.
//

#ifndef message_type_h
#define message_type_h

#include <stdio.h>


typedef struct mssg {
    void *content;   // generico contenuto del messaggio
    struct mssg* (*msg_init)(void*);        // creazione msg
    void (* msg_destroy)(struct mssg *);      // deallocazione msg
    struct mssg * (*msg_copy)(struct mssg *); // creazione/copia msg
} msg_t;

// creare ed allocare un nuovo messaggio ospitante un generico contenuto content di tipo void*
msg_t *msg_init(void *content);

//deallocare un messaggio
void msg_destroy(msg_t *msg);

//creare ed allocare un nuovo messaggio ospitante il medesimo contenuto di un messaggio dato, similarmente ai classici costruttori di copia
msg_t *msg_copy(msg_t* msg);

/*contenuto stringhe*/
msg_t* msg_copy_string(msg_t* msg);
msg_t* msg_init_string(void* content);
void msg_destroy_string(msg_t* msg);

#endif /* message_type_h */
