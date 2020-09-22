#include<stdlib.h>
#include<stdio.h>
#include<stdint.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<err.h>
#include<errno.h>
#include<string.h>

int main(int argc,char** argv) {

	if(argc != 4) errx(1,"error arguments");

	struct stat st1;
	struct stat st2;

	if (stat(argv[1],&st1) == -1) err(2,"Error stat");
	if (stat(argv[2],&st2) == -1) err(3,"Error stat");

	if( st1.st_size%sizeof(uint8_t) != 0) errx(4,"Error size file 1");
	if( st2.st_size%sizeof(uint8_t) != 0) errx(5,"Error size file 2");

	int fd1 = open(argv[1],O_RDONLY);
	if( fd1 == -1 ) err(6,"Error open argument1");
	int fd2 = open(argv[2],O_RDONLY);
	if( fd2 == -1) {
		const int _errno = errno;
		close(fd1);
		errno = _errno;
		err(7,"Error open argument 2");
	}
	int fd3 = open(argv[3], O_CREAT | O_TRUNC | O_WRONLY, S_IRUSR | S_IWUSR);
	if( fd3 == -1) {
		const int _errno = errno;
        close(fd1);
		close(fd2);
        errno = _errno;
        err(8,"Error open argument 3");
	}
		
	struct triple {
		uint16_t offset;
		uint8_t original;
		uint8_t new;
	} a;

	uint8_t b,c;
	uint16_t cnt = 0;
	while( read(fd1,&b,sizeof(b)) == sizeof(b) && read(fd2,&c,sizeof(c)) == sizeof(c)) {

		if( b != c) {
			a.offset = cnt;
			a.original = b;
			a.new = c;
			write(fd3,&a,sizeof(a));
		}
		cnt++;
	}
	close(fd1);
	close(fd2);
	close(fd3);
	exit(0); 
}




















