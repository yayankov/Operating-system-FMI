#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<stdint.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<err.h>
#include<errno.h>
#include <time.h>
#include<string.h>

int main(int argc,char** argv) {

	if(argc < 3) errx(1,"error arguments");

	if(argv[1][0] < '0' || argv[1][0] > '9' || argv[1][1] != '\0') errx(2,"Error number");

    int fd = open("run.log",O_CREAT | O_TRUNC | O_WRONLY, S_IRUSR | S_IWUSR);
    if( fd == -1) err(7,"Error open file");

	pid_t child;
	int cnt = 0;
	int booll=0;
	while(1) {
	    child = fork();
        if(child == -1) err(3,"Error fork");

		time_t start;
		if (time(&start) == -1) err(5,"Error start time");	

		if(child == 0) {
			if( execv(argv[2],argv+2) == -1) err(4,"Error exec");
		}
		int s;
		wait(&s);

		time_t end;
		if( time(&end) == -1 ) err(6,"Error end time");

		int status;
		if(WEXITSTATUS(s) == 0) {
			status = WEXITSTATUS(s);

		} else {
			status = 129;
		}
		printf("status: %d\n",status);
		time_t duration = end - start;
		
		char buff[1024];
		sprintf(buff,"%lld %lld %d\n",(long long)start,(long long)end,status);
		
		if( write(fd,&buff,strlen(buff)) == -1) {
			close(fd);
			err(8,"Error writing in new file");
		}
	
		if(status != 0 && duration < (argv[1][0] - '0')) {
			cnt++;
			printf("cnt: %d booll: %d duration: %lld\n",cnt,booll,(long long)duration);
			if( (cnt-booll) == 2) {

				close(fd);
				break;

			} else {
				continue;
			}
		}
		booll = cnt;
	}

	exit(0);
}

