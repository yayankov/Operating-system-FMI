

int main(int argc, char* argv[])
{
	if(argc < 3) {
		errx(1,"error arguments");
	}
	char* dirpath = argv[argc-1];

	int fd_dir = open(dirpath,O_DIRECTORY);
	if(fd_dir == -1) {
		err(2,"error open directory");
	}

	int wr=access(dirpath, W_OK);
	if(wr == -1) {
		err(3,"dir not writable");
	}
	
	for(int i=1; i<argc-1; i++) {
		
	}	
	
}
