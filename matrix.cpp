#include "matrix.h"
#include <fstream>
#include <iostream>
#include <typeinfo>
#include <iomanip>

void matrix_input(char *filename, double *matrix, int n){
    std::ifstream file(filename);

    if(!(file.is_open())){
        delete[] matrix;
        throw "Can not open file";
    }

    int count = 0;

    while(file){
        if(!(file >> matrix[count])){
            delete[] matrix;
            throw "Incorrect type";
        }
        count++;
        if(count == n * n) break;
    }

    if(count < n * n){
        delete[] matrix;
        throw "Not enough data";
    }

    file.close();
}

void matrix_output(double *matrix, int l, int n, int r){
    for(int i = 0; i < l && i < r; i++){
        for(int j = 0; j < n && j < r; j++){
            std::cout << std::scientific << matrix[i * n + j] << " ";
        }
        std::cout << std::endl;
    }
}
