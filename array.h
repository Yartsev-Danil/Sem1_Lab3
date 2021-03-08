#ifndef ARRAY_H
#define ARRAY_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct {

	unsigned int count;
	unsigned int buffer_size;

	void *_array;
	unsigned int _allocated;
	unsigned int _element_size;

} Array;

bool array_create(Array *array, unsigned int element_size, unsigned int allocate);
bool array_realloc(Array *array, unsigned int new_allocated);
bool array_extend(Array *array);
void array_free(Array *array);

void array_copy(Array *to, Array *from);

void* array_get_pointer(const Array *array, unsigned int pos);
bool array_set(Array *array, unsigned int pos, const void *element);
bool array_get(const Array *array, unsigned int pos, void *element);

bool array_fill(Array *array, const void *element);
bool array_generate(Array *array, unsigned int count, void (*f)(unsigned int, void*));

bool array_push_back(Array *array, void *element);
void array_pop_back(Array *array);

void array_map(Array *array, void (*f)(void *));
void array_where(Array *array, bool (*h)(void*));
void array_reduce(Array *array, void *start, void (*f)(void*, void*, void*), void* result);

bool array_concat(Array *dest, Array *a, Array *b);

#endif // ARRAY_H
