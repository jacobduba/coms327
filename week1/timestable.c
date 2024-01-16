#include <stdio.h>

#define MAX_INDEX 12

void times_table(int a[MAX_INDEX][MAX_INDEX])
{
	int i, j;
	for (j = 0; j < MAX_INDEX; j++) {
		for (i = 0; i < MAX_INDEX; i++) {
			a[j][i] = (i + 1) * (j + 1);
		}
 	}
}

int main(int argc, char *argv[])
{
	/* 2D array */
	int a[MAX_INDEX][MAX_INDEX];

	int i, j;

	times_table(a);

	for (j = 0; j < 12; j++) {
		for (i = 0; i < 12; i++) {
			printf("%3d ", a[j][i]);
		}
		printf("\n");
  }

	return 0;
}
