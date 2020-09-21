
#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<err.h>
#include <sys/types.h>
       #include <sys/stat.h>
       #include <fcntl.h>
#include<string.h>
#include <sys/wait.h>

int main() 
{
	int fd = open("tempfile",O_CREAT | O_TRUNC | O_WRONLY,S_IRUSR | S_IWUSR);
	if (fd == -1) err(1,"error open fd");
	close(fd);
	const pid_t child=fork();
	if(child == -1) err(2,"error open fork");

	if(child == 0) {
		int fd = open("tempfile",O_WRONLY | O_APPEND);
		char* str="foo";
		ssize_t len = strlen(str);
		if (write(fd,str,len) != len) err(3,"error writing");
		close(fd);
	}  
	int status;
	waitpid(child, &status, 0);
	if (WEXITSTATUS(status) == 0) {
		fd = open("tempfile",O_WRONLY | O_APPEND);
        char* str="bar";
        ssize_t len = strlen(str);
        if (write(fd,str,len) != len) err(5,"error writing");
		close(fd);
	}	
	
	exit(0);

		//fd = open("tempfile",O_WRONLY | O_APPEND);
        //char* str="bar";
	   // ssize_t len = strlen(str);
     //   if (write(fd,str,len) != len) err(5,"error writing");
	
}
