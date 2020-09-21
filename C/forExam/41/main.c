#include <stdlib.h>
#include <err.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <stdint.h>

int main(int argc, char** argv)
{
	if(argc != 3) errx(1,"error. the arguments should be 3");

	char* input = argv[1];
	char* output= argv[2];
	
	struct stat st;

	if(stat(input,&st) < 0) {
		err(2,"error. could not stat file1");
	}
	if(st.st_size % sizeof(uint16_t) != 0 ) {
		errx(3,"error file1 is corrupted");
	}
	if(!(st.st_mode & S_IRUSR)) {
		errx(4,"The file is not readable");
	}

	ssize_t in = open(input,O_RDONLY);
	if(in == -1) {
		err(5,"Error open input file");
	}

	ssize_t out = open(output, O_CREAT | O_RDWR | O_TRUNC, S_IRUSR | S_IWUSR);
	if( out == -1) {
		const int _errno = errno;
		close(in);
		errno = _errno;
		err(6,"Error open output file");
	}

	uint32_t counting[0xffff +1] = { 0 };
	uint16_t buf[1<<10];
	ssize_t rd;

	while( (rd = read(in,&buf,sizeof(buf))) > 0) {
		if( rd == -1) {
			const int _errno = errno;
			close(in);
			close(out);
			errno = _errno;
			err(7,"error reading file1");
		}
		for(uint16_t i=0;i < rd/sizeof(uint16_t); i++) {
			counting[buf[i]]++;
		}
	}
	ssize_t wr;
	for(uint32_t i = 0; i <= 0xffff; i++) {
		while(counting[i]--) {
			wr = write(out,&i,sizeof(uint16_t));
			if(wr == -1) {
				const int _errno = errno;
				close(in);
				close(out);
				errno = _errno;
				err(8,"error writing in file2");
			}
		}
	}

	close(in);
	close(out);
	exit(0);

}
