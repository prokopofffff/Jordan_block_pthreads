#include "jordan.h"

void ReduceSum(int p, int* a, int n) {
    static pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
    static pthread_cond_t c_in = PTHREAD_COND_INITIALIZER;
    static pthread_cond_t c_out = PTHREAD_COND_INITIALIZER;
    static int t_in = 0;
    static int t_out = 0;
    static int* r = nullptr;
    int i;
    if (p <= 1) return;
    pthread_mutex_lock(&m);
    if (r == nullptr) r = a;
    else for(i = 0; i < n; ++i) r[i] += a[i];
    ++t_in;
    if (t_in >= p) {
        t_out = 0;
        pthread_cond_broadcast(&c_in);
    } else {
        while (t_in < p) {
            pthread_cond_wait(&c_in, &m);
        }
    }
    if (r != a) {
        for (i = 0; i < n; ++i) a[i] = r[i];
    }
    ++t_out;
    if (t_out >= p) {
        t_in = 0;
        r = nullptr;
        pthread_cond_broadcast(&c_out);
    } else {
        while (t_out < p) {
            pthread_cond_wait(&c_out, &m);
        }
    }
    pthread_mutex_unlock(&m);
}

void* thread_func(void* args){
    thread_args* a = (thread_args*)args;
    int k = a->n / a->m;
    int l = a->n % a->m;
    int h = (l == 0 ? k : k + 1);
    int red = 0;

    for(int i = 0; i < h; i++){
        int kp = (h - i - 1) / a->p;
        int lp = (h - i - 1) % a->p;
        get_block(a->A, a->block, a->n, a->m, i, i);
        a->status = -1;
        a->status = inverse(a->block, a->C, i != k ? a->m : l, a->norm);
        ReduceSum(a->p, &red, 1);
        if(a->status == -1) return 0;
        set_block(a->A, a->block, a->n, a->m, i, i);
        for(int x = 0; x < kp; x++){
            int s = i + x * a->p + a->index;
            get_block(a->A, a->block, a->n, a->m, i, s);
            multiply(a->C, a->block, a->dop_mat, i != k ? a->m : l, i != k ? a->m : l, i != k ? a->m : l, s != k ? a->m : l);
            set_block(a->A, a->dop_mat, a->n, a->m, i, s); 
        }
        if(a->index <= lp){
            int s = i + kp * a->p + a->index;
            get_block(a->A, a->block, a->n, a->m, i, s);
            multiply(a->C, a->block, a->dop_mat, i != k ? a->m : l, i != k ? a->m : l, i != k ? a->m : l, s != k ? a->m : l);
            set_block(a->A, a->dop_mat, a->n, a->m, i, s);
        }
        if(a->index == a->p){
            get_vector(a->B, a->X, a->n, a->m, i);
            multiply(a->C, a->X, a->dop_mat, i != k ? a->m : l, i != k ? a->m : l, i != k ? a->m : l, 1);
            set_vector(a->B, a->dop_mat, a->n, a->m, i);
            
        }
        
        // ReduceSum(a->p, &red, 1);

        for(int y = 0; y < h; y++){
            if(y == i) continue;
            get_block(a->A, a->block, a->n, a->m, y, i);
            //ReduceSum(a->p, &red, 1);
            for(int x = 0; x < kp; x++){
                int s = i + x * a->p + a->index;
                get_block(a->A, a->C, a->n, a->m, i, s);
                multiply(a->block, a->C, a->dop_mat, (y != k ? a->m : l), a->m, a->m, (s != k ? a->m : l));
                get_block(a->A, a->C, a->n, a->m, y, s);
                subtract(a->C, a->dop_mat, (y != k ? a->m : l), (s != k ? a->m : l));
                set_block(a->A, a->C, a->n, a->m, y, s);
            }
            if(a->index <= lp){
                int s = i + kp * a->p + a->index;
                get_block(a->A, a->C, a->n, a->m, i, s);
                multiply(a->block, a->C, a->dop_mat, (y != k ? a->m : l), a->m, a->m, (s != k ? a->m : l));
                get_block(a->A, a->C, a->n, a->m, y, s);
                subtract(a->C, a->dop_mat, (y != k ? a->m : l), (s != k ? a->m : l));
                set_block(a->A, a->C, a->n, a->m, y, s);
            }
            if(a->index == a->p){
                get_vector(a->B, a->X, a->n, a->m, i);
                multiply(a->block, a->X, a->dop_mat, (y != k ? a->m : l), i != k ? a->m : l, i != k ? a->m : l, 1);
                get_vector(a->B, a->X, a->n, a->m, y);
                subtract(a->X, a->dop_mat, (y != k ? a->m : l), 1);
                set_vector(a->B, a->X, a->n, a->m, y);
                //get_block(a->A, a->block, a->n, a->m, y, i);
                //zero(a->block, (y != k ? a->m : l), i != k ? a->m : l);
                //set_block(a->A, a->block, a->n, a->m, y, i);
            }
            // ReduceSum(a->p, &red, 1);
        }
        ReduceSum(a->p, &red, 1);
    }

    return 0;
}

