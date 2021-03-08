#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>
#include <string.h>

#include "array.h"
#include "polynomial.h"
#include "test.h"


void generate_function(unsigned int pos, int *val)
{
	*val = pos*pos;
}

void sqr(int *val)
{
	*val = *val * *val;
}

bool positive_filter(int *a){
	return *a > 0;
}

void reducer(int *a, int *b, int *r){
	*r = 2 * (*a) + 3 * (*b);
}

bool less(int *a, int *b)
{
	return *a < *b;
}


unsigned int get_random_size(){
	srand(time(NULL));
	return abs(100 + rand() % 10000);
}

Error test_array_create()
{
	Array *array = malloc(sizeof(Array));

	if(array_create(array, 0, 0))
		return ERROR;

	unsigned int n = get_random_size();

	if(!array_create(array, 49, n))
		return ERROR;
	if(array->_allocated != n)
		return ERROR;

	array_free(array);
	return OK;
}

Error test_array_realloc()
{
	Array *array = malloc(sizeof(Array));
	array_create(array, 1, 10);
	unsigned int n = get_random_size();

	for(unsigned short i = 0; i < 3; ++i){
		if(!array_realloc(array, i*n))
			return ERROR;
	}

	array_free(array);
	return OK;
}

Error test_array_get_set_fill()
{
	Array *array = malloc(sizeof(Array));

	int p = 65578;

	array_create(array, sizeof(int), get_random_size());
	if(!array_fill(array, &p))
		return ERROR;

	for(unsigned int i = 0; i < array->count; ++i){
		if(!array_set(array, i, &i))
			return ERROR;
	}

	for(unsigned int i = 0; i < array->count; ++i){
		if(!array_get(array, i, &p) || p != i)
			return ERROR;
	}

	array_free(array);
	return OK;
}

Error test_array_generate()
{
	Array *array = malloc(sizeof(Array));
	unsigned int n = get_random_size();
	array_create(array, sizeof(int), n);

	if(array_generate(array, n+1, generate_function))
		return ERROR;
	if(!array_generate(array, n, generate_function))
		return ERROR;

	int p = 0;
	for(unsigned int i = 0; i < n; ++i){
		if(!array_get(array, i, &p) || p != i*i)
			return ERROR;
	}

	array_free(array);
	return OK;
}

Error test_array_push_back()
{
	Array *array = malloc(sizeof(Array));
	unsigned int n = get_random_size();
	array_create(array, sizeof(int), 2);
	int t = 0;

	for(unsigned int i = 0; i < n; ++i){
		t = i*i;
		if(!array_push_back(array, &t))
			return ERROR;
		array_get(array, i-1, &t);

	}

	for(unsigned int i = 0; i < n; ++i){
		array_get(array, i, &t);
		if(t != i*i)
			return ERROR;
	}

	array_free(array);
	return OK;
}

Error test_array_map()
{
	Array *array = malloc(sizeof(Array));
	unsigned int n = get_random_size();
	array_create(array, sizeof(int), n);

	int t = 2000;
	array_fill(array, &t);
	array_map(array, sqr);

	for(unsigned int i = 0; i < array->count; ++i){
		array_get(array, i, &t);
		if(t != 2000*2000)
			return ERROR;
	}

	array_free(array);
	return OK;
}

Error test_array_where()
{
	Array *array = malloc(sizeof(Array));
	unsigned int n = get_random_size();
	array_create(array, sizeof(int), n);

	int t = -1;
	array_fill(array, &t);
	array_where(array, positive_filter);
	if(array->count > 0)
		return ERROR;

	for(unsigned int i = 0; i < n; ++i){
		if(i < floorf(n / 2))
			t = 1;
		else
			t = -1;

		array_push_back(array, &t);
	}

	array_where(array, positive_filter);
	if(array->count != floor(n/2))
		return ERROR;

	array_free(array);
	return OK;
}

