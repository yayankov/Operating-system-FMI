#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdint.h>
#include <err.h>
#include <errno.h>
#include <string.h>

int main(int argc, char* argv[])
{
	if(argc < 1) errx(1,"wrong input");
	
	int cnt = 1;
	if(strcmp(argv[1],"-n") == 0) {
		//printf("ima -n");
		
		for(int i=2; i<argc; i++) {
			//printf("%d ",i-1);
			char c;			
			if(strcmp(argv[i],"-") == 0) {
				int start =1;
				while(read(0,&c,sizeof(c))) {

					if(start == 1) {
						setbuf(stdout,NULL);
						printf("%d ",cnt);
						 cnt++;
						start = 0;
					}
					write(1,&c,sizeof(c));
					if( c =='\n') {
						start = 1;
					}
				}

			} else {
				//printf("YEEES\n");
				int start = 1;
				int fd = open(argv[i],O_RDONLY);
				while(read(fd,&c,sizeof(c))) {
                    if(start == 1) {
                        setbuf(stdout,NULL);
                        printf("%d ",cnt);
                        cnt++;
                        start = 0;
                    }
                    write(1,&c,sizeof(c));
                    if( c =='\n') {
                        start = 1;
                    }

				}
			}
		}	
	
	} else {

		for(int i=1; i<argc; i++) {
//        printf("nqma -n\n");
    
	        char c;            
            if(strcmp(argv[i],"-") == 0) {
                
				while(read(0,&c,sizeof(c))) {
                	write(1,&c,sizeof(c));
    			}

	        } else {
//				printf("else\n");
                int fd = open(argv[i],O_RDONLY);
				while(read(fd,&c,sizeof(c))) {
	                write(1,&c,sizeof(c));
    			}
	        }
    
	    }
//		exit(0);      
	}

	exit(0);
}
