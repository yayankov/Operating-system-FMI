#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<stdint.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<err.h>
#include<errno.h>
#include<string.h>

int main(int argc,char** argv) {

	if( argc != 3) errx(1,"Wrong arguments");

	if( strcmp(argv[1],"-d") == 0 ) {

		char c;
		char* niz1 = argv[2];
		while( read(0,&c,sizeof(c)) > 0) {
			int len = strlen(niz1);
			int bool = 0;
			for(int i=0; i<len; i++) {
				if( niz1[i] == c ) {
					bool = 1;
				}
			}
			if( bool == 1) continue;
			write(1,&c,sizeof(c));
		}

	} else if (strcmp(argv[1],"-s") == 0) {
		char* letter = argv[2];
        char a;
        int cnt = 0;
        while( read(0,&a,sizeof(a)) == sizeof(a)) {
            
            if (a != letter[0]) {
                write(1,&a,sizeof(a));
                cnt = 0;
            } else {
                if (cnt == 0) write(1,&a,sizeof(a));
                cnt++;
                continue;
            }
        }
	} else {

		char* niz1 = argv[1];
		char* niz2 = argv[2];
		char c;
        while( read(0,&c,sizeof(c)) > 0) {
            int len = strlen(niz1);
            int bol = -1;
            for(int i=0; i<len; i++) {
                if( niz1[i] == c ) {
                    bol = i;
                }
            }
            if( bol != -1) {
				write(1,&niz2[bol],1);
			} else {
	            write(1,&c,sizeof(c));
			}
        }


	}
		
}























