#include "funcs.h"

void set_block(double *A, double *block, int n, int m, int i, int j) {
    int k = n / m;
    int w = j < k ? m : n % m, h = i < k ? m : n % m;
    double *corner = A + i * n * m + j * m;
    for(int x = 0; x < h; x++){
        for(int y = 0; y < w; y++){
            corner[x * n + y] = block[x * w + y];
        }
    }
}

void get_block(double *A, double *block, int n, int m, int i, int j){
    int k = n / m;
    int w = j < k ? m : n % m, h = i < k ? m : n % m;
    double *corner = A + i * n * m + j * m;
    for(int x = 0; x < h; x++){
        for(int y = 0; y < w; y++){
            block[x * w + y] = corner[x * n + y];
        }
    }
}

void set_vector(double *A, double *vector, int n, int m, int i){
    int k = n / m;
    int h = i < k ? m : n % m;
    double *corner = A + i * m;
    for(int x = 0; x < h; x++){
        corner[x] = vector[x];
    }
}

void get_vector(double *A, double *vector, int n, int m, int i){
    int k = n / m;
    int h = i < k ? m : n % m;
    double *corner = A + i * m;
    for(int x = 0; x < h; x++){
        vector[x] = corner[x];
    }
}

double f(int s, int n, int i, int j){
    switch(s){
        case 1:
            return n - max(i , j) + 1;
            break;
        case 2:
            return max(i, j);
            break;
        case 3:
            return absolute(i - j);
            break;
        case 4:
            return 1. / (i + j + 1.);
            break;
        default:
            return -1;
            break;
    }
}

double get_r1(double *A, double *X, double *B, int n){
    double res = 0;
    double norma = 0;
    double sum;

    for(int i = 0; i < n; i++){
        sum = 0;
        for(int j = 0; j < n; j++){
            sum += A[i * n + j] * X[j];
        }
        sum -= B[i];
        norma += absolute(B[i]);
        res += absolute(sum);
    }

    return res / norma;
}

double get_r2(double *X, int n){
    double sum = 0;

    for(int i = 0; i < n; i++){
        sum += absolute(X[i] - ((i + 1) % 2));
    }

    return sum;
}

// struct args_mult{
//     int row;
//     int col;
//     double* A;
//     double* C;
//     double res;
// };

// void* mult(void* args){

// }

void multiply(double *A, double *B, double *C, int rows_a, int cols_a, int rows_b, int cols_b){
	int i, j, k;
	double sum;
	int rows_a3 = rows_a % 3;
	if(cols_a != rows_b) throw "Can't multiply";
	int cols_b3 = cols_b % 3;
	double s00, s01, s02, s10, s11, s12, s20, s21, s22;
	for(i = 0; i < rows_a; i++){
        for(j = 0; j < cols_b; j++){
            C[i * cols_b + j] = 0;
        }
    }
	for(i = 0; i < rows_a3; i++){
		for(j = 0; j < cols_b3; j++){
			sum = 0;
			for(k = 0; k < cols_a; k++){
				sum += A[i * cols_a + k] * B[k * cols_b + j];
			}
			C[i * cols_b + j] = sum;
		}
		for(; j < cols_b; j += 3){
			s00 = 0;
            s01 = 0;
            s02 = 0;
            for(k = 0; k < cols_a; k++){
                s00 += A[i * cols_a + k] * B[k * cols_b + j];
                s01 += A[i * cols_a + k] * B[k * cols_b + j + 1];
                s02 += A[i * cols_a + k] * B[k * cols_b + j + 2];
            }
            C[i * cols_b + j] += s00;
            C[i * cols_b + j + 1] += s01;
            C[i * cols_b + j + 2] += s02;	
		}
	}
	for(; i < rows_a ; i += 3){
		for(j = 0; j < cols_b3; j++){
			s00 = 0;
            s10 = 0;
            s20 = 0;
            for(k = 0; k < cols_a; k++){
                s00 += A[i * cols_a + k] * B[k * cols_b + j];
                s10 += A[(i + 1) * cols_a + k] * B[k * cols_b + j];
                s20 += A[(i + 2) * cols_a + k] * B[k * cols_b + j];

            }
            C[i * cols_b + j] += s00;
            C[(i + 1) * cols_b + j] += s10;
            C[(i + 2) * cols_b + j] += s20;
		}
		for(; j < cols_b; j += 3){
            s00 = 0;
            s01 = 0;
            s02 = 0;
            s10 = 0;
            s11 = 0;
            s12 = 0;
            s20 = 0;
            s21 = 0;
            s22 = 0;
            for(k = 0; k < cols_a; k++){
                s00 += A[i * cols_a + k] * B[k * cols_b + j];
                s01 += A[i * cols_a + k] * B[k * cols_b + j + 1];
                s02 += A[i * cols_a + k] * B[k * cols_b + j + 2];
                s10 += A[(i + 1) * cols_a + k] * B[k * cols_b + j];
                s11 += A[(i + 1) * cols_a + k] * B[k * cols_b + j + 1];
                s12 += A[(i + 1) * cols_a + k] * B[k * cols_b + j + 2];
                s20 += A[(i + 2) * cols_a + k] * B[k * cols_b + j];
                s21 += A[(i + 2) * cols_a + k] * B[k * cols_b + j + 1];
                s22 += A[(i + 2) * cols_a + k] * B[k * cols_b + j + 2];
            }
            C[i * cols_b + j] += s00;
            C[i * cols_b + j + 1] += s01;
            C[i * cols_b + j + 2] += s02;
            C[(i + 1) * cols_b + j] += s10;
            C[(i + 1) * cols_b + j + 1] += s11;
            C[(i + 1) * cols_b + j + 2] += s12;
            C[(i + 2) * cols_b + j] += s20;
            C[(i + 2) * cols_b + j + 1] += s21;
            C[(i + 2) * cols_b + j + 2] += s22;
        }
		
	}
}

