#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[]) 
{
	printf("System page size = %ld\n", sysconf(_SC_PAGESIZE));

	return 0;
}
