// C program to implement sighup(), sigint() 
// and sigquit() signal functions 
#include <signal.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <sys/types.h> 
#include <unistd.h> 
  
// function declaration 
void sighup(); 
void sigint(); 


int fd[2], fd1[2], pid;
char responder[2][13] = {"Meneawkelehe","Bet"};
char asker[3][28] = {"Enkokileshe", "Zoro Zoro megbiyaw cheraro?", "Melsu -> Asphalt"};
char readmessage[28];  

// driver code 

    void main() 
{
    for(int i = 0; i < 3; i++){ 
        int pid; 
        if (pipe(fd) == -1)
        {
            printf("Unable to create pipe \n");
            exit(1);
        }
        if(pipe(fd1) == -1){
            printf("Unable to create pipe\n");
            exit(1);
        }
    
        /* get child process */
        if ((pid = fork()) < 0) { 
            perror("fork"); 
            exit(1); 
        } 
    
        if (pid == 0) { /* child */
            signal(SIGHUP, sighup); 
            signal(SIGINT, sigint);
            for (;;) 
                ; /* loop for ever */
        } 
    
        else /* parent */
        { /* pid hold id of child */
            printf("In Parent: Writing to pipe – Message is %s\n", asker[0]);    // Said Enkokelesh
            write(fd[1], asker[0], sizeof(asker[0]));
            kill(pid, SIGHUP); 
    
            sleep(3); /* pause for 3 secs */
            read(fd1[0], readmessage, sizeof(readmessage));
            printf("In Parent: Reading from pipe – Message is %s\n", readmessage);
            printf("In Parent: Writing to pipe - Message is %s\n", asker[1]);
            write(fd[1], asker[1], sizeof(asker[1])); 
            kill(pid, SIGINT); 
    
            sleep(3); /* pause for 3 secs */
            read(fd1[0], readmessage, sizeof(readmessage));
            printf("In Parent: Reading from pipe - Message is %s\n", readmessage);
            printf("In Parent: Writing to pipe - Message is %s\n", asker[2]);
            write(fd[1], asker[2], sizeof(asker[2]));
            wait(NULL);
            
        } 
    } 
}

  
// sighup() function definition 
void sighup() 
  
{ 
    signal(SIGHUP, sighup); /* reset signal */
    read(fd[0], readmessage, sizeof(readmessage));
    printf("In Child: Reading from pipe – Message is %s\n", readmessage);
    printf("In Child: Writing to pipe – Message is %s\n", responder[0]);
    write(fd1[1], responder[0], sizeof(responder[0]));
} 
  
// sigint() function definition 
void sigint() 
  
{ 
    signal(SIGINT, sigint); /* reset signal */
    read(fd[0], readmessage, sizeof(readmessage));
    printf("In Child: Reading from pipe - Message is %s\n", readmessage);
    printf("In Child: Writing to pipe - Message is %s\n", responder[1]);
    write(fd1[1], responder[1], sizeof(responder[1]));
} 
