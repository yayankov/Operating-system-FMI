// find . - maxdepth 2 -type f -printf "%f %T@\n"  | sort -r -k 2 | head -n 1| cut -d ' ' -f1
#include <stdlib.h>
#include <err.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>

int main(int argc, char** argv)
{
	if(argc != 2) errx(1,"error arguments");
	char* directory = argv[1];
	int a[2];
	if(pipe(a) == -1) err(2,"error pipe");
	const pid_t child = fork();
	if(child == -1) err(3,"error fork");
	if(child == 0) {
		close(a[0]);
		if(dup2(a[1],1) == -1) err(4,"error dup2");
		if(execlp("find","find",directory, "-type","f", "-printf","%T@:%p\n",0) == -1) err(5,"error execlp");
	}
	close(a[1]);

	int b[2];
    if(pipe(b) == -1) err(2,"error pipe");
    const pid_t child2 = fork();
    if(child2 == -1) err(3,"error fork");
    if(child2 == 0) {
        dup2(a[0],0);
		close(b[0]);
        if(dup2(b[1],1) == -1) err(4,"error dup2");
        if(execlp("sort","sort","-n",0) == -1) err(5,"error execlp");
    }
	close(a[0]);
    close(b[1]);

	int c[2];
    if(pipe(c) == -1) err(2,"error pipe");
    const pid_t child3 = fork();
    if(child3 == -1) err(3,"error fork");
    if(child3 == 0) {
    	dup2(b[0],0);
	    close(c[0]);
        if(dup2(c[1],1) == -1) err(4,"error dup2");
        if(execlp("head","head","-n1",0) == -1) err(5,"error execlp");
    }
    close(c[1]);
	close(b[0]);
	 if(dup2(c[0],0) == -1) err(4,"error dup2");
     if(execlp("cut","cut","-d:","-f2",0) == -1) err(5,"error execlp");

	exit(0);
}

