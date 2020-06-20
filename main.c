#include <unistd.h>
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "protocol.h"
//CLIENT

const char * mail_merge (char * message){
    FILE *template = NULL;
    template = fopen( "C:\\Users\\harsh\\CLionProjects\\Q2\\template.txt", "r" );

    char * return_message;

    char input[128];
    char cust_data[9][30];
    int  element = 0;
    char *next;
    char ch;

    char send_message[BUFSIZ];
    strcpy(send_message,"");
    int verify = 0;
    if ( template != NULL )
    {
        // read in the customers until we're done


        // parse the data into separate elements
        next = strtok( message, "|" );
        for ( element=0 ; element<9 && next!=NULL ; element++ )
        {
            strcpy( cust_data[element], next );

            next = strtok( NULL, "|" );
            verify ++;
        }

        if(verify != 9){
            strcat(send_message,"Bad Message");
            return_message = send_message;
        }else {

            // generate the output by reading and parsing the template
            // instead of reading it into a buffer it just re-reads the file each time
            rewind(template);

            while (fgets(input, 128, template) != NULL) {
                element = 0;
                ch = input[element++];
                while (ch != '\n' && element < 128) {
                    switch (ch) {
                        case '$':
                            ch = input[element++];
                            if (ch == '$') {
                                strncat(send_message, &ch, 1);
                            } else {
                                strcat(send_message, cust_data[atoi(&ch)]);
                            }
                            break;

                        default:

                            strncat(send_message, &ch, 1);
                            break;
                    }

                    ch = input[element++];
                }

                strcat(send_message, "\n");
            }

            return_message = send_message;
        }
    }
    fclose(template);
    return return_message;
}




int main(int argc, char *argv[]) {
    char message_received[BUFSIZ];
    strcpy(message_received,"ST7Public|Jane|Q|Ms.|600|Maple Street|Your Town|Iowa|12345ET7");

    char decoded_message[BUFSIZ];
    strcpy(decoded_message,"");
    int client_number = get_client_number(message_received);

    strcpy(decoded_message,decode_message(message_received)); // client get
    printf("no:%d %s,",client_number,decoded_message);

    char clientfifo[14];
    snprintf(clientfifo, 14, "./3430client%d", client_number);
    printf("\n%s\n",clientfifo);

    char message_merged[BUFSIZ];
    strcpy(message_merged,mail_merge(decoded_message));
    printf("\n%s\n",message_merged);


    return 0;
}