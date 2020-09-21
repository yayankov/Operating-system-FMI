#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<err.h>
#include<string.h>

int main(int argc, char** argv) 
{
	if(argc > 5 || argc < 3) err(1,"error arguments");

	if(strcmp(argv[1],"-c") == 0) {
		int len = strlen(argv[2]);

    	if( (len == 3) && (argv[2][0] >= 49 && argv[2][0] <= 58) && (argv[2][1] == '-') && (argv[2][2] >= 49 && argv[2][2] <=58) && (argv[2][0] <= argv[2][2])) {
			int start = argv[2][0]-48;
			int end = argv[2][2]-48;
			char c;			
			int cnt=1;
            while( read(0,&c,1) > 0 ) {
                if(cnt >=  start && cnt <= end) {
                    write(1,&c,1);
				}
				cnt++;
			}
            printf("\n");  
            	
        }	else if( len == 1 && argv[2][0] >= 49 && argv[2][0] <= 58) {
			int num = argv[2][0]-48;
//			printf("num: %d\n",num);
			char c;
			int cnt=1;
			while( read(0,&c,1) > 0 ) {
				if(cnt == num) {
					write(1,&c,1);
					printf("\n");
				}
				cnt++;
			}
		} else {
			err(2,"error argument 2 should be number or format <num-num>");
		}
		
	} else if(strcmp(argv[1],"-d") == 0  && strcmp(argv[3],"-f") == 0) {
		
		char del = argv[2][0];
		int start;
		int end;
		if( (strlen(argv[4]) == 3) && (argv[4][0] >= 49 && argv[4][0] <= 58) && (argv[4][1] == '-') && (argv[4][2] >= 49 && argv[4][2] <=58) && (argv[4][0] <= argv[4][2])) {
            start = argv[4][0]-48;
            end = argv[4][2]-48;	
		}
		if( strlen(argv[4]) == 1 && argv[4][0] >= 49 && argv[4][0] <= 58) {
			start = argv[4][0] - '0';
			end = start;
		}
		
		char c;
		int col = 1;
        while( read(0,&c,1) > 0 ) {
            if(del == c) {
				col++;
				if(col == start || col == (end+1)) continue;
			}
			if(col >= start && col <= end) {
                write(1,&c,1);	
			}
        }
		printf("\n");
		
	}
}









