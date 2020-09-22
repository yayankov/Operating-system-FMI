#include<stdlib.h>
#include<stdio.h>
#include<stdint.h>
#include<sys/stat.h>
#include<err.h>
#include<errno.h>
#include<sys/types.h>
#include<fcntl.h>
#include<unistd.h>

int cmp(const void* a, const void* b) {
	if ( *((uint32_t*)a) > *((uint32_t*)b) ) {
		return 1;
	} else if ( *((uint32_t*)a) < *((uint32_t*)b) ) {
		return -1;
	}
	return 0;
}

int main(int argc, char** argv) {
	
	if ( argc != 2 ) errx(1,"Error arguments");
	
	struct stat st;
	if ( stat(argv[1],&st) == -1 ) err(3,"Error stat file");
	if ( (st.st_size % sizeof(uint32_t)) != 0 ) errx(2,"Error size of file");
	
	int fd1 = open(argv[1],O_RDONLY);
	if(fd1 == -1) err(4,"Error open file");
	
	uint32_t num_el = st.st_size/sizeof(uint32_t);
	
	uint32_t lhalf = num_el/2;
	
	uint32_t* buff1 = malloc(sizeof(uint32_t)*lhalf);
	if ( buff1 == NULL ) err(5,"Error malloc1");
	read(fd1, buff1, sizeof(uint32_t)*lhalf);
	
	qsort(buff1,lhalf,sizeof(uint32_t),cmp);
	
	int m1 = open("temp1",O_CREAT | O_TRUNC | O_RDWR, S_IRUSR | S_IWUSR);
	write(m1,buff1,sizeof(uint32_t)*lhalf);

	uint32_t rhalf = num_el - lhalf;
	uint32_t* buff2 = malloc(sizeof(uint32_t)*rhalf);
	read(fd1,buff2,sizeof(uint32_t)*rhalf);
	
	qsort(buff2,rhalf,sizeof(uint32_t),cmp);
	
	int m2 = open("temp2",O_CREAT | O_TRUNC | O_RDWR, S_IRUSR | S_IWUSR);
	write(m2,buff2,sizeof(uint32_t)*rhalf);

	lseek(m1,0,SEEK_SET);
	lseek(m2,0,SEEK_SET);

	int fd2 = open("binary",O_CREAT | O_WRONLY | O_TRUNC, S_IRUSR | S_IWUSR);

	uint32_t a;
	uint32_t b;
	size_t read1 = -1;
	size_t read2 = -1;

	while( ((read1 = read(m1,&a,sizeof(a))) == sizeof(uint32_t)) && ((read2 = read(m2,&b,sizeof(b))) == sizeof(uint32_t)) ) {
		if ( a <= b ) {
			write(fd2,&a,sizeof(a));
			lseek(m2,-1*sizeof(uint32_t),SEEK_CUR);
		} 
		else if ( a > b ) {
			write(fd2,&b,sizeof(b));
			lseek(m1,-1*sizeof(uint32_t),SEEK_CUR);
		}
	}
	
	if(read1 == sizeof(uint32_t)) {
		write(fd2,&a,sizeof(a));
	}

	while( (read1 = read(m1,&a,sizeof(a))) == sizeof(a)) {
		write(fd2,&a,sizeof(a));
	}

	while( (read2 = read(m2,&b,sizeof(b))) == sizeof(b)) {
		write(fd2,&b,sizeof(b));
	}

	unlink("temp1");
	unlink("temp2");
	close(fd1);
	close(fd2);
	close(m1);
	close(m2);
	free(buff1);
	free(buff2);
	exit(0);
}
/*
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <err.h>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <stdint.h>
// 33
int cmp(const void* a, const void* b) {
	if( *((uint32_t*)a) > *((uint32_t*)b) )
		return 1;
	else if( *((uint32_t*)a) < *((uint32_t*)b) )
		return -1;
	return 0;
}

int main(int argc,char** argv)
{
	if(argc != 2) errx(1,"error arguments");
	char* file = argv[1];
	struct stat st;
	if(stat(file,&st) == -1) err(2,"error while stat file");
	if(st.st_size % sizeof(uint32_t) != 0) errx(3,"error type of numbers");	
	
	uint32_t num = st.st_size / sizeof(uint32_t);
	uint32_t half = num/2;
	uint32_t* m1 = malloc(half*sizeof(uint32_t));
	if(!m1) err(4,"error malloc");
	
	ssize_t fd =  open(file,O_RDONLY);
	if(fd == -1) {
		free(m1);
		err(5,"error open file");
	}

	if( read(fd,m1,half*sizeof(uint32_t)) != (ssize_t)(half*sizeof(uint32_t)) ) {
		const int _errno = errno;
		close(fd);
		free(m1);
		errno = _errno;
		err(6,"error read fd to m1");
	}
	
	qsort(m1,half,sizeof(uint32_t),cmp);

	ssize_t merge1 = open("temp1",O_CREAT | O_TRUNC | O_RDWR, S_IRUSR | S_IWUSR);
	if(merge1 == -1) {
		const int _errno = errno;
		close(fd);
		free(m1);
		errno = _errno;
		err(7, "error open merge1");
	}

	if( write(merge1,m1,half*sizeof(uint32_t)) != (ssize_t)(half*sizeof(uint32_t))){
		const int _errno = errno;
        close(fd);
        free(m1);
		close(merge1);
        errno = _errno;
		err(8,"error write in merge1");
	}
	free(m1);
	uint32_t half2 = num-half;
	uint32_t* m2 = malloc(half2*sizeof(uint32_t));
	if (!m2) {
		const int _errno = errno;
        close(fd);
        close(merge1);
        errno = _errno;
		err(9,"error malloc2");
	}	
	if(read(fd,m2,half2*sizeof(uint32_t)) != (ssize_t)(half2*sizeof(uint32_t))) {
		const int _errno = errno;
        close(fd);
        free(m2);
		close(merge1);
        errno = _errno;
		err(10,"error read file to m2");
	}

	qsort(m2,half2,sizeof(uint32_t),cmp);

	ssize_t merge2 = open("temp2", O_CREAT | O_TRUNC | O_RDWR, S_IRUSR | S_IWUSR);
	if(merge2 == -1) {
		const int _errno = errno;
        close(fd);
        free(m2);
		close(merge1);
        errno = _errno;
		err(11,"error open merge2");
	}
	
	if( write(merge2,m2,half2*sizeof(uint32_t)) !=  (ssize_t)(half2*sizeof(uint32_t))) {
		const int _errno = errno;
        close(fd);
        free(m2);
		close(merge2);
		close(merge1);
        errno = _errno;
		err(12,"error write in merge2");
	}	
	
	free(m2);

	if( lseek(merge1,0,SEEK_SET) == -1) {
		const int _errno = errno;
        close(fd);
		close(merge1);
		close(merge2);
        errno = _errno;
		err(13,"error lseek merge1");
	}
	if( lseek(merge2,0,SEEK_SET) == -1) {
        const int _errno = errno;
        close(fd);
        close(merge1);
        close(merge2);
        errno = _errno;
        err(23,"error lseek merge1");
    }
	
	ssize_t fd2 = open("binary",  O_CREAT | O_TRUNC | O_RDWR, S_IRUSR | S_IWUSR);
	if(fd2==-1){
        const int _errno = errno;
        close(fd);
        close(merge1);
        close(merge2);
        errno = _errno;
        err(14,"error lseek merge1");
    }

	uint32_t a,b;
	ssize_t read1 = -1;
	ssize_t read2 = -1;
	int printTotal = 0;
	int printA = 0;
	int printB = 0;

	while(((read1 = read(merge1,&a,sizeof(a))) == sizeof(a)) && ((read2 = read(merge2,&b,sizeof(b))) == sizeof(b))) {
		if(a <= b) {
			write(fd2,&a,sizeof(a));
			lseek(merge2,-1*sizeof(b),SEEK_CUR);
		} else {
			write(fd2,&b,sizeof(b));
			lseek(merge1,-1*sizeof(a),SEEK_CUR);
		}
		printTotal++;
	}
	if(read1 == sizeof(a))	{
		write(fd2,&a,sizeof(a));
		printA++;
	}

	if(read1 == -1 || read2 == -1 ) err(17,"error while reading temp1");	

	while((read1 = read(merge1,&a,sizeof(a))) == sizeof(a)) {
		write(fd2,&a,sizeof(a));
		printA++;
	}
	if( read1 == -1)	{
        const int _errno = errno;
        close(fd);
		close(fd2);
        close(merge1);
        close(merge2);
        errno = _errno;
        err(13,"error lseek merge1");
    }

	while((read2 = read(merge2,&b,sizeof(b))) == sizeof(b)) {
		write(fd2,&b,sizeof(b));
		printB++;
	}
	if( read2 == -1)	{
        const int _errno = errno;
        close(fd);
        close(fd2);
        close(merge1);
        close(merge2);
        errno = _errno;
        err(13,"error lseek merge1");
    }

	printf("obshi: %d\n",printTotal);
    printf("ot A: %d\n",printA);
    printf("ot B: %d\n",printB);
	
	unlink("temp1");
	unlink("temp2");	

	close(fd);
	close(merge1);
	close(fd2);
	close(merge2);
	exit(0);
}
*/
