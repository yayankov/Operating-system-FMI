#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<err.h>

int main(int argc, char** argv) {
	if(argc != 4) errx(1,"ERROR arguments");
	struct stat st1;
	stat(argv[1],&st1);
	if( (st1.st_size%sizeof(uint8_t)) != 0) errx(2,"ERROR size of f1");
	
	struct stat st2;
    stat(argv[2],&st2);
    if( (st2.st_size%sizeof(uint8_t)) != 0) errx(3,"ERROR size of f2");

	if( st2.st_size != st1.st_size) errx(4,"Not equal size of files");

	int fd1 = open(argv[1],O_RDONLY);
	int fd2 = open(argv[2],O_RDONLY);
	int fd3 = open(argv[3],O_CREAT | O_TRUNC | O_WRONLY, S_IRUSR | S_IWUSR);

	struct triple {
		uint16_t pos;
		uint8_t old;
		uint8_t new;
	} a;

	uint8_t temp1;
	uint8_t temp2;
	uint16_t cnt=0;
	while( (read(fd1,&temp1,sizeof(temp1)) == sizeof(temp1)) &&(read(fd2,&temp2,sizeof(temp2)) == sizeof(temp2)) ) {
		
		if( temp1 != temp2 ) {
			a.old = temp1;
			a.new = temp2;
			a.pos = cnt;			
			write(fd3,&a,sizeof(a));
		}
		cnt++;
	}
	close(fd1);
	close(fd2);
	close(fd3);
	exit(0);
}


















