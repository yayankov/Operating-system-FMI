       #include <sys/types.h>
       #include <sys/stat.h>
       #include <unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<err.h>
#include <fcntl.h>
#include<string.h>
#include<stdint.h>
// 31
int cmp(const void* a, const void* b) {
	if( *((uint8_t*)a) > *((uint8_t*)b) )
		return 1;
	else if( *((uint8_t*)a) < *((uint8_t*)b))
		return -1;
	return 0;
}

int main(int argc,char** argv)
{
	if(argc != 3) {
		errx(1,"Error 2 arguments expected");
	}
	int fd1 = open(argv[1],O_RDONLY);
	if(fd1 == -1) {
		err(2,"Error open file 1");
	}
	int fd2 = open(argv[2],O_CREAT | O_TRUNC | O_RDWR, S_IRUSR | S_IWUSR);
	if(fd2 == -1) {
		close(fd1);
		err(3,"error argument 2");
	}
	
	struct stat st;
	if(stat(argv[1],&st) == -1)	{
		err(4,"error stat for file 1");
	}

	if(st.st_size == 0) {
		close(fd1);
		close(fd2);
		err(5, "no memory");
	}
	
	void* buff = malloc(st.st_size);
	if(buff == NULL) {
		close(fd1);
		close(fd2);
		err(6,"error malloc");
	}
	if(read(fd1,buff,st.st_size) != st.st_size) {
		close(fd1);
		close(fd2);
		free(buff);
		err(7,"error reading from file1");
	}
	qsort(buff,st.st_size,sizeof(uint8_t),cmp);
	if(write(fd2,buff,st.st_size) != st.st_size) {
	    close(fd1);
        close(fd2);
		free(buff);
        err(7,"error reading from file1");
	}
	free(buff);
	close(fd1);
	close(fd2);
	exit(0);
	
}






