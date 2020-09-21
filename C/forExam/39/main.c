#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <err.h>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <stdint.h>


int cmp(const void* a, const void* b)
{
	if( *((uint32_t*)a) > *((uint32_t*)b) )
		return 1;
	else if( *((uint32_t*)a) < *((uint32_t*)b) )
		return -1;
	return 0;
}

int main(int argc,char** argv)
{
	if(argc != 3) err(1,"error. The arguments should be 2");

	ssize_t fd1 = open(argv[1],O_RDONLY);
	if(fd1 == -1) err(2,"Error open %s",argv[1]);
	
	ssize_t fd2 = open(argv[2],O_CREAT | O_TRUNC | O_RDWR, S_IRUSR | S_IWUSR);
	if(fd2 == -1) {
		const int _errno = errno;
		close(fd1);
		errno = _errno;
		err(3,"Error open %s",argv[2]);
	}

	struct stat st;
	if(stat(argv[1],&st) == -1) {
		const int _errno = errno;
        close(fd1);
		close(fd2);
        errno = _errno;
        err(4,"Error stat %s",argv[1]);
    }
	if(st.st_size % sizeof(uint16_t) != 0) {
		const int _errno = errno;
        close(fd1);
		close(fd2);
        errno = _errno;
        err(5,"Error type of byte in %s",argv[1]);
    }

	uint32_t num = st.st_size / sizeof(uint32_t);
	uint32_t half = num/2;
	uint32_t* m1 = malloc(half*(sizeof(uint32_t)));
	if(!m1) {
		const int _errno = errno;
        close(fd1);
		close(fd2);
        errno = _errno;
        err(6,"Error open first buffer");
    }
	printf("All num: %d\n",num);
	uint32_t half2 = num-half;
	uint32_t* m2 = malloc(half2*sizeof(uint32_t));
	if(!m2) {
        const int _errno = errno;
        close(fd1);
        close(fd2);
		free(m1);
        errno = _errno;
        err(7,"Error open second buffer");
    }

	ssize_t merge1 = open("temp1",O_CREAT | O_TRUNC | O_RDWR, S_IRUSR | S_IWUSR);
	if(merge1 == -1) {
        const int _errno = errno;
        close(fd1);
        close(fd2);
		free(m1);
		free(m2);
        errno = _errno;
        err(8,"Error open temp file1");
    }

	ssize_t merge2 = open("temp2",O_CREAT | O_TRUNC | O_RDWR, S_IRUSR | S_IWUSR);
    if(merge2 == -1) {
        const int _errno = errno;
        close(fd1);
        close(fd2);
        free(m1);
        free(m2);
		close(merge1);
        errno = _errno;
        err(9,"Error open temp file2");
    }

	if(read(fd1,m1,half*sizeof(uint32_t)) != (ssize_t)(half*sizeof(uint32_t))) {
        const int _errno = errno;
        close(fd1);
        close(fd2);
        free(m1);
        free(m2);
        close(merge1);
		close(merge2);
		errno = _errno;
        err(8,"Error open temp file1");
    }

	qsort(m1,half,sizeof(uint32_t),cmp);
	
	if(write(merge1,m1,half*sizeof(uint32_t)) != (ssize_t)(half*sizeof(uint32_t))) {
	    const int _errno = errno;
        close(fd1);
        close(fd2);
       free(m1);
        free(m2);
        close(merge1);
        close(merge2);
        errno = _errno;
        err(8,"Error open temp file1");
    }
	free(m1);

	if(read(fd1,m2,half2*sizeof(uint32_t)) != (ssize_t)(half2*sizeof(uint32_t))) {
        const int _errno = errno;
        close(fd1);
        close(fd2);
        free(m2);
        close(merge1);
        close(merge2);
        errno = _errno;
        err(8,"Error open temp file1");
    }

	qsort(m2,half2,sizeof(uint32_t),cmp);

    if(write(merge2,m2,half2*sizeof(uint32_t)) != (ssize_t)(half2*sizeof(uint32_t))) {
        const int _errno = errno;
        close(fd1);
        close(fd2);
        free(m2);
        close(merge1);
        close(merge2);
        errno = _errno;
        err(8,"Error open temp file1");
    }
    free(m2);

	lseek(merge1,0,SEEK_SET);
	lseek(merge2,0,SEEK_SET);

	uint32_t a,b;
	ssize_t read1 = -1;
	ssize_t read2 = -1;	
	int cntAB = 0;
	int cntA = 0;
	int cntB = 0;

	while( ( (read1 = read(merge1,&a,sizeof(a))) == sizeof(a) ) && ( (read2 = read(merge2,&b,sizeof(b))) == sizeof(b) ) ) {
		if(a <= b) {
			if(write(fd2,&a,sizeof(a)) != sizeof(a)) {
				close(fd1);
				close(fd2);
				close(merge1);
				close(merge2);
				err(11,"error writing in fd2");
			}
			if( lseek(merge2,-1*sizeof(b),SEEK_CUR) == -1) {
                close(fd1);
                close(fd2);
                close(merge1);
                close(merge2);
                err(11,"error lseek");
            }
		} else {
			if( write(fd2,&b,sizeof(b)) != sizeof(b) ) {
                close(fd1);
                close(fd2);
                close(merge1);
                close(merge2);
                err(11,"error writing in fd2");
            }
			if( lseek(merge1,-1*sizeof(a),SEEK_CUR) == -1 ) {
                close(fd1);
                close(fd2);
                close(merge1);
                close(merge2);
                err(11,"error lseek");
            }
		}
		cntAB++;
	}
	if( read1 == sizeof(uint32_t)) {
		if(write(fd2,&a,sizeof(a)) != sizeof(a) ) {
                close(fd1);
                close(fd2);
                close(merge1);
                close(merge2);
                err(11,"error writing in fd2");
        }
		cntA++;
	}
	if (read1 == -1 || read2 == -1) {
                close(fd1);
                close(fd2);
                close(merge1);
                close(merge2);
                err(11,"error writing in fd2");
    }
	
	while( (read1 = read(merge1,&a,sizeof(a))) == sizeof(a) ) {
		write(fd2,&a,sizeof(a));
		cntA++;
	}
	if(read1 == -1) {
                close(fd1);
                close(fd2);
                close(merge1);
                close(merge2);
                err(11,"error writing in fd2");            
    }

	while((read2 = read(merge2,&b,sizeof(b)))  == sizeof(b) ) {
        write(fd2,&b,sizeof(b));
		cntB++;
    }
	if(read2 == -1) {
                close(fd1);
                close(fd2);
                close(merge1);
                close(merge2);
                err(11,"error writing in fd2");            
    }
	
	printf("obshi: %d\nmasiv1: %d\nmasiv2: %d\n",cntAB,cntA,cntB);
	
	unlink("temp1");
	unlink("temp2");

                close(fd1);
                close(fd2);
                close(merge1);
                close(merge2);
      exit(0);    

}

