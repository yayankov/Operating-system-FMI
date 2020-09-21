#include<stdlib.h>
#include<stdio.h>
#include<err.h>
#include<unistd.h>
#include<string.h>
#include<sys/types.h>
#include<sys/wait.h>
#include <sys/stat.h>
       #include <fcntl.h>


int main(int argc, char** argv)
{
	if( argc != 4) errx(1,"error arguments");

	char* cmd = argv[1];
	char* cmd2 = argv[2];
	
	int fd = open(argv[3],O_CREAT | O_TRUNC | O_WRONLY, S_IRUSR | S_IWUSR);
	if (fd == -1) err(2,"error open file");

	const pid_t child1 = fork();
	if(child1 == -1) err(3,"error fork child1");
	if(child1 == 0){
		if( execlp(cmd,cmd,0) == -1) err(4,"error execlp cmd1");
		exit(0);
	}

	int status;
	wait(&status);
	if(WEXITSTATUS(status) == 0) {
		ssize_t len = strlen(cmd);
		if(write(fd,cmd,len) != len) err(6,"error write first cmd");
		write(fd,"\n",1);
	} else {
		printf("The command %s failed\n",cmd);
	}

	const pid_t child2 = fork();
    if(child2 == -1) err(5,"error fork child1");
    if(child2 == 0){
        if( execlp(cmd2,cmd2,0) == -1) err(4,"error execlp cmd1");
        exit(0);
    }

    int status2;
    wait(&status2);
    if(WEXITSTATUS(status2) == 0) {
        ssize_t len2 = strlen(cmd2);
        if(write(fd,cmd2,len2) != len2) err(6,"error write first cmd");
            write(fd,"\n",2);
	} else {
        printf("The command %s failed\n",cmd2);
    }

	close(fd);
	exit(0);
}