void subtract(double *A, double *B, int n, int m){
    //n - количесвто строк
    //m - количество столбцов

    int i, j;

    for(i = 0; i < n; i++){
        for(j = 0; j < m; j++){
            A[i * m + j] -= B[i * m + j];
        }
    }
}

int inverse(double *A, double *C, int n, double norm){
    int i, j, k, row = 0;
    double min, a;
    if(n == 1){
        if(A[0] > 0 || A[0] < 0){
            C[0] = 1 / A[0];
            A[0] = 1;
            return 1;
        }
        else{
            return -1;
        }
    }
    else{
        for(i = 0; i < n; i++){
            for(j = 0; j < n; j++){
                C[i * n + j] = i == j ? 1 : 0;
            }
        }
        // double norm = A[0];
        // for(int i = 0; i < n; i++){
        //     for(int j = 0; j < n; j++){
        //         if(norm < A[i * n + j]) norm = A[i * n + j];
        //     }
        // }
        for(j = 0; j < n; j++){
            min = DBL_MAX;
            for(i = j; i < n; i++){
                if(min > absolute(A[i * n + j]) && absolute(A[i * n + j]) > EPSILON * norm){
                    min = absolute(A[i * n + j]);
                    row = i;
                }
            }
            if(min < DBL_MAX){
                if(row != j){
                    for(i = 0; i < n; i++){
                        a = A[j * n + i];
                        A[j * n + i] = A[row * n + i];
                        A[row * n + i] = a;
                        a = C[j * n + i];
                        C[j * n + i] = C[row * n + i];
                        C[row * n + i] = a;
                    }
                }
                min = A[j * n + j];
                for(i = j; i < n; i++){
                    A[j * n + i] /= min;
                }
                for(i = 0; i < n; i++){
                    C[j * n + i] /= min;
                }
                for(i = 0; i < n; i++){
                    if(i == j) continue;
                    a = A[i * n + j];
                    for(k = 0; k < n; k++){
                        A[i * n + k] -= A[j * n + k] * a;
                    }
                    for(k = 0; k < n; k++){
                        C[i * n + k] -= C[j * n + k] * a;
                    }
                }
            }
            else{
                return -1;
            }
        }
        return 1;
    }
}

void get_free_memb(double *A, double *B, int n){

    int i, j;

    for(i = 0; i < n; i++){
        B[i] = 0;
        for(j = 0; j < n; j += 2)
        B[i] += A[n * i + j];
    }
}

double det(double *A, int n){
    int min = A[0];
    int p = 0;
    double c;
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            if(A[j * n + i] < min){
                min = A[j * n + i];
                p = j;
            }
        }
        for(int j = 0; j < n; j++){
            c = A[p * n + j];
            A[p * n + j] = A[i * n + j];
            A[i * n + j] = c / min;
            for(int k = i + 1; k < n; k++){
               A[k * n + j] = A[k * n + j] - A[k * n] * A[i * n + j]; 
            }
        }
        
    }
    return A[n * n - 1];
}

int max(int i, int j){
    return i > j ? i : j;
}

int absolute(int i){
    return i >= 0 ? i : -i;
}

double absolute(double i){
    return i >= 0 ? i : -i;
}

void E(double *A, int n){
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            A[i * n + j] = i == j ? 1 : 0;
        }
    }
}

void zero(double *A, int n, int m){
    for(int i = 0; i < n; i++){
        for(int j = 0; j < m; j++){
            A[i * m + j] = 0;
        }
    }
}

double get_full_time(){
    struct timeval buf;
    gettimeofday(&buf, NULL);
    return buf.tv_sec + buf.tv_usec / 1.e6;
}
