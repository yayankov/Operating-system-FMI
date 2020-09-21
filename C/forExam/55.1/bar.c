#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
       #include <unistd.h>
#include <sys/types.h>
       #include <sys/stat.h>
       #include <fcntl.h>
#include<string.h>
#include<err.h>
#include<sys/wait.h>

int main(int argc, char** argv) {
	
	if (argc != 2) errx(1,"error arguments");
	
	mkfifo("fifo",0644);
	int fd_fifo = open("fifo",O_RDONLY);
	
	dup2(fd_fifo,0);
	if (execlp(argv[1],argv[1],(char*)0) == -1) err(2,"error exec");	

}
