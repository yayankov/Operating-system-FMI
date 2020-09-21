#include <sys/stat.h>
       #include <fcntl.h>
#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<err.h>

int main(int argc, char** argv)
{
	if(argc != 2) err(8,"error arguments");

	const ssize_t fd = open(argv[1],O_RDONLY);
	if(fd == -1) err(9,"error. could not open file");
	close(fd);

	int a[2];
	if(pipe(a) == -1) err(1,"error pipe");
	const pid_t child = fork();
	if(child == -1) err(2,"error fork");
	if(child == 0) {
		close(a[0]);
		if(dup2(a[1],1) == -1) err(4,"error dup2");
		if(execlp("cat","cat",argv[1],0) == -1) err(3,"error execlp");
	}
	close(a[1]);
	
	int b[2];
	pipe(b);
	const pid_t child2 = fork();
	if(child2 == 0) {
		close(b[0]);
		dup2(a[0],0);
		dup2(b[1],1);
		if(execlp("uniq","uniq",0) == -1) err(9,"error execlp");
	}
	close(b[1]);
	close(a[0]);
	
	if(dup2(b[0],0) == -1) err(6,"error dup2");
	if(execlp("sort","sort",0) == -1) err(7,"error sort");
	exit(0);
}















