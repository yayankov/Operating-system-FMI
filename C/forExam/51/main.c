#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<stdint.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<errno.h>
#include<err.h>

int main() {
	
	int a[2];
	if (pipe(a) == -1) err(1,"error pipe");
	const pid_t child1 = fork();
	if (child1 == -1) err(2,"error fork");

	if (child1 == 0) {
		close(a[0]);
		dup2(a[1],1);
		if (execlp("cat","cat","/etc/passwd",(char*)0) == -1) err(3,"error exec");
	}
	close(a[1]);
	
	int b[2];
    if (pipe(b) == -1) err(1,"error pipe");
    const pid_t child2 = fork();
    if (child2 == -1) err(2,"error fork");

    if (child2 == 0) {
		dup2(a[0],0);
        close(b[0]);
        dup2(b[1],1);
        if (execlp("cut","cut","-d",":","-f7",(char*)0) == -1) err(3,"error exec");
    }
	close(b[1]);

	int c[2];
    if (pipe(c) == -1) err(1,"error pipe");
    const pid_t child3 = fork();
    if (child3 == -1) err(2,"error fork");

    if (child3 == 0) {
        dup2(b[0],0);
        close(c[0]);
        dup2(c[1],1);
        if (execlp("sort","sort",(char*)0) == -1) err(3,"error exec");
    }

    close(c[1]);

    int d[2];
    if (pipe(d) == -1) err(1,"error pipe");
    const pid_t child4 = fork();
    if (child4 == -1) err(2,"error fork");

    if (child4 == 0) {
        dup2(c[0],0);
        close(d[0]);
        dup2(d[1],1);
        if (execlp("uniq","uniq","-c",(char*)0) == -1) err(3,"error exec");
    }
	
	close(d[1]);
	dup2(d[0],0);
	if(execlp("sort","sort","-n",(char*)0) == -1) err(4,"error exec");

	exit(0);
}




















