#include<stdlib.h>
#include<stdio.h>
#include<stdint.h>
#include<unistd.h>
#include<fcntl.h>
#include<errno.h>
#include<err.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<string.h>

int main(int argc, char** argv) {

	if( argc == 3 && strcmp(argv[1],"-c") == 0) {
		char* niz = argv[2];
		if( strlen(argv[2]) == 1) {
			if ( niz[0] < '0' || niz[0] > '9') {
				errx(2,"Argument 2 should be a number");
			} else {
				int start = niz[0] - '0';
			
	            char c;
				int cnt = 1;
    	        while(read(0,&c,sizeof(c)) == sizeof(c)) {
        	        if(cnt == start) {
						write(1,&c,sizeof(c));
						printf("\n");
					}
					cnt++;
            	}
			}
		}
		if( strlen(argv[2]) == 3) {

			if ( niz[0] >= '0' && niz[0] <= '9' && niz[1] == '-' && niz[2] >= '0' && niz[2] <= '9' && niz[0] < niz[2]) {
				int start = niz[0] - '0';
				int end = niz[2] - '0';
	            char c;
				int cnt = 1;
	            while(read(0,&c,sizeof(c)) == sizeof(c)) {
					if(cnt >= start && cnt <= end) {
						write(1,&c,sizeof(c));
					}
					cnt++;
	            }
				printf("\n");
			} else {
				err(4,"Argument 2 is not number - number");
			}
		}

	} else if (argc == 5 && strcmp(argv[1],"-d") == 0 && strcmp(argv[3],"-f") == 0 ) {
		char* niz = argv[4];
		char* delimiter = argv[2];
		char c;
		int column = 1;
		
		if( strlen(argv[4]) == 1) {
            if ( niz[0] < '0' || niz[0] > '9' ) {
				err(9,"Wrong argument 4");
			} else {
				int target = niz[0] - '0';
				while( read(0,&c,sizeof(c)) == sizeof(c)) {
					if (delimiter[0] == c) {
						column++;			
						continue;
					}
					if ( column == target ) {
						write(1,&c,sizeof(c));
					}
				}
				printf("\n");
			}
		} 
		if( strlen(argv[4]) == 3) {
            if ( niz[0] >= '0' && niz[0] <= '9' && niz[1] == '-' && niz[2] >= '0' && niz[2] <= '9' && niz[0] < niz[2]) {
				int start = niz[0] - '0';
				int end = niz[2] - '0';
				while( read(0,&c,sizeof(c)) == sizeof(c)) {
                    if (delimiter[0] == c) {
                        column++;
                        continue;
                    }
                    if ( column >= start && column <= end ) {
                        write(1,&c,sizeof(c));
                    }
                }
                printf("\n");
			}
		}
	} else {
		err(1,"Wronng arguments");
	}
}


















