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
	if (argc != 2) errx(1,"Error arguments");
	
	int a[2];
	if (pipe(a) == -1) err(2,"Error pipe");
	const pid_t child1 = fork();
	if (child1 == -1) err(3,"Error fork");
	if (child1 == 0) {
		close(a[0]);
		dup2(a[1],1);
		if (execlp("find","find",argv[1],"-type","f","-printf","%Ts %f\n",(char*)0) == -1) err(4,"Error exec");
	}
	
	close(a[1]);
	int b[2];
    if (pipe(b) == -1) err(2,"Error pipe");
    const pid_t child2 = fork();
    if (child2 == -1) err(3,"Error fork");
    if (child2 == 0) {
        dup2(a[0],0);
		close(b[0]);
        dup2(b[1],1);
        if (execlp("sort","sort","-rn",(char*)0) == -1) err(4,"Error exec");
    }
	
    close(b[1]);
    int c[2];
    if (pipe(c) == -1) err(2,"Error pipe");
    const pid_t child3 = fork();
    if (child3 == -1) err(3,"Error fork");
    if (child3 == 0) {
        dup2(b[0],0);
        close(c[0]);
        dup2(c[1],1);
        if (execlp("head","head","-n","1",(char*)0) == -1) err(4,"Error exec");
    }

    close(c[1]);
    dup2(c[0],0);

    if (execlp("cut","cut","-d"," ","-f2",(char*)0) == -1) err(4,"Error exec");
    
}













