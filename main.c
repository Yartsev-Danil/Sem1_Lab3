#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


#include "array.h"


int main()
{

	Array *array = malloc(sizeof(Array));
	//array->_buffer_size = 5;
	array_create(array, sizeof(int), 5);
	{
		int t = 15;
		array_fill(array, &t);
	}


	{
		int t = 0;

		t = 19;
		array_push_back(array, &t);
		t = 21;
		array_push_back(array, &t);

		for(unsigned int i = 0; i < array->count; ++i){
			array_get(array, i, &t);
			printf("%d\n", t);
		}
	}


	return 0;
}












