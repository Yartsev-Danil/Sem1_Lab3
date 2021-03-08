#ifndef POLYNOMIAL_H
#define POLYNOMIAL_H

#include "array.h"

void double_add(double *res, double *a, double *b);;
void double_subtract(double *res, double *a, double *b);;
void double_mul(double *res, double *a, double *b);

typedef struct {
    double real, imaginary;
} DComplex_t;

void dcompelx_add(DComplex_t *res, DComplex_t *a, DComplex_t *b);
void dcomplex_subtract(DComplex_t *res, DComplex_t *a, DComplex_t *b);
void dcomplex_mul(DComplex_t *res, DComplex_t *a, DComplex_t *b);

typedef struct {

    Array *coefficients;
    void (*add_func)(void*, void*, void*);
    void (*sub_func)(void*, void*, void*);
    void (*mul_func)(void*, void*, void*);
    void *zero, *one;

} Polynomial;

bool polynomial_create(unsigned int size, unsigned int element_size, Polynomial *p, void(*add_func)(void*, void*, void*), void (*sub_func)(void*, void*, void*), void (*mul_func)(void*, void*, void*), void* zero, void* one);
void polynomial_free(Polynomial *p);

void polynomial_copy(Polynomial *to, Polynomial *from);
bool double_polynomial_create(unsigned int size, Polynomial* p);
bool complex_polynomial_create(unsigned int size, Polynomial* p);

void polynomial_add(Polynomial *p_res, Polynomial *p1, Polynomial *p2);
void polynomial_subtract(Polynomial *p_res, Polynomial *p1, Polynomial *p2);

void polynomial_substitute(Polynomial *p, void *y, void *x);

#endif //POLYNOMIAL_H
