#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include<string.h>
#include<err.h>
#include<sys/wait.h>
#include<stdint.h>

int main(int argc, char** argv) {
	if(argc != 3) err(1,"Error arguments");
	char* cmd_Odd = argv[1];
	char* cmd_Even = argv[2]; 
	
	char c;
	char lines[1024][1024];
	int i = 0;
	int j = 0;
	while(read(0,&c,sizeof(c)) > 0) {
		
		if( c == '\n' ) {
			lines[i][j] = '\0';
			j = 0;
			i++;	
			continue;
		}
		lines[i][j] = c;
		j++;
	}
	int k;
	for( k=0; k < i; k++) {
		const pid_t pid = fork();
		if(pid == -1) err(2,"Error fork");
		if(pid == 0) {
			if(k%2 == 0) {
				if(execlp(cmd_Even,cmd_Even,lines[k],0) == -1) err(3,"error exec");
			} else {
				if(execlp(cmd_Odd,cmd_Odd,lines[k],0) == -1) err(4,"error exec");
			}
		}
		int status;
		wait(&status);
	}

}
