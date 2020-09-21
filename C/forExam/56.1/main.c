#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
       #include <unistd.h>
#include <sys/types.h>
       #include <sys/stat.h>
       #include <fcntl.h>
#include<string.h>
#include<err.h>
#include<sys/wait.h>
#include<stdint.h>

int main(int argc, char** argv) {
	if(argc != 3 ) errx(1,"error arguments");
	
	int a[2];
	if( pipe(a) == -1) err(2,"Error pipe");
	const pid_t pid = fork();
	if (pid == -1) err(4,"error fork");
	if( pid == 0) {
		close(a[0]);
		dup2(a[1],1);
		if( execlp("cat","cat",argv[1],(char*)0) == -1) err(5,"Error exec");
	}
	close(a[1]);
	dup2(a[0],0);

	int fd2 = open(argv[2],O_CREAT | O_TRUNC | O_RDWR, S_IRUSR | S_IWUSR);
	if( fd2 == -1) {
		err(3,"error open file 2");
	}
	
	int booll = 0;
	uint8_t byte;
	while( read(a[0],&byte,sizeof(byte)) > 0) {
		if(byte == 0x7D) {
			booll = 1;
			continue;
		}
		if(booll == 1) {
			uint8_t c = byte^0x20;
			if( c != 0x55) {
				write(fd2,&c,sizeof(c));
			}
			booll = 0;
		} else {
			write(fd2,&byte,sizeof(byte));
		}		
	}
	close(fd2);
	exit(0);
}
