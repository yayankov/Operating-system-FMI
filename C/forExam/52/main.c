#include<stdlib.h>
#include<stdio.h>
#include<stdint.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<err.h>
#include<string.h>
#include<sys/wait.h>

int main(int argc,char** argv) {
	if (argc > 2 || argc < 1) errx(1,"Wrong arguments");
	
	char* cmd;
	if (argc == 2) {
		cmd = argv[1];
	} else {
		cmd = "echo";
	}
	if ( strlen(cmd) > 4) errx(2,"Command could be 4 characters");
	
	char c;
	char file[1024][1024];
	int j = 0;
	int i = 0;
	while( read(0,&c,sizeof(c)) == sizeof(c)) {
		
		if( c == 0x20 || c == 0x0A ) {
			file[i][j] = '\0';
			j = 0;
			i++;
		} else {
			file[i][j] = c;
			j++;
		}
		if ( j > 3) errx(6,"Error the parameters could be 4 characters");
	}
	int k;
	for(k=0; k < i; k+=2) {
	
		const pid_t child = fork();
        if( child == -1) err(3,"Error fork");
        if(child == 0) {
 	        if( k+1 >= i) {
				if( execlp(cmd,cmd,file[k],(char*)0) == -1 ) err(4,"Error exec");
			}
         	if( k+1 < i ) {
     			if(execlp(cmd,cmd,file[k],file[k+1],(char*)0) == -1) err(5,"Eror exec");	
			}
		}
        int status;
        wait(&status);
	}		
	exit(0);
}













