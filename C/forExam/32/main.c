

int main(int argc, char** argv)
{
	if(argc != 4) errx(1,"Error. The arguments should be 3");

	struct stat st;
	if(stat(argv[1],&st) == -1) err(2,"Error stat file 1");
	if(st.st_size%sizeof(uint32_t) != 0) errx(3,"Error stat file 1");
	
	struct stat st2;
    if(stat(argv[2],&st2) == -1) err(4,"Error stat file 2");
    if(st2.st_size%sizeof(uint32_t) != 0) errx(5,"Error stat file 2");

	ssize_t fd1 = open(argv[1],O_RDONLY);
	if(fd1 == -1) err(6,"Error open file 1");

	ssize_t fd2 = open(argv[2],O_RDONLY);
	if(fd2 == -1) {
		const int _errno = errno;
		close(fd1);
		errno = _errno;
		err(7,"Error open file 2");
	}

	ssize_t fd3 = open(argv[3],O_CREAT | O_TRUNC | O_RDWR, S_IRUSR | S_IWUSR);
	if(fd3 == -1) {
		const int _errno = errno;
		close(fd1);
		close(fd2);
		errno = _errno;
		err(8,"Error open file 3");
	}

	struct double {
		uint32_t pos;
		uint32_t len;
	};	

	struct double a;

	while(read(fd1,&a,sizeof(a)) == sizeof(a)) {
		if(lseek(fd2,(a.pos-1)*sizeof(uint32_t),SEEK_SET) == -1) {
			close(fd1);
			close(fd2);
			close(fd3);
			err(9,"Error lseek");
		}
		uint32_t* buf = malloc(a.len);

		while(read(f1,&buf,sizeof(buf)) == sizeof(buf)) {
			
			write(f3,&buf,sizeof(buf);
		}
	}

	exit(0);
}


























