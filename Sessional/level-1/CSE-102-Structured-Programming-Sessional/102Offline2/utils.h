#include <stdio.h>
#include <stdlib.h>

typedef struct Color 
{
	int R;
	int G;
	int B;
} Color;


typedef struct Image
{
	Color **image;
	int rows;
	int cols;
} Image;
