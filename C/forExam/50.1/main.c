#include <sys/stat.h>
#include <fcntl.h>
#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<err.h>
#include<string.h>
#include<sys/wait.h>

int main() {

	while(1) {
		char* prompt = "Input a command: ";
		int len = strlen(prompt);
		if (write(1,prompt,len) != len) {
			err(1,"Error writing");
		}
		char buff[1024];
		int rd = read(0,&buff,sizeof(buff));
		if( rd == -1 ) err(2,"Error reading");
//		printf("%s\n",buff);
		buff[rd -1] ='\0';
		if ( strcmp(buff,"exit") == 0 ) break;

		const pid_t child = fork();
		if( child == -1) err(3,"Error fork");
	
		if(child == 0) {
			if(execlp(buff,buff,0) == -1) err(4,"error exec");
		}
		wait(NULL);		
	}

}
