#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>


int main()
{
    int fd[2];
    char messages[2][20] = {"enkokileshe", "Meneawkelehe"};
    char readmessage[20];

    if (pipe(fd) == -1)
    {
        printf("Unable to create pipe \n");
        return 1;
    }

    for (int i = 0; i < 3; i++)
    {
        if (fork() != 0)
        {                       // Parent process
            printf("In Parent: Writing to pipe – Message is %s\n", messages[0]);
            write(fd[1], messages[0], sizeof(messages[0]));
            wait(NULL);
            read(fd[0], readmessage, sizeof(readmessage));
            printf("In Parent: Reading from pipe – Message is %s\n", readmessage);
        }
        else
        {                       //child process
            read(fd[0], readmessage, sizeof(readmessage));
            printf("In Child: Reading from pipe – Message is %s\n", readmessage);
            printf("In Child: Writing to pipe – Message is %s\n", messages[1]);
            write(fd[1], messages[1], sizeof(messages[1]));
            exit(0);
        }
    }

    return 0;
}