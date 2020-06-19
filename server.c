#include <unistd.h>
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <signal.h>
//SERVER
FILE *template = NULL;
const char * mail_merge (char * message){
    char * return_message;

    char input[128];
    char cust_data[9][30];
    int  element = 0;
    char *next;
    char ch;

    char send_message[BUFSIZ];
    strcpy(send_message,"");

    if ( template != NULL )
    {
        // read in the customers until we're done


        // parse the data into separate elements
        next = strtok( message, "|" );
        for ( element=0 ; element<9 && next!=NULL ; element++ )
        {
            strcpy( cust_data[element], next );

            next = strtok( NULL, "|" );
        }



        // generate the output by reading and parsing the template
        // instead of reading it into a buffer it just re-reads the file each time
        rewind( template );

        while ( fgets( input, 128, template ) != NULL )
        {
            element = 0;
            ch = input[element++];
            while ( ch != '\n' && element < 128 )
            {
                switch( ch )
                {
                    case '$':
                        ch = input[element++];
                        if ( ch == '$' ){
                            strncat(send_message,&ch,1);}
                        else{
                            strcat(send_message,cust_data[atoi( &ch )]);
                        }
                        break;

                    default:

                        strncat(send_message,&ch,1);
                        break;
                }

                ch = input[element++];
            }

            strcat (send_message,"\n");
        }

        return_message = send_message;
        fclose( template );
    }

    return return_message;
}

void sighup_handler(int num){
    template = fopen( "template.txt", "r" );
}

int main(int argc, char* argv[])
{

    template = fopen( "template.txt", "r" );

    bool terminate = false;
    int fds[2];

    char client_message[2];
    char message_received[BUFSIZ];
    strcpy(message_received,"");
    char message_merged[BUFSIZ];
    strcpy(message_merged,"");

    char * serverfifo = "./3430server";
    char * clientfifo = "./3430client6";

    int result = mkfifo(serverfifo,0666);
    if (result) {
        perror("Unable to create named pipe");
        exit(EXIT_FAILURE);
    }

    fds[0]=open(serverfifo,O_RDONLY);
    //fds[1]=open(clientfifo,O_WRONLY);
    if (fds[0] < 0) {
        perror("Unable to open named pipe");
        exit(EXIT_FAILURE);
    }
    printf("done opening file\n");

    while(!terminate)
    {
        signal(SIGHUP,sighup_handler);
        result = read(fds[0], client_message, 1);
        if (result < 0) {
            perror("ERROR: Error reading from pipe");
            exit(EXIT_FAILURE);
        }
        else if (result == 0){
            // last client disconnected.
            // end of file
            printf("End of file");
            terminate = true;
        } else if ('\a' == client_message[0]) {
            // bell to quit
            terminate = true;
        }
        else if ('\0' == client_message[strlen(client_message)-1]) {
            printf("\n");
            printf("message received: %s\n",message_received);
            printf("After merging : \n%s\n", mail_merge(message_received));
            strcpy(message_received,"");
        }
        else {
            printf("%c", client_message[0]);
            char append = (char) client_message[0];
            strncat(message_received,&append,1);
        }
    }

    unlink(serverfifo);

    return 0;
}