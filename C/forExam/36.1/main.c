#include<stdlib.h>
#include<stdio.h>
#include<stdint.h>
#include<err.h>
#include<sys/stat.h>
#include<unistd.h>
#include<sys/types.h>
#include<fcntl.h>
#include<errno.h>

int main(int argc, char** argv) {

	if (argc != 4) errx(1,"Wrong number of arguments");
	struct stat st;
	if ( stat(argv[1],&st) == -1) err(2,"Error with stat file 1");
	if ( st.st_size%sizeof(uint32_t) != 0 ) errx(3,"Error file 1 is not uint32_t");

	struct stat st2;
	if ( stat(argv[2],&st2) == -1 ) err(4,"Error stat file 2");
	if ( st2.st_size%sizeof(uint32_t) != 0 ) errx(5,"Error file 2 is not uint32_t");
	
	int fd1 = open(argv[1],O_RDONLY);
	if (fd1 == -1) err(6,"Error open file 1");

	int fd2 = open(argv[2],O_RDONLY);
	if (fd2 == -1) {
		const int _errno = errno;
		close(fd1);
		errno = _errno;
		err(7,"Error open file 2");
	}

	int fd3=open(argv[3],O_CREAT | O_TRUNC | O_WRONLY, S_IRUSR | S_IWUSR);
	if (fd3 == -1) {
		const int _errno = errno;
		close(fd1);
		close(fd2);
		errno = _errno;
		err(9,"Error open file 3");
	}
	
	struct doubble {
		uint32_t pos;
		uint32_t len;
	};

	struct doubble a;

	while ( read(fd1,&a,sizeof(a)) == sizeof(a) ) {
		printf("Ti tashak lisi pravish\n");
		if ( lseek(fd2,(a.pos-1)*sizeof(uint32_t),SEEK_SET) == -1 ) {
			const int _errno = errno;
			close(fd1);
			close(fd2);
			errno = _errno;
			err(8,"Error lseek");
		}
		uint32_t* buff = malloc(a.len*sizeof(uint32_t));
		if ( read(fd2,buff,sizeof(buff))  ) {
			write(fd3,buff,sizeof(buff));
		}
		free(buff);
	}
	close(fd1);
	close(fd2);
	exit(0);
}














