#include <unistd.h>
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>    // exit
#include <stdbool.h>   // bool

//CLIENT
int main(int argc, char* argv[])
{
    FILE *template = NULL;
    char input[BUFSIZ] = "Public|Jane|Q|Ms.|600|Maple Street|Your Town|Iowa|12345";

    char input2[128];
    char cust_data[9][30];
    int  element = 0;
    char *next;
    char ch;

    char send_message[BUFSIZ];
    strcpy(send_message,"");

    template = fopen( "template.txt", "r" );
    if ( template != NULL )
    {
        // read in the customers until we're done


        // parse the data into separate elements
        next = strtok( input, "|" );
        for ( element=0 ; element<9 && next!=NULL ; element++ )
        {
            strcpy( cust_data[element], next );

            next = strtok( NULL, "|" );
        }



        // generate the output by reading and parsing the template
        // instead of reading it into a buffer it just re-reads the file each time
        rewind( template );

        while ( fgets( input2, 128, template ) != NULL )
        {
            element = 0;
            ch = input2[element++];
            while ( ch != '\n' && element < 128 )
            {
                switch( ch )
                {
                    case '$':
                        ch = input2[element++];
                        if ( ch == '$' ){
                            printf( "$" );
                            strncat(send_message,&ch,1);}
                        else{
                            printf( "%s", cust_data[atoi( &ch )] );
                            strcat(send_message,cust_data[atoi( &ch )]);}
                        break;

                    default:
                        printf( "%c", ch );
                        strncat(send_message,&ch,1);
                        break;
                }

                ch = input2[element++];
            }
            printf( "\n" );
            strcat (send_message,"\n");
        }


        printf("%s\n",send_message);
        fclose( template );
    }

    return 0;
}