#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdint.h>
#include <err.h>
#include <errno.h>
#include <stdlib.h>

int main(int argc, char** argv)
{
	if(argc != 4) errx(1,"error arguments");
	int f1 = open(argv[1],O_RDONLY);
	int f2 = open(argv[2],O_RDONLY);
	int fd = open(argv[3],O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
	
	struct triple {
		uint16_t begin;
		uint8_t old;
		uint8_t new;
	};
	
	uint8_t s1;
	uint8_t s2;	

	struct triple a;
	a.begin = 0;

	while( (read(f1,&s1,sizeof(s1)) == sizeof(s1)) && (read(f2,&s2,sizeof(s2)) == sizeof(s2)) ) {
		if(s2 != s1) {
	  
			a.old = s1;
			a.new = s2;
			write(fd,&a,sizeof(a));
		}
		a.begin++;
	}

	exit(0);
}
