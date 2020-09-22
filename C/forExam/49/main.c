#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<stdint.h>
#include<errno.h>
#include<err.h>
#include<sys/stat.h>
#include<sys/types.h>

int main(int argc, char** argv) {
	if(argc != 2) errx(1,"Error arguments");

	int a[2];
	if( pipe(a) == -1) err(2,"Error pipe");
	const pid_t child = fork();
	if( child == -1) err(3,"Error child");
	
	if( child == 0) {
		close(a[0]);
		if (dup2(a[1],1) == -1) err(6,"Error dup2 in child");
		if (execlp("cat","cat",argv[1],0) == -1) err(4,"error exec in child");
	}

	close(a[1]);
	if (dup2(a[0],0) == -1) err(5,"Error dup2 in parent");
	if (execlp("sort","sort",0)	== -1) err(6,"error exec in parent");

	exit(0);
}
