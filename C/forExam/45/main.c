#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
       #include <unistd.h>
#include <sys/types.h>
       #include <sys/stat.h>
       #include <fcntl.h>

#include<err.h>


 //cat /etc/passwd | awk -F: '{print $7}' | sort | uniq -c | sort -n

int main()
{
	int a[2];
	if(pipe(a) < 0) err(1,"error pipe");
	
	const pid_t child = fork();
	if(child < 0) err(2,"error fork");
	if(child == 0) {
		close(a[0]);
		if(dup2(a[1],1) == -1) err(3,"error dup2");
		if(execlp("cat","cat","/etc/passwd",0) < 0) err(20,"error execlp");
	}
	close(a[1]);
	
	int b[2];
	if(pipe(b) < 0) err(1,"error pipe");

    const pid_t child2 = fork();
    if(child2 < 0) err(2,"error fork");
    if(child2 == 0) {
        if(dup2(a[0],0) == -1) err(6,"error dup2 0");
		close(b[0]);
        if(dup2(b[1],1) == -1) err(7,"error dup2 1");
        if(execlp("cut","cut","-d:","-f7",0) < 0) err(8,"error execlp");
    }   
    close(b[1]);

	int c[2];
    if( pipe(c) < 0) err(1,"error pipe");

    const pid_t child3 = fork();
    if(child3 < 0) err(2,"error fork");
    if(child3 == 0) {
        if(dup2(b[0],0) == -1) err(9,"error dup2 ");
        close(c[0]);
        if(dup2(c[1],1) == -1) err(10,"error dup2 ");
        if(execlp("sort","sort",0) < 0) err(11,"error execlp");
    }   
    close(c[1]);

	int d[2];
	if( pipe(d) < 0 ) err(1,"error pipe");
	
	const pid_t child4 = fork();
	if(child4 == -1) err(12,"error child4");
	if(child4 == 0) {
		if( dup2(c[0],0) == -1 ) err(13,"error dup2");
		close(d[0]);
		if( dup2(d[1],1) == -1) err(14,"error dup2");
		if(execlp("uniq","uniq","-c",(char *)NULL) == -1) err(15,"error execlp");
	}
	close(d[1]);
	
	if(dup2(d[0],0)== -1) err(16,"error dup2");
	if(execlp("sort","sort","-n",(char*)NULL) == -1) err(17,"error execlp");
	
}









