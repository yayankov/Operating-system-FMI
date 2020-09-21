#include <stdio.h> 
#include <string.h> 
#include <fcntl.h> 
#include <sys/stat.h> 
#include <sys/types.h> 
#include <unistd.h> 
#include <sys/wait.h>
#include<err.h>
#include <stdlib.h>

int main(int argc, char** argv) {

	if( argc!= 2) err(1,"Error arguments");

	mkfifo("fifo",0644);
	int fd = open("fifo",O_RDONLY);

	dup2(fd,0);

	mkfifo("fifo2",0644);
	int fd2 = open("fifo2",O_WRONLY);

	dup2(fd2,1);

	execlp(argv[1],argv[1],0);

//	exit(0);
}
