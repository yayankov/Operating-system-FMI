//date
       #include <unistd.h>
#include <stdlib.h>
#include<err.h>
#include<stdio.h>
int main()
{
	if(execlp("date","date",(char*)0) == -1) errx(1,"Err exelp");
}
