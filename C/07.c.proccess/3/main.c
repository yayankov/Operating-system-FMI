#include <unistd.h>
#include <stdlib.h>
#include <err.h>
#include <stdio.h>

int main()
{
	if (execl("/bin/sleep", "pesho", "60", 0) == -1) {
		err(99, "err execling");
	}
}
