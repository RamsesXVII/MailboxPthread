#include "message_type.h"

msg_t *msg_init(void *content){
    
    msg_t* new_msg=(msg_t*)malloc(sizeof(msg_t));
    new_msg->content=content;
    
    new_msg->msg_init=msg_init;
    new_msg->msg_destroy=msg_destroy;
    new_msg->msg_copy=msg_copy;
    
    return new_msg;
    
}

void msg_destroy(msg_t *msg){
    free(msg->content); 
    free(msg);
}

msg_t *msg_copy(msg_t* msg){
    
    msg_t* new_msg=msg_init(msg->content);
    return new_msg;
}

void msg_destroy_string(msg_t* msg) {
    free(msg->content);
    free(msg);
}

msg_t* msg_copy_string(msg_t* msg){
    msg_t* new_msg=msg_init_string(msg->content);
    return new_msg;
}

msg_t* msg_init_string(void* content) {

    msg_t* new_msg = (msg_t*)malloc( sizeof(msg_t) );
    char* string = (char*)content;
    char* new_content = (char*)malloc(strlen(string)+1); 
    strcpy(new_content, string);
    new_msg->content     = new_content;
    new_msg->msg_init    = msg_init_string;
    new_msg->msg_destroy = msg_destroy_string;
    new_msg->msg_copy    = msg_copy_string;
    return new_msg;
}



