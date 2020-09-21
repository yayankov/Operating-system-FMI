#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdint.h>
#include <err.h>
#include <errno.h>
#include <string.h>

int main(int argc, char** argv)
{
	if(argc != 4) errx(1,"error arguments");
	
	struct triple {
		uint16_t begin;
		uint8_t old;
		uint8_t new;
	};
	struct triple a;

	char* patch = argv[1];
	char* file1 = argv[2];
	char* file2 = argv[3];
	
	int fd = open(patch,O_RDWR);
	int f1 = open(file1,O_RDWR);
	int f2 = open(file2,O_CREAT | O_RDWR | O_TRUNC, S_IRUSR | S_IWUSR);
	
	uint8_t c;
		
		while( read(f1,&c,sizeof(c)) == sizeof(c) ) {
	
				write(f2, &c, sizeof(c));
		}
	
    while( read(fd,&a,sizeof(a)) == sizeof(a) )  {
		lseek(f2,a.begin,SEEK_SET);
		read(f2,&c,sizeof(c));
		if(c == a.old) {
			lseek(f2,-1,SEEK_CUR);
			write(f2,&(a.new),sizeof(a.new));
		} else {
			err(4,"this is no such byte");
		}
	}
		
	exit(0);
}
