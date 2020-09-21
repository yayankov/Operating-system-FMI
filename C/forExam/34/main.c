#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdint.h>
#include <err.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

int main(int argc, char** argv)
{	
	if(argc != 5 ) errx(1,"error arguments");
	
	int fd1 = open(argv[1],O_RDONLY);
	int fd2 = open(argv[2],O_RDONLY);	
	int fd3 = open(argv[3],O_CREAT | O_TRUNC | O_RDWR, S_IRUSR | S_IWUSR);
    int fd4 = open(argv[4],O_CREAT | O_TRUNC | O_RDWR, S_IRUSR | S_IWUSR);
	
	struct triple {
		uint16_t begin;
		uint8_t len;
		uint8_t reserved;
	};
	
	struct triple a;
	struct triple b={0,0,0};

	while( (read(fd2,&a,sizeof(a))) == sizeof(a) ) {
		uint16_t pos = a.begin;
		lseek(fd1,pos,SEEK_SET);
		uint8_t letter;
		read(fd1,&letter,sizeof(letter));
		
		if( letter >= 'A' && letter <= 'Z') {
			write(fd3,&letter,sizeof(letter));
			uint8_t other;
			uint8_t cnt = 1;
			while( cnt < a.len ) {
				read(fd1,&other,sizeof(other));
				write(fd3,&other,sizeof(other));
				cnt++;
			}
		
			b.len = a.len;
			write(fd4,&b,sizeof(b));
			b.begin += b.len;
		}	
	}

/*	while( read(fd2,&a,sizeof(a)) == sizeof(a)) {
		
		uint8_t* buff = malloc(a.len);

		if( lseek(fd1,a.begin,SEEK_SET) < 0) continue;
			
		read(fd1,buff,sizeof(uint8_t)*a.len);

		if(buff[0] < 'A' || buff[0] > 'Z') continue;
	
		write(fd3,buff,sizeof(uint8_t)*a.len);
		
		printf("YEEES\n");

		b.len = a.len;

		write(fd4,&b,sizeof(b));
        b.begin += b.len;
		

	}
*/
	exit(0);
}
