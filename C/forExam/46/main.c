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

int main( int argc, char** argv)
{
	if(argc > 2) err(1,"error. wrong number of arguments");
	char* cmd;
	if(argc == 2) {
		cmd = argv[1];
	} else if(argc == 1) {
		cmd = "echo";
	} else {
		errx(5,"error arguments");
	}

	char c;
	char par[1024][1024];
	int i = 0;
	int i2 = 0;
	while( read(0,&c,1) > 0 ) {
	
		if(c == '\n' || c == ' '  ) {
			i++;
			par[i][i2] = '\0';
			i2 = 0;
			continue;
		}
		if(i2 > 3) {
			err(6,"error not 4 elements");
        }
		par[i][i2] = c;
		i2++;
	}	

    for(int j=0; j < i; j+=2) {

		const pid_t child=fork();
		if(child == -1) err(3,"error fork child");
	
		if(child == 0) {
			if(j+1 >= i) {
				if(execlp(cmd,cmd,par[j],0) == -1) err(7,"error exec"); 

			} else {
				 if(execlp(cmd,cmd,par[j],par[j+1],0) == -1) err(7,"error exec");				
			}
		}
		wait(NULL);
	}
}
