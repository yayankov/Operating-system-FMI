
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
	
	const pid_t child=fork();
	if(child == -1) err(2,"error open fork");

	if(child == 0) {
		int fd = open("tempfile",O_WRONLY | O_APPEND);
		char* str="foo";
		ssize_t len = strlen(str);
		if (write(fd,str,len) != len) {
			close(fd);
			 err(3,"error writing");
		}
		printf("1");
		exit(0);	
	}
//	wait(NULL);  
//	printf("parent");
	const pid_t child2=fork();
	if(child2 == -1) err(4,"error open fork2");
	
	if (child2 == 0) {
		int fd = open("tempfile",O_WRONLY | O_APPEND);
        char* str="bar";
        ssize_t len = strlen(str);
        if (write(fd,str,len) != len) {
			 close(fd);
			 err(5,"error writing");
		}
		printf("2");
		exit(0);
	}	
//	wait(NULL);
	close(fd);
	exit(0);
}
