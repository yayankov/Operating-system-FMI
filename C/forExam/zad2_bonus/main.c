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

	if(argc != 2) err(1,"Error arguments");

	char c;
	char lines[1024][1024];
	int i = 0;
	int j = 0;
	while(read(0,&c,sizeof(c)) > 0) {
		if(c == '\n') {
			lines[i][j] = '\0';
			j = 0;
			i++;
			continue;
		}
		lines[i][j] = c;
		j++;
	}

	char words[10][10];

	for(int k1=0;k1<i;k1++) {
//		lines[k1][k2];
//		words[w1][w2];

		int k2 = 0;
	    int w1 = 0;
    	int w2 = 0;

		while(lines[k1][k2] != '\0') {
			
			if(lines[k1][k2] == ' ') {
				words[w1][w2] = '\0';

				const pid_t pid = fork();
				if ( pid == -1) err(2,"Error fork");
				if(pid == 0) {
					execlp(argv[1],argv[1],words[w1],0);
				}
				wait(NULL);

				w1++;
				w2 = 0;
				k2++;
				continue;			

			} else {
				words[w1][w2] = lines[k1][k2];
				k2++;	
				w2++;
			}
		}	
		words[w1][w2] = '\0';
		const pid_t pid = fork();
        if ( pid == -1) err(2,"Error fork");
        if(pid == 0) {
              execlp(argv[1],argv[1],words[w1],0);
        }
        wait(NULL);
	}
	exit(0);
}

















