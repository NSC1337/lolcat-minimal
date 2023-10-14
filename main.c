#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <time.h>

int hue_to_rgb(float h);
void putchar_rgb(char c, int hex);
int* color_array_init();
void lolcat(FILE* fp);


int n = 240, offset = 4, color_index, *colors;

int main(int argc, char **argv)
{
	char c;
	while ((c = getopt(argc, argv, "n:o:s")) != -1)
		switch (c)
		{
			case 'n':
				n = atoi(optarg);
				break;
			case 'o':
				offset = atoi(optarg);
				break;
			case 's':
				n = 45;
				offset = 1;
				break;
		}
	srand(time(NULL));
	color_index = rand();
	colors = color_array_init();


	FILE* fp;
	if (argc == optind)
		lolcat(stdin);
	else
		for (int i = optind; i < argc; i++)
		{
			fp = fopen(argv[i], "r");
			if (fp == NULL)
			{
				printf("%s: %s is an invalid file\n", argv[0],argv[i]);
				break;
			}
			lolcat(fp);
			fclose(fp);	
		}

	free(colors);
	return 0;
}


int hue_to_rgb(float h)
{
	float hprime = h / 60.0;
	int x = 255 * (1 - fabsf(fmodf(hprime, 2) - 1));

	switch ((int) hprime)
	{
		case 0: 
			return 0xFF0000 + (x << 8);
			break;
		case 1:
			return 0x00FF00 + (x << 16);
			break;
		case 2:
			return 0x00FF00 + (x << 0);
			break;
		case 3:
			return 0x0000FF + (x << 8);
			break;
		case 4:
			return 0x0000FF + (x << 16);
			break;
		case 5:
			return 0xFF0000 + (x << 0);
			break;
	}
}


void putchar_rgb(char c, int hex)
{
        printf("\033[38;2;%d;%d;%dm%c",
                        0xFF & hex >> 16,
                        0xFF & hex >> 8,
                        0xFF & hex >> 0,
                        c);
}

int* color_array_init()
{
	int *a, *p;
	float I;

	a = malloc(sizeof(int) * n);
	if (a == NULL)
	{
		printf("out of memory");
		exit(0);
	}

	p = a;
	for (I = 0; I < 360; I += 360.0/n)
		*p++ = hue_to_rgb(I);

	return a;
}

void lolcat(FILE* fp)
{
	int shift = color_index;
	char c;
	while ((c = getc(fp)) != EOF)
	{
		if (c == '\n')
		{
			shift += offset;
			color_index = shift;
		}
		putchar_rgb(c, colors[color_index++ % n]);
	}
}
