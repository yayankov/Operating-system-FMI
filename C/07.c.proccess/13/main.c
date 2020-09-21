#include<stdlib.h>
       #include <sys/types.h>
       #include <unistd.h>
#include<sys/wait.h>
#include<err.h>
#include<stdio.h>

int main(int argc, char** argv)
{
	if(argc != 3) errx(1,"error arguments");

	const pid_t child =fork();
	if(child == -1) err(2,"error fork1");
	if(child == 0){
		if(execlp(argv[1],argv[1],0) == -1) err(3,"error execlp1");
	}
	
	const pid_t child2 = fork();
	if(child2 == 0) {
		if(execlp(argv[2],argv[2],0) == -1) err(4,"error execlp2");
	}

    int status;
	pid_t pid = wait(&status);
    
	if(WIFEXITED(status) && WEXITSTATUS(status) == 0) {
		printf("First: %d\n", pid);
		wait(NULL);
		exit(0);
	} else {
		pid = waitpid(-1,&status,0);
		if(WIFEXITED(status) && WEXITSTATUS(status) == 0) {
        	printf("First: %d\n", pid);
			wait(NULL);
        	exit(0);
		} else	printf("-1\n");
	}
	wait(NULL);
	exit(0);
}
