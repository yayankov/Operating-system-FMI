
// cat /etc/passwd | cut -d ':' -f 7 | sort | uniq
//                 A      B
#include <stdlib.h>
#include <err.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main()
{
	int a[2];
	if(pipe(a) == -1) {
		err(3,"error pipe1");
	}
	const pid_t child1 = fork();
	if(child1 == -1) {
		err(4, "error fork");
	}
	
	if(child1 == 0) {
		//child write 
		
		close(a[0]);
		dup2(a[1],1);
	    if( execlp("cat","cat", "/etc/passwd",0) == -1) {
    	    err(2,"Error execlp");
	    }
	
	}	

	close(a[1]);
	
	int a2[2];
	if(pipe(a2) == -1) {
        err(5,"error pipe1");
    }
    const pid_t child2 = fork();
    if(child2 == -1) {
        err(1, "error fork");
    }
	
	if( child2 == 0 ) {
		dup2(a[0],0);
		close(a2[0]);
		dup2(a2[1],1);
		if( execlp("cut","cut","-d:","-f7",0) == -1) {
			err(6,"error exelp cut");
		}
	}
	close(a2[1]);

	int a3[2];
	if(pipe(a3) == -1) {
		err(7,"error pipe 3");
	}
	const pid_t child3 = fork();
	if(child3 == -1) {
		err(8,"error child 3");
	}
	if(child3 == 0) {
		dup2(a2[0],0);
		close(a3[0]);
		dup2(a3[1],1);
		if( execlp("sort","sort",(char*)0) == -1) {
            err(9,"error exelp sort");
        }
	}
	close(a3[1]);
	
	dup2(a3[0],0);
	if( execlp("uniq","uniq",(char*)0) == -1) {
   		 err(10,"error exelp uniq");
    }
}
