
#include<stdlib.h>
#include<stdio.h>
#include<err.h>
#include<unistd.h>

int main(int argc, char** argv)
{
	if(argc != 2) errx(1,"error arguments");
	if(execlp("ls","ls",argv[1],(char*)0) == -1) err(2,"error ls");
}
