#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<stdint.h>
#include<unistd.h>
#include<fcntl.h>
#include<err.h>
#include<errno.h>

int main(int argc,char** argv) {
	if (argc == 1) {
		printf("WTF\n");
		char c;
		while(read(0,&c,sizeof(c))) {
			write(1,&c,sizeof(c));
	 	}
	}
	if(argc == 2 && strcmp(argv[1],"-n") == 0) {
		err(2,"Error arguments");	
	}
	int buul = 0;
	int cnt = 1;
	if( argc > 1 ) {
		for(int i=1;i<argc;i++) {
			if( strcmp(argv[i],"-n") == 0) {
				buul=1;
				continue;
			}
			if(strcmp(argv[i],"-") == 0) {
				char c;
				int start = 1;
		        while(read(0,&c,sizeof(c))) {
					
       		    	if(buul == 1 && start == 1) {
						setbuf(stdout,NULL);
						printf("%d ",cnt);
						cnt++;
						start = 0;
					}
					write(1,&c,sizeof(c));
					if( c == '\n' ) start = 1;
        		}	
			} else {
				int fd=open(argv[i],O_RDONLY);
				int start = 1;
				char c;
				while(read(fd,&c,sizeof(c))) {
                    if(buul == 1 && start == 1) {
						setbuf(stdout,NULL);
						printf("%d ",cnt);
						cnt++;
						start = 0;
					}
					write(1,&c,sizeof(c));
					if (c == '\n') start = 1;
				}
			}
		}
	}	

	exit(0);
}
