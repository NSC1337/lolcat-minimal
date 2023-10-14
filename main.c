#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int* color_array_init(int delta);
void putchar_rgb(char c, int hex);

int main(int argc, char* argv[])
{
	int delta, offset, *a, i, shift;
	char c;
	FILE *fp;

	delta = 17;
	offset = 1;

	a = color_array_init(delta);

	if (argc == 1)
		fp = stdin;
	else
		fp = fopen(argv[1], "r");

	srand(time(NULL));
	shift = rand() % (6*0xFF/delta);
	i = shift;
	while ((c = getc(fp)) != EOF)
	{
		if (c == '\n')
		{
			shift += offset;
			i = shift;
		}
		putchar_rgb(c, a[i++ % (6*0xFF/delta)]);
	}

	free(a);
	return 0;
}

int* color_array_init(int delta)
{
	int *a, *p, i, cond, clr1, clr2;

	a = malloc(sizeof(int)*6*0xFF/delta);
	if (a == NULL)
	{
		printf("out of space\n");
		exit(1);
	}

	p = a;
	for (cond=0; cond<3; cond++)
	{
		clr1 = 8 * cond;
		clr2 = 8 * ((cond + 1) % 3);
		for (i=0; i<=0xFF; i+=delta)
			*p++ = (0xFF << clr1) + (i << clr2);
		for (i=0xFF-delta; i>0; i-=delta)
			*p++ = (0xFF << clr2) + (i << clr1);
	}
	return a;
}

void putchar_rgb(char c, int hex)
{
	printf("\033[38;2;%d;%d;%dm%c",
			0xFF & hex >> 16,
			0xFF & hex >> 8,
			0xFF & hex >> 0,
			c);
}
