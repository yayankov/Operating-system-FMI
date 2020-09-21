#include<stdlib.h>
#include<unistd.h>
#include<err.h>
#include <sys/types.h>
       #include <sys/stat.h>
       #include <fcntl.h>
#include<sys/stat.h>
#include<stdio.h>
#include<fcntl.h>
#include<string.h>
#include<stdint.h>

int cmp(const void* a, const void* b) {
	if( *((uint8_t*)a) > *((uint8_t*)b) ) 
		return 1;
	else if ( *((uint8_t*)a) < *((uint8_t*)b) ) 
		return -1;
	return 0;
}

int main(int argc,char** argv) {

	if (argc != 3) errx(1,"Wrong number of arguments");

	int	fd = open(argv[1],O_RDONLY);
	if ( fd == -1 ) err(2,"Error with file");

	struct stat st;
	if ( stat(argv[1],&st) == -1 ) {
		close(fd);
		err(3,"Error stat");
	}
	if ( st.st_size == 0 ) {
		close(fd);
		err(4,"Error size file1");
	}
	int fd_rev = open(argv[2],O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IWUSR );
	if ( fd_rev == -1 ) {
		close(fd);	
		err(5,"Error open file 2");
	}
	void* buff = malloc(st.st_size);
	if ( buff == NULL ) {
		close(fd);
		close(fd_rev);
		err(6,"Error malloc");
	}
	if ( read(fd,buff,st.st_size) != st.st_size) {
		close(fd);
		close(fd_rev);
		free(buff);
		err(7,"Wrong read from file");
	}
	qsort(buff,st.st_size,sizeof(uint8_t),cmp);
	if ( write(fd_rev,buff,st.st_size) != st.st_size ) {
		close(fd);
		close(fd_rev);
		free(buff);
		err(8,"Error writing in file 2");
	}		
	free(buff);
	close(fd);
	close(fd_rev);
	exit(0);
}







