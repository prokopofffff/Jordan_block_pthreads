#ifndef JORDAN
#define JORDAN

#include "funcs.h"
#include <iostream>
#include <pthread.h>
#include "matrix.h"
#include <sys/resource.h>
#ifdef __linux__
#include <sys/sysinfo.h>
#elif __APPLE__
#include <sys/types.h>
#include <sys/sysctl.h>
#endif
#include <sys/time.h>
#include <time.h>

struct thread_args{
    double* A;
    double* B;
    double* C;
    double* dop_mat;
    double* block;
    double* X;
    int n;
    int m;
    int p;
    double norm;
    int status;
    int index;
    pthread_t tid;
};

int Jordan(double *A, double *B, double *X, double *C, double *block, double *dop_mat, int n, int m, int p);

void* thread_func(void* args);

void* thread_func2(void* args);

void ReduceSum(int p, int* a = nullptr, int n = 0);

#endif