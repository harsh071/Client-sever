#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "protocol.h"

//
// Created by harsh on 2020-06-17.
// DO THE STx ETx and STS and ETS here.
// 	Public|Jane|Q|Ms.|600|Maple Street|Your Town|Iowa|12345 to the server.
//  Processed after template to the client.
//

const char * decode_message(char * message ){
    int i;
    bool done = false;
    char ret[BUFSIZ]; // value to return
    strcpy(ret, "");
    char * return_message;
    i = 0;

    while (i < strlen(message) && !done){
        if(message[i] == 'S'){
            if(message[i+1] == 'T'){
                i++;
                i++;
                i++;

                while (message[i] != 'E' || message[i+1] != 'T'){
                    strncat(ret,&message[i],1);
                    i++;

                } // while

                if(message[i]=='E'&&message[i+1]=='T'){
                }else{
                    printf("bad message\n");
                }
            } // T
            else{
                i++;
            }
        } // S
        else{
            i++;
        }
    }
    return_message = ret;
    return return_message;
}

int get_client_number(char *message) {
    int i;
    bool done = false;
    int return_message = -1;
    i = 0;

    while (i < strlen(message) && !done) {
        if (message[i] == 'S') {
            if (message[i + 1] == 'T') {
                i++;
                i++;
                return_message = message[i] - '0';
            } else{
                i++;
            }
        } else{
            i++;
        }
    }

    return return_message;
}


const char * process_client_message(char * message,int number){
    int haha = number;
    char ret[BUFSIZ];
    char * processed_message;
    sprintf(ret,"ST%d%sET%d",number,message,number);
    processed_message = ret;
    return processed_message;
}

const char * process_server_message(char * message){
    char ret[BUFSIZ];
    char * processed_message;
    sprintf(ret,"STS%sETS",message);
    processed_message = ret;
    return processed_message;
}

