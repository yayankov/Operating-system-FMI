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
	
	if(argc != 2) errx(1,"error arguments");

	mkfifo("fifo2",0644);
	int fd = open("fifo2",O_RDONLY);

	dup2(fd,0);
	execlp(argv[1],argv[1],0);
	
}
