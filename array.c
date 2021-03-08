#include <string.h>

#include "array.h"


void swap(void *a, void *b, unsigned int size)
{
	void *temp = malloc(size);
	memcpy(temp, a, size);
	memcpy(a, b, size);
	memcpy(b, temp, size);

	free(temp);
}


bool array_create(Array *array, unsigned int element_size, unsigned int allocate)
{
	if(element_size == 0 || allocate == 0)
		return false;

	array->count = 0;
	array->_element_size = 0;
	array->_allocated = 0;
	array->buffer_size = allocate;

	array->_array = malloc(allocate * element_size);
	if(array->_array != NULL){
		array->_allocated = allocate;
		array->_element_size = element_size;
		return true;
	}
	else{
		return false;
	}
}

bool array_realloc(Array *array, unsigned int new_allocated)
{
	void *temp = malloc(new_allocated*array->_element_size);
	if(temp == NULL)
		return false;

	memmove(temp, array->_array, array->count * array->_element_size);
	free(array->_array);
	array->_array = temp;
	array->_allocated = new_allocated;

	return true;
}

bool array_extend(Array *array)
{
	return array_realloc(array, array->_allocated + array->buffer_size);
}

void array_free(Array *array)
{
	free(array->_array);
	array->count = 0;
	array->_allocated = 0;
}

void array_copy(Array *to, Array *from)
{
    unsigned int bytes = from->_element_size * from->count;
    to->_array = malloc(bytes);
    memcpy(to->_array, from->_array, bytes);
    to->count = from->count;
    to->_allocated = from->count;
    to->buffer_size = from->buffer_size;
    to->_element_size = from->_element_size;
}


void* array_get_pointer(const Array *array, unsigned int pos)
{
	return (char*)array->_array + pos*array->_element_size;
}

bool array_set(Array *array, unsigned int pos, const void *element)
{
	if(pos >= array->count)
		return false;
	memcpy(array_get_pointer(array, pos), element, array->_element_size);
	return true;
}

bool array_get(const Array *array, unsigned int pos, void *element)
{
	if(pos >= array->count)
		return false;
	memcpy(element, array_get_pointer(array, pos), array->_element_size);
	return true;
}


bool array_fill(Array *array, const void *element)
{
	for(array->count = 1; array->count <= array->_allocated; array->count += 1){
		if(!array_set(array, array->count-1, element))
			return false;
	}
	array->count -= 1;
	return true;
}

bool array_generate(Array *array, unsigned int count, void (*f)(unsigned int, void*))
{
	if(count > array->_allocated)
		return false;

	for(unsigned int i = 0; i < count; ++i){
		f(i, array_get_pointer(array, i));
	}
	array->count = count;
	return true;
}


bool array_push_back(Array *array, void *element)
{
	if(array->count == array->_allocated)
		if(!array_extend(array))
			return false;

	array->count += 1;
	array_set(array, array->count-1, element);
	return true;
}

void array_pop_back(Array *array)
{
	array->count -= 1;
}


void array_map(Array *array, void (*f)(void*))
{
	for(unsigned int i = 0; i < array->count; ++i){
		f(array_get_pointer(array, i));
	}
}

void array_where(Array *array, bool (*h)(void*))
{
	unsigned int last_pos = 0;

	for(unsigned int i = 0; i < array->count; ++i){

		if(h(array_get_pointer(array, i))){
			if(i > last_pos)
				array_set(array, last_pos, array_get_pointer(array, i));
			++last_pos;
		}

	}

	array->count = last_pos;
}

void array_reduce(Array *array, void *start, void (*f)(void*, void*, void*), void* result)
{
	f(array_get_pointer(array, 0), start, result);
	for(unsigned int i = 1; i < array->count; ++i){
		f(array_get_pointer(array, i), result, result);
	}
}


bool array_concat(Array *dest, Array *a, Array *b)
{
	if(dest->_allocated < a->count + b->count)
		if(!array_realloc(dest, a->count + b->count))
			return false;

	if(a != dest){
		memcpy(dest->_array, a->_array, a->count * a->_element_size);
		dest->count = a->count;
	}
	void *s = array_get_pointer(dest, dest->count);
	memcpy(s, b->_array, b->count * b->_element_size);
	dest->count = a->count + b->count;

	return true;
}















