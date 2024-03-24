#include <stdio.h>

void func(int row[4])
{
	for (int i = 0; i < 4; i++)
	{
		printf("%d ", row[i]);
	}
	puts("");
}

void main()
{
	int matrix[4][4] = {
		{1, 2, 3, 4},
		{1, 2, 3, 4},
		{10, 20, 30, 40},
		{1, 2, 3, 4}};
	func(matrix[2]);
}
