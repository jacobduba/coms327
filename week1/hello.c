#include <stdio.h>

int main(int argc, char *argv[])
{
	/* printf("%d: %s\n", argc, argv[0]); */

	int i;

	for (i = 1; i < argc; i++)
	{
		printf("Hello %s\n", argv[i]);
	}

	if (argc == 1)
	{
		printf("Hello World!\n");
	}

	return 0;
}
