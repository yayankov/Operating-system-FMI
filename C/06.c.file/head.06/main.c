// komanda head za fail

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <err.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
	
	if (argc != 2) {
		errx(1,"Error number of arguments");
	}
	
	int fd1 = open(argv[1], O_RDONLY);
	if ( fd1 == -1 ) {
		err(2, "Error open");
	}
	
	char c;
	int lines = 0;

	while( read(fd1, &c, 1) ) {
		if (c == '\n') {
			lines++;
		}
		write(1,&c,1);
		if (lines == 10) {
			break;
		}
	}
	
	close(fd1);
	exit(0);

}