/*

int cmp(const void *a, const void* b){
	if ( *((uint32_t*)a) > *((uint32_t*)b) ) 
		return 1;
	else if ( *((uint32_t*)a) < *((uint32_t*)b) )
		return -1; 
	return 0;
}

int main(int argc, char **argv){
	if(argc != 3)
		errx(1, "Invalid number of arguments. Usage: %s <file.bin> <sorted.bin>", argv[0]);
	
	char *bin = argv[1], *sorted = argv[2];

	struct stat st;
	if(stat(bin, &st) == -1)
		err(2, "error while stat %s", bin);

	if (st.st_size % sizeof(uint32_t) != 0)
		errx(3, "file %s is corrupted", bin);
	
	uint32_t numel = st.st_size / sizeof(uint32_t);
	
	printf("total uint32_t elements: %u\n", numel);

	uint32_t half = numel/2;
	uint32_t *p = malloc(half * sizeof(uint32_t));
	if (!p)
		err(4, "not enough memory");
	
	ssize_t fd1 = open(bin, O_RDONLY);
	if(fd1 == -1){
		free(p);
		err(5, "error while opening file %s", bin);
	}

	ssize_t t1 = open("temp1", O_CREAT | O_RDWR | O_TRUNC , 0644);
	if(t1 == -1){
		const int olderrno = errno;
		free(p);
		close(fd1);
		errno = olderrno;
		err(6, "error while opening file temp1");
	}
	
	ssize_t res = read(fd1, p, half * sizeof(uint32_t));
	if (res != (ssize_t)(half*sizeof(uint32_t))){
		const int olderrno = errno;
		free(p);
		close(fd1);
		close(t1);
		errno = olderrno;
		err(7, "error while reading file %s", bin);
	}
		
	qsort(p, half, sizeof(uint32_t), cmp);	

	res = write(t1, p, half * sizeof(uint32_t));	
	if (res != (ssize_t)(half * sizeof(uint32_t))){
		const int olderrno = errno;
		free(p);
		close(fd1);
		close(t1);
		errno = olderrno;
		err(8, "error while writing to file temp1");
	}
	
	free(p);
	
	uint32_t rhalf = numel-half;
	
	uint32_t *rp = malloc(rhalf * sizeof(uint32_t));
	
	if (!rp){
		const int olderrno = errno;
		close(fd1);
		close(t1);
		errno = olderrno;
		err(9, "not enough memory");
	}
	
	ssize_t t2 = open("temp2", O_CREAT | O_RDWR | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH); // same as 0644
	if(t2 == -1){
		const int olderrno = errno;
		close(fd1);
		close(t1);
		free(rp);
		errno = olderrno;
		err(10, "error while opening file temp2");
	}

	res = read(fd1, rp, rhalf * sizeof(uint32_t));
	if(res != (ssize_t)(rhalf * sizeof(uint32_t))){
		const int olderrno = errno;
		close(fd1);
		close(t1);
		close(t2);
		free(rp);
		errno = olderrno;
		err(11, "error while reading file %s", bin);
	}

	qsort(rp, rhalf, sizeof(uint32_t), cmp);

	res = write(t2, rp, rhalf * sizeof(uint32_t));
	if (res != (ssize_t)(rhalf*sizeof(uint32_t))){
		const int olderrno = errno;
		close(fd1);
		close(t1);
		close(t2);
		free(rp);
		errno = olderrno;
		err(12,"error while writing to file temp2");
	}

	free(rp);
	close(fd1);

	lseek(t1, 0, SEEK_SET); //TODO: check lseek
	lseek(t2, 0, SEEK_SET); //TODO: check lseek
	
	ssize_t fd2 = open(sorted, O_CREAT | O_RDWR | O_TRUNC, 0644);
	if (fd2 == -1){
		const int olderrno = errno;
		close(t1);
		close(t2);
		errno = olderrno;
		err(13,"error while opening file %s", sorted);
	}

	uint32_t a, b;
	size_t d1 = 0, d2 = 0, d3 = 0;

	ssize_t rd1 = -1, rd2 = -1;
	while (((rd1 = read(t1, &a, sizeof(a))) == sizeof(a)) && ((rd2 = read(t2, &b, sizeof(b))) == sizeof(b))) {
		if ( a <= b) {
			write(fd2, &a, sizeof(a)); //TODO: check write
			lseek(t2, -1 * sizeof(b), SEEK_CUR);
			//printf("write from A, turn back B\n");
		} else {
			write(fd2, &b, sizeof(b)); //TODO: check write
			lseek(t1, -1 * sizeof(a), SEEK_CUR);
			//printf("write from B, turn back A\n");
		}
		d1++;
	}
	*/
	/*	printf("out of loop, final:\n");
		printf("rd1: %lu\n", rd1);
		printf("rd2: %lu\n", rd2);
	*/
	
	/*
		Here is the tricky part: if the first reader rd1 read one uint32_t element,
		but the second one did not, it will break the cycle and we will loose one number.
		So we need to check if this is the case and recover that number.
		Notice that there is no other corner case, since there cannot be a case in which
		the second reader rd2 read one uint32_t element, but the first did not - if the
		first condition is not true, then it will not check the second.
	*/
