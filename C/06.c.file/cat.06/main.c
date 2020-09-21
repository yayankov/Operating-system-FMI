// komanda cat

#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#include <err.h>

int main(int argc, char* argv[])
{
	if (argc < 2) {
		errx(1, "Wrong number of arguments");
	}
	
	for(int i=1; i < argc; i++) {
		
		int fd = open(argv[i], O_RDONLY);
		if ( fd == -1 ) {
			printf("permissions denied: %s\n", argv[i]);
			continue;
		}
		char c[4096];	
		ssize_t read_size; 

		while( (read_size = read(fd,&c,sizeof(c))) > 0) {
			if ( write(1,&c,read_size) != read_size) {
				close(fd);
				err(3,"Error writing");	
			}
		}
		close(fd);		
	}
	exit(0);
}
