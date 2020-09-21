#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <err.h>
#include <unistd.h>

int main() 
{
	int fd1 = open("/etc/passwd", O_RDONLY);
	if ( fd1 == -1 ) {
		err(1, "Cannot open file %s\n","/etc/passwd");
	}

	int fd2 = open("copyfile.txt", O_CREAT | O_WRONLY | O_TRUNC, S_IRUSR | S_IWUSR);
	if ( fd2 == -1 ) {
   		close(fd1);
	    err(2, "Cannot open file %s\n","copyfile.txt");
    }
	
	char c;

	while ( read(fd1, &c, 1) ) {
		if ( c == ':' ) {
			c = '?';
		}
		if ( write(fd2, &c, 1) != 1)
		{
			close(fd1);
		    close(fd2);
			err(3, "error while writing %s\n","copyfile.txt");
	}

	close(fd1);
	close(fd2);
	exit(0);
	
}
