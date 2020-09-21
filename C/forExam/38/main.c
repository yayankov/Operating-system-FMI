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

int compare(char *str, char c)
{
	int i = 0;
    while(str[i] != '\0') {
    	if( c == str[i] )   {
        	return i;
       	}
        i++;
   	}
	return -1;	
}

int main(int argc, char** argv)
{
	if(argc < 2 || argc > 4) errx(1,"error arguments");
	
	
	if(strcmp(argv[1],"-d") == 0) {
		if(argc != 3) errx(2,"error arguments");
		char* str = argv[2];
		char c;

		while( read(0,&c,1) == 1 ) {

			if( compare(str,c) == -1 ) {
				write(1,&c,1);
			}
		}
		
	} else if(strcmp(argv[1],"-s") == 0) {		  
	    if(argc != 3) errx(3, "error arguments");
		char* str = argv[2];	
		char a;
		char last = '\0';
		while( read(0,&a,1) == 1) {
			if(compare(str,a) == -1) {
				write(1,&a,1);
			} else {
				if(a != last) {
					write(1,&a,1);
				}
			}
			last = a;
		}
			
	} else {
		
		char* str1 = argv[1];
		char* str2 = argv[2];		

		char c;
   
		while( read(0,&c,1) == 1 ) {
				int pos = compare(str1,c);
                if( pos == -1 ) {
					 write(1,&c,1);
            	}
				if( pos != -1 ) {
					write(1,&str2[pos],1);
				}
		}
	
	}

}
