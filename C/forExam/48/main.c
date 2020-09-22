#include<unistd.h>
#include <err.h>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <stdint.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<stdlib.h>

int main(int argc, char** argv) {
	if (argc != 7) errx(1,"Error arguments");

	struct stat st1;
	struct stat st2;
	struct stat st3;
	struct stat st4;
	struct stat st5;

	if( stat(argv[1],&st1) == -1) err(2,"Error stat");
	if( stat(argv[2],&st2) == -1) err(2,"Error stat");
	if( stat(argv[3],&st3) == -1) err(2,"Error stat");
	if( stat(argv[4],&st4) == -1) err(2,"Error stat");
	if( stat(argv[5],&st5) == -1) err(2,"Error stat");

	if( st1.st_size%sizeof(uint64_t) == 0) errx(3,"Error size");
    if( st2.st_size%sizeof(uint32_t) == 0) errx(3,"Error size");
    if( st3.st_size%sizeof(uint8_t) == 0) errx(3,"Error size");
    if( st4.st_size%sizeof(uint16_t) == 0) errx(3,"Error size");
    if( st5.st_size%sizeof(uint64_t) == 0) errx(3,"Error size");

	struct header {
		uint32_t reserved1;
		uint16_t count;
		uint64_t reversed2;
		uint16_t reversed3;
	} a;
	struct complect {
		uint8_t posPost;
		uint8_t lenPost;
		uint8_t posPre;
		uint8_t lenPre;
        uint8_t posIn;
        uint8_t lenIn;
        uint8_t posSuf;
        uint8_t lenSuf;
    } b;
	
	int fd_af = open(argv[1],O_RDONLY);
    int fd_post = open(argv[2],O_RDONLY);
    int fd_pre = open(argv[3],O_RDONLY);
    int fd_in = open(argv[4],O_RDONLY);
    int fd_suf = open(argv[5],O_RDONLY);
    int fd_cruc = open(argv[6],O_CREAT | O_TRUNC | O_RDWR, S_IRUSR | S_IWUSR);

	lseek(fd_post,4,SEEK_SET);
	uint16_t cntPost;
	read(fd_post,&cntPost,sizeof(cntPost));

	lseek(fd_pre,4,SEEK_SET);
	uint16_t cntPre;
	read(fd_pre,&cntPre,sizeof(cntPre));

	lseek(fd_in,4,SEEK_SET);
	uint16_t cntIn;
	read(fd_in,&cntIn,sizeof(cntIn));

	lseek(fd_suf,4,SEEK_SET);
	uint16_t cntSuf;
	read(fd_suf,&cntSuf,sizeof(cntSuf));

	read(fd_af,&a,sizeof(a));

	uint8_t cnt = 1;
	while (cnt <= a.count && read(fd_af,&b,sizeof(b))==sizeof(b)) {
		
		lseek(fd_post,16+(b.posPost-1)*sizeof(uint32_t),SEEK_SET);
		uint32_t* word = malloc(b.lenPost*sizeof(uint32_t));
		if ( (b.posPost+b.lenPost-1) <= cntPost ) {	
			read(fd_post,word,sizeof(word));
			write(fd_cruc,word,sizeof(word));
		}
		free(word);

		lseek(fd_pre, 16 + (b.posPre -1)*sizeof(uint8_t),SEEK_SET);
		uint8_t* word2 = malloc(b.lenPre*sizeof(uint8_t));
		if( (b.posPre + b.lenPre-1) <= cntPre) {
			read(fd_pre,word2,sizeof(word2));
			write(fd_cruc,word2,sizeof(word2));
		}
		free(word2);

		lseek(fd_in,16 + (b.posIn - 1)*sizeof(uint16_t),SEEK_SET);
		uint16_t* word3 = malloc(b.lenIn*sizeof(uint16_t));
		if( (b.posIn + b.lenIn-1) <= cntIn ) {
			read(fd_in,word3,sizeof(word3));
			write(fd_cruc,word3,sizeof(word3));
		}
		free(word3);

		lseek(fd_suf,16 + (b.posSuf-1)*sizeof(uint64_t),SEEK_SET);
		uint64_t* word4 = malloc(sizeof(uint64_t)*b.lenSuf);
		if( (b.posSuf + b.lenSuf-1) <= cntSuf) {
			read(fd_suf,word4,sizeof(word4));
			write(fd_cruc,word4,sizeof(word4));
		}
		free(word4);		
		
		cnt+=4;
	}

	write(fd_cruc,&a,sizeof(a));
	struct stat st;
	stat(argv[6],&st);
	uint16_t size = st.st_size/sizeof(uint8_t) -16;
	lseek(fd_cruc,4*sizeof(uint8_t),SEEK_SET);
	write(fd_cruc,&size,sizeof(size));

	close(fd_af);
	close(fd_post);
	close(fd_pre);
	close(fd_in);
	close(fd_suf);
	close(fd_cruc);
}
