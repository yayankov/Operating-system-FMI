#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <err.h>
#include <string.h>
       #include <sys/types.h>
       #include <sys/stat.h>
       #include <fcntl.h>
       #include <unistd.h>


int main(int argc, char* argv[])
{
	if ( argc != 3 ) {
		errx(1, "Wrong number of arguments");
	}
	char* cmd = argv[1];	

	if( !( strcmp(cmd,"--print")==0 || strcmp(cmd,"--min")==0 || strcmp(cmd,"--max")==0 ) ) {
		errx(2, "Argument 1 is not [--min,--max or --print]");
	}

	char* binary = argv[2];

	struct stat st;
	
	if(stat(binary,&st)==-1) {
		err(3, "Error stat");
	}

	if(st.st_size % sizeof(uint16_t) != 0) {
		errx(4, "Doesnt contain only uint16_t");
	}
	
	if(access(binary,F_OK | R_OK) == -1) {
		err(5, "error not access");
	}		

	uint16_t* nums = malloc(st.st_size);

	if (nums == NULL) {
		err(6, "not enough memory");
	}

	int fd = open(binary, O_RDONLY); 
	if (fd == -1) {
		free(nums);
		err(7,"error opening");
	}
	uint16_t c;
	ssize_t read_size;
	int i=0;	
	
	while((read_size = read(fd,&c,sizeof(uint16_t))) > 0) {
		if(read_size != sizeof(uint16_t) || read_size == -1) {
			free(nums);
			close(fd);
			err(8,"error reading");
		}
		nums[i++] = c;
	}
	uint16_t min = nums[0];
	uint16_t max = nums[0];
	
	for(int j=0; j<i; j++) {
		if((strcmp(argv[1],"--print"))==0) printf("%d\n",nums[j]);
		if(max < nums[j]) max = nums[j];        
		if(min > nums[j]) min = nums[j];
    }
	if((strcmp(argv[1],"--min"))==0) printf("%d\n",min);
    if((strcmp(argv[1],"--max"))==0) printf("%d\n",max);
		
	free(nums);
	close(fd);
	exit(0);

}



























