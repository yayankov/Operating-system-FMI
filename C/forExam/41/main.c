#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<stdint.h>
#include<sys/types.h>
#include<fcntl.h>
#include<errno.h>
#include<sys/stat.h>
#include<err.h>

int main(int argc,char** argv) {

	if(argc != 4) errx(1,"Error arguments");
	struct stat st;
	if (stat(argv[2],&st) == -1) err(2,"Error stat file 2");
	if( (st.st_size%sizeof(uint8_t)) != 0) errx(3,"error size of f1.bin");

	int fd = open(argv[2],O_RDONLY);
	if (fd == -1) err(5,"Error open f1.bin");
	
	int fd2 = open(argv[3],O_CREAT | O_TRUNC | O_RDWR, S_IRUSR | S_IWUSR );
	if( fd2 == -1) {	
		const int _errno=errno;
		close(fd);
		errno = _errno;
		err(4,"Error open f2.bin");
	}
	
	uint8_t a;
	ssize_t rd_size;
	while( (rd_size = read(fd,&a,sizeof(a))) == sizeof(a)) {
		if(write(fd2,&a,rd_size) != rd_size) {
			const int _errno = errno;
			close(fd);
			close(fd2);
			errno = _errno;
			err(6,"Error writing in f2.bin");
		}
	}

	int fd3= open(argv[1],O_RDONLY);
	if( fd3 == -1) {
		const int _errno = errno;
        close(fd);
        close(fd2);
        errno = _errno;
        err(7,"Error open patch.bin");
	}

	struct triple {
		uint16_t pos;
		uint8_t old;
		uint8_t new;
	} b;

	
	while( (rd_size = read(fd3,&b,sizeof(b))) == sizeof(b)) {
		if ( lseek(fd2,b.pos,SEEK_SET) == -1) {
			const int _errno = errno;
            close(fd);
            close(fd2);
			close(fd3);
            errno = _errno;
            err(8,"Error lseek");
		}
		uint8_t symbol;
		read(fd2,&symbol,sizeof(symbol));
		if (symbol == b.old) {
			lseek(fd2,-1,SEEK_CUR);
			write(fd2,&b.new,sizeof(b.new));
		}
		else {
			err(9,"Error patch format");
		}
	}

}




















