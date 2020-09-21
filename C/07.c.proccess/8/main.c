#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <err.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char** argv)
{
	if (argc != 2) errx(1,"error arguments");
	
//	if(access(argv[1],W_OK | R_OK) == -1) errx(2,"error file not readable or writable");
	
	int fd=open(argv[1],O_RDWR | O_CREAT | O_TRUNC,S_IRUSR | S_IWUSR);
	if(fd == -1) err(4,"Error open file");
	lseek(fd,0,SEEK_SET);

	const pid_t child= fork();
	if(child == -1) err(3,"error open fork");

	if(child == 0) {
		char* str="foobar\n";
		ssize_t len = strlen(str);
		if(write(fd, str, len) != len){
			const int old_errno = errno;
			close(fd);
			errno = old_errno;
			err(4, "error while writing to file");
		}
			
		close(fd);
		exit(0);
	}

	int status;
	waitpid(child,&status,0);

	if(WEXITSTATUS(status) != 0) err(6,"error status");
	
	fd = open(argv[1],O_RDONLY);
	char c[2];
	ssize_t read_size=-1;

	while((read_size = read(fd, c, sizeof(c))) > 0){

		if( write(1, c, read_size) !=read_size) {
			const int old_errno = errno;
			close(fd);
			errno = old_errno;
			err(9, "error while writing to STDOUT in parent process");
		}
        write(1, " ", 1);
	}
//	write(1, "\n", 1);
	close(fd);
	exit(0);
}