int Jordan(double *A, double *B, double *X, double *C, double *block, double *dop_mat, int n, int m, int p){
    int i, j, s;
    int k = n / m;
    int l = n % m;
    int h = (l == 0 ? k : k + 1);
    int flag;
    double norm = A[0];

    for(i = 0; i < n; i++){
        for(j = 0; j < n; j++){
            if(norm < A[i * n + j]) norm = A[i * n + j];
        }
    }
    if(p == 1){
        for(p = 0; p < h; p++){
            get_block(A, block, n, m, p, p);
            flag = -1;
            flag = inverse(block, C, p != k ? m : l, norm);
            if(flag == -1) return -1;
            set_block(A, block, n, m, p, p);
            for(s = p + 1; s < h; s++){
                get_block(A, block, n, m, p, s);
                multiply(C, block, dop_mat, p != k ? m : l, p != k ? m : l, p != k ? m : l, s != k ? m : l);
                set_block(A, dop_mat, n, m, p, s);
            }
            
            get_vector(B, X, n, m, p);
            multiply(C, X, dop_mat, p != k ? m : l, p != k ? m : l, p != k ? m : l, 1);
            set_vector(B, dop_mat, n, m, p);

            for(i = 0; i < h; i++){
                if(i == p) continue;
                get_block(A, block, n, m, i, p);
                for(j = p + 1; j < h; j++){
                    
                    get_block(A, C, n, m, p, j);
                    multiply(block, C, dop_mat, (i != k ? m : l), m, m, (j != k ? m : l));
                    get_block(A, C, n, m, i, j);
                    subtract(C, dop_mat, (i != k ? m : l), (j != k ? m : l));
                    set_block(A, C, n, m, i, j);
                }
                get_vector(B, X, n, m, p);
                multiply(block, X, dop_mat, (i != k ? m : l), p != k ? m : l, p != k ? m : l, 1);
                get_vector(B, X, n, m, i);
                subtract(X, dop_mat, (i != k ? m : l), 1);
                set_vector(B, X, n, m, i);
                get_block(A, block, n, m, i, p);
                zero(block, (i != k ? m : l), p != k ? m : l);
                set_block(A, block, n, m, i, p);
            }
        }

        for(i = 0; i < n; i++){
            X[i] = B[i];
        }
    }
    else{
        pthread_t* threads = new pthread_t[p];
        thread_args* args = new thread_args[p];

        for(int x = 0; x < p; x++){
            args[x].A = A;
            args[x].n = n;
            args[x].m = m;
            args[x].B = B;
            args[x].C = new double[m * m];
            args[x].dop_mat = new double[m * m];
            args[x].block = new double[m * m];
            args[x].X = new double[m];
            args[x].norm = norm;
            args[x].p = p;
            args[x].index = x + 1;
            args[x].status = 0;
            args[x].tid = threads[x];
            if(pthread_create(&threads[x], NULL, thread_func, (void*)&args[x])){
                std::cerr << "THREAD CREATION ERROR" << std::endl;
                for(int y = 0; y <= x; y++){
                    delete[] args[y].C;
                    delete[] args[y].dop_mat;
                    delete[] args[y].block;
                    delete[] args[y].X;
                }
                delete[] threads;
                delete[] args;
                return -1;
            };
        }
        for(int x = 0; x < p; x++){
            pthread_join(threads[x], 0);
        }
        if(args[0].status == -1){
            for(int y = 0; y < p; y++){
                delete[] args[y].C;
                delete[] args[y].dop_mat;
                delete[] args[y].block;
                delete[] args[y].X;
            }
            delete[] threads;
            delete[] args;
            return -1;
        }
        for(i = 0; i < n; i++){
            X[i] = B[i];
        }
        for(int y = 0; y < p; y++){
            delete[] args[y].C;
            delete[] args[y].dop_mat;
            delete[] args[y].block;
            delete[] args[y].X;
        }
        delete[] threads;
        delete[] args;
    }
    return 1;
}
