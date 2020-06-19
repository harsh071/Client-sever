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
    bool done = false;
    bool killserver = false;
    char* result;
    int errno;
    char server_message[BUFSIZ];

    int fds[2];
    char * serverfifo = "./3430server";
    char clientfifo[14];

//    /* Get the client number and make a client fifo */
//    int option;
//    while((option = getopt(argc,argv,"c:") != -1)){
//        snprintf(clientfifo,14,"./3430client%s",optarg);
//        printf("client fifo: %s\n",clientfifo);
//    }

    /* making a client FIFO */
    mkfifo(clientfifo, 0666);

    fds[0] = open(serverfifo, O_WRONLY);
    fds[1] = open(clientfifo, O_RDONLY);
//    if (fds[1] < 0) {
//        perror("Unable to open named pipe");
//        exit(EXIT_FAILURE);
//    }

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