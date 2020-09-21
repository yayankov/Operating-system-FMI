// Kopiratite sudurjanieto ot file1 v file2

#include <stdio.h>
#include <err.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char* argv[]) 
{
	if (argc != 3) {
		errx(1, "The arguments should be 3");
	}
	
	int fd1 = open(argv[1], O_RDONLY);
	if (fd1 == -1) {
		err(2, "Error open");
	}
	
	int fd2 = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    if (fd1 == -1) {
		close(fd1);
        err(3, "Error open");
    }

	char c;

	while( read(fd1, &c, 1)) {
		write(fd2, &c, 1);
	}
	
	close(fd1);
	close(fd2);
	exit(0);
	
}
