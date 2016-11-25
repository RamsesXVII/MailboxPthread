//
//  buffer_type.h
//  HWtest
//
//  Created by Mattia Iodice on 22/11/16.
//  Copyright © 2016 Mattia Iodice. All rights reserved.
//

#ifndef buffer_type_h
#define buffer_type_h

#include <stdio.h>
#include "message_type.h"

#define BUFFER_ERROR (msg_t*) NULL



typedef struct buffer{
    int T;
    int D;
    int full;   // variabili che mi servono per il while della condizione
    int empty;  //
    int size;
    msg_t* cells;
    
    pthread_mutex_t T_mutex;
    pthread_mutex_t D_mutex;
    pthread_mutex_t Full_mutex;


    pthread_cond_t notEmpty;
    pthread_cond_t notFull;
    
     struct buffer* (*buffer_init)(unsigned int);
     void (*buffer_destroy)(struct buffer*);

    /* è meglio invocarle sul buffer stesso o no?*/
    msg_t* (*put_bloccante)(struct buffer*, msg_t*);
    msg_t* (*put_non_bloccante)(struct buffer*, msg_t*);
    msg_t* (*get_bloccante)(struct buffer* buffer);
    msg_t* (*get_non_bloccante)(struct buffer* buffer);



}buffer_t;

struct arg_struct{
    buffer_t *buffer;
    msg_t* msg;
};


/* allocazione / deallocazione  buffer */

// creazione di un buffer vuoto di dim. max nota
buffer_t* buffer_init(unsigned int maxsize);

// deallocazione di un buffer
void buffer_destroy(buffer_t* buffer);

/* operazioni sul buffer */
// CHI FA QUESTE OPERAZIONI?


// inserimento bloccante: sospende se pieno, quindi
// effettua l’inserimento non appena si libera dello spazio
// restituisce il messaggio inserito; N.B.: msg!=null
msg_t* put_bloccante(buffer_t* buffer, msg_t* msg);

// inserimento non bloccante: restituisce BUFFER_ERROR se pieno,
// altrimenti effettua l’inserimento e restituisce il messaggio
// inserito; N.B.: msg!=null
msg_t* put_non_bloccante(buffer_t* buffer, msg_t* msg);

// estrazione bloccante: sospende se vuoto, quindi
// restituisce il valore estratto non appena disponibile
msg_t* get_bloccante(buffer_t* buffer);

// estrazione non bloccante: restituisce BUFFER_ERROR se vuoto
// ed il valore estratto in caso contrario
msg_t* get_non_bloccante(buffer_t* buffer);

msg_t* do_put_bloccante(void* arguments);
msg_t* do_put_non_bloccante(void* arguments);

msg_t* do_get_non_bloccante(void* arguments);
msg_t* do_get_bloccante(void* arguments);


#endif /* buffer_type_h */

