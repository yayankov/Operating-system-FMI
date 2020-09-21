#include<stdio.h>
#include<stdlib.h>
#include<err.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

int main()
{
	const pid_t child=fork();
	if(child < 0) err(1,"Error fork");
	int status;
	
	if(child == 0) {
	
		printf("child\n");

	} else {
		wait(&status);
		printf("1parent\n");
		printf("2parent\n");
	}
	wait(NULL);
	exit(0);
}
