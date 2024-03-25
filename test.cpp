#include <iostream>
#include "funcs.h"

int main(){
    double* A = new double[16];
    double* B = new double[16];
    A[0] = 0.8;
    A[1] = 0.6;
    A[2] = 0.4;
    A[3] = 0.6;
    A[4] = 1.2;
    A[5] = 0.8;
    A[6] = 0.4;
    A[7] = 0.8;
    A[8] = 1.2;

    inverse(A, B, 3);
    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++){
            std::cout << B[i * 3 + j] << " ";
        }
        std::cout << std::endl;
    }
}