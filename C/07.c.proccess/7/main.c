#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <err.h>

void exec_cmd(char* cmd_name) {
	
	const pid_t child=fork();
	if(child == 0) {
		if(execlp(cmd_name,cmd_name,0) == -1) {
			err(2,"Error exec");
		}
	}
	int status;
	waitpid(child,&status,0);

	printf("pid: %d, exit code: %d\n",child,WEXITSTATUS(status));

}

int main(int argc, char** argv)
{
	if(argc != 4) errx(1,"error arguments");
	
	for(int i=1; i<=3; i++) {
		exec_cmd(argv[i]);
	}
}

