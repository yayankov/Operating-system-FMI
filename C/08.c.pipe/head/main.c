// head
#include <stdlib.h>
#include <stdint.h>
//#include <std.h>
#include <stdio.h>
#include <sys/types.h>
#include <err.h>
       #include <unistd.h>

int main()
{
	int a[2];
	if( pipe(a) == -1 ) err(1,"Error pipe1");
	const pid_t child1 = fork();
	
	if (child1 == -1) err(2, "Error child1");
	if (child1 == 0) {
		close(a[0]);
		if( dup2(a[1],1) == -1) err(3, "Error dup2 in child1");
		if( execlp("cat", "cat", "/etc/passwd",0) == -1 ){
			err(4, "error execlp1");
		}
	}

	close(a[1]);
	if( dup2(a[0],0) == -1) err(5, "error dup2");
	if( execlp("head","head",0) == -1) {
		err(6,"error execlp 2" );
	}

	exit(0);
}
