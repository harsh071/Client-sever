//
// Created by harsh on 2020-06-19.
//

#ifndef Q2_PROTOCOL_H
#define Q2_PROTOCOL_H

const char * process_client_message(char * message,int number);
const char * process_server_message(char * message);
const char * decode_message(char * message);
int get_client_number(char * message);


#endif //Q2_PROTOCOL_H
