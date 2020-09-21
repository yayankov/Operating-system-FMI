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
	
	if ( argc != 2) err(1,"Error arguments");
	
	mkfifo("fifo",0644);
	int fd = open("fifo", O_WRONLY);
	if( fd == -1) err(2,"Error open");

	dup2(fd,1);
	execlp("cat","cat",argv[1],0);

}
