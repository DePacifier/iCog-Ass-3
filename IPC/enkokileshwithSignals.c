#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <wait.h>
#include <signal.h>
#include <stdlib.h>

pid_t pid;
int counter = 0;
int fd[2];
char messages[2][20] = {">> Enkokelehe\n", "<< Meneawkelehe\n"};
char readmessage[20];

void parentHandle()
{
	if (counter > 0)
	{
		read(fd[0], readmessage, sizeof(readmessage));
		printf("Message Recieved: %s", readmessage);
	}
	printf("%s", messages[0]);
	printf("Writing Message : %s", messages[0]);
	write(fd[1], messages[0], sizeof(messages[0]));
	puts("___________________________________________");
	counter++;
	kill(pid, SIGINT);
}

void childHandle()
{
	read(fd[0], readmessage, sizeof(readmessage));
	printf("Message Recieved: %s", readmessage);
	printf("%s", messages[1]);
	printf("Writing Message : %s", messages[1]);
	write(fd[1], messages[1], sizeof(messages[1]));
	puts("___________________________________________");
	counter++;
	if (counter < 3)
	{
		kill(getppid(), SIGINT);
	}
	else
	{
		exit(0);
	}
}

int main()
{
	pid_t p;
	int status;

	if (pipe(fd) == -1)
	{
		printf("Unable to create pipe \n");
		return 1;
	}

	signal(SIGINT, parentHandle);
	if ((pid = fork()) == 0)
	{
		signal(SIGINT, childHandle);
		kill(getppid(), SIGINT);
		while (1)
			;
	}
	if ((p = wait(&status)) > 0)
	{
		puts("Conversation Ends");
	}
}
