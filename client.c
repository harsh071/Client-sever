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
    bool done,terminate = false;
    bool killserver = false;
    char* result;
    int errno,ret;
    char server_message[BUFSIZ];
    char client_message[BUFSIZ];

    char message_received[BUFSIZ];
    strcpy(message_received,"");

    int fds[2];
    char * serverfifo = "./3430server";
    char clientfifo[14];

    /* Get the client number and make a client fifo */
    int option;
    while((option = getopt(argc,argv,"c:") != -1)){
        snprintf(clientfifo,14,"./3430client%s",optarg);
    }

    /* making a client FIFO */
    int fifo = mkfifo(clientfifo, 0666);

    fds[0] = open(serverfifo, O_WRONLY);
    fds[1] = open(clientfifo, O_RDONLY);
    if (fds[1] < 0) {
        perror("Unable to open named pipe");
        exit(EXIT_FAILURE);
    }

    while(!done){
        // input from stdin
        result = fgets((char*)&server_message,BUFSIZ,stdin);
        // TODO: Do string processing

        if (result != NULL) {
            // send this character to the FIFO
            // sends with \n, replace with \0
            server_message[strlen(server_message)-1] = '\0';
            if (strcmp(server_message, "exit") == 0)
            {
                done = true;
                killserver = true;
            }
            else
            {
                printf("sending %s\n", server_message);
                errno = write(fds[0], &server_message, strlen(server_message)+1);

                if (errno < 0) {
                    perror("ERROR: Error writing to pipe");
                }
                terminate = false;

                while(!terminate)
                {
                    ret = read(fds[1], client_message, 1);
                    if (ret < 0) {
                        perror("ERROR: Error reading from server");
                        exit(EXIT_FAILURE);
                    }
                    else if (ret == 0){
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
                        terminate = true;
                    }
                    else {
                        //printf("%c", client_message[0]);
                        char append = (char) client_message[0];
                        strncat(message_received,&append,1);
                    }
                }
            }
        }
        else {
            done = true;

        }





//        write(fds[1], input, BUFSIZ); // add to file descriptor
//        perror("Write:"); //Very crude error check
//
//        while (read(fds[0], server_message, BUFSIZ)==1){ // read message from server
//            perror("Read:"); // Very crude error check
//            printf("Server returned this: %s\n",server_message);
//        }

    }
    /* read from Standard input and store it in the fd */

//    unlink(clientfifo);
//    close(fds[0]);
//    close(fds[1]);

    if (killserver)
        write(fds[0], "\a", 1);
    return 0;
}