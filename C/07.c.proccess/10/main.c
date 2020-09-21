#include<stdlib.h>
#include<stdio.h>
#include<err.h>
#include<unistd.h>
#include <sys/types.h>
       #include <sys/wait.h>


int main(int argc, char** argv)
{
	if(argc != 3) errx(1,"error arguments");
	char* cmd1 = argv[1];
	char* cmd2 = argv[2];
	
	const pid_t child = fork();
	if( child == 0 ) {
	
		if(execlp(cmd1,cmd1,0) == -1) err(2,"error execlp1");
		exit(0);
	}
	int status;
	//waitpid(child,&status,0);
	wait(&status);
	if( WEXITSTATUS(status)  == 0) {
		const pid_t child2 = fork();
    	if( child2 == 0 ) {
        	if(execlp(cmd2,cmd2,0) == -1) err(2,"error execlp1");
	   		write(1,"\n",1);
			exit(0);
		}	
	} else {
		write(1,"42\n",3);
		exit(0);
	}
	wait(NULL);
	exit(0);
}