/*
	if (rd1 == sizeof(a)){ 
*/	/* we are out of the cycle and we check if the first reader has captured an element
	   - here is where the real headache hides, if you do not consider it. */
/*		write(fd2, &a, sizeof(a));
		++d2;
	}

	if (rd1 == -1 || rd2 == -1){
		const int olderrno = errno;
		close(t1);
		close(t2);
		close(fd2);
		errno = olderrno;
		err(14, "error while reading from temp files");
	}

	printf("combined write: %ld\n", d1);

	while ((rd1 = read(t1, &a, sizeof(a))) == sizeof(a)) {
		write(fd2, &a, sizeof(a)); //TODO: check write
		d2++;
	}
	if (rd1 == -1){
      		const int olderrno = errno;
      		close(t1);
      		close(t2);
      		close(fd2);
      		errno = olderrno;
      		err(15, "error while reading from file temp1");
  	}
	
	printf("left A write: %ld\n", d2);
	
	while ((rd2 = read(t2, &b, sizeof(b))) == sizeof(b)) {
		write(fd2, &b, sizeof(b)); //TODO: check write
		d3++;
	}
	if (rd2 == -1){
      		const int olderrno = errno;
      		close(t1);
      		close(t2);
      		close(fd2);
      		errno = olderrno;
      		err(16, "error while reading from files temp2");
   	}
	
	printf("left B write: %ld\n", d3);

	// erasing temporary files
	unlink("temp1"); //TODO: check unlink
	unlink("temp2"); //TODO: check unlink
	
	close(t1);
	close(t2);
	close(fd2);
	exit(0);
}


*/




















































