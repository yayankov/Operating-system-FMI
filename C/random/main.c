#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
       #include <unistd.h>

int main()
{
	execl("/bin/date","date",(char*)NULL);

	printf("IN\n");
}


