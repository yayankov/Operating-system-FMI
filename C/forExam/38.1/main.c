#include<stdlib.h>
#include<stdio.h>
#include<stdint.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<errno.h>
#include<err.h>
#include<unistd.h>
#include<fcntl.h> 

int main(int argc,char** argv) {
	if (argc != 5) errx(1,"Error arguments");
	struct stat st1;
	if ( stat(argv[1],&st1) == -1) err(2,"Error stat");
	if ( (st1.st_size%sizeof(uint8_t)) != 0 ) errx(3,"Error file 1 format");
	
	int fd1 = open(argv[1],O_RDONLY);
	if(fd1 == -1) err(4,"ERRor open file 1");
	
	int fd2 = open(argv[2],O_RDONLY);
	if( fd2 == -1 ) {
		const int _errno = errno;
		close(fd1);
		errno = _errno;
		err(5,"ERROR open file 2");
	}

	int fd3 = open(argv[3],O_CREAT | O_TRUNC | O_RDWR, S_IRUSR | S_IWUSR);
	if ( fd3 == -1 ) {
		const int _errno = errno;
        close(fd1);
     	close(fd2);
		errno = _errno;
        err(6,"ERROR open file 3");
	}
	
	int fd4 = open(argv[4],O_CREAT | O_TRUNC | O_RDWR, S_IRUSR | S_IWUSR);
    if ( fd4 == -1 ) {
        const int _errno = errno;
        close(fd1);
        close(fd2);
		close(fd3);
        errno = _errno;
        err(7,"ERROR open file 4");
    }

	struct triple {
		uint16_t pos;
		uint8_t len;
		uint8_t nothing;
	} a;

	while( read(fd2,&a,sizeof(a)) == sizeof(a) ) {
		lseek(fd1,(a.pos)*sizeof(uint8_t),SEEK_SET);
		uint8_t start;
		read(fd1,&start,sizeof(start));
		if( start < 'A' || start > 'Z' ) {
			continue;
		}
		printf("Start: %d\n",start);
		write(fd3,&start,sizeof(start));
		uint16_t cnt = 2;
		uint8_t buf;
     	while( (cnt <= a.len) && (read(fd1,&buf,sizeof(buf)) == sizeof(buf)) ) {
			write(1,&buf,sizeof(buf));
			printf("cnt: %d, a.len: %d\n",cnt,a.len);
			write(fd3,&buf,sizeof(buf));
			cnt = cnt + 1;
		}
		uint16_t buf2;
		write(fd4,&buf2,sizeof(buf2));
		cnt = a.len; 
		write(fd4,&cnt,sizeof(cnt));
		buf2 += cnt;
	}	

	close(fd1);
	close(fd2);
	close(fd3);
	close(fd4);
	exit(0);
}
















