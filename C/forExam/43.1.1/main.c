#include<stdlib.h>
#include<stdint.h>
#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<err.h>
#include<errno.h>
#include<sys/stat.h>
#include<sys/types.h>

int cmp(const void* a,const void* b) {
	if( *((uint32_t*)a) > *((uint32_t*)b) ) {
		return 1;
	} else if ( *((uint32_t*)a) < *((uint32_t*)b) ) {
		return -1;
	}
	return 0;
}

int main(int argc, char** argv) {
	
	if (argc != 3) errx(1,"Error arguments");
	
	struct stat st1;
//	struct stat st2;

	if ( stat(argv[1],&st1) == -1 ) err(2,"Error stat 1");
//	if (stat(argv[2],&st2) == -1 ) err(3,"Error stat 2");	
	
	if ( st1.st_size%sizeof(uint32_t) != 0) errx(4,"Error size of file 1");
//	if (st2.st_size%sizeof(uint32_t) != 0 ) errx(5,"error size of file 2");
	
	int fd1 = open(argv[1],O_RDONLY);
	if( fd1 == -1) err(6,"Error open file 1");
	int fd2 = open(argv[2],O_CREAT | O_TRUNC | O_WRONLY, S_IRUSR | S_IWUSR);
	if (fd2 == -1) {
		const int _errno = errno;
		close(fd1);
		errno = _errno;
		err(7,"Error open file 2");
	}

	uint32_t num_el = st1.st_size/sizeof(uint32_t);
	uint32_t lhalf = num_el/2;
	
	uint32_t* m1 = malloc(lhalf*sizeof(uint32_t));
	if(!m1) {
		const int _errno = errno;
        close(fd1);
        close(fd2);
		errno = _errno;
		err(8,"error malloc");
	}	

	ssize_t merge1 = open("temp1",O_CREAT | O_TRUNC | O_RDWR, S_IRUSR | S_IWUSR);
	if( merge1 == -1) {
		const int _errno = errno;
        close(fd1);
		close(fd2);
        errno = _errno;
		err(9,"error open temp1");
	}

	if( (unsigned)read(fd1,m1,lhalf*sizeof(uint32_t)) != (ssize_t)lhalf*sizeof(uint32_t)) {
		const int _errno = errno;
        close(fd1);
        close(fd2);
		close(merge1);
        errno = _errno;
        err(10,"error malloc");	
	}

	qsort(m1,lhalf,sizeof(uint32_t),cmp);

	if ( (unsigned)write(merge1,m1,lhalf*sizeof(uint32_t)) != (ssize_t)lhalf*sizeof(uint32_t)) {
		 const int _errno = errno;
        close(fd1);
        close(fd2);
        close(merge1);
        errno = _errno;
        err(10,"error malloc");
	}

	ssize_t rhalf = num_el - lhalf;

	 uint32_t* m2 = malloc(rhalf*sizeof(uint32_t));
    if(!m2) {
        const int _errno = errno;
        close(fd1);
        close(fd2);
        errno = _errno;
        err(11,"error malloc");
    }

    ssize_t merge2 = open("temp2",O_CREAT | O_TRUNC | O_RDWR, S_IRUSR | S_IWUSR);
    if( merge2 == -1) {
        const int _errno = errno;
        close(fd1);
        close(fd2);
		close(merge1);
        errno = _errno;
        err(12,"error open temp1");
    }

    if( (unsigned)read(fd1,m2,rhalf*sizeof(uint32_t)) != (ssize_t)rhalf*sizeof(uint32_t)) {
        const int _errno = errno;
        close(fd1);
        close(fd2);
        close(merge1);
		close(merge2);
        errno = _errno;
        err(13,"error malloc");
    }

    qsort(m2,rhalf,sizeof(uint32_t),cmp);

    if ( (unsigned)write(merge2,m2,rhalf*sizeof(uint32_t)) != (ssize_t)rhalf*sizeof(uint32_t)) {
         const int _errno = errno;
        close(fd1);
        close(fd2);
        close(merge1);
		close(merge2);
        errno = _errno;
        err(14,"error malloc");
    }

	lseek(merge1,0,SEEK_SET);
	lseek(merge2,0,SEEK_SET);

	uint32_t a;
	uint32_t b;
	ssize_t rd1 = -1;
	ssize_t rd2 = -1;

	while( (rd1=read(merge1,&a,sizeof(a))) == sizeof(a) && (rd2=read(merge2,&b,sizeof(b))) == sizeof(b) ) {
		if( a <= b) {
			lseek(merge2,-1*sizeof(uint32_t),SEEK_CUR);
			if (write(fd2,&a,sizeof(a)) != rd1 ) {
		         const int _errno = errno;
		        close(fd1);
		        close(fd2);
		        close(merge1);
		        close(merge2);
		        errno = _errno;
		        err(14,"error malloc");
		    }
		} else {
			lseek(merge1,-1*sizeof(uint32_t),SEEK_CUR);
            if (write(fd2,&b,sizeof(b)) != rd2 ) {
                 const int _errno = errno;
                close(fd1);
                close(fd2);
                close(merge1);
                close(merge2);
                errno = _errno;
                err(14,"error malloc");
            }
		}
	}

	if ( rd1 == sizeof(a)) {
		write(fd2,&a,sizeof(a));
	}

	while(read(merge1,&a,sizeof(a)) >0) {
		write(fd2,&a,sizeof(a));
	}
	
	while(read(merge2,&b,sizeof(b)) > 0) {
		write(fd2,&b,sizeof(b));
	}

}



















