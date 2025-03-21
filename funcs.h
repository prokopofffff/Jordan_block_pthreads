#ifndef FUNCTIONS
#define FUNCTIONS

#include <cstring>
#include <float.h>
#include <iostream>
#include <sys/time.h>
#include <sys/resource.h>
#ifdef __linux__
#include <sys/sysinfo.h>
#elif __APPLE__
#include <sys/types.h>
#include <sys/sysctl.h>
#endif
#include "matrix.h"

#define EPSILON 1.0e-14

void set_block (double *A, double *block, int n, int m, int i, int j);

void get_block (double *A, double *block, int n, int m, int i, int j);

void set_vector(double *A, double *vector, int n, int m, int i);

void get_vector(double *A, double *vector, int n, int m, int i);

double f(int s, int n, int i, int j);

double get_r1(double *A, double *X, double *B, int n);

double get_r2(double *X, int n);

void multiply(double *A, double *B, double *C, int rows_a, int cols_a, int rows_b, int cols_b);

void subtract(double *A, double *B, int n, int m);

int inverse(double *A, double *C, int n, double norm);

void get_free_memb(double *A, double *B, int n);

double det(double *A, int n);

int max(int i, int j);

int absolute(int i);

double absolute(double i);

void E(double *A, int n);

void zero(double *A, int n, int m);

double get_full_time();

#endif