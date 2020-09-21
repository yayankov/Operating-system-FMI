#include <unistd.h>
#include <stdlib.h>
#include <err.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>

int main()
{
	while(1) {
		if( write(1,"<Input command> ",16) < 0) {
			err(1,"Error prompt");
		}
		
		char buf[1024];
		ssize_t rd;
		if(( rd = read(0,&buf,sizeof(buf))) < 0 ) {
			err(2,"error read from stdin");
		}
		buf[rd-1] = '\0';

		if(strcmp(buf,"exit") == 0) {
			break;
		}

		pid_t child = fork();
		if(child == -1) err(3,"error fork");
		if(child == 0) {
			if( execlp(buf,buf,0) < 0 ) {
				warn("Not correct comand");
			}
		}
		wait(NULL);
	}


	exit(0);
}


/*
int main() {
	while (1) {
		if (write(1, "Enter command> ", 15) <= 0) {
			err(1, "Error writing to stdout");
		}

		char buff[1024];
		ssize_t bytes_read = read(0, &buff, sizeof(buff));
		if (bytes_read < 0) {
			err(2, "Error reading from stdin!");
		}
		// Change newline to terminating zero
		buff[bytes_read - 1] = '\0';

		if (strcmp(buff, "exit") == 0) {
			break;
		}
		
		pid_t pid = fork();
		if (pid < 0) {
			warn("Error forking process");
			continue;
		}
		else if (pid == 0) {
			if (execlp(buff, buff, (char*)NULL) < 0) {
				err(1,"Error executing command %s", buff);
//				continue;
			}
		}

		wait(NULL);
	}

	exit(0);
}*/
/*
#include <sys/types.h>
       #include <unistd.h>


int main()
{	
	exit(0);
	const pid_t child = fork();
	if( fork\
}*/
