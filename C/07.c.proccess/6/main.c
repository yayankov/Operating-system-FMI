
#include<stdlib.h>
#include<err.h>
#include<unistd.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char** argv)
{
	if(argc != 2) errx(1,"error arguments");

	const pid_t child = fork();
	if (child == 0) {
		if(execlp(argv[1],argv[1],0) == -1 ) err(2,"error execlp");
	}
	int status;
	waitpid(child,&status,0);
	if(WEXITSTATUS(status) == 0) {
	//	wait(NULL);
		printf("%s\n",argv[1]);
	}
	exit(0);
}	