Error test_array_reduce()
{
	Array *array = malloc(sizeof(Array));
	array_create(array, sizeof(int), 10);

	int t = -1;
	for(unsigned int i = 1; i <= 3; ++i)
		array_push_back(array, &i);
	t = 4;
	array_reduce(array, &t, reducer, &t);

	if(t != 144)
		return ERROR;

	array_free(array);
	return OK;
}

Error test_array_concat()
{
	Array *a = malloc(sizeof(Array));
	unsigned int n = get_random_size();
	array_create(a, sizeof(int), n);

	Array *b = malloc(sizeof(Array));
	unsigned int k = get_random_size();
	array_create(b, sizeof(int), k);

	int t = 10;
	array_fill(a, &t);
	t = 20;
	array_fill(b, &t);

	array_concat(a, a, b);

	if(a->count != n + k)
		return ERROR;

	for(unsigned int i = 0; i < a->count; ++i){
		array_get(a, i, &t);
		if((i < n && t != 10) || (i >= n && t != 20))
			return ERROR;
	}

	return OK;
}

Error test_polynomial()
{
    Polynomial *p = malloc(sizeof(Polynomial));
    complex_polynomial_create(1, p);
    array_realloc(p->coefficients, 3);
    array_fill(p->coefficients, p->zero);

    DComplex_t num, num1;
    for(unsigned int i = 0; i < 3; ++i)
    {
        num.real = i;
        num.imaginary = 0;
        array_set(p->coefficients, i, &num);
    }
    //p(x) = x + 2x^2

    num.real = 2;
    polynomial_substitute(p, &num1, &num);
    if(num1.real != 10 || num1.imaginary != 0)
        return ERROR;
    //p(2) = 2 + 2*2^2 = 10

    Polynomial *p2 = malloc(sizeof(Polynomial));
    complex_polynomial_create(1, p2);
    array_realloc(p2->coefficients, 5);
    array_fill(p2->coefficients, p2->zero);

    for(unsigned int i = 0; i < 5; ++i)
    {
        num.real = 0;
        num.imaginary = 2.0*i;
        array_set(p2->coefficients, i, &num);
    }
    //p2(x) = 2jx + 4jx^2 + 6jx^3 + 8jx^4

    Polynomial p3;
    complex_polynomial_create(1, &p3);
    polynomial_add(&p3, p, p2);
    //p3(x) = (1+2j)x + (2+4j)x^2 + 6jx^3 + 8jx^4

    num.real = 1;
    num.imaginary = 0;
    polynomial_substitute(&p3, &num1, &num);
    if(num1.real != 3 || num1.imaginary != 20)
        return ERROR;


    polynomial_subtract(&p3, p, p2);
    //p3(x) = (1-2j)x + (2-4j)x^2 - 6jx^3 - 8jx^4

    polynomial_substitute(&p3, &num1, &num);
    if(num1.real != 3 || num1.imaginary != -20)
        return ERROR;

    polynomial_free(p);
    polynomial_free(p2);
    free(p);
    free(p2);
    return OK;
}

int main(){

	test_function functions[] = {
		{test_array_create, "array_create"},
		{test_array_realloc, "array_realloc"},
		{test_array_get_set_fill, "array_get_set_fill"},
		{test_array_generate, "array_generate"},
		{test_array_push_back, "array_push_back"},
		{test_array_map, "array_map"},
		{test_array_where, "array_where"},
		{test_array_reduce, "array_reduce"},
		{test_array_concat, "array_concat"},
        {test_polynomial, "test_polynomial"}
	};
	const unsigned short n = sizeof(functions) / sizeof (test_function);
	unsigned short errors = 0;

	for(unsigned short i = 0; i < n; ++i){
		printf("[%d/%d] Test %s: ", i+1, n, functions[i].name);

		Error result = functions[i].function();
		if(result == ERROR){
			errors += 1;
			printf("ERROR!");
		}
		else{
			printf("OK");
		}

		printf("\n");
	}

	printf("\n\nTotal: tests: %d, errors: %d\n", n, errors);
	if(errors == 0)
		printf("ALL OK\n");
}












