#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <err.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

int main(int argc, char** argv)
{
	if(argc != 3) errx(1,"Error arguments");
	
	int fd=open(argv[2],O_RDONLY);
	if(fd == -1) err(2,"Error fd");	
	if(access(argv[2],R_OK) == -1) errx(3, "File doesnt exist or is not readable!");

	uint16_t c;
		

	if( strcmp(argv[1],"--print") == 0 ) {
		while( read(fd,&c,sizeof(c)) > 0) {
			if( sizeof(c) != sizeof(uint16_t)) {
				const int _errno=errno;
				close(fd);
				errno=_errno;
				err(3,"Error writing");	
			}
			printf("%d\n",c);
		}
	
	} else if(strcmp(argv[1],"--max") == 0) {
		uint16_t max;
		read(fd,&max,sizeof(max));
			
		while( read(fd,&c,sizeof(c)) > 0) {
			if(sizeof(c) != sizeof(uint16_t)) {
				const int _errno=errno;
				close(fd);
				errno=_errno;
				err(4,"Error max");
			}
			if (max < c) max = c;
		}
		printf("%d\n",max);		
	
	} else if(strcmp(argv[1],"--min") == 0) {
		uint16_t min = 0;
        read(fd,&min,sizeof(min));  
        while( read(fd,&c,sizeof(c)) > 0) {
            if(sizeof(c) != sizeof(uint16_t)) {
                const int _errno=errno;
                close(fd);
                errno=_errno;
                err(5,"Error max");
            }
            if (min > c) min = c;
        }       
        printf("%d\n",min); 
	}

	close(fd);
	exit(0);
}





