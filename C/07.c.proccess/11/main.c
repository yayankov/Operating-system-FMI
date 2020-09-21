#include<stdlib.h>
#include<stdio.h>
#include<err.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>

void cmd_exec(char* cmd,int no) {
	const pid_t child = fork();
	if(child == -1) err(no,"error fork");
	if(child == 0) {
		if(execlp(cmd,cmd,0) == -1) err(no,"error execlp");	
		exit(0);
	}
}

int main(int argc, char** argv)
{
	if( argc < 2) errx(1,"error arguments");
	int success = 0;
	int unsuccess = 0;
	for(int i=1; i<argc; i++) {
		cmd_exec(argv[i],i);

		int status;
		wait(&status);

		if(WEXITSTATUS(status) == 0) {
			success++;
		} else {
			unsuccess++;
		}
	}
	printf("Success: %d, Unsuccess: %d\n",success,unsuccess);
	exit(0);
}
