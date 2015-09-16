#include<stdio.h>
#include<stdlib.h>

int main(void)
{
	int *p;
	int size;
	int i;
	
	printf("Enter a size: ");
	scanf("%d", &size);
	
	p = malloc(size);
	
	for(i = 0; i < size; ++i)
	{
		if(i % 10 == 0)
		{
			printf("\n");
		}

		printf("\t%d", i);
	}
	printf("\n");
	
	return 0;
}
