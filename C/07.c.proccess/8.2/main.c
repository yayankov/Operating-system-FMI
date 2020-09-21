#include <stdlib.h>
#include <err.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int main(int argc, char **argv){
	if(argc!=2){
		errx(1, "Invalid number of arguments. Usage: %s <filename>", argv[0]);
	}

	const char *file = argv[1];
	
	ssize_t fd = open(file, O_RDWR|O_CREAT|O_TRUNC, S_IRUSR|S_IWUSR|S_IRGRP);
	if(fd == -1){
		err(2, "could not open file %s", file);
	}

	const pid_t child_pid = fork();
	if(child_pid == -1){
		err(3, "could not fork process");
	}

	if(child_pid == 0){
		// we are in child process		
		const char *text = "foobar\n";
		const ssize_t len = strlen(text);		

		if(write(fd, text, len) != len){
			const int old_errno = errno;
			close(fd);
			errno = old_errno;
			err(4, "error while writing to file %s", file);
		}

		close(fd);
		exit(0);
	}

	int status;
	const pid_t wait_code = wait(&status);

	if(wait_code == -1){
		int old_errno = errno;
		close(fd);
		errno = old_errno;
		err(5, "could not wait child process");
	}

	if(!WIFEXITED(status)){
		const int old_errno = errno;
		close(fd);
		errno = old_errno;
		err(6, "child did not terminate normally");
	}

	if(WEXITSTATUS(status) != 0){
		err(7, "child exit code not 0, fd should be closed");
	}
	
	fd = open(file, O_RDONLY);
	if(fd == -1){
		const int old_errno = errno;
		close(fd);
		errno = old_errno;
		err(8,"errow while opening file %s", file);
	}

	char c[2];
	ssize_t read_size = -1;
	while((read_size = read(fd, c, sizeof(c))) > 0){
		write(1, c, read_size);
		write(1, " ", 1);
	}
	if(read_size == -1){
		const int old_errno = errno;
		close(fd);
		errno = old_errno;
		err(9, "error while writing to STDOUT in parent process");
	}
	write(1, "\n", 1);

	close(fd);
	exit(0);
}
