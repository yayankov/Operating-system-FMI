#include<stdlib.h>
#include<stdio.h>
#include<stdint.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<err.h>
#include<errno.h>

int main(int argc, char** argv) {

	if (argc != 4) errx(1,"error arguments");
	
	struct header {
		uint32_t magic;
		uint8_t header_version;
		uint8_t data_version;
		uint16_t count;
		uint32_t reserved1;
		uint32_t reserved2;
	} __attribute__((packed));
	struct header a;

	struct version0x00 {
		uint16_t offset;
		uint8_t original_byte;
		uint8_t new_byte;
	} b;

	struct version0x01 {
		uint32_t offset2;
		uint16_t original_word;
		uint16_t new_word;
	} c;

	int fd = open(argv[1],O_RDONLY);
	int fd2 = open(argv[2],O_RDONLY);
	int fd3 = open(argv[3],O_CREAT | O_TRUNC | O_RDWR, S_IRUSR | S_IWUSR);

	uint8_t symbol;
	while(read(fd2,&symbol,sizeof(symbol)) == sizeof(symbol)) {
		write(fd3,&symbol,sizeof(symbol));
	}

	read(fd,&a,sizeof(a));
	if(a.magic != 0xEFBEADDE) errx(5,"Error magic header");
	if(a.header_version != 0x01) errx(6,"Error header version is not 0x01");
	if(a.data_version == 0x00) {
		int cnt = 1;
		while(cnt <= a.count && read(fd,&b,sizeof(b)) == sizeof(b)) {
			lseek(fd3,b.offset*sizeof(uint8_t),SEEK_SET);
			uint8_t byte;
			read(fd3,&byte,sizeof(byte));
			if( b.original_byte == byte) {
				lseek(fd3,-1*sizeof(uint8_t),SEEK_CUR);
				write(fd3,&b.new_byte,sizeof(b.new_byte));
			} else {
				err(4,"ERROR patch format");	
			}
			cnt++;
		}
		
	} else if (a.data_version == 0x01) {
		int cnt = 1;
        while(cnt <= a.count && read(fd,&c,sizeof(c)) == sizeof(c)) {
            lseek(fd3,c.offset2*sizeof(uint16_t),SEEK_SET);
            uint16_t word;
            read(fd3,&word,sizeof(word));
            if( c.original_word == word) {
                lseek(fd3,-1*sizeof(uint16_t),SEEK_CUR);
                write(fd3,&c.new_word,sizeof(c.new_word));
            } else {
                err(5,"ERROR patch2 format");
            }
            cnt++;
        }
	} else {
		err(10,"Error data_version in header");	
	}
	exit(0);
}
